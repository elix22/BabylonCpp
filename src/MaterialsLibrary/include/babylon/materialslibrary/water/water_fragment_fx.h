﻿#ifndef BABYLON_MATERIALS_LIBRARY_WATER_WATER_FRAGMENT_FX_H
#define BABYLON_MATERIALS_LIBRARY_WATER_WATER_FRAGMENT_FX_H

namespace BABYLON {

extern const char* waterPixelShader;

const char* waterPixelShader
  = "#ifdef LOGARITHMICDEPTH\n"
    "#extension GL_EXT_frag_depth : enable\n"
    "#endif\n"
    "\n"
    "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "// Constants\n"
    "uniform vec3 vEyePosition;\n"
    "uniform vec4 vDiffuseColor;\n"
    "\n"
    "#ifdef SPECULARTERM\n"
    "uniform vec4 vSpecularColor;\n"
    "#endif\n"
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
    "#ifdef BUMP\n"
    "varying vec2 vNormalUV;\n"
    "varying vec2 vNormalUV2;\n"
    "uniform sampler2D normalSampler;\n"
    "uniform vec2 vNormalInfos;\n"
    "#endif\n"
    "\n"
    "uniform sampler2D refractionSampler;\n"
    "uniform sampler2D reflectionSampler;\n"
    "\n"
    "// Water uniforms\n"
    "const float LOG2 = 1.442695;\n"
    "\n"
    "uniform vec3 cameraPosition;\n"
    "\n"
    "uniform vec4 waterColor;\n"
    "uniform float colorBlendFactor;\n"
    "\n"
    "uniform vec4 waterColor2;\n"
    "uniform float colorBlendFactor2;\n"
    "\n"
    "uniform float bumpHeight;\n"
    "\n"
    "uniform float time;\n"
    "\n"
    "// Water varyings\n"
    "varying vec3 vRefractionMapTexCoord;\n"
    "varying vec3 vReflectionMapTexCoord;\n"
    "varying vec3 vPosition;\n"
    "\n"
    "#include<clipPlaneFragmentDeclaration>\n"
    "#include<logDepthDeclaration>\n"
    "\n"
    "// Fog\n"
    "#include<fogFragmentDeclaration>\n"
    "\n"
    "void main(void) {\n"
    "  // Clip plane\n"
    "  #include<clipPlaneFragment>\n"
    "\n"
    "  vec3 viewDirectionW = normalize(vEyePosition - vPositionW);\n"
    "\n"
    "  // Base color\n"
    "  vec4 baseColor = vec4(1., 1., 1., 1.);\n"
    "  vec3 diffuseColor = vDiffuseColor.rgb;\n"
    "\n"
    "  // Alpha\n"
    "  float alpha = vDiffuseColor.a;\n"
    "\n"
    "#ifdef BUMP\n"
    "  #ifdef BUMPSUPERIMPOSE\n"
    "  baseColor = 0.6 * texture2D(normalSampler, vNormalUV) + 0.4 * texture2D(normalSampler,vec2(vNormalUV2.x,vNormalUV2.y));\n"
    "  #else\n"
    "  baseColor = texture2D(normalSampler, vNormalUV);\n"
    "  #endif\n"
    "  vec3 bumpColor = baseColor.rgb;\n"
    "\n"
    "#ifdef ALPHATEST\n"
    "  if (baseColor.a < 0.4)\n"
    "  discard;\n"
    "#endif\n"
    "\n"
    "  baseColor.rgb *= vNormalInfos.y;\n"
    "#else\n"
    "  vec3 bumpColor = vec3(1.0);\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "  baseColor.rgb *= vColor.rgb;\n"
    "#endif\n"
    "\n"
    "  // Bump\n"
    "#ifdef NORMAL\n"
    "  vec2 perturbation = bumpHeight * (baseColor.rg - 0.5);\n"
    "  #ifdef BUMPAFFECTSREFLECTION\n"
    "  vec3 normalW = normalize(vNormalW + vec3(perturbation.x*8.0,0.0,perturbation.y*8.0));\n"
    "  if (normalW.y<0.0) {\n"
    "  normalW.y = -normalW.y;\n"
    "  }\n"
    "  #else\n"
    "  vec3 normalW = normalize(vNormalW);\n"
    "  #endif\n"
    "#else\n"
    "  vec3 normalW = vec3(1.0, 1.0, 1.0);\n"
    "  vec2 perturbation = bumpHeight * (vec2(1.0, 1.0) - 0.5);\n"
    "#endif\n"
    "\n"
    "#ifdef FRESNELSEPARATE\n"
    "  #ifdef REFLECTION\n"
    "  // Water\n"
    "  vec3 eyeVector = normalize(vEyePosition - vPosition);\n"
    "\n"
    "  vec2 projectedRefractionTexCoords = clamp(vRefractionMapTexCoord.xy / vRefractionMapTexCoord.z + perturbation*0.5, 0.0, 1.0);\n"
    "  vec4 refractiveColor = texture2D(refractionSampler, projectedRefractionTexCoords);\n"
    "\n"
    "  vec2 projectedReflectionTexCoords = clamp(vec2(\n"
    "  vReflectionMapTexCoord.x / vReflectionMapTexCoord.z + perturbation.x * 0.3,\n"
    "  vReflectionMapTexCoord.y / vReflectionMapTexCoord.z + perturbation.y\n"
    "  ),0.0, 1.0);\n"
    "\n"
    "  vec4 reflectiveColor = texture2D(reflectionSampler, projectedReflectionTexCoords);\n"
    "\n"
    "  vec3 upVector = vec3(0.0, 1.0, 0.0);\n"
    "\n"
    "  float fresnelTerm = clamp(abs(pow(dot(eyeVector, upVector),3.0)),0.05,0.65);\n"
    "  float IfresnelTerm = 1.0 - fresnelTerm;\n"
    "\n"
    "  refractiveColor = colorBlendFactor*waterColor + (1.0-colorBlendFactor)*refractiveColor;\n"
    "  reflectiveColor = IfresnelTerm*colorBlendFactor2*waterColor + (1.0-colorBlendFactor2*IfresnelTerm)*reflectiveColor;\n"
    "\n"
    "  vec4 combinedColor = refractiveColor * fresnelTerm + reflectiveColor * IfresnelTerm;\n"
    "  baseColor = combinedColor;\n"
    "  #endif\n"
    "\n"
    "  // Lighting\n"
    "  vec3 diffuseBase = vec3(0., 0., 0.);\n"
    "  lightingInfo info;\n"
    "  float shadow = 1.;\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  float glossiness = vSpecularColor.a;\n"
    "  vec3 specularBase = vec3(0., 0., 0.);\n"
    "  vec3 specularColor = vSpecularColor.rgb;\n"
    "  #else\n"
    "  float glossiness = 0.;\n"
    "  #endif\n"
    "\n"
    "  #include<lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "  vec3 finalDiffuse = clamp(baseColor.rgb, 0.0, 1.0);\n"
    "\n"
    "  #ifdef VERTEXALPHA\n"
    "  alpha *= vColor.a;\n"
    "  #endif\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  vec3 finalSpecular = specularBase * specularColor;\n"
    "  #else\n"
    "  vec3 finalSpecular = vec3(0.0);\n"
    "  #endif\n"
    "\n"
    "\n"
    "#else // !FRESNELSEPARATE\n"
    "  #ifdef REFLECTION\n"
    "  // Water\n"
    "  vec3 eyeVector = normalize(vEyePosition - vPosition);\n"
    "\n"
    "  vec2 projectedRefractionTexCoords = clamp(vRefractionMapTexCoord.xy / vRefractionMapTexCoord.z + perturbation, 0.0, 1.0);\n"
    "  vec4 refractiveColor = texture2D(refractionSampler, projectedRefractionTexCoords);\n"
    "\n"
    "  vec2 projectedReflectionTexCoords = clamp(vReflectionMapTexCoord.xy / vReflectionMapTexCoord.z + perturbation, 0.0, 1.0);\n"
    "  vec4 reflectiveColor = texture2D(reflectionSampler, projectedReflectionTexCoords);\n"
    "\n"
    "  vec3 upVector = vec3(0.0, 1.0, 0.0);\n"
    "\n"
    "  float fresnelTerm = max(dot(eyeVector, upVector), 0.0);\n"
    "\n"
    "  vec4 combinedColor = refractiveColor * fresnelTerm + reflectiveColor * (1.0 - fresnelTerm);\n"
    "\n"
    "  baseColor = colorBlendFactor * waterColor + (1.0 - colorBlendFactor) * combinedColor;\n"
    "  #endif\n"
    "\n"
    "  // Lighting\n"
    "  vec3 diffuseBase = vec3(0., 0., 0.);\n"
    "  lightingInfo info;\n"
    "  float shadow = 1.;\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  float glossiness = vSpecularColor.a;\n"
    "  vec3 specularBase = vec3(0., 0., 0.);\n"
    "  vec3 specularColor = vSpecularColor.rgb;\n"
    "  #else\n"
    "  float glossiness = 0.;\n"
    "  #endif\n"
    "\n"
    "  #include<lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "  vec3 finalDiffuse = clamp(baseColor.rgb, 0.0, 1.0);\n"
    "\n"
    "\n"
    "  #ifdef VERTEXALPHA\n"
    "  alpha *= vColor.a;\n"
    "  #endif\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  vec3 finalSpecular = specularBase * specularColor;\n"
    "  #else\n"
    "  vec3 finalSpecular = vec3(0.0);\n"
    "  #endif\n"
    "\n"
    "#endif\n"
    "\n"
    "// Composition\n"
    "vec4 color = vec4(finalDiffuse + finalSpecular, alpha);\n"
    "\n"
    "#include<logDepthFragment>\n"
    "#include<fogFragment>\n"
    "  \n"
    "  gl_FragColor = color;\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_MATERIALS_LIBRARY_WATER_WATER_FRAGMENT_FX_H
