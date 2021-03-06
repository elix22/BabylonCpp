#ifndef BABYLON_POSTPROCESS_RENDERPIPELINE_POST_PROCESS_RENDER_EFFECT_H
#define BABYLON_POSTPROCESS_RENDERPIPELINE_POST_PROCESS_RENDER_EFFECT_H

#include <functional>
#include <memory>

#include <babylon/babylon_api.h>
#include <babylon/babylon_common.h>

namespace BABYLON {

class Camera;
class Engine;
class PostProcess;
using CameraPtr = std::shared_ptr<Camera>;

/**
 * @brief This represents a set of one or more post processes in Babylon.
 * A post process can be used to apply a shader to a texture after it is
 * rendered. Example:
 * https://doc.babylonjs.com/how_to/how_to_use_postprocessrenderpipeline
 */
class BABYLON_SHARED_EXPORT PostProcessRenderEffect {

public:
  /**
   * @brief Instantiates a post process render effect.
   * A post process can be used to apply a shader to a texture after it is
   * rendered.
   * @param engine The engine the effect is tied to
   * @param name The name of the effect
   * @param getPostProcesses A function that returns a set of post processes
   * which the effect will run in order to be run.
   * @param singleInstance False if this post process can be run on multiple
   * cameras. (default: true)
   */
  PostProcessRenderEffect(
    Engine* engine, const std::string& name,
    const std::function<std::vector<PostProcess*>()>& getPostProcesses,
    bool singleInstance = true);
  ~PostProcessRenderEffect();

  /**
   * @brief Updates the current state of the effect.
   * Hidden
   */
  void _update();

  /**
   * @brief Attaches the effect on cameras.
   * @param cameras The camera to attach to.
   * Hidden
   */
  void _attachCameras(const std::vector<CameraPtr>& cameras);

  /**
   * @brief Detatches the effect on cameras.
   * @param cameras The camera to detatch from.
   * Hidden
   */
  void _detachCameras(const std::vector<CameraPtr>& cameras);

  /**
   * @brief Enables the effect on given cameras.
   * @param cameras The camera to enable.
   * Hidden
   */
  void _enable(const std::vector<CameraPtr>& cameras);

  /**
   * @brief Disables the effect on the given cameras.
   * @param cameras The camera to disable.
   * Hidden
   */
  void _disable(const std::vector<CameraPtr>& cameras);

  /**
   * @brief Gets a list of the post processes contained in the effect.
   * @param camera The camera to get the post processes on.
   * @returns The list of the post processes in the effect.
   */
  std::vector<PostProcess*> getPostProcesses(Camera* camera = nullptr);

protected:
  /**
   * @brief Checks if all the post processes in the effect are supported.
   */
  bool get_isSupported() const;

public:
  /**
   * Name of the effect
   * Hidden
   */
  std::string _name;

  Engine* _engine;

  /**
   * Whether all the post processes in the effect are supported
   */
  ReadOnlyProperty<PostProcessRenderEffect, bool> isSupported;

private:
  std::unordered_map<std::string, std::vector<PostProcess*>> _postProcesses;
  std::function<std::vector<PostProcess*>()> _getPostProcesses;
  bool _singleInstance;
  std::unordered_map<std::string, CameraPtr> _cameras;
  std::unordered_map<std::string, IndicesArray> _indicesForCamera;

}; // end of class PostProcessRenderEffect

} // end of namespace BABYLON

#endif // end of BABYLON_POSTPROCESS_RENDERPIPELINE_POST_PROCESS_RENDER_EFFECT_H
