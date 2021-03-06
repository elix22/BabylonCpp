#ifndef BABYLON_POSTPROCESS_RENDER_PIPELINE_PIPELINES_SSAO_RENDERING_PIPLINE_H
#define BABYLON_POSTPROCESS_RENDER_PIPELINE_PIPELINES_SSAO_RENDERING_PIPLINE_H

#include <map>

#include <babylon/babylon_api.h>
#include <babylon/babylon_common.h>
#include <babylon/interfaces/idisposable.h>
#include <babylon/postprocess/renderpipeline/post_process_render_pipeline.h>

namespace picojson {
class value;
typedef std::vector<value> array;
typedef std::map<std::string, value> object;
} // end of namespace picojson

namespace BABYLON {

class DynamicTexture;
class PassPostProcess;
class PostProcess;
class Scene;
class Texture;
class Vector3;
using TexturePtr        = std::shared_ptr<Texture>;
using DynamicTexturePtr = std::shared_ptr<DynamicTexture>;

namespace Json {
typedef picojson::value value;
typedef picojson::array array;
typedef picojson::object object;
} // namespace Json

struct SSAO2Ratio {
  float ssaoRatio;
  float blurRatio;
}; // end of struct SSAO2Ratio

class BABYLON_SHARED_EXPORT SSAO2RenderingPipeline
    : public PostProcessRenderPipeline {

public:
  /**
   * The PassPostProcess id in the pipeline that contains the original scene
   * color
   */
  static constexpr const char* SSAOOriginalSceneColorEffect
    = "SSAOOriginalSceneColorEffect";
  /**
   * The SSAO PostProcess id in the pipeline
   */
  static constexpr const char* SSAORenderEffect = "SSAORenderEffect";
  /**
   * The horizontal blur PostProcess id in the pipeline
   */
  static constexpr const char* SSAOBlurHRenderEffect = "SSAOBlurHRenderEffect";
  /**
   * The vertical blur PostProcess id in the pipeline
   */
  static constexpr const char* SSAOBlurVRenderEffect = "SSAOBlurVRenderEffect";
  /**
   * The PostProcess id in the pipeline that combines the SSAO-Blur output with
   * the original scene color (SSAOOriginalSceneColorEffect)
   */
  static constexpr const char* SSAOCombineRenderEffect
    = "SSAOCombineRenderEffect";

public:
  /**
   * @brief Constructor
   * @param {string} name - The rendering pipeline name
   * @param {BABYLON.Scene} scene - The scene linked to this pipeline
   * @param {any} ratio - The size of the postprocesses. Can be a number shared
   * between passes or an object for more precision: { ssaoRatio: 0.5,
   * combineRatio: 1.0 }
   * @param {BABYLON.Camera[]} cameras - The array of cameras that the rendering
   * pipeline will be attached to
   */
  SSAO2RenderingPipeline(const std::string& name, Scene* scene, float ratio,
                         const std::vector<CameraPtr>& cameras);
  SSAO2RenderingPipeline(const std::string& name, Scene* scene,
                         const SSAO2Ratio& ratio,
                         const std::vector<CameraPtr>& cameras);
  virtual ~SSAO2RenderingPipeline() override;

  /** Methods */
  static bool IsSupported();
  /** Hidden */
  void _rebuild() override;

  /**
   * @brief Removes the internal pipeline assets and detatches the pipeline from
   * the scene cameras.
   */
  void dispose(bool disableGeometryBufferRenderer = false,
               bool disposeMaterialAndTextures    = false) override;

  /**
   * @brief Serialize the rendering pipeline (Used when exporting).
   * @returns the serialized object
   */
  Json::object serialize() const;

  /**
   * @brief Parse the serialized pipeline.
   * @param source Source pipeline.
   * @param scene The scene to load the pipeline to.
   * @param rootUrl The URL of the serialized pipeline.
   * @returns An instantiated pipeline from the serialized object.
   */
  static std::unique_ptr<SSAO2RenderingPipeline>
  Parse(const Json::value& source, Scene* scene, const std::string& url);

private:
  void set_samples(unsigned int n);
  unsigned int get_samples() const;
  void set_textureSamples(unsigned int n);
  unsigned int get_textureSamples() const;
  void set_expensiveBlur(bool b);
  bool get_expensiveBlur() const;
  void _createBlurPostProcess(float ssaoRatio, float blurRatio);
  // Van der Corput radical inverse
  float _radicalInverse_VdC(uint32_t i);
  std::array<float, 2> _hammersley(uint32_t i, uint32_t n);
  Vector3 _hemisphereSample_uniform(float u, float v);
  Float32Array _generateHemisphere();
  void _createSSAOPostProcess(float ratio);
  void _createSSAOCombinePostProcess(float ratio);
  void _createRandomTexture();

public:
  /**
   * The output strength of the SSAO post-process. Default value is 1.0.
   */
  float totalStrength;

  /**
   * Maximum depth value to still render AO. A smooth falloff makes the dimming
   * more natural, so there will be no abrupt shading change.
   */
  float maxZ;

  /**
   * In order to save performances, SSAO radius is clamped on close geometry.
   * This ratio changes by how much
   */
  float minZAspect;

  /**
   * Number of samples used for the SSAO calculations.
   */
  Property<SSAO2RenderingPipeline, unsigned int> samples;

  /**
   * Number of samples to use for antialiasing
   */
  Property<SSAO2RenderingPipeline, unsigned int> textureSamples;

  /**
   * Are we using bilateral blur ?
   */
  Property<SSAO2RenderingPipeline, bool> expensiveBlur;

  /**
   * The radius around the analyzed pixel used by the SSAO post-process. Default
   * value is 2.0
   */
  float radius;

  /**
   * Related to fallOff, used to interpolate SSAO samples (first interpolate
   * function input) based on the occlusion difference of each pixel
   * Must not be equal to fallOff and superior to fallOff.
   * Default value is 0.0075
   */
  float area;

  /**
   * Related to area, used to interpolate SSAO samples (second interpolate
   * function input) based on the occlusion difference of each pixel
   * Must not be equal to area and inferior to area.
   * Default value is 0.000001
   */
  float fallOff;

  /**
   * The base color of the SSAO post-process
   * The final result is "base + ssao" between [0, 1]
   */
  float base;

private:
  /**
   * Number of samples used for the SSAO calculations. Default value is 8
   */
  unsigned int _samples;

  /**
   * Number of samples to use for antialiasing
   */
  unsigned int _textureSamples;

  /**
   * Ratio object used for SSAO ratio and blur ratio
   */
  SSAO2Ratio _ratio;

  /**
   * Dynamically generated sphere sampler.
   */

  Float32Array _sampleSphere;

  /**
   * Blur filter offsets
   */
  Float32Array _samplerOffsets;

  /**
   * Are we using bilateral blur ?
   */
  bool _expensiveBlur;

  Scene* _scene;
  TexturePtr _depthTexture;
  TexturePtr _normalTexture;
  DynamicTexturePtr _randomTexture;

  PassPostProcess* _originalColorPostProcess;
  PostProcess* _ssaoPostProcess;
  PostProcess* _blurHPostProcess;
  PostProcess* _blurVPostProcess;
  PostProcess* _ssaoCombinePostProcess;

  bool _firstUpdate;
  Uint32Array _bits;

}; // end of class SSAORenderingPipeline

} // end of namespace BABYLON

#endif // end of
       // BABYLON_POSTPROCESS_RENDER_PIPELINE_PIPELINES_SSAO_RENDERING_PIPLINE_H
