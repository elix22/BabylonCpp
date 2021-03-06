#include <babylon/inspector/adapters/material_adapter.h>

#include <babylon/materials/material.h>
#include <babylon/materials/standard_material.h>

#include <babylon/inspector/properties/properties_view.h>

namespace BABYLON {

MaterialAdapter::MaterialAdapter(const MaterialPtr& material)
    : _material{material}, _standardMaterial{nullptr}, _properties{nullptr}
{
  // Cast material
  if (_material->type() == IReflect::Type::STANDARDMATERIAL) {
    _standardMaterial = std::static_pointer_cast<StandardMaterial>(_material);
  }
  // Build properties view
  _properties = std::make_unique<PropertiesView>();
  _buildPropertiesView();
}

MaterialAdapter::~MaterialAdapter()
{
}

std::string MaterialAdapter::id()
{
  return _material->name;
}

std::string MaterialAdapter::type()
{
  return _material->getClassName();
}

std::unique_ptr<PropertiesView>& MaterialAdapter::getProperties()
{
  return _properties;
}

std::vector<std::unique_ptr<AbstractTreeTool>>& MaterialAdapter::getTools()
{
  return _tools;
}

void MaterialAdapter::_buildPropertiesView()
{
  // -- Add properties -- //
  auto& view = *_properties;
  /** Material properties **/
  // - alpha
  view.addFloatProperty("alpha", [&]() -> float { return _material->alpha(); },
                        [&](const float& value) { _material->alpha = value; });
  // - alphaMode
  view.addSizeTProperty("alphaMode",
                        [&]() -> size_t { return _material->alphaMode(); },
                        [&](const size_t& value) {
                          _material->alphaMode = static_cast<unsigned>(value);
                        });
  // - backFaceCulling
  view.addBoolProperty(
    "backFaceCulling", [&]() -> bool { return _material->backFaceCulling(); },
    [&](const bool& value) { _material->backFaceCulling = value; });
  // - id
  view.addStringProperty(
    "id", [&]() -> std::string& { return _material->id; },
    [&](const std::string& value) { _material->id = value; });
  // - name
  view.addStringProperty(
    "name", [&]() -> std::string& { return _material->name; },
    [&](const std::string& value) { _material->name = value; });
  // - checkReadyOnEveryCall
  view.addBoolProperty(
    "checkReadyOnEveryCall",
    [&]() -> bool { return _material->checkReadyOnEveryCall; },
    [&](const bool& value) { _material->checkReadyOnEveryCall = value; });
  // - checkReadyOnlyOnce
  view.addBoolProperty(
    "checkReadyOnlyOnce",
    [&]() -> bool { return _material->checkReadyOnlyOnce; },
    [&](const bool& value) { _material->checkReadyOnlyOnce = value; });
  // - disableDepthWrite
  view.addBoolProperty(
    "disableDepthWrite", [&]() -> bool { return _material->disableDepthWrite; },
    [&](const bool& value) { _material->disableDepthWrite = value; });
  // - doNotSerialize
  view.addBoolProperty(
    "doNotSerialize", [&]() -> bool { return _material->doNotSerialize; },
    [&](const bool& value) { _material->doNotSerialize = value; });
  // - fillMode
  view.addSizeTProperty("fillMode",
                        [&]() -> size_t { return _material->fillMode(); },
                        [&](const size_t& value) {
                          _material->fillMode = static_cast<unsigned>(value);
                        });
  // - fogEnabled
  view.addBoolProperty(
    "fogEnabled", [&]() -> bool { return _material->fogEnabled(); },
    [&](const bool& value) { _material->fogEnabled = value; });
  // - forceDepthWrite
  view.addBoolProperty(
    "forceDepthWrite", [&]() -> bool { return _material->forceDepthWrite; },
    [&](const bool& value) { _material->forceDepthWrite = value; });
  // - isFrozen
  view.addBoolProperty("isFrozen",
                       [&]() -> bool { return _material->isFrozen(); },
                       [&](const bool& /*value*/) {});
  // - needDepthPrePass
  view.addBoolProperty(
    "needDepthPrePass", [&]() -> bool { return _material->needDepthPrePass(); },
    [&](const bool& value) { _material->needDepthPrePass = value; });
  // - pointsCloud
  view.addBoolProperty(
    "pointsCloud", [&]() -> bool { return _material->pointsCloud(); },
    [&](const bool& value) { _material->pointsCloud = value; });
  // - pointSize
  view.addFloatProperty(
    "pointSize", [&]() -> float { return _material->pointSize; },
    [&](const float& value) { _material->pointSize = value; });
  // - separateCullingPass
  view.addBoolProperty(
    "separateCullingPass",
    [&]() -> bool { return _material->separateCullingPass; },
    [&](const bool& value) { _material->separateCullingPass = value; });
  // - sideOrientation
  view.addIntProperty(
    "sideOrientation", [&]() -> int { return _material->sideOrientation; },
    [&](const int& value) { _material->sideOrientation = value; });
  // - storeEffectOnSubMeshes
  view.addBoolProperty(
    "storeEffectOnSubMeshes",
    [&]() -> bool { return _material->storeEffectOnSubMeshes; },
    [&](const bool& value) { _material->storeEffectOnSubMeshes = value; });
  // - wireframe
  view.addBoolProperty(
    "wireframe", [&]() -> bool { return _material->wireframe(); },
    [&](const bool& value) { _material->wireframe = value; });
  // - zOffset
  view.addFloatProperty(
    "zOffset", [&]() -> float { return _material->zOffset; },
    [&](const float& value) { _material->zOffset = value; });
  /** StandardMaterial properties **/
  if (_standardMaterial) {
    // - alphaCutOff
    view.addFloatProperty(
      "alphaCutOff", [&]() -> float { return _standardMaterial->alphaCutOff; },
      [&](const float& value) { _standardMaterial->alphaCutOff = value; });
    // - ambientColor
    view.addColor3Property(
      "ambientColor",
      [&]() -> Color3 const& { return _standardMaterial->ambientColor; },
      [&](const Color3& value) { _standardMaterial->ambientColor = value; });
    // - cameraColorCurvesEnabled
    view.addBoolProperty(
      "cameraColorCurvesEnabled",
      [&]() -> bool { return _standardMaterial->cameraColorCurvesEnabled(); },
      [&](const bool& value) {
        _standardMaterial->cameraColorCurvesEnabled = value;
      });
    // - cameraColorGradingEnabled
    view.addBoolProperty(
      "cameraColorGradingEnabled",
      [&]() -> bool { return _standardMaterial->cameraColorGradingEnabled(); },
      [&](const bool& value) {
        _standardMaterial->cameraColorGradingEnabled = value;
      });
    // - cameraContrast
    view.addFloatProperty(
      "cameraContrast",
      [&]() -> float { return _standardMaterial->cameraContrast(); },
      [&](const float& value) { _standardMaterial->cameraContrast = value; });
    // - cameraExposure
    view.addFloatProperty(
      "cameraExposure",
      [&]() -> float { return _standardMaterial->cameraExposure(); },
      [&](const float& value) { _standardMaterial->cameraExposure = value; });
    // - cameraToneMappingEnabled
    view.addBoolProperty(
      "cameraToneMappingEnabled",
      [&]() -> bool { return _standardMaterial->cameraToneMappingEnabled(); },
      [&](const bool& value) {
        _standardMaterial->cameraToneMappingEnabled = value;
      });
    // - cameraColorCurves
    if (_standardMaterial->cameraColorCurves()) {
      view.addColorCurvesProperty("cameraColorCurves", [&]() -> ColorCurves& {
        return *_standardMaterial->cameraColorCurves();
      });
    }
    // - diffuseColor
    view.addColor3Property(
      "diffuseColor",
      [&]() -> Color3 const& { return _standardMaterial->diffuseColor; },
      [&](const Color3& value) { _standardMaterial->diffuseColor = value; });
    // - disableLighting
    view.addBoolProperty(
      "disableLighting",
      [&]() -> bool { return _standardMaterial->disableLighting(); },
      [&](const bool& value) { _standardMaterial->disableLighting = value; });
    // - emissiveColor
    view.addColor3Property(
      "emissiveColor",
      [&]() -> Color3 const& { return _standardMaterial->emissiveColor; },
      [&](const Color3& value) { _standardMaterial->emissiveColor = value; });
    // - imageProcessingConfiguration
    if (_standardMaterial->imageProcessingConfiguration()) {
      view.addImageProcessingConfigurationProperty(
        "imageProcessingConfiguration", [&]() -> ImageProcessingConfiguration& {
          return *_standardMaterial->imageProcessingConfiguration();
        });
    }
    // - indexOfRefraction
    view.addFloatProperty(
      "indexOfRefraction",
      [&]() -> float { return _standardMaterial->indexOfRefraction; },
      [&](const float& value) {
        _standardMaterial->indexOfRefraction = value;
      });
    // - invertNormalMapX
    view.addBoolProperty(
      "invertNormalMapX",
      [&]() -> bool { return _standardMaterial->invertNormalMapX(); },
      [&](const bool& value) { _standardMaterial->invertNormalMapX = value; });
    // - invertNormalMapY
    view.addBoolProperty(
      "invertNormalMapY",
      [&]() -> bool { return _standardMaterial->invertNormalMapY(); },
      [&](const bool& value) { _standardMaterial->invertNormalMapY = value; });
    // - invertRefractionY
    view.addBoolProperty(
      "invertRefractionY",
      [&]() -> bool { return _standardMaterial->invertRefractionY; },
      [&](const bool& value) { _standardMaterial->invertRefractionY = value; });
    // - linkEmissiveWithDiffuse
    view.addBoolProperty(
      "linkEmissiveWithDiffuse",
      [&]() -> bool { return _standardMaterial->linkEmissiveWithDiffuse(); },
      [&](const bool& value) {
        _standardMaterial->linkEmissiveWithDiffuse = value;
      });
    // - maxSimultaneousLights
    view.addSizeTProperty(
      "maxSimultaneousLights",
      [&]() -> size_t { return _standardMaterial->maxSimultaneousLights(); },
      [&](const size_t& value) {
        _standardMaterial->maxSimultaneousLights = static_cast<unsigned>(value);
      });
    // - parallaxScaleBias
    view.addFloatProperty(
      "parallaxScaleBias",
      [&]() -> float { return _standardMaterial->parallaxScaleBias; },
      [&](const float& value) {
        _standardMaterial->parallaxScaleBias = value;
      });
    // - roughness
    view.addFloatProperty(
      "roughness", [&]() -> float { return _standardMaterial->roughness(); },
      [&](const float& value) { _standardMaterial->roughness = value; });
    // - specularColor
    view.addColor3Property(
      "specularColor",
      [&]() -> Color3 const& { return _standardMaterial->specularColor; },
      [&](const Color3& value) { _standardMaterial->specularColor = value; });
    // - specularPower
    view.addFloatProperty(
      "specularPower",
      [&]() -> float { return _standardMaterial->specularPower; },
      [&](const float& value) { _standardMaterial->specularPower = value; });
    // - twoSidedLighting
    view.addBoolProperty(
      "twoSidedLighting",
      [&]() -> bool { return _standardMaterial->twoSidedLighting(); },
      [&](const bool& value) { _standardMaterial->twoSidedLighting = value; });
    // - useAlphaFromDiffuseTexture
    view.addBoolProperty(
      "useAlphaFromDiffuseTexture",
      [&]() -> bool { return _standardMaterial->useAlphaFromDiffuseTexture(); },
      [&](const bool& value) {
        _standardMaterial->useAlphaFromDiffuseTexture = value;
      });
    // - useEmissiveAsIllumination
    view.addBoolProperty(
      "useEmissiveAsIllumination",
      [&]() -> bool { return _standardMaterial->useEmissiveAsIllumination(); },
      [&](const bool& value) {
        _standardMaterial->useEmissiveAsIllumination = value;
      });
    // - useGlossinessFromSpecularMapAlpha
    view.addBoolProperty(
      "useGlossinessFromSpecularMapAlpha",
      [&]() -> bool {
        return _standardMaterial->useGlossinessFromSpecularMapAlpha();
      },
      [&](const bool& value) {
        _standardMaterial->useGlossinessFromSpecularMapAlpha = value;
      });
    // - useLightmapAsShadowmap
    view.addBoolProperty(
      "useLightmapAsShadowmap",
      [&]() -> bool { return _standardMaterial->useLightmapAsShadowmap(); },
      [&](const bool& value) {
        _standardMaterial->useLightmapAsShadowmap = value;
      });
    // - useLogarithmicDepth
    view.addBoolProperty(
      "useLogarithmicDepth",
      [&]() -> bool { return _standardMaterial->useLogarithmicDepth(); },
      [&](const bool& value) {
        _standardMaterial->useLogarithmicDepth = value;
      });
    // - useObjectSpaceNormalMap
    view.addBoolProperty(
      "useObjectSpaceNormalMap",
      [&]() -> bool { return _standardMaterial->useObjectSpaceNormalMap(); },
      [&](const bool& value) {
        _standardMaterial->useObjectSpaceNormalMap = value;
      });
    // - useParallax
    view.addBoolProperty(
      "useParallax", [&]() -> bool { return _standardMaterial->useParallax(); },
      [&](const bool& value) { _standardMaterial->useParallax = value; });
    // - useParallaxOcclusion
    view.addBoolProperty(
      "useParallaxOcclusion",
      [&]() -> bool { return _standardMaterial->useParallaxOcclusion(); },
      [&](const bool& value) {
        _standardMaterial->useParallaxOcclusion = value;
      });
    // - useReflectionFresnelFromSpecular
    view.addBoolProperty(
      "useReflectionFresnelFromSpecular",
      [&]() -> bool {
        return _standardMaterial->useReflectionFresnelFromSpecular();
      },
      [&](const bool& value) {
        _standardMaterial->useReflectionFresnelFromSpecular = value;
      });
    // - useReflectionOverAlpha
    view.addBoolProperty(
      "useReflectionOverAlpha",
      [&]() -> bool { return _standardMaterial->useReflectionOverAlpha(); },
      [&](const bool& value) {
        _standardMaterial->useReflectionOverAlpha = value;
      });
    // - useSpecularOverAlpha
    view.addBoolProperty(
      "useSpecularOverAlpha",
      [&]() -> bool { return _standardMaterial->useSpecularOverAlpha(); },
      [&](const bool& value) {
        _standardMaterial->useSpecularOverAlpha = value;
      });
  }
  // -- Sort properties by property name -- //
  view.sortPropertiesByName();
}

} // end of namespace BABYLON
