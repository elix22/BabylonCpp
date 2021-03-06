#ifndef BABYLON_LOADING_PLUGINS_BABYLON_BABYLON_FILE_LOADER_H
#define BABYLON_LOADING_PLUGINS_BABYLON_BABYLON_FILE_LOADER_H

#include <map>

#include <babylon/babylon_api.h>
#include <babylon/loading/iscene_loader_plugin.h>

namespace picojson {
class value;
typedef std::vector<value> array;
typedef std::map<std::string, value> object;
} // end of namespace picojson

namespace BABYLON {

class Material;
using MaterialPtr = std::shared_ptr<Material>;

namespace Json {
typedef picojson::value value;
typedef picojson::array array;
typedef picojson::object object;
} // namespace Json

struct BABYLON_SHARED_EXPORT BabylonFileLoader : public ISceneLoaderPlugin {

  BabylonFileLoader();
  virtual ~BabylonFileLoader();

  MaterialPtr parseMaterialById(const std::string& id,
                                const Json::value& parsedData, Scene* scene,
                                const std::string& rootUrl) const;
  bool isDescendantOf(const Json::value& mesh,
                      const std::vector<std::string>& names,
                      std::vector<std::string>& hierarchyIds) const;
  std::string logOperation(const std::string& operation) const;
  std::string logOperation(const std::string& operation,
                           const Json::value& producer) const;

  bool
  importMesh(const std::vector<std::string>& meshesNames, Scene* scene,
             const std::string& data, const std::string& rootUrl,
             std::vector<AbstractMeshPtr>& meshes,
             std::vector<IParticleSystemPtr>& particleSystems,
             std::vector<SkeletonPtr>& skeletons,
             const std::function<void(const std::string& message,
                                        const std::string& exception)>& onError
             = nullptr) const override;
  bool load(Scene* scene, const std::string& data, const std::string& rootUrl,
            const std::function<void(const std::string& message,
                                       const std::string& exception)>& onError
            = nullptr) const override;

}; // end of struct BabylonFileLoader

} // end of namespace BABYLON

#endif // end of BABYLON_LOADING_PLUGINS_BABYLON_BABYLON_FILE_LOADER_H
