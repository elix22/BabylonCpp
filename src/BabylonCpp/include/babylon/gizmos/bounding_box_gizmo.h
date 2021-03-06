#ifndef BABYLON_GIZMOS_BOUNDING_BOX_GIZMO_H
#define BABYLON_GIZMOS_BOUNDING_BOX_GIZMO_H

#include <babylon/babylon_api.h>
#include <babylon/core/structs.h>
#include <babylon/gizmos/gizmo.h>
#include <babylon/math/color3.h>
#include <babylon/math/quaternion.h>
#include <babylon/math/vector3.h>
#include <babylon/rendering/utility_layer_renderer.h>
#include <babylon/tools/observable.h>

namespace BABYLON {

class Node;
class UtilityLayerRenderer;
using NodePtr                 = std::shared_ptr<Node>;
using UtilityLayerRendererPtr = std::shared_ptr<UtilityLayerRenderer>;

/**
 * @brief Bounding box gizmo.
 */
class BABYLON_SHARED_EXPORT BoundingBoxGizmo : public Gizmo {

public:
  /**
   * @brief Creates an BoundingBoxGizmo.
   * @param color The color of the gizmo
   * @param gizmoLayer The utility layer the gizmo will be added to
   */
  BoundingBoxGizmo(const Color3& color = Color3::Gray(),
                   const UtilityLayerRendererPtr& gizmoLayer
                   = UtilityLayerRenderer::DefaultUtilityLayer());
  ~BoundingBoxGizmo() override;

  /**
   * @brief Updates the bounding box information for the Gizmo.
   */
  void updateBoundingBox();

  /**
   * @brief Enables rotation on the specified axis and disables rotation on the
   * others.
   * @param axis The list of axis that should be enabled (eg. "xy" or "xyz")
   */
  void setEnabledRotationAxis(const std::string axis);

  /**
   * @brief Disposes of the gizmo.
   */
  void dispose(bool doNotRecurse               = false,
               bool disposeMaterialAndTextures = false) override;

  /**
   * @brief Makes a mesh not pickable and wraps the mesh inside of a bounding
   * box mesh that is pickable. (This is useful to avoid picking within complex
   * geometry).
   * @param mesh the mesh to wrap in the bounding box mesh and make not pickable
   * @returns the bounding box mesh with the passed in mesh as a child
   */
  static MeshPtr MakeNotPickableAndWrapInBoundingBox(Mesh* mesh);

  /**
   * @brief CustomMeshes are not supported by this gizmo.
   * @param mesh The mesh to replace the default mesh of the gizmo
   */
  void setCustomMesh(const MeshPtr& mesh,
                     bool useGizmoMaterial = false) override;

protected:
  void _attachedMeshChanged(AbstractMesh* value) override;

private:
  void removeAndStorePivotPoint();
  void restorePivotPoint();
  void _selectNode(const MeshPtr& selectedMesh);
  void _recurseComputeWorld(Node* node);

public:
  /**
   * If child meshes should be ignored when calculating the boudning box. This
   * should be set to true to avoid perf hits with heavily nested meshes
   * (Default: false)
   */
  bool ignoreChildren;
  /**
   * The size of the rotation spheres attached to the bounding box (Default:
   * 0.1)
   */
  float rotationSphereSize;
  /**
   * The size of the scale boxes attached to the bounding box (Default: 0.1)
   */
  float scaleBoxSize;
  /**
   * If set, the rotation spheres and scale boxes will increase in size based on
   * the distance away from the camera to have a consistent screen size
   * (Default: false)
   */
  bool fixedDragMeshScreenSize;

  /**
   * The distance away from the object which the draggable meshes should appear
   * world sized when fixedDragMeshScreenSize is set to true (default: 10)
   */
  float fixedDragMeshScreenSizeDistanceFactor;
  /**
   * Fired when a rotation sphere or scale box is dragged
   */
  Observable<DragStartOrEndEvent> onDragStartObservable;
  /**
   * Fired when a scale box is dragged
   */
  Observable<DragMoveEvent> onScaleBoxDragObservable;
  /**
   * Fired when a scale box drag is ended
   */
  Observable<DragStartOrEndEvent> onScaleBoxDragEndObservable;
  /**
   * Fired when a rotation sphere is dragged
   */
  Observable<DragStartOrEndEvent> onRotationSphereDragObservable;
  /**
   * Fired when a rotation sphere drag is ended
   */
  Observable<DragStartOrEndEvent> onRotationSphereDragEndObservable;
  /**
   * Relative bounding box pivot used when scaling the attached mesh. When null
   * object with scale from the opposite corner. 0.5,0.5,0.5 for center and
   * 0.5,0,0.5 for bottom (Default: null)
   */
  std::optional<Vector3> scalePivot;

private:
  AbstractMeshPtr _lineBoundingBox;
  AbstractMeshPtr _rotateSpheresParent;
  AbstractMeshPtr _scaleBoxesParent;
  Vector3 _boundingDimensions;
  Observer<Scene>::Ptr _renderObserver;
  Observer<PointerInfo>::Ptr _pointerObserver;
  float _scaleDragSpeed;

  Quaternion _tmpQuaternion;
  Vector3 _tmpVector;
  Matrix _tmpRotationMatrix;

  AbstractMeshPtr _anchorMesh;
  Vector3 _existingMeshScale;

  Vector3 _oldPivotPoint;
  Vector3 _pivotTranslation;

}; // end of class BoundingBoxGizmo

} // end of namespace BABYLON

#endif // end of BABYLON_GIZMOS_BOUNDING_BOX_GIZMO_H
