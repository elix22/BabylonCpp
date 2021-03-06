#include <babylon/postprocess/highlights_post_process.h>

#include <babylon/core/variant.h>

namespace BABYLON {

HighlightsPostProcess::HighlightsPostProcess(const std::string& iName,
                                             float ratio,
                                             const CameraPtr& camera,
                                             unsigned int samplingMode,
                                             Engine* engine, bool reusable,
                                             unsigned int textureType)
    : PostProcess{iName,
                  "highlights",
                  {},
                  {},
                  ToVariant<float, PostProcessOptions>(ratio),
                  camera,
                  samplingMode,
                  engine,
                  reusable,
                  nullptr,
                  textureType}
{
}

HighlightsPostProcess::~HighlightsPostProcess()
{
}

} // end of namespace BABYLON
