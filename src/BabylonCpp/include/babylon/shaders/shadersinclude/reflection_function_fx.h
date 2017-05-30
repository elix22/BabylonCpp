﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_REFLECTION_FUNCTION_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_REFLECTION_FUNCTION_FX_H

namespace BABYLON {

extern const char* reflectionFunction;

const char* reflectionFunction
  = "vec3 computeReflectionCoords(vec4 worldPos, vec3 worldNormal)\n"
    "{\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "  vec3 direction = normalize(vDirectionW);\n"
    "\n"
    "  float t = clamp(direction.y * -0.5 + 0.5, 0., 1.0);\n"
    "  float s = atan(direction.z, direction.x) * RECIPROCAL_PI2 + 0.5;\n"
    "\n"
    "   #ifdef REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED\n"
    "  return vec3(1.0 - s, t, 0);\n"
    "  #else\n"
    "  return vec3(s, t, 0);\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_EQUIRECTANGULAR\n"
    "\n"
    "  vec3 cameraToVertex = normalize(worldPos.xyz - vEyePosition);\n"
    "  vec3 r = reflect(cameraToVertex, worldNormal);\n"
    "  float t = clamp(r.y * -0.5 + 0.5, 0., 1.0);\n"
    "  float s = atan(r.z, r.x) * RECIPROCAL_PI2 + 0.5;\n"
    "\n"
    "  return vec3(s, t, 0);\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_SPHERICAL\n"
    "  vec3 viewDir = normalize(vec3(view * worldPos));\n"
    "  vec3 viewNormal = normalize(vec3(view * vec4(worldNormal, 0.0)));\n"
    "\n"
    "  vec3 r = reflect(viewDir, viewNormal);\n"
    "  r.z = r.z - 1.0;\n"
    "\n"
    "  float m = 2.0 * length(r);\n"
    "\n"
    "  return vec3(r.x / m + 0.5, 1.0 - r.y / m - 0.5, 0);\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_PLANAR\n"
    "  vec3 viewDir = worldPos.xyz - vEyePosition;\n"
    "  vec3 coords = normalize(reflect(viewDir, worldNormal));\n"
    "\n"
    "  return vec3(reflectionMatrix * vec4(coords, 1));\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_CUBIC\n"
    "  vec3 viewDir = worldPos.xyz - vEyePosition;\n"
    "  vec3 coords = reflect(viewDir, worldNormal);\n"
    "#ifdef INVERTCUBICMAP\n"
    "  coords.y = 1.0 - coords.y;\n"
    "#endif\n"
    "  return vec3(reflectionMatrix * vec4(coords, 0));\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_PROJECTION\n"
    "  return vec3(reflectionMatrix * (view * worldPos));\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "  return vPositionUVW;\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_EXPLICIT\n"
    "  return vec3(0, 0, 0);\n"
    "#endif\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_REFLECTION_FUNCTION_FX_H
