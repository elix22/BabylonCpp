#include <babylon/samples/materialslibrary/normal_material_scene.h>

#include <babylon/cameras/arc_rotate_camera.h>
#include <babylon/lights/hemispheric_light.h>
#include <babylon/materialslibrary/normal/normal_material.h>
#include <babylon/mesh/mesh.h>

namespace BABYLON {
namespace Samples {

NormalMaterialScene::NormalMaterialScene(ICanvas* iCanvas)
    : IRenderableScene(iCanvas)
{
}

NormalMaterialScene::~NormalMaterialScene()
{
}

const char* NormalMaterialScene::getName()
{
  return "Normal Material Scene";
}

void NormalMaterialScene::initializeScene(ICanvas* canvas, Scene* scene)
{
  auto camera
    = ArcRotateCamera::New("camera1", 0, 0, 30, Vector3::Zero(), scene);
  camera->attachControl(canvas, true);

  auto light = HemisphericLight::New("light1", Vector3(0.f, 1.f, 0.f), scene);
  light->intensity = 1.f;

  auto sphere = Mesh::CreateSphere("sphere1", 16, 20, scene);

  auto normalMaterial
    = MaterialsLibrary::NormalMaterial::New("normalMat", scene);
  sphere->material = normalMaterial;
}

} // end of namespace Samples
} // end of namespace BABYLON
