#ifndef BABYLON_LAYER_IEFFECT_LAYER_OPTIONS_H
#define BABYLON_LAYER_IEFFECT_LAYER_OPTIONS_H

#include <babylon/babylon_global.h>
#include <babylon/core/nullable.h>
#include <babylon/engine/engine_constants.h>

namespace BABYLON {

/**
 * @brief Effect layer options. This helps customizing the behaviour of the
 * effect layer.
 */
struct BABYLON_SHARED_EXPORT IEffectLayerOptions {

  /**
   * Multiplication factor apply to the canvas size to compute the render target
   * size used to generated the objects (the smaller the faster).
   */
  float mainTextureRatio = 0.5f;

  /**
   * Enforces a fixed size texture to ensure effect stability across devices.
   */
  Nullable<int> mainTextureFixedSize = nullptr;

  /**
   * Alpha blending mode used to apply the blur. Default depends of the
   * implementation.
   */
  unsigned int alphaBlendingMode = EngineConstants::ALPHA_COMBINE;

  /**
   * The camera attached to the layer.
   */
  Camera* camera = nullptr;

}; // end of struct IEffectLayerOptions

} // end of namespace BABYLON

#endif // end of BABYLON_LAYER_IEFFECT_LAYER_OPTIONS_H