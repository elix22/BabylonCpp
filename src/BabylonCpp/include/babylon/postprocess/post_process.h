#ifndef BABYLON_POSTPROCESS_POST_PROCESS_H
#define BABYLON_POSTPROCESS_POST_PROCESS_H

#include <babylon/babylon_api.h>
#include <babylon/core/structs.h>
#include <babylon/core/variant.h>
#include <babylon/engine/engine_constants.h>
#include <babylon/materials/textures/texture_constants.h>
#include <babylon/math/color4.h>
#include <babylon/tools/observable.h>
#include <babylon/tools/observer.h>

namespace BABYLON {

class Animation;
class Camera;
class Effect;
class Engine;
class Scene;
class InternalTexture;
using CameraPtr          = std::shared_ptr<Camera>;
using InternalTexturePtr = std::shared_ptr<InternalTexture>;

/**
 * @brief PostProcess can be used to apply a shader to a texture after it has
 * been rendered See https://doc.babylonjs.com/how_to/how_to_use_postprocesses
 */
class BABYLON_SHARED_EXPORT PostProcess {

public:
  /**
   * @brief Creates a new instance PostProcess.
   * @param name The name of the PostProcess.
   * @param fragmentUrl The url of the fragment shader to be used.
   * @param parameters Array of the names of uniform non-sampler2D variables
   * that will be passed to the shader.
   * @param samplers Array of the names of uniform sampler2D variables that will
   * be passed to the shader.
   * @param options The required width/height ratio to downsize to before
   * computing the render pass. (Use 1.0 for full size)
   * @param camera The camera to apply the render pass to.
   * @param samplingMode The sampling mode to be used when computing the pass.
   * (default: 0)
   * @param engine The engine which the post process will be applied. (default:
   * current engine)
   * @param reusable If the post process can be reused on the same frame.
   * (default: false)
   * @param defines String of defines that will be set when running the fragment
   * shader. (default: null)
   * @param textureType Type of textures used when performing the post process.
   * (default: 0)
   * @param vertexUrl The url of the vertex shader to be used. (default:
   * "postprocess")
   * @param indexParameters The index parameters to be used for babylons include
   * syntax "#include<kernelBlurVaryingDeclaration>[0..varyingCount]". (default:
   * undefined) See usage in babylon.blurPostProcess.ts and kernelBlur.vertex.fx
   * @param blockCompilation If the shader should not be compiled imediatly.
   * (default: false)
   */
  PostProcess(
    const std::string& name, const std::string& fragmentUrl,
    const std::vector<std::string>& parameters,
    const std::vector<std::string>& samplers,
    const Variant<float, PostProcessOptions>& options, const CameraPtr& camera,
    unsigned int samplingMode = TextureConstants::NEAREST_SAMPLINGMODE,
    Engine* engine = nullptr, bool reusable = false,
    const std::string& defines   = "",
    unsigned int textureType     = EngineConstants::TEXTURETYPE_UNSIGNED_INT,
    const std::string& vertexUrl = "postprocess",
    const std::unordered_map<std::string, unsigned int>& indexParameters = {},
    bool blockCompilation = false);
  virtual ~PostProcess();

  // Events

  /**
   * @brief A function that is added to the onActivateObservable.
   */
  void setOnActivate(
    const std::function<void(Camera* camera, EventState&)>& callback);

  /**
   * @brief A function that is added to the onSizeChangedObservable.
   */
  void setOnSizeChanged(
    const std::function<void(PostProcess* postProcess, EventState&)>& callback);

  /**
   * @brief A function that is added to the onApplyObservable.
   */
  void
  setOnApply(const std::function<void(Effect* effect, EventState&)>& callback);

  /**
   * @brief A function that is added to the onBeforeRenderObservable.
   */
  void setOnBeforeRender(
    const std::function<void(Effect* effect, EventState&)>& callback);

  /**
   * @brief A function that is added to the onAfterRenderObservable.
   */
  void setOnAfterRender(
    const std::function<void(Effect* effect, EventState&)>& callback);

  /**
   * @brief The input texture for this post process and the output texture of
   * the previous post process. When added to a pipeline the previous post
   * process will render it's output into this texture and this texture will be
   * used as textureSampler in the fragment shader of this post process.
   */
  InternalTexturePtr& inputTexture();
  void setInputTexture(const InternalTexturePtr& value);

  /**
   * @brief Gets the camera which post process is applied to.
   * @returns The camera the post process is applied to.
   */
  CameraPtr& getCamera();

  /**
   * @brief Gets the texel size of the postprocess.
   * See https://en.wikipedia.org/wiki/Texel_(graphics)
   */
  Vector2 texelSize();

  /**
   * @brief Gets the engine which this post process belongs to.
   * @returns The engine the post process was enabled with.
   */
  Engine* getEngine();

  /**
   * @brief The effect that is created when initializing the post process.
   * @returns The created effect corrisponding the the postprocess.
   */
  Effect* getEffect();

  /**
   * @brief To avoid multiple redundant textures for multiple post process, the
   * output the output texture for this post process can be shared with another.
   * @param postProcess The post process to share the output with.
   * @returns This post process.
   */
  PostProcess& shareOutputWith(PostProcess* postProcess);

  /**
   * @brief Reverses the effect of calling shareOutputWith and returns the post
   * process back to its original state. This should be called if the post
   * process that shares output with this post process is disabled/disposed.
   */
  void useOwnOutput();

  /**
   * @brief Updates the effect with the current post process compile time values
   * and recompiles the shader.
   * @param defines Define statements that should be added at the beginning of
   * the shader. (default: null)
   * @param uniforms Set of uniform variables that will be passed to the shader.
   * (default: null)
   * @param samplers Set of Texture2D variables that will be passed to the
   * shader. (default: null)
   * @param indexParameters The index parameters to be used for babylons include
   * syntax "#include<kernelBlurVaryingDeclaration>[0..varyingCount]". (default:
   * undefined) See usage in babylon.blurPostProcess.ts and kernelBlur.vertex.fx
   * @param onCompiled Called when the shader has been compiled.
   * @param onError Called if there is an error when compiling a shader.
   */
  virtual void updateEffect(
    const std::string& defines                                           = "",
    const std::vector<std::string>& uniforms                             = {},
    const std::vector<std::string>& samplers                             = {},
    const std::unordered_map<std::string, unsigned int>& indexParameters = {},
    const std::function<void(Effect* effect)>& onCompiled = nullptr,
    const std::function<void(Effect* effect, const std::string& errors)>&
      onError
    = nullptr);

  /**
   * @brief The post process is reusable if it can be used multiple times within
   * one frame.
   * @returns If the post process is reusable
   */
  bool isReusable() const;

  /**
   * @brief Invalidate frameBuffer to hint the postprocess to create a depth
   * buffer
   */
  void markTextureDirty();

  /**
   * @brief Activates the post process by intializing the textures to be used
   * when executed. Notifies onActivateObservable. When this post process is
   * used in a pipeline, this is call will bind the input texture of this post
   * process to the output of the previous.
   * @param camera The camera that will be used in the post process. This camera
   * will be used when calling onActivateObservable.
   * @param sourceTexture The source texture to be inspected to get the width
   * and height if not specified in the post process constructor. (default:
   * null)
   * @param forceDepthStencil If true, a depth and stencil buffer will be
   * generated. (default: false)
   * @returns The target texture that was bound to be written to.
   */
  InternalTexturePtr activate(const CameraPtr& camera,
                              const InternalTexturePtr& sourceTexture = nullptr,
                              bool forceDepthStencil                  = false);

  /**
   * @brief If the post process is supported.
   */
  bool isSupported() const;

  /**
   * @brief The aspect ratio of the output texture.
   */
  float aspectRatio() const;

  /**
   * @brief Get a value indicating if the post-process is ready to be used
   * @returns true if the post-process is ready (shader is compiled)
   */
  bool isReady() const;

  /**
   * @brief Binds all textures and uniforms to the shader, this will be run on
   * every pass.
   * @returns the effect corrisponding to this post process. Null if not
   * compiled or not ready.
   */
  Effect* apply();

  void _disposeTextures();

  /**
   * @brief Disposes the post process.
   * @param camera The camera to dispose the post process on.
   */
  virtual void dispose(Camera* camera = nullptr);

protected:
  /**
   * @brief Gets the number of sample textures (default: 1)
   */
  unsigned int get_samples() const;

  /**
   * @brief Sets the number of sample textures (default: 1)
   */
  void set_samples(unsigned int n);

public:
  /**
   * Name of the PostProcess
   */
  std::string name;

  /**
   * Width of the texture to apply the post process on
   */
  int width;

  /**
   * Height of the texture to apply the post process on
   */
  int height;

  /**
   * Internal, reference to the location where this postprocess was output to.
   * (Typically the texture on the next postprocess in the chain)
   * Hidden
   */
  InternalTexturePtr _outputTexture;

  /**
   * Sampling mode used by the shader
   * See https://doc.babylonjs.com/classes/3.1/texture
   */
  unsigned int renderTargetSamplingMode;

  /**
   * Clear color to use when screen clearing
   */
  std::unique_ptr<Color4> clearColor;

  /**
   * If the buffer needs to be cleared before applying the post process.
   * (default: true) Should be set to false if shader will overwrite all
   * previous pixels.
   */
  bool autoClear;

  /**
   * Type of alpha mode to use when performing the post process (default:
   * Engine.ALPHA_DISABLE)
   */
  unsigned int alphaMode;

  /**
   * Sets the setAlphaBlendConstants of the babylon engine
   */
  std::optional<Color4> alphaConstants;

  /**
   * Animations to be used for the post processing
   */
  std::vector<Animation*> animations;

  /**
   * Enable Pixel Perfect mode where texture is not scaled to be power of 2.
   * Can only be used on a single postprocess or on the last one of a chain.
   * (default: false)
   */
  bool enablePixelPerfectMode;

  /**
   * Force the postprocess to be applied without taking in account viewport
   */
  bool forceFullscreenViewport;
  /**
   * Scale mode for the post process (default: Engine.SCALEMODE_FLOOR)
   *
   * | Value | Type                          | Description                     |
   * | ----- | ------------------------------| ------------------------------- |
   * | 1     | SCALEMODE_FLOOR               | [engine.scalemode_floor](*)     |
   * | 2     | SCALEMODE_NEAREST             | [engine.scalemode_nearest](**)  |
   * | 3     | SCALEMODE_CEILING             | [engine.scalemode_ceiling](***) |
   *
   * * http://doc.babylonjs.com/api/classes/babylon.engine#scalemode_floor
   * ** http://doc.babylonjs.com/api/classes/babylon.engine#scalemode_nearest
   * *** http://doc.babylonjs.com/api/classes/babylon.engine#scalemode_ceiling
   */
  unsigned int scaleMode;

  /**
   * Force textures to be a power of two (default: false)
   */
  bool alwaysForcePOT;

  /**
   * Number of sample textures (default: 1)
   */
  Property<PostProcess, unsigned int> samples;

  /**
   * Modify the scale of the post process to be the same as the viewport
   * (default: false)
   */
  bool adaptScaleToCurrentViewport;

  /**
   * Smart array of input and output textures for the post process.
   * Hidden
   */
  std::vector<InternalTexturePtr> _textures;

  /**
   * The index in _textures that corresponds to the output texture.
   * Hidden
   */
  unsigned int _currentRenderTextureInd;

  // Events

  /**
   * An event triggered when the postprocess is activated.
   */
  Observable<Camera> onActivateObservable;

  /**
   * An event triggered when the postprocess changes its size.
   */
  Observable<PostProcess> onSizeChangedObservable;

  /**
   * An event triggered when the postprocess applies its effect.
   */
  Observable<Effect> onApplyObservable;

  /**
   * An event triggered before rendering the postprocess
   */
  Observable<Effect> onBeforeRenderObservable;

  /**
   * An event triggered after rendering the postprocess
   */
  Observable<Effect> onAfterRenderObservable;

protected:
  std::unordered_map<std::string, unsigned int> _indexParameters;

private:
  unsigned int _samples;
  CameraPtr _camera;
  Scene* _scene;
  Engine* _engine;
  float _renderRatio;
  Variant<float, PostProcessOptions> _options;
  bool _reusable;
  unsigned int _textureType;
  Effect* _effect;
  std::vector<std::string> _samplers;
  std::string _fragmentUrl;
  std::string _vertexUrl;
  std::vector<std::string> _parameters;
  Vector2 _scaleRatio;
  PostProcess* _shareOutputWithPostProcess;
  Vector2 _texelSize;
  InternalTexturePtr _forcedOutputTexture;
  // Events
  Observer<Camera>::Ptr _onActivateObserver;
  Observer<PostProcess>::Ptr _onSizeChangedObserver;
  Observer<Effect>::Ptr _onApplyObserver;
  Observer<Effect>::Ptr _onBeforeRenderObserver;
  Observer<Effect>::Ptr _onAfterRenderObserver;

}; // end of class PostProcess

} // end of namespace BABYLON

#endif // end of BABYLON_POSTPROCESS_POST_PROCESS_H
