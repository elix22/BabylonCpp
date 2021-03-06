#include <babylon/postprocess/renderpipeline/post_process_render_pipeline_manager_scene_component.h>

#include <babylon/engine/scene.h>
#include <babylon/postprocess/renderpipeline/post_process_render_pipeline_manager.h>

namespace BABYLON {

PostProcessRenderPipelineManagerSceneComponent::
  PostProcessRenderPipelineManagerSceneComponent(Scene* iScene)
{
  scene = iScene;
}

PostProcessRenderPipelineManagerSceneComponent::
  ~PostProcessRenderPipelineManagerSceneComponent()
{
}

void PostProcessRenderPipelineManagerSceneComponent::_register()
{
  scene->_gatherRenderTargetsStage.registerStep(
    SceneComponentConstants::
      STEP_GATHERRENDERTARGETS_POSTPROCESSRENDERPIPELINEMANAGER,
    this, [this](const std::vector<RenderTargetTexturePtr>& renderTargets) {
      _gatherRenderTargets(renderTargets);
    });
}

void PostProcessRenderPipelineManagerSceneComponent::rebuild()
{
  // Nothing to do for this component
}

void PostProcessRenderPipelineManagerSceneComponent::dispose()
{
  if (scene->postProcessRenderPipelineManager()) {
    scene->postProcessRenderPipelineManager()->dispose();
  }
}

void PostProcessRenderPipelineManagerSceneComponent::_gatherRenderTargets(
  const std::vector<RenderTargetTexturePtr>& /*renderTargets*/)
{
  if (scene->postProcessRenderPipelineManager()) {
    scene->postProcessRenderPipelineManager()->update();
  }
}

void PostProcessRenderPipelineManagerSceneComponent::_rebuildGeometry()
{
  if (scene->postProcessRenderPipelineManager()) {
    scene->postProcessRenderPipelineManager()->_rebuild();
  }
}

} // end of namespace BABYLON
