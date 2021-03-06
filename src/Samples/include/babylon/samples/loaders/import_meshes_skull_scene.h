#ifndef BABYLON_SAMPLES_LOADERS_IMPORT_MESHES_SKULL_SCENE_H
#define BABYLON_SAMPLES_LOADERS_IMPORT_MESHES_SKULL_SCENE_H

#include <babylon/interfaces/irenderable_scene.h>

namespace BABYLON {

class ArcRotateCamera;
class PointLight;
using ArcRotateCameraPtr = std::shared_ptr<ArcRotateCamera>;
using PointLightPtr      = std::shared_ptr<PointLight>;

namespace Samples {

/**
 * @brief Import Meshes Skull Scene.
 * @see https://www.babylonjs-playground.com/#UKNERM#0
 */
class ImportMeshesSkullScene : public IRenderableScene {

public:
  ImportMeshesSkullScene(ICanvas* iCanvas);
  ~ImportMeshesSkullScene() override;

  const char* getName() override;
  void initializeScene(ICanvas* canvas, Scene* scene) override;

private:
  PointLightPtr _light;
  ArcRotateCameraPtr _camera;

}; // end of class ImportMeshesSkullScene

} // end of namespace Samples
} // end of namespace BABYLON

#endif // end of BABYLON_SAMPLES_LOADERS_IMPORT_MESHES_SKULL_SCENE_H
