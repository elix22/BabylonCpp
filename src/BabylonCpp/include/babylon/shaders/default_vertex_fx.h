﻿#ifndef BABYLON_SHADERS_DEFAULT_VERTEX_FX_H
#define BABYLON_SHADERS_DEFAULT_VERTEX_FX_H

namespace BABYLON {

extern const char* defaultVertexShader;

const char* defaultVertexShader
  = "#include<__decl__defaultVertex>\n"
    "\n"
    "// Attributes\n"
    "attribute vec3 position;\n"
    "#ifdef NORMAL\n"
    "attribute vec3 normal;\n"
    "#endif\n"
    "#ifdef TANGENT\n"
    "attribute vec4 tangent;\n"
    "#endif\n"
    "#ifdef UV1\n"
    "attribute vec2 uv;\n"
    "#endif\n"
    "#ifdef UV2\n"
    "attribute vec2 uv2;\n"
    "#endif\n"
    "#ifdef VERTEXCOLOR\n"
    "attribute vec4 color;\n"
    "#endif\n"
    "\n"
    "#include<bonesDeclaration>\n"
    "\n"
    "// Uniforms\n"
    "#include<instancesDeclaration>\n"
    "\n"
    "#ifdef DIFFUSE\n"
    "varying vec2 vDiffuseUV;\n"
    "#endif\n"
    "\n"
    "#ifdef AMBIENT\n"
    "varying vec2 vAmbientUV;\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITY\n"
    "varying vec2 vOpacityUV;\n"
    "#endif\n"
    "\n"
    "#ifdef EMISSIVE\n"
    "varying vec2 vEmissiveUV;\n"
    "#endif\n"
    "\n"
    "#ifdef LIGHTMAP\n"
    "varying vec2 vLightmapUV;\n"
    "#endif\n"
    "\n"
    "#if defined(SPECULAR) && defined(SPECULARTERM)\n"
    "varying vec2 vSpecularUV;\n"
    "#endif\n"
    "\n"
    "#ifdef BUMP\n"
    "varying vec2 vBumpUV;\n"
    "#endif\n"
    "\n"
    "// Output\n"
    "varying vec3 vPositionW;\n"
    "#ifdef NORMAL\n"
    "varying vec3 vNormalW;\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "varying vec4 vColor;\n"
    "#endif\n"
    "\n"
    "#include<bumpVertexDeclaration>\n"
    "\n"
    "#include<clipPlaneVertexDeclaration>\n"
    "\n"
    "#include<fogVertexDeclaration>\n"
    "#include<shadowsVertexDeclaration>[0..maxSimultaneousLights]\n"
    "\n"
    "#include<morphTargetsVertexGlobalDeclaration>\n"
    "#include<morphTargetsVertexDeclaration>[0..maxSimultaneousMorphTargets]\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "varying vec3 vPositionUVW;\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "varying vec3 vDirectionW;\n"
    "#endif\n"
    "\n"
    "#include<logDepthDeclaration>\n"
    "\n"
    "void main(void) {\n"
    "  vec3 positionUpdated = position;\n"
    "#ifdef NORMAL  \n"
    "  vec3 normalUpdated = normal;\n"
    "#endif\n"
    "#ifdef TANGENT\n"
    "  vec4 tangentUpdated = tangent;\n"
    "#endif\n"
    "\n"
    "#include<morphTargetsVertex>[0..maxSimultaneousMorphTargets]\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "  vPositionUVW = positionUpdated;\n"
    "#endif \n"
    "\n"
    "#include<instancesVertex>\n"
    "#include<bonesVertex>\n"
    "\n"
    "  gl_Position = viewProjection * finalWorld * vec4(positionUpdated, 1.0);\n"
    "\n"
    "  vec4 worldPos = finalWorld * vec4(positionUpdated, 1.0);\n"
    "  vPositionW = vec3(worldPos);\n"
    "\n"
    "#ifdef NORMAL\n"
    "  vNormalW = normalize(vec3(finalWorld * vec4(normalUpdated, 0.0)));\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "  vDirectionW = normalize(vec3(finalWorld * vec4(positionUpdated, 0.0)));\n"
    "#endif\n"
    "\n"
    "  // Texture coordinates\n"
    "#ifndef UV1\n"
    "  vec2 uv = vec2(0., 0.);\n"
    "#endif\n"
    "#ifndef UV2\n"
    "  vec2 uv2 = vec2(0., 0.);\n"
    "#endif\n"
    "\n"
    "#ifdef DIFFUSE\n"
    "  if (vDiffuseInfos.x == 0.)\n"
    "  {\n"
    "  vDiffuseUV = vec2(diffuseMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vDiffuseUV = vec2(diffuseMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef AMBIENT\n"
    "  if (vAmbientInfos.x == 0.)\n"
    "  {\n"
    "  vAmbientUV = vec2(ambientMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vAmbientUV = vec2(ambientMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITY\n"
    "  if (vOpacityInfos.x == 0.)\n"
    "  {\n"
    "  vOpacityUV = vec2(opacityMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vOpacityUV = vec2(opacityMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef EMISSIVE\n"
    "  if (vEmissiveInfos.x == 0.)\n"
    "  {\n"
    "  vEmissiveUV = vec2(emissiveMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vEmissiveUV = vec2(emissiveMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef LIGHTMAP\n"
    "  if (vLightmapInfos.x == 0.)\n"
    "  {\n"
    "  vLightmapUV = vec2(lightmapMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vLightmapUV = vec2(lightmapMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(SPECULAR) && defined(SPECULARTERM)\n"
    "  if (vSpecularInfos.x == 0.)\n"
    "  {\n"
    "  vSpecularUV = vec2(specularMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vSpecularUV = vec2(specularMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef BUMP\n"
    "  if (vBumpInfos.x == 0.)\n"
    "  {\n"
    "  vBumpUV = vec2(bumpMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vBumpUV = vec2(bumpMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#include<bumpVertex>\n"
    "#include<clipPlaneVertex>\n"
    "#include<fogVertex>\n"
    "#include<shadowsVertex>[0..maxSimultaneousLights]\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "  // Vertex color\n"
    "  vColor = color;\n"
    "#endif\n"
    "\n"
    "#include<pointCloudVertex>\n"
    "#include<logDepthVertex>\n"
    "\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_DEFAULT_VERTEX_FX_H
