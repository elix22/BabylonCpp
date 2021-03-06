#include <babylon/samples/meshes/colored_cube_faces_scene.h>

#include <babylon/cameras/arc_rotate_camera.h>
#include <babylon/lights/hemispheric_light.h>
#include <babylon/materials/multi_material.h>
#include <babylon/materials/standard_material.h>
#include <babylon/mesh/mesh.h>
#include <babylon/mesh/sub_mesh.h>

namespace BABYLON {
namespace Samples {

ColoredCubeFacesScene::ColoredCubeFacesScene(ICanvas* iCanvas)
    : IRenderableScene(iCanvas)
{
}

ColoredCubeFacesScene::~ColoredCubeFacesScene()
{
}

const char* ColoredCubeFacesScene::getName()
{
  return "Colored Cube Faces Scene";
}

void ColoredCubeFacesScene::initializeScene(ICanvas* canvas, Scene* scene)
{
  // Create a camera
  auto camera
    = ArcRotateCamera::New("camera", 1.f, 1.f, 12, Vector3::Zero(), scene);

  // Attach camera to canvas
  camera->attachControl(canvas, false);

  // Add a light
  auto light = HemisphericLight::New("hemi", Vector3(0, 1, 0), scene);

  // Reflect the light off the ground to light the mesh bottom
  light->groundColor = Color3(0.5f, 0.5f, 0.5f);

  // Create a builtin shape
  auto box             = Mesh::CreateBox("box", 5, scene);
  box->showBoundingBox = false;

  // Define a material
  auto f           = StandardMaterial::New("material0", scene);
  f->diffuseColor  = Color3(0.75f, 0.f, 0.f);
  auto ba          = StandardMaterial::New("material1", scene);
  ba->diffuseColor = Color3(0.f, 0.f, 0.75f);
  auto l           = StandardMaterial::New("material2", scene);
  l->diffuseColor  = Color3(0.f, 0.75f, 0.75f);
  auto r           = StandardMaterial::New("material3", scene);
  r->diffuseColor  = Color3(0.75f, 0.f, 0.75f);
  auto t           = StandardMaterial::New("material4", scene);
  t->diffuseColor  = Color3(0.f, 0.75f, 0.f);
  auto bo          = StandardMaterial::New("material5", scene);
  bo->diffuseColor = Color3(1.f, 1.f, 0.f);

  // Put into one material
  auto multi = MultiMaterial::New("nuggetman", scene);
  multi->subMaterials().emplace_back(f);
  multi->subMaterials().emplace_back(ba);
  multi->subMaterials().emplace_back(l);
  multi->subMaterials().emplace_back(r);
  multi->subMaterials().emplace_back(t);
  multi->subMaterials().emplace_back(bo);

  // apply material
  box->subMeshes.clear();
  auto verticesCount = box->getTotalVertices();
  box->subMeshes.emplace_back(SubMesh::New(0, 0, verticesCount, 0, 6, box));
  box->subMeshes.emplace_back(SubMesh::New(1, 1, verticesCount, 6, 6, box));
  box->subMeshes.emplace_back(SubMesh::New(2, 2, verticesCount, 12, 6, box));
  box->subMeshes.emplace_back(SubMesh::New(3, 3, verticesCount, 18, 6, box));
  box->subMeshes.emplace_back(SubMesh::New(4, 4, verticesCount, 24, 6, box));
  box->subMeshes.emplace_back(SubMesh::New(5, 5, verticesCount, 30, 6, box));
  box->material = multi;
}

} // end of namespace Samples
} // end of namespace BABYLON
