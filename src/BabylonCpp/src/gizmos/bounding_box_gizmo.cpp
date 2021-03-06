#include <babylon/gizmos/bounding_box_gizmo.h>

#include <babylon/babylon_stl_util.h>
#include <babylon/behaviors/mesh/pointer_drag_behavior.h>
#include <babylon/cameras/camera.h>
#include <babylon/core/logging.h>
#include <babylon/core/string.h>
#include <babylon/engine/scene.h>
#include <babylon/materials/standard_material.h>
#include <babylon/mesh/abstract_mesh.h>
#include <babylon/mesh/lines_mesh.h>
#include <babylon/mesh/mesh.h>
#include <babylon/mesh/mesh_builder.h>
#include <babylon/mesh/vertex_data_options.h>
#include <babylon/rendering/utility_layer_renderer.h>

namespace BABYLON {

BoundingBoxGizmo::BoundingBoxGizmo(
  const Color3& color, const std::shared_ptr<UtilityLayerRenderer>& iGizmoLayer)
    : Gizmo{iGizmoLayer}
    , ignoreChildren{false}
    , rotationSphereSize{0.1f}
    , scaleBoxSize{0.1f}
    , fixedDragMeshScreenSize{false}
    , fixedDragMeshScreenSizeDistanceFactor{10.f}
    , scalePivot{std::nullopt}
    , _lineBoundingBox{nullptr}
    , _rotateSpheresParent{nullptr}
    , _scaleBoxesParent{nullptr}
    , _boundingDimensions{Vector3{1.f, 1.f, 1.f}}
    , _renderObserver{nullptr}
    , _pointerObserver{nullptr}
    , _scaleDragSpeed{0.2f}
    , _tmpVector{Vector3{0.f, 0.f, 0.f}}
{
  // Do not update the gizmo's scale so it has a fixed size to the object its
  // attached to
  _updateScale = false;

  _anchorMesh
    = AbstractMesh::New("anchor", iGizmoLayer->utilityLayerScene.get());
  // Create Materials
  auto coloredMaterial
    = StandardMaterial::New("", gizmoLayer->utilityLayerScene.get());
  coloredMaterial->disableLighting = true;
  coloredMaterial->emissiveColor   = color;
  auto hoverColoredMaterial
    = StandardMaterial::New("", gizmoLayer->utilityLayerScene.get());
  hoverColoredMaterial->disableLighting = true;
  hoverColoredMaterial->emissiveColor
    = color.copy().add(Color3(0.3f, 0.3f, 0.3f));

  // Build bounding box out of lines
  _lineBoundingBox = AbstractMesh::New("", gizmoLayer->utilityLayerScene.get());
  _lineBoundingBox->rotationQuaternion = Quaternion();
  std::vector<LinesMeshPtr> lines;
  std::vector<std::vector<Vector3>> linesPoints{
    {Vector3(0.f, 0.f, 0.f), Vector3(_boundingDimensions.x, 0.f, 0.f)},
    {Vector3(0.f, 0.f, 0.f), Vector3(0.f, _boundingDimensions.y, 0.f)},
    {Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, _boundingDimensions.z)},
    {Vector3(_boundingDimensions.x, 0.f, 0.f),
     Vector3(_boundingDimensions.x, _boundingDimensions.y, 0.f)},
    {Vector3(_boundingDimensions.x, 0.f, 0.f),
     Vector3(_boundingDimensions.x, 0.f, _boundingDimensions.z)},
    {Vector3(0.f, _boundingDimensions.y, 0.f),
     Vector3(_boundingDimensions.x, _boundingDimensions.y, 0.f)},
    {Vector3(0.f, _boundingDimensions.y, 0.f),
     Vector3(0.f, _boundingDimensions.y, _boundingDimensions.z)},
    {Vector3(0.f, 0.f, _boundingDimensions.z),
     Vector3(_boundingDimensions.x, 0.f, _boundingDimensions.z)},
    {Vector3(0.f, 0.f, _boundingDimensions.z),
     Vector3(0.f, _boundingDimensions.y, _boundingDimensions.z)},
    {Vector3(_boundingDimensions.x, _boundingDimensions.y,
             _boundingDimensions.z),
     Vector3(0.f, _boundingDimensions.y, _boundingDimensions.z)},
    {Vector3(_boundingDimensions.x, _boundingDimensions.y,
             _boundingDimensions.z),
     Vector3(_boundingDimensions.x, 0.f, _boundingDimensions.z)},
    {Vector3(_boundingDimensions.x, _boundingDimensions.y,
             _boundingDimensions.z),
     Vector3(_boundingDimensions.x, _boundingDimensions.y, 0.f)},
  };
  for (const auto& linePoints : linesPoints) {
    LinesOptions options;
    options.points = linePoints;
    lines.emplace_back(MeshBuilder::CreateLines(
      "lines", options, gizmoLayer->utilityLayerScene.get()));
  }
  for (auto& l : lines) {
    l->color = color;
    l->position().addInPlace(Vector3(-_boundingDimensions.x / 2.f,
                                     -_boundingDimensions.y / 2.f,
                                     -_boundingDimensions.z / 2.f));
    l->isPickable = false;
    _lineBoundingBox->addChild(*l);
  }
  _rootMesh->addChild(*_lineBoundingBox);

  // Create rotation spheres
  _rotateSpheresParent
    = AbstractMesh::New("", gizmoLayer->utilityLayerScene.get());
  _rotateSpheresParent->rotationQuaternion = Quaternion();
  for (unsigned int i = 0; i < 12; ++i) {
    SphereOptions sphereOptions{1.f};
    auto sphere = MeshBuilder::CreateSphere(
      "", sphereOptions, gizmoLayer->utilityLayerScene.get());
    sphere->rotationQuaternion = Quaternion();
    sphere->material           = coloredMaterial;

    // Drag behavior
    PointerDragBehavior _dragBehavior;
    _dragBehavior.moveAttached    = false;
    _dragBehavior.updateDragPlane = false;
    sphere->addBehavior(&_dragBehavior);
    Vector3 startingTurnDirection{1.f, 0.f, 0.f};
    auto totalTurnAmountOfDrag = 0;
    _dragBehavior.onDragStartObservable.add(
      [&](DragStartOrEndEvent* /*event*/, EventState& /*es*/) {
        startingTurnDirection.copyFrom(sphere->forward());
        totalTurnAmountOfDrag = 0;
      });
    _dragBehavior.onDragObservable.add([&](DragMoveEvent* event,
                                           EventState& /*es*/) {
      onRotationSphereDragObservable.notifyObservers({});
      if (attachedMesh()) {
        removeAndStorePivotPoint();

        auto worldDragDirection = startingTurnDirection;

        // Project the world right on to the drag plane
        auto toSub = event->dragPlaneNormal.scale(
          Vector3::Dot(event->dragPlaneNormal, worldDragDirection));
        auto dragAxis = worldDragDirection.subtract(toSub).normalizeToNew();

        // project drag delta on to the resulting drag axis and rotate based
        // on that
        auto projectDist = -Vector3::Dot(dragAxis, event->delta);

        // Make rotation relative to size of mesh.
        projectDist = (projectDist / _boundingDimensions.length())
                      * _anchorMesh->scaling().length();

        // Rotate based on axis
        if (!attachedMesh()->rotationQuaternion()) {
          attachedMesh()->rotationQuaternion = Quaternion::RotationYawPitchRoll(
            attachedMesh()->rotation().y, attachedMesh()->rotation().x,
            attachedMesh()->rotation().z);
        }
        if (!_anchorMesh->rotationQuaternion()) {
          _anchorMesh->rotationQuaternion = Quaternion::RotationYawPitchRoll(
            _anchorMesh->rotation().y, _anchorMesh->rotation().x,
            _anchorMesh->rotation().z);
        }

        // Do not allow the object to turn more than a full circle
        totalTurnAmountOfDrag += projectDist;
        if (std::abs(totalTurnAmountOfDrag) <= Math::PI2) {
          if (i >= 8) {
            Quaternion::RotationYawPitchRollToRef(0.f, 0.f, projectDist,
                                                  _tmpQuaternion);
          }
          else if (i >= 4) {
            Quaternion::RotationYawPitchRollToRef(projectDist, 0.f, 0.f,
                                                  _tmpQuaternion);
          }
          else {
            Quaternion::RotationYawPitchRollToRef(0.f, projectDist, 0.f,
                                                  _tmpQuaternion);
          }

          // Rotate around center of bounding box
          _anchorMesh->addChild(*attachedMesh());
          _anchorMesh->rotationQuaternion()->multiplyToRef(
            _tmpQuaternion, *_anchorMesh->rotationQuaternion());
          _anchorMesh->removeChild(*attachedMesh());
        }
        updateBoundingBox();

        restorePivotPoint();
      }
    });

    // Selection/deselection
    _dragBehavior.onDragStartObservable.add(
      [&](DragStartOrEndEvent* /*event*/, EventState& /*es*/) {
        onDragStartObservable.notifyObservers(nullptr);
        _selectNode(sphere);
      });
    _dragBehavior.onDragEndObservable.add(
      [&](DragStartOrEndEvent* /*event*/, EventState& /*es*/) {
        onRotationSphereDragEndObservable.notifyObservers(nullptr);
        _selectNode(nullptr);
      });

    _rotateSpheresParent->addChild(*sphere);
  }
  _rootMesh->addChild(*_rotateSpheresParent);

  // Create scale cubes
  _scaleBoxesParent
    = AbstractMesh::New("", gizmoLayer->utilityLayerScene.get());
  _scaleBoxesParent->rotationQuaternion = Quaternion();
  for (unsigned int i = 0; i < 2; i++) {
    for (unsigned int j = 0; j < 2; j++) {
      for (unsigned int k = 0; k < 2; k++) {
        BoxOptions boxOptions{0.1f};
        auto box      = MeshBuilder::CreateBox("", boxOptions,
                                          gizmoLayer->utilityLayerScene.get());
        box->material = coloredMaterial;

        // Dragging logic
        auto dragAxis
          = Vector3(i == 0 ? -1 : 1, j == 0 ? -1 : 1, k == 0 ? -1 : 1);
        PointerDragBehaviorOptions options;
        options.dragAxis = dragAxis;
        PointerDragBehavior _dragBehavior(options);
        _dragBehavior.moveAttached = false;
        box->addBehavior(&_dragBehavior);
        _dragBehavior.onDragObservable.add([&](DragMoveEvent* event,
                                               EventState& /*es*/) {
          onScaleBoxDragObservable.notifyObservers(nullptr);
          if (attachedMesh()) {
            removeAndStorePivotPoint();

            auto relativeDragDistance
              = (event->dragDistance / _boundingDimensions.length())
                * _anchorMesh->scaling().length();
            Vector3 deltaScale(relativeDragDistance, relativeDragDistance,
                               relativeDragDistance);
            deltaScale.scaleInPlace(_scaleDragSpeed);
            updateBoundingBox();

            if (scalePivot) {
              attachedMesh()->getWorldMatrix()->getRotationMatrixToRef(
                _tmpRotationMatrix);
              // Move anchor to desired pivot point (Bottom left corner +
              // dimension/2)
              _boundingDimensions.scaleToRef(0.5f, _tmpVector);
              Vector3::TransformCoordinatesToRef(_tmpVector, _tmpRotationMatrix,
                                                 _tmpVector);
              _anchorMesh->position().subtractInPlace(_tmpVector);
              _boundingDimensions.multiplyToRef(*scalePivot, _tmpVector);
              Vector3::TransformCoordinatesToRef(_tmpVector, _tmpRotationMatrix,
                                                 _tmpVector);
              _anchorMesh->position().addInPlace(_tmpVector);
            }
            else {
              // Scale from the position of the opposite corner
              box->absolutePosition().subtractToRef(_anchorMesh->position(),
                                                    _tmpVector);
              _anchorMesh->position().subtractInPlace(_tmpVector);
            }

            _anchorMesh->addChild(*attachedMesh());
            _anchorMesh->scaling().addInPlace(deltaScale);
            if (_anchorMesh->scaling().x < 0.f || _anchorMesh->scaling().y < 0.f
                || _anchorMesh->scaling().z < 0.f) {
              _anchorMesh->scaling().subtractInPlace(deltaScale);
            }
            _anchorMesh->removeChild(*attachedMesh());

            restorePivotPoint();
          }
        });

        // Selection/deselection
        _dragBehavior.onDragStartObservable.add(
          [&](DragStartOrEndEvent* /*event*/, EventState& /*es*/) {
            onDragStartObservable.notifyObservers(nullptr);
            _selectNode(box);
          });
        _dragBehavior.onDragEndObservable.add(
          [&](DragStartOrEndEvent* /*event*/, EventState& /*es*/) {
            onScaleBoxDragEndObservable.notifyObservers(nullptr);
            _selectNode(nullptr);
          });

        _scaleBoxesParent->addChild(*box);
      }
    }
  }
  _rootMesh->addChild(*_scaleBoxesParent);

  // Hover color change
  std::unordered_map<int, AbstractMeshPtr> pointerIds;
  _pointerObserver
    = gizmoLayer->utilityLayerScene.get()->onPointerObservable.add(
      [&](PointerInfo* pointerInfo, EventState& /*es*/) {
        if (!stl_util::contains(pointerIds,
                                (pointerInfo->pointerEvent).pointerId)) {
          const auto changeHoverColor
            = [&](const std::vector<AbstractMeshPtr>& meshes) {
                for (auto& mesh : meshes) {
                  if (pointerInfo->pickInfo.pickedMesh == mesh.get()) {
                    pointerIds[(pointerInfo->pointerEvent).pointerId] = mesh;
                    mesh->material = hoverColoredMaterial;
                  }
                }
              };
          changeHoverColor(_rotateSpheresParent->getChildMeshes());
          changeHoverColor(_scaleBoxesParent->getChildMeshes());
        }
        else {
          if (stl_util::contains(pointerIds,
                                 (pointerInfo->pointerEvent).pointerId)
              && pointerInfo->pickInfo.pickedMesh
                   != pointerIds[(pointerInfo->pointerEvent).pointerId].get()) {
            pointerIds[(pointerInfo->pointerEvent).pointerId]->material
              = coloredMaterial;
            pointerIds.erase((pointerInfo->pointerEvent).pointerId);
          }
        }
      });

  // Update bounding box positions
  _renderObserver = gizmoLayer->originalScene->onBeforeRenderObservable.add(
    [&](Scene* /*scene*/, EventState& /*es*/) {
      // Only update the bouding box if scaling has changed
      if (attachedMesh()
          && !_existingMeshScale.equals(attachedMesh()->scaling())) {
        updateBoundingBox();
      }
    });
  updateBoundingBox();
}

BoundingBoxGizmo::~BoundingBoxGizmo()
{
}

void BoundingBoxGizmo::removeAndStorePivotPoint()
{
  if (attachedMesh()) {
    // Save old pivot and set pivot to 0,0,0
    attachedMesh()->getPivotPointToRef(_oldPivotPoint);
    if (_oldPivotPoint.equalsToFloats(0.f, 0.f, 0.f)) {
      return;
    }
    auto identityReadOnly = Matrix::IdentityReadOnly();
    attachedMesh()->setPivotMatrix(identityReadOnly);
    _oldPivotPoint.subtractToRef(attachedMesh()->getPivotPoint(),
                                 _pivotTranslation);
    _tmpVector.copyFromFloats(1.f, 1.f, 1.f);
    _tmpVector.subtractInPlace(attachedMesh()->scaling());
    _tmpVector.multiplyInPlace(_pivotTranslation);
    attachedMesh()->position().addInPlace(_tmpVector);
  }
}

void BoundingBoxGizmo::restorePivotPoint()
{
  if (attachedMesh() && !_oldPivotPoint.equalsToFloats(0, 0, 0)) {
    attachedMesh()->setPivotPoint(_oldPivotPoint);
    _oldPivotPoint.subtractToRef(attachedMesh()->getPivotPoint(),
                                 _pivotTranslation);
    _tmpVector.copyFromFloats(1, 1, 1);
    _tmpVector.subtractInPlace(attachedMesh()->scaling());
    _tmpVector.multiplyInPlace(_pivotTranslation);
    attachedMesh()->position().subtractInPlace(_tmpVector);
  }
}

void BoundingBoxGizmo::_attachedMeshChanged(AbstractMesh* value)
{
  if (value) {
    // Reset anchor mesh to match attached mesh's scale
    // This is needed to avoid invalid box/sphere position on first drag
    _anchorMesh->addChild(*value);
    _anchorMesh->removeChild(*value);
    updateBoundingBox();
  }
}

void BoundingBoxGizmo::_selectNode(const MeshPtr& selectedMesh)
{
  auto childMeshes = _rotateSpheresParent->getChildMeshes();
  for (const auto& m : _scaleBoxesParent->getChildMeshes()) {
    m->isVisible = (!selectedMesh || m == selectedMesh);
  }
}

void BoundingBoxGizmo::_recurseComputeWorld(Node* node)
{
  node->computeWorldMatrix(true);

  if (!ignoreChildren) {
    for (const auto& n : node->getDescendants()) {
      _recurseComputeWorld(n.get());
    }
  }
}

void BoundingBoxGizmo::updateBoundingBox()
{
  if (attachedMesh()) {
    removeAndStorePivotPoint();
    _update();

    // Rotate based on axis
    if (!attachedMesh()->rotationQuaternion()) {
      attachedMesh()->rotationQuaternion = Quaternion::RotationYawPitchRoll(
        attachedMesh()->rotation().y, attachedMesh()->rotation().x,
        attachedMesh()->rotation().z);
    }
    if (!_anchorMesh->rotationQuaternion()) {
      _anchorMesh->rotationQuaternion = Quaternion::RotationYawPitchRoll(
        _anchorMesh->rotation().y, _anchorMesh->rotation().x,
        _anchorMesh->rotation().z);
    }
    _anchorMesh->rotationQuaternion()->copyFrom(
      *attachedMesh()->rotationQuaternion());

    // Store original position and reset mesh to origin before computing the
    // bounding box
    _tmpQuaternion.copyFrom(*attachedMesh()->rotationQuaternion());
    _tmpVector.copyFrom(attachedMesh()->position());
    attachedMesh()->rotationQuaternion()->set(0.f, 0.f, 0.f, 1.f);
    attachedMesh()->position().set(0.f, 0.f, 0.f);

    // Update bounding dimensions/positions
    auto boundingMinMax
      = attachedMesh()->getHierarchyBoundingVectors(!ignoreChildren);
    boundingMinMax.max.subtractToRef(boundingMinMax.min, _boundingDimensions);

    // Update gizmo to match bounding box scaling and rotation
    _lineBoundingBox->scaling().copyFrom(_boundingDimensions);
    _lineBoundingBox->position().set(
      (boundingMinMax.max.x + boundingMinMax.min.x) / 2.f,
      (boundingMinMax.max.y + boundingMinMax.min.y) / 2.f,
      (boundingMinMax.max.z + boundingMinMax.min.z) / 2.f);
    _rotateSpheresParent->position().copyFrom(_lineBoundingBox->position());
    _scaleBoxesParent->position().copyFrom(_lineBoundingBox->position());
    _lineBoundingBox->computeWorldMatrix();
    _anchorMesh->position().copyFrom(_lineBoundingBox->absolutePosition());

    // restore position/rotation values
    attachedMesh()->rotationQuaternion()->copyFrom(_tmpQuaternion);
    attachedMesh()->position().copyFrom(_tmpVector);
    _recurseComputeWorld(attachedMesh());
  }

  // Update rotation sphere locations
  auto rotateSpheres = _rotateSpheresParent->getChildMeshes();
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 2; ++j) {
      for (unsigned int k = 0; k < 2; ++k) {
        auto index = ((i * 4) + (j * 2)) + k;
        if (index < rotateSpheres.size()) {
          if (i == 0) {
            rotateSpheres[index]->position().set(_boundingDimensions.x / 2.f,
                                                 _boundingDimensions.y * j,
                                                 _boundingDimensions.z * k);
            rotateSpheres[index]->position().addInPlace(Vector3(
              -_boundingDimensions.x / 2.f, -_boundingDimensions.y / 2.f,
              -_boundingDimensions.z / 2.f));
            rotateSpheres[index]->lookAt(
              Vector3::Cross(Vector3::Right(),
                             rotateSpheres[index]->position().normalizeToNew())
                .normalizeToNew()
                .add(rotateSpheres[index]->position()));
          }
          if (i == 1) {
            rotateSpheres[index]->position().set(_boundingDimensions.x * j,
                                                 _boundingDimensions.y / 2.f,
                                                 _boundingDimensions.z * k);
            rotateSpheres[index]->position().addInPlace(Vector3(
              -_boundingDimensions.x / 2.f, -_boundingDimensions.y / 2.f,
              -_boundingDimensions.z / 2.f));
            rotateSpheres[index]->lookAt(
              Vector3::Cross(Vector3::Up(),
                             rotateSpheres[index]->position().normalizeToNew())
                .normalizeToNew()
                .add(rotateSpheres[index]->position()));
          }
          if (i == 2) {
            rotateSpheres[index]->position().set(_boundingDimensions.x * j,
                                                 _boundingDimensions.y * k,
                                                 _boundingDimensions.z / 2.f);
            rotateSpheres[index]->position().addInPlace(Vector3(
              -_boundingDimensions.x / 2.f, -_boundingDimensions.y / 2.f,
              -_boundingDimensions.z / 2.f));
            rotateSpheres[index]->lookAt(
              Vector3::Cross(Vector3::Forward(),
                             rotateSpheres[index]->position().normalizeToNew())
                .normalizeToNew()
                .add(rotateSpheres[index]->position()));
          }
          if (fixedDragMeshScreenSize) {
            _rootMesh->computeWorldMatrix();
            _rotateSpheresParent->computeWorldMatrix();
            rotateSpheres[index]->computeWorldMatrix();
            rotateSpheres[index]->absolutePosition().subtractToRef(
              gizmoLayer->utilityLayerScene->activeCamera->position,
              _tmpVector);
            auto distanceFromCamera = rotationSphereSize * _tmpVector.length()
                                      / fixedDragMeshScreenSizeDistanceFactor;
            rotateSpheres[index]->scaling().set(
              distanceFromCamera, distanceFromCamera, distanceFromCamera);
          }
          else {
            rotateSpheres[index]->scaling().set(
              rotationSphereSize, rotationSphereSize, rotationSphereSize);
          }
        }
      }
    }
  }

  // Update scale box locations
  auto scaleBoxes = _scaleBoxesParent->getChildMeshes();
  for (unsigned int i = 0; i < 2; ++i) {
    for (unsigned int j = 0; j < 2; ++j) {
      for (unsigned int k = 0; k < 2; ++k) {
        auto index = ((i * 4) + (j * 2)) + k;
        if (index < scaleBoxes.size() && scaleBoxes[index]) {
          scaleBoxes[index]->position().set(_boundingDimensions.x * i,
                                            _boundingDimensions.y * j,
                                            _boundingDimensions.z * k);
          scaleBoxes[index]->position().addInPlace(
            Vector3(-_boundingDimensions.x / 2.f, -_boundingDimensions.y / 2.f,
                    -_boundingDimensions.z / 2.f));
          if (fixedDragMeshScreenSize) {
            _rootMesh->computeWorldMatrix();
            _scaleBoxesParent->computeWorldMatrix();
            scaleBoxes[index]->computeWorldMatrix();
            scaleBoxes[index]->absolutePosition().subtractToRef(
              gizmoLayer->utilityLayerScene->activeCamera->position,
              _tmpVector);
            auto distanceFromCamera = scaleBoxSize * _tmpVector.length()
                                      / fixedDragMeshScreenSizeDistanceFactor;
            scaleBoxes[index]->scaling().set(
              distanceFromCamera, distanceFromCamera, distanceFromCamera);
          }
          else {
            scaleBoxes[index]->scaling().set(scaleBoxSize, scaleBoxSize,
                                             scaleBoxSize);
          }
        }
      }
    }
  }
  if (attachedMesh()) {
    _existingMeshScale.copyFrom(attachedMesh()->scaling());
    restorePivotPoint();
  }
}

void BoundingBoxGizmo::setEnabledRotationAxis(const std::string axis)
{
  size_t i = 0;
  for (auto& m : _rotateSpheresParent->getChildMeshes()) {
    if (i < 4) {
      m->setEnabled(String::contains(axis, "x"));
    }
    else if (i < 8) {
      m->setEnabled(String::contains(axis, "y"));
    }
    else {
      m->setEnabled(String::contains(axis, "z"));
    }
    ++i;
  }
}

void BoundingBoxGizmo::dispose(bool doNotRecurse,
                               bool disposeMaterialAndTextures)
{
  gizmoLayer->utilityLayerScene->onPointerObservable.remove(_pointerObserver);
  gizmoLayer->originalScene->onBeforeRenderObservable.remove(_renderObserver);
  _lineBoundingBox->dispose();
  _rotateSpheresParent->dispose();
  _scaleBoxesParent->dispose();
  Gizmo::dispose(doNotRecurse, disposeMaterialAndTextures);
}

MeshPtr BoundingBoxGizmo::MakeNotPickableAndWrapInBoundingBox(Mesh* mesh)
{
  std::function<void(AbstractMesh * root)> makeNotPickable;
  makeNotPickable = [&makeNotPickable](AbstractMesh* root) {
    root->isPickable = false;
    for (auto& c : root->getChildMeshes()) {
      makeNotPickable(c.get());
    };
  };
  makeNotPickable(mesh);

  // Reset position to get boudning box from origin with no rotation
  if (!mesh->rotationQuaternion()) {
    mesh->rotationQuaternion = Quaternion::RotationYawPitchRoll(
      mesh->rotation().y, mesh->rotation().x, mesh->rotation().z);
  }
  auto oldPos = mesh->position();
  auto oldRot = *mesh->rotationQuaternion();
  mesh->rotationQuaternion()->set(0.f, 0.f, 0.f, 1.f);
  mesh->position().set(0.f, 0.f, 0.f);

  // Update bounding dimensions/positions
  BoxOptions options(1.f);
  auto box = MeshBuilder::CreateBox("box", options, mesh->getScene());
  auto boundingMinMax = mesh->getHierarchyBoundingVectors();
  boundingMinMax.max.subtractToRef(boundingMinMax.min, box->scaling());
  box->position().set((boundingMinMax.max.x + boundingMinMax.min.x) / 2.f,
                      (boundingMinMax.max.y + boundingMinMax.min.y) / 2.f,
                      (boundingMinMax.max.z + boundingMinMax.min.z) / 2.f);

  // Restore original positions
  mesh->addChild(*box);
  mesh->rotationQuaternion()->copyFrom(oldRot);
  mesh->position().copyFrom(oldPos);

  // Reverse parenting
  mesh->removeChild(*box);
  box->addChild(*mesh);
  box->visibility = 0.f;
  return box;
}

void BoundingBoxGizmo::setCustomMesh(const MeshPtr& /*mesh*/,
                                     bool /*useGizmoMaterial*/)
{
  BABYLON_LOG_ERROR("BoundingBoxGizmo",
                    "Custom meshes are not supported on this gizmo");
}

} // end of namespace BABYLON
