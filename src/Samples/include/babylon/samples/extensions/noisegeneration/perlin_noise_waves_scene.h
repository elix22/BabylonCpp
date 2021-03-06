#ifndef BABYLON_SAMPLES_EXTENSIONS_NOISE_GENERATION_PERLIN_NOISE_WAVES_SCENE_H
#define BABYLON_SAMPLES_EXTENSIONS_NOISE_GENERATION_PERLIN_NOISE_WAVES_SCENE_H

#include <babylon/extensions/noisegeneration/perlin_noise.h>
#include <babylon/interfaces/irenderable_scene.h>

namespace BABYLON {

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

namespace Samples {

class PerlinNoiseWavesScene : public IRenderableScene {

public:
  PerlinNoiseWavesScene(ICanvas* iCanvas);
  ~PerlinNoiseWavesScene() override;

  const char* getName() override;
  void initializeScene(ICanvas* canvas, Scene* scene) override;

private:
  MeshPtr _ground;
  MeshPtr _ground2;
  Float32Array _positions;
  Uint32Array _indices;
  Extensions::PerlinNoise _perlineNoise;
  float _alpha;

}; // end of class WavesScene

} // end of namespace Samples
} // end of namespace BABYLON

#endif // end of
       // BABYLON_SAMPLES_EXTENSIONS_NOISE_GENERATION_PERLIN_NOISE_WAVES_SCENE_H
