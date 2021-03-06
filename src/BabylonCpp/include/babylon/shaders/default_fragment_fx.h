﻿#ifndef BABYLON_SHADERS_DEFAULT_FRAGMENT_FX_H
#define BABYLON_SHADERS_DEFAULT_FRAGMENT_FX_H

namespace BABYLON {

extern const char* defaultPixelShader;

const char* defaultPixelShader
  = "#include<__decl__defaultFragment>\n"
    "\n"
    "#if defined(BUMP) || !defined(NORMAL)\n"
    "#extension GL_OES_standard_derivatives : enable\n"
    "#endif\n"
    "\n"
    "#define CUSTOM_FRAGMENT_BEGIN\n"
    "\n"
    "#ifdef LOGARITHMICDEPTH\n"
    "#extension GL_EXT_frag_depth : enable\n"
    "#endif\n"
    "\n"
    "// Constants\n"
    "#define RECIPROCAL_PI2 0.15915494\n"
    "\n"
    "uniform vec3 vEyePosition;\n"
    "uniform vec3 vAmbientColor;\n"
    "\n"
    "// Input\n"
    "varying vec3 vPositionW;\n"
    "\n"
    "#ifdef NORMAL\n"
    "varying vec3 vNormalW;\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "varying vec4 vColor;\n"
    "#endif\n"
    "\n"
    "#ifdef MAINUV1\n"
    "  varying vec2 vMainUV1;\n"
    "#endif\n"
    "\n"
    "#ifdef MAINUV2\n"
    "  varying vec2 vMainUV2;\n"
    "#endif\n"
    "\n"
    "// Helper functions\n"
    "#include<helperFunctions>\n"
    "\n"
    "// Lights\n"
    "#include<__decl__lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "#include<lightsFragmentFunctions>\n"
    "#include<shadowsFragmentFunctions>\n"
    "\n"
    "// Samplers\n"
    "#ifdef DIFFUSE\n"
    "  #if DIFFUSEDIRECTUV == 1\n"
    "  #define vDiffuseUV vMainUV1\n"
    "  #elif DIFFUSEDIRECTUV == 2\n"
    "  #define vDiffuseUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vDiffuseUV;\n"
    "  #endif\n"
    "  uniform sampler2D diffuseSampler;\n"
    "#endif\n"
    "\n"
    "#ifdef AMBIENT\n"
    "  #if AMBIENTDIRECTUV == 1\n"
    "  #define vAmbientUV vMainUV1\n"
    "  #elif AMBIENTDIRECTUV == 2\n"
    "  #define vAmbientUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vAmbientUV;\n"
    "  #endif\n"
    "  uniform sampler2D ambientSampler;\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITY  \n"
    "  #if OPACITYDIRECTUV == 1\n"
    "  #define vOpacityUV vMainUV1\n"
    "  #elif OPACITYDIRECTUV == 2\n"
    "  #define vOpacityUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vOpacityUV;\n"
    "  #endif\n"
    "  uniform sampler2D opacitySampler;\n"
    "#endif\n"
    "\n"
    "#ifdef EMISSIVE\n"
    "  #if EMISSIVEDIRECTUV == 1\n"
    "  #define vEmissiveUV vMainUV1\n"
    "  #elif EMISSIVEDIRECTUV == 2\n"
    "  #define vEmissiveUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vEmissiveUV;\n"
    "  #endif\n"
    "  uniform sampler2D emissiveSampler;\n"
    "#endif\n"
    "\n"
    "#ifdef LIGHTMAP\n"
    "  #if LIGHTMAPDIRECTUV == 1\n"
    "  #define vLightmapUV vMainUV1\n"
    "  #elif LIGHTMAPDIRECTUV == 2\n"
    "  #define vLightmapUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vLightmapUV;\n"
    "  #endif\n"
    "  uniform sampler2D lightmapSampler;\n"
    "#endif\n"
    "\n"
    "#ifdef REFRACTION\n"
    "\n"
    "#ifdef REFRACTIONMAP_3D\n"
    "uniform samplerCube refractionCubeSampler;\n"
    "#else\n"
    "uniform sampler2D refraction2DSampler;\n"
    "#endif\n"
    "\n"
    "#endif\n"
    "\n"
    "#if defined(SPECULAR) && defined(SPECULARTERM)\n"
    "  #if SPECULARDIRECTUV == 1\n"
    "  #define vSpecularUV vMainUV1\n"
    "  #elif SPECULARDIRECTUV == 2\n"
    "  #define vSpecularUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vSpecularUV;\n"
    "  #endif\n"
    "  uniform sampler2D specularSampler;\n"
    "#endif\n"
    "\n"
    "#ifdef ALPHATEST\n"
    "  uniform float alphaCutOff;\n"
    "#endif\n"
    "\n"
    "// Fresnel\n"
    "#include<fresnelFunction>\n"
    "\n"
    "// Reflection\n"
    "#ifdef REFLECTION\n"
    "#ifdef REFLECTIONMAP_3D\n"
    "uniform samplerCube reflectionCubeSampler;\n"
    "#else\n"
    "uniform sampler2D reflection2DSampler;\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "varying vec3 vPositionUVW;\n"
    "#else\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "varying vec3 vDirectionW;\n"
    "#endif\n"
    "\n"
    "#endif\n"
    "\n"
    "#include<reflectionFunction>\n"
    "\n"
    "#endif\n"
    "\n"
    "#include<imageProcessingDeclaration>\n"
    "\n"
    "#include<imageProcessingFunctions>\n"
    "\n"
    "#include<bumpFragmentFunctions>\n"
    "#include<clipPlaneFragmentDeclaration>\n"
    "#include<logDepthDeclaration>\n"
    "#include<fogFragmentDeclaration>\n"
    "\n"
    "#define CUSTOM_FRAGMENT_DEFINITIONS\n"
    "\n"
    "void main(void) {\n"
    "\n"
    "#define CUSTOM_FRAGMENT_MAIN_BEGIN\n"
    "\n"
    "#include<clipPlaneFragment>\n"
    "\n"
    "\n"
    "\n"
    "  vec3 viewDirectionW = normalize(vEyePosition - vPositionW);\n"
    "\n"
    "  // Base color\n"
    "  vec4 baseColor = vec4(1., 1., 1., 1.);\n"
    "  vec3 diffuseColor = vDiffuseColor.rgb;\n"
    "  \n"
    "  \n"
    "\n"
    "  // Alpha\n"
    "  float alpha = vDiffuseColor.a;\n"
    "\n"
    "  // Bump\n"
    "#ifdef NORMAL\n"
    "  vec3 normalW = normalize(vNormalW);\n"
    "#else\n"
    "  vec3 normalW = normalize(-cross(dFdx(vPositionW), dFdy(vPositionW)));\n"
    "#endif\n"
    "\n"
    "#include<bumpFragment>\n"
    "\n"
    "#ifdef TWOSIDEDLIGHTING\n"
    "  normalW = gl_FrontFacing ? normalW : -normalW;\n"
    "#endif\n"
    "\n"
    "#ifdef DIFFUSE\n"
    "  baseColor = texture2D(diffuseSampler, vDiffuseUV + uvOffset);\n"
    "\n"
    "  #ifdef ALPHATEST\n"
    "  if (baseColor.a < alphaCutOff)\n"
    "  discard;\n"
    "  #endif\n"
    "\n"
    "  #ifdef ALPHAFROMDIFFUSE\n"
    "  alpha *= baseColor.a;\n"
    "  #endif\n"
    "  \n"
    "  #define CUSTOM_FRAGMENT_UPDATE_ALPHA\n"
    "\n"
    "  baseColor.rgb *= vDiffuseInfos.y;\n"
    "#endif\n"
    "\n"
    "\n"
    "\n"
    "#include<depthPrePass>\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "  baseColor.rgb *= vColor.rgb;\n"
    "#endif\n"
    "\n"
    "#define CUSTOM_FRAGMENT_UPDATE_DIFFUSE\n"
    "\n"
    "  // Ambient color\n"
    "  vec3 baseAmbientColor = vec3(1., 1., 1.);\n"
    "\n"
    "#ifdef AMBIENT\n"
    "  baseAmbientColor = texture2D(ambientSampler, vAmbientUV + uvOffset).rgb * vAmbientInfos.y;\n"
    "#endif\n"
    "\n"
    "#define CUSTOM_FRAGMENT_BEFORE_LIGHTS\n"
    "\n"
    "  // Specular map\n"
    "#ifdef SPECULARTERM\n"
    "  float glossiness = vSpecularColor.a;\n"
    "  vec3 specularColor = vSpecularColor.rgb;\n"
    "\n"
    "#ifdef SPECULAR\n"
    "  vec4 specularMapColor = texture2D(specularSampler, vSpecularUV + uvOffset);\n"
    "  specularColor = specularMapColor.rgb;\n"
    "#ifdef GLOSSINESS\n"
    "  glossiness = glossiness * specularMapColor.a;\n"
    "#endif\n"
    "#endif\n"
    "#else\n"
    "  float glossiness = 0.;\n"
    "#endif\n"
    "\n"
    "  // Lighting\n"
    "  vec3 diffuseBase = vec3(0., 0., 0.);\n"
    "  lightingInfo info;\n"
    "#ifdef SPECULARTERM\n"
    "  vec3 specularBase = vec3(0., 0., 0.);\n"
    "#endif\n"
    "  float shadow = 1.;\n"
    "\n"
    "#ifdef LIGHTMAP\n"
    "  vec3 lightmapColor = texture2D(lightmapSampler, vLightmapUV + uvOffset).rgb * vLightmapInfos.y;\n"
    "#endif\n"
    "\n"
    "#include<lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "  // Refraction\n"
    "  vec3 refractionColor = vec3(0., 0., 0.);\n"
    "\n"
    "#ifdef REFRACTION\n"
    "  vec3 refractionVector = normalize(refract(-viewDirectionW, normalW, vRefractionInfos.y));\n"
    "  #ifdef REFRACTIONMAP_3D\n"
    "  refractionVector.y = refractionVector.y * vRefractionInfos.w;\n"
    "\n"
    "  if (dot(refractionVector, viewDirectionW) < 1.0) {\n"
    "  refractionColor = textureCube(refractionCubeSampler, refractionVector).rgb;\n"
    "  }\n"
    "  #else\n"
    "  vec3 vRefractionUVW = vec3(refractionMatrix * (view * vec4(vPositionW + refractionVector * vRefractionInfos.z, 1.0)));\n"
    "\n"
    "  vec2 refractionCoords = vRefractionUVW.xy / vRefractionUVW.z;\n"
    "\n"
    "  refractionCoords.y = 1.0 - refractionCoords.y;\n"
    "  \n"
    "  refractionColor = texture2D(refraction2DSampler, refractionCoords).rgb;\n"
    "  #endif\n"
    "  #ifdef IS_REFRACTION_LINEAR\n"
    "  refractionColor = toGammaSpace(refractionColor);\n"
    "  #endif\n"
    "  refractionColor *= vRefractionInfos.x;\n"
    "#endif\n"
    "\n"
    "// Reflection\n"
    "vec3 reflectionColor = vec3(0., 0., 0.);\n"
    "\n"
    "#ifdef REFLECTION\n"
    "  vec3 vReflectionUVW = computeReflectionCoords(vec4(vPositionW, 1.0), normalW);\n"
    "\n"
    "  #ifdef REFLECTIONMAP_3D\n"
    "  #ifdef ROUGHNESS\n"
    "  float bias = vReflectionInfos.y;\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  #ifdef SPECULAR\n"
    "  #ifdef GLOSSINESS\n"
    "  bias *= (1.0 - specularMapColor.a);\n"
    "  #endif\n"
    "  #endif\n"
    "  #endif\n"
    "\n"
    "  reflectionColor = textureCube(reflectionCubeSampler, vReflectionUVW, bias).rgb;\n"
    "  #else\n"
    "  reflectionColor = textureCube(reflectionCubeSampler, vReflectionUVW).rgb;\n"
    "  #endif\n"
    "  #else\n"
    "  vec2 coords = vReflectionUVW.xy;\n"
    "\n"
    "  #ifdef REFLECTIONMAP_PROJECTION\n"
    "  coords /= vReflectionUVW.z;\n"
    "  #endif\n"
    "\n"
    "  coords.y = 1.0 - coords.y;\n"
    "  reflectionColor = texture2D(reflection2DSampler, coords).rgb;\n"
    "  #endif\n"
    "  #ifdef IS_REFLECTION_LINEAR\n"
    "  reflectionColor = toGammaSpace(reflectionColor);\n"
    "  #endif\n"
    "  reflectionColor *= vReflectionInfos.x;\n"
    "  #ifdef REFLECTIONFRESNEL\n"
    "  float reflectionFresnelTerm = computeFresnelTerm(viewDirectionW, normalW, reflectionRightColor.a, reflectionLeftColor.a);\n"
    "\n"
    "  #ifdef REFLECTIONFRESNELFROMSPECULAR\n"
    "  #ifdef SPECULARTERM\n"
    "  reflectionColor *= specularColor.rgb * (1.0 - reflectionFresnelTerm) + reflectionFresnelTerm * reflectionRightColor.rgb;\n"
    "  #else\n"
    "  reflectionColor *= reflectionLeftColor.rgb * (1.0 - reflectionFresnelTerm) + reflectionFresnelTerm * reflectionRightColor.rgb;\n"
    "  #endif\n"
    "  #else\n"
    "  reflectionColor *= reflectionLeftColor.rgb * (1.0 - reflectionFresnelTerm) + reflectionFresnelTerm * reflectionRightColor.rgb;\n"
    "  #endif\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#ifdef REFRACTIONFRESNEL\n"
    "  float refractionFresnelTerm = computeFresnelTerm(viewDirectionW, normalW, refractionRightColor.a, refractionLeftColor.a);\n"
    "\n"
    "  refractionColor *= refractionLeftColor.rgb * (1.0 - refractionFresnelTerm) + refractionFresnelTerm * refractionRightColor.rgb;\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITY\n"
    "  vec4 opacityMap = texture2D(opacitySampler, vOpacityUV + uvOffset);\n"
    "\n"
    "#ifdef OPACITYRGB\n"
    "  opacityMap.rgb = opacityMap.rgb * vec3(0.3, 0.59, 0.11);\n"
    "  alpha *= (opacityMap.x + opacityMap.y + opacityMap.z)* vOpacityInfos.y;\n"
    "#else\n"
    "  alpha *= opacityMap.a * vOpacityInfos.y;\n"
    "#endif\n"
    "\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXALPHA\n"
    "  alpha *= vColor.a;\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITYFRESNEL\n"
    "  float opacityFresnelTerm = computeFresnelTerm(viewDirectionW, normalW, opacityParts.z, opacityParts.w);\n"
    "\n"
    "  alpha += opacityParts.x * (1.0 - opacityFresnelTerm) + opacityFresnelTerm * opacityParts.y;\n"
    "#endif\n"
    "\n"
    "  // Emissive\n"
    "  vec3 emissiveColor = vEmissiveColor;\n"
    "#ifdef EMISSIVE\n"
    "  emissiveColor += texture2D(emissiveSampler, vEmissiveUV + uvOffset).rgb * vEmissiveInfos.y;\n"
    "#endif\n"
    "\n"
    "#ifdef EMISSIVEFRESNEL\n"
    "  float emissiveFresnelTerm = computeFresnelTerm(viewDirectionW, normalW, emissiveRightColor.a, emissiveLeftColor.a);\n"
    "\n"
    "  emissiveColor *= emissiveLeftColor.rgb * (1.0 - emissiveFresnelTerm) + emissiveFresnelTerm * emissiveRightColor.rgb;\n"
    "#endif\n"
    "\n"
    "  // Fresnel\n"
    "#ifdef DIFFUSEFRESNEL\n"
    "  float diffuseFresnelTerm = computeFresnelTerm(viewDirectionW, normalW, diffuseRightColor.a, diffuseLeftColor.a);\n"
    "\n"
    "  diffuseBase *= diffuseLeftColor.rgb * (1.0 - diffuseFresnelTerm) + diffuseFresnelTerm * diffuseRightColor.rgb;\n"
    "#endif\n"
    "\n"
    "  // Composition\n"
    "#ifdef EMISSIVEASILLUMINATION\n"
    "  vec3 finalDiffuse = clamp(diffuseBase * diffuseColor + vAmbientColor, 0.0, 1.0) * baseColor.rgb;\n"
    "#else\n"
    "#ifdef LINKEMISSIVEWITHDIFFUSE\n"
    "  vec3 finalDiffuse = clamp((diffuseBase + emissiveColor) * diffuseColor + vAmbientColor, 0.0, 1.0) * baseColor.rgb;\n"
    "#else\n"
    "  vec3 finalDiffuse = clamp(diffuseBase * diffuseColor + emissiveColor + vAmbientColor, 0.0, 1.0) * baseColor.rgb;\n"
    "#endif\n"
    "#endif\n"
    "\n"
    "#ifdef SPECULARTERM\n"
    "  vec3 finalSpecular = specularBase * specularColor;\n"
    "  #ifdef SPECULAROVERALPHA\n"
    "  alpha = clamp(alpha + dot(finalSpecular, vec3(0.3, 0.59, 0.11)), 0., 1.);\n"
    "  #endif\n"
    "#else\n"
    "  vec3 finalSpecular = vec3(0.0);\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONOVERALPHA\n"
    "  alpha = clamp(alpha + dot(reflectionColor, vec3(0.3, 0.59, 0.11)), 0., 1.);\n"
    "#endif\n"
    "\n"
    "  // Composition\n"
    "#ifdef EMISSIVEASILLUMINATION\n"
    "  vec4 color = vec4(clamp(finalDiffuse * baseAmbientColor + finalSpecular + reflectionColor + emissiveColor + refractionColor, 0.0, 1.0), alpha);\n"
    "#else\n"
    "  vec4 color = vec4(finalDiffuse * baseAmbientColor + finalSpecular + reflectionColor + refractionColor, alpha);\n"
    "#endif\n"
    "\n"
    "//Old lightmap calculation method\n"
    "#ifdef LIGHTMAP\n"
    "  #ifndef LIGHTMAPEXCLUDED\n"
    "  #ifdef USELIGHTMAPASSHADOWMAP\n"
    "  color.rgb *= lightmapColor;\n"
    "  #else\n"
    "  color.rgb += lightmapColor;\n"
    "  #endif\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#define CUSTOM_FRAGMENT_BEFORE_FOG\n"
    "color.rgb = max(color.rgb, 0.);\n"
    "#include<logDepthFragment>\n"
    "#include<fogFragment>\n"
    "\n"
    "// Apply image processing if relevant. As this applies in linear space, \n"
    "// We first move from gamma to linear.\n"
    "#ifdef IMAGEPROCESSINGPOSTPROCESS\n"
    "  color.rgb = toLinearSpace(color.rgb);\n"
    "#else\n"
    "  #ifdef IMAGEPROCESSING\n"
    "  color.rgb = toLinearSpace(color.rgb);\n"
    "  color = applyImageProcessing(color);\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#ifdef PREMULTIPLYALPHA\n"
    "  // Convert to associative (premultiplied) format if needed.\n"
    "  color.rgb *= color.a;\n"
    "#endif\n"
    "\n"
    "#define CUSTOM_FRAGMENT_BEFORE_FRAGCOLOR\n"
    "  gl_FragColor = color;\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_DEFAULT_FRAGMENT_FX_H
