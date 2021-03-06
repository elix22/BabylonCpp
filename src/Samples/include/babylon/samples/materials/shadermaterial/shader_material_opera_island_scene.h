#ifndef BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_OPERA_ISLAND_SCENE_H
#define BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_OPERA_ISLAND_SCENE_H

#include <babylon/interfaces/irenderable_scene.h>

namespace BABYLON {

class ShaderMaterial;
using ShaderMaterialPtr = std::shared_ptr<ShaderMaterial>;

namespace Samples {

class ShaderMaterialOperaIslandScene : public IRenderableScene {

public:
  /** Vertex Shader **/
  static constexpr const char* customVertexShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "// Attributes\n"
      "attribute vec3 position;\n"
      "attribute vec2 uv;\n"
      "\n"
      "// Uniforms\n"
      "uniform mat4 worldViewProjection;\n"
      "\n"
      "// Varying\n"
      "varying vec2 vUV;\n"
      "\n"
      "void main(void) {\n"
      "    gl_Position = worldViewProjection * vec4(position, 1.0);\n"
      "    vUV = uv;\n"
      "}\n";

  /** Pixel (Fragment) Shader **/
  // Opera Island ( https://www.shadertoy.com/view/MlSBWG )
  static constexpr const char* customFragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "// Varying\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "// Uniforms\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float iTime;\n"
      "uniform float iAspectRatio;\n"
      "uniform vec2 iMouse;\n"
      "uniform vec2 iResolution;\n"
      "\n"
      "// 'Opera Island' by dr2 - 2018\n"
      "// License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0\n"
      "// Unported License\n"
      "\n"
      "float PrRoundBox2Df(vec2 p, vec2 b, float r);\n"
      "float PrSphDf(vec3 p, float r);\n"
      "float PrSphAnDf(vec3 p, float r, float w);\n"
      "float PrCylDf(vec3 p, float r, float h);\n"
      "float PrTorusDf(vec3 p, float ri, float rc);\n"
      "float SmoothBump(float lo, float hi, float w, float x);\n"
      "vec2 Rot2D(vec2 q, float a);\n"
      "float Noisefv2(vec2 p);\n"
      "float Fbm1(float p);\n"
      "float Fbm2(vec2 p);\n"
      "vec3 VaryNf(vec3 p, vec3 n, float f);\n"
      "\n"
      "vec3 sunDir = vec3(0., 0., 0.);\n"
      "float tCur = 0., dstFar = 0.;\n"
      "int idObj;\n"
      "const float pi = 3.14159;\n"
      "\n"
      "#define DMIN(id) if (d < dMin) { dMin = d;  idObj = id; }\n"
      "\n"
      "float ObjDf(vec3 p) {\n"
      "  vec3 q, qq;\n"
      "  float dMin, d;\n"
      "  dMin = dstFar;\n"
      "  q = p;\n"
      "  q.x = 3.6 - abs(q.x);\n"
      "  q.z = 3.6 + q.z;\n"
      "  d = max(max(PrSphAnDf(vec3(abs(q.x) + 2.2, q.y - 0.6, q.z),\n"
      "                             5., 0.03),\n"
      "              10. - length(vec2(q.y - 0.6, q.z + 9.))),\n"
      "          1. - q.y);\n"
      "  DMIN(1);\n"
      "  d = max(max(length(vec2(q.x, q.z - 6.1)) - 5.5, 0.1 - q.y),\n"
      "          PrSphDf(vec3(abs(q.x) + 2.2, q.y - 0.6, q.z), 5.));\n"
      "  DMIN(2);\n"
      "  q = p;\n"
      "  q.xz = 3.6 - abs(q.xz);\n"
      "  d = max(\n"
      "      min(max(PrSphAnDf(vec3(abs(q.x) + 1.8, q.y - 0.2, q.z + 1.),\n"
      "                             4.7, 0.03),\n"
      "              8.8 - length(vec2(q.y - 0.6, q.z + 9.))),\n"
      "          max(PrSphAnDf(vec3(abs(q.x) + 1.4, q.y - 0.1, q.z + 1.6),\n"
      "                        4.1, 0.03),\n"
      "              8. - length(vec2(q.y - 0.6, q.z + 9.3)))),\n"
      "      1. - q.y);\n"
      "  DMIN(1);\n"
      "  d = min(max(max(length(vec2(q.x, q.z - 4.5)) - 5.3, 0.1 - q.y),\n"
      "              PrSphDf(vec3(abs(q.x) + 1.8, q.y - 0.2, q.z + 1.),\n"
      "                           4.7)),\n"
      "          max(max(length(vec2(q.x, q.z - 3.3)) - 5., 0.1 - q.y),\n"
      "              PrSphDf(vec3(abs(q.x) + 1.4, q.y - 0.1, q.z + 1.6),\n"
      "                      4.1)));\n"
      "  DMIN(2);\n"
      "  d = min(min(max(PrCylDf(vec3(abs(q.x) + 2.2, q.y - 0.6, q.z).xzy,\n"
      "                               5.05, 0.5),\n"
      "                  -q.z - 1.4),\n"
      "              max(PrCylDf(vec3(q.x, q.y - 0.85, q.z + 0.7).xzy, 2.5,\n"
      "                               0.25),\n"
      "                  q.z + 1.4)),\n"
      "          PrCylDf(vec3(q.x, q.y - 0.35, q.z + 0.7).xzy, 2.9, 0.25));\n"
      "  DMIN(3);\n"
      "  d = max(\n"
      "      PrTorusDf(vec3(q.x, abs(q.y - 1.27) - 0.07, q.z + 0.7).xzy,\n"
      "                     0.015, 2.45),\n"
      "      q.z + 1.1);\n"
      "  qq = q;\n"
      "  qq.z += 0.7;\n"
      "  qq.xz =\n"
      "      Rot2D(qq.xz,\n"
      "            2. * pi * (floor(64. * atan(qq.z, -qq.x) / (2. * pi) +\n"
      "            0.5) / 64.));\n"
      "  d = min(d, max(PrCylDf(vec3(qq.x + 2.45, qq.y - 1.2, qq.z).xzy,\n"
      "                              0.015, 0.14),\n"
      "                 q.z + 1.1));\n"
      "  DMIN(4);\n"
      "  q = p;\n"
      "  d = max(PrRoundBox2Df(vec2(q.xz), vec2(3.9, 4.7), 3.2),\n"
      "                        abs(q.y) - 0.1);\n"
      "  DMIN(5);\n"
      "  return dMin;\n"
      "}\n"
      "\n"
      "float ObjRay(vec3 ro, vec3 rd) {\n"
      "  float dHit, d;\n"
      "  dHit = 0.;\n"
      "  for (int j = 0; j < 120; j++) {\n"
      "    d = ObjDf(ro + rd * dHit);\n"
      "    if (d < 0.0005 || dHit > dstFar)\n"
      "      break;\n"
      "    dHit += d;\n"
      "  }\n"
      "  return dHit;\n"
      "}\n"
      "\n"
      "vec3 ObjNf(vec3 p) {\n"
      "  vec4 v;\n"
      "  vec2 e = vec2(0.0005, -0.0005);\n"
      "  v = vec4(ObjDf(p + e.xxx), ObjDf(p + e.xyy), ObjDf(p + e.yxy),\n"
      "           ObjDf(p + e.yyx));\n"
      "  return normalize(vec3(v.x - v.y - v.z - v.w) + 2. * v.yzw);\n"
      "}\n"
      "\n"
      "float WaveHt(vec2 p) {\n"
      "  mat2 qRot = mat2(0.8, -0.6, 0.6, 0.8);\n"
      "  vec4 t4, v4;\n"
      "  vec2 q, t, tw;\n"
      "  float wFreq, wAmp, h;\n"
      "  q = 0.5 * p + vec2(0., tCur);\n"
      "  h = 0.6 * sin(dot(q, vec2(-0.05, 1.))) +\n"
      "      0.45 * sin(dot(q, vec2(0.1, 1.2))) +\n"
      "      0.3 * sin(dot(q, vec2(-0.2, 1.4)));\n"
      "  q = 2. * p;\n"
      "  wFreq = 1.;\n"
      "  wAmp = 1.;\n"
      "  tw = 0.1 * tCur * vec2(1., -1.);\n"
      "  for (int j = 0; j < 3; j++) {\n"
      "    q *= qRot;\n"
      "    t4 = q.xyxy * wFreq + tw.xxyy;\n"
      "    t = vec2(Noisefv2(t4.xy), Noisefv2(t4.zw));\n"
      "    t4 += 2. * t.xxyy - 1.;\n"
      "    v4 = (1. - abs(sin(t4))) * (abs(sin(t4)) + abs(cos(t4)));\n"
      "    t = 1. - sqrt(v4.xz * v4.yw);\n"
      "    t *= t;\n"
      "    t *= t;\n"
      "    h += wAmp * dot(t, t);\n"
      "    wFreq *= 2.;\n"
      "    wAmp *= 0.5;\n"
      "  }\n"
      "  return h;\n"
      "}\n"
      "\n"
      "vec3 WaveNf(vec3 p, float d) {\n"
      "  vec3 vn;\n"
      "  vec2 e;\n"
      "  e = vec2(max(0.01, 0.005 * d * d), 0.);\n"
      "  p *= 0.5;\n"
      "  vn.xz = 0.2 * (WaveHt(p.xz) - vec2(WaveHt(p.xz + e.xy),\n"
      "                                     WaveHt(p.xz + e.yx)));\n"
      "  vn.y = e.x;\n"
      "  return normalize(vn);\n"
      "}\n"
      "\n"
      "vec3 SkyHrzCol(vec3 ro, vec3 rd) {\n"
      "  vec3 col;\n"
      "  float sd, a, f;\n"
      "  a = atan(rd.z, rd.x) + 0.001 * tCur;\n"
      "  if (rd.y < 0.01 * Fbm1(32. * a) + 0.005)\n"
      "    col = mix(vec3(0.25, 0.3, 0.25), vec3(0.3, 0.35, 0.33),\n"
      "              0.5 + 0.5 * dot(normalize(rd.xz),\n"
      "              -normalize(sunDir.xz))) *\n"
      "          (1. - 0.3 * Fbm2(128. * vec2(a, rd.y)));\n"
      "  else {\n"
      "    ro.x += 0.5 * tCur;\n"
      "    f = Fbm2(0.05 * (rd.xz * (100. - ro.y) / rd.y + ro.xz));\n"
      "    sd = pow(max(dot(rd, sunDir), 0.), 64.);\n"
      "    col = vec3(0.2, 0.3, 0.5) + 0.1 * pow(1. - max(rd.y, 0.), 4.) +\n"
      "          (0.35 * sd + 0.65 * min(pow(sd, 4.), 0.3)) *\n"
      "          vec3(1., 1., 0.3);\n"
      "    col = mix(col, vec3(0.85), clamp(f * rd.y + 0.1, 0., 1.));\n"
      "  }\n"
      "  return col;\n"
      "}\n"
      "\n"
      "float ObjSShadow(vec3 ro, vec3 rd) {\n"
      "  float sh, d, h;\n"
      "  sh = 1.;\n"
      "  d = 0.1;\n"
      "  for (int j = 0; j < 30; j++) {\n"
      "    h = ObjDf(ro + rd * d);\n"
      "    sh = min(sh, smoothstep(0., 0.05 * d, h));\n"
      "    d += min(0.07, 3. * h);\n"
      "    if (sh < 0.001)\n"
      "      break;\n"
      "  }\n"
      "  return 0.7 + 0.3 * sh;\n"
      "}\n"
      "\n"
      "vec3 ShowScene(vec3 ro, vec3 rd) {\n"
      "  vec4 col4;\n"
      "  vec3 col, vn;\n"
      "  vec2 sf;\n"
      "  float dstObj, dstWat, sh, s;\n"
      "  bool watRefl, winfRefl;\n"
      "  watRefl = false;\n"
      "  winfRefl = false;\n"
      "  dstObj = ObjRay(ro, rd);\n"
      "  dstWat = (rd.y < 0.) ? -ro.y / rd.y : dstFar;\n"
      "  if (dstWat < min(dstObj, dstFar)) {\n"
      "    watRefl = true;\n"
      "    ro += dstWat * rd;\n"
      "    rd = reflect(rd, WaveNf(ro, dstWat));\n"
      "    ro += 0.01 * rd;\n"
      "    dstObj = ObjRay(ro, rd);\n"
      "  }\n"
      "  if (dstObj < dstFar) {\n"
      "    sf = vec2(0.);\n"
      "    ro += dstObj * rd;\n"
      "    vn = ObjNf(ro);\n"
      "    if (idObj == 1) {\n"
      "      col4 = vec4(0.8, 0.8, 0.85, 0.05) *\n"
      "             (1. - 0.1 * SmoothBump(0.45, 0.55, 0.02,\n"
      "                         mod(8. * ro.z, 1.)));\n"
      "      sf = vec2(32., 0.2);\n"
      "    } else if (idObj == 2) {\n"
      "      if (abs(mod(4. * (ro.x - 3.6 * sign(ro.x)), 1.) - 0.5) < 0.1 ||\n"
      "          ro.y < 1.2) {\n"
      "        if (ro.y < 1.2)\n"
      "          col4 = 1.2 * vec4(0.6, 0.5, 0.4, 0.1);\n"
      "        else\n"
      "          col4 = vec4(0.5, 0.5, 0.55, 0.2);\n"
      "      } else\n"
      "        winfRefl = true;\n"
      "    } else if (idObj == 3) {\n"
      "      col4 = vec4(0.6, 0.5, 0.4, 0.1);\n"
      "      if (abs(ro.y - 0.86) < 0.15 && abs(ro.z) > 5.35 ||\n"
      "          abs(ro.y - 0.36) < 0.15 && abs(ro.z) > 3.9)\n"
      "        winfRefl = (abs(mod(32. *\n"
      "                                atan(ro.z - 4.3 * sign(ro.z),\n"
      "                                     ro.x - 3.6 * sign(ro.x)) /\n"
      "                                pi,\n"
      "                            1.) -\n"
      "                        0.5) > 0.15);\n"
      "      else if (abs(abs(ro.y - 0.61) - 0.25) < 0.15 &&\n"
      "               abs(ro.z) > 0.4 &&\n"
      "               abs(ro.z) < 3.4)\n"
      "        winfRefl = (abs(mod(48. *\n"
      "                                atan(ro.z - 3.6 * sign(ro.z),\n"
      "                                     ro.x - 1.4 * sign(ro.x)) /\n"
      "                                pi,\n"
      "                            1.) -\n"
      "                        0.5) > 0.15);\n"
      "      else {\n"
      "        s = length(vec2(ro.z - 4.3 * sign(ro.z),\n"
      "                        ro.x - 3.6 * sign(ro.x)));\n"
      "        if (s < 2.4)\n"
      "          col4 *= 1. - 0.1 * SmoothBump(0.45, 0.55, 0.02,\n"
      "                                        mod(4. * s, 1.));\n"
      "        sf = vec2(32., 0.2);\n"
      "      }\n"
      "    } else if (idObj == 4) {\n"
      "      col4 = vec4(0.5, 0.5, 0.6, 0.2);\n"
      "    } else if (idObj == 5) {\n"
      "      col4 = vec4(0.3, 0.5, 0.3, 0.05) *\n"
      "             (1. - 0.2 * Fbm2(16. * ro.xz));\n"
      "      sf = vec2(16., 1.);\n"
      "    }\n"
      "    if (sf.x > 0.)\n"
      "      vn = VaryNf(sf.x * ro, vn, sf.y);\n"
      "    if (winfRefl) {\n"
      "      rd = reflect(rd, vn);\n"
      "      if (rd.y > 0.)\n"
      "        col = SkyHrzCol(ro, rd);\n"
      "      else {\n"
      "        watRefl = true;\n"
      "        dstWat = -ro.y / rd.y;\n"
      "        ro += dstWat * rd;\n"
      "        col = SkyHrzCol(ro, reflect(rd, WaveNf(ro, dstWat)));\n"
      "      }\n"
      "      col = mix(vec3(0.3, 0.3, 0.2), col, 0.3);\n"
      "    } else {\n"
      "      sh = ObjSShadow(ro, sunDir);\n"
      "      col = col4.rgb * (0.2 + 0.1 * max(vn.y, 0.) +\n"
      "                        0.8 * sh * max(dot(vn, sunDir), 0.)) +\n"
      "            col4.a * sh *\n"
      "            pow(max(dot(normalize(sunDir - rd), vn), 0.), 128.);\n"
      "    }\n"
      "  } else if (rd.y < 0.) {\n"
      "    watRefl = true;\n"
      "    dstWat = -ro.y / rd.y;\n"
      "    ro += dstWat * rd;\n"
      "    col = SkyHrzCol(ro, reflect(rd, WaveNf(ro, dstWat)));\n"
      "  } else {\n"
      "    col = SkyHrzCol(ro, rd);\n"
      "  }\n"
      "  if (watRefl) {\n"
      "    col = mix(mix(vec3(0.01, 0.01, 0.), vec3(0., 0.05, 0.05),\n"
      "                  smoothstep(0.4, 0.6, Fbm2(0.5 * ro.xz))),\n"
      "              0.8 * col, 1. - pow(abs(rd.y), 4.));\n"
      "  }\n"
      "  return clamp(col, 0., 1.);\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      "  mat3 vuMat;\n"
      "  vec4 mPtr;\n"
      "  vec3 ro, rd;\n"
      "  vec2 canvas, uv, ori, ca, sa;\n"
      "  float el, az;\n"
      "  canvas = iResolution.xy;\n"
      "  uv = 2. * (-1.0 + 2.0 * vUV.xy);\n"
      "  uv.x *= canvas.x / canvas.y;\n"
      "  tCur = iTime;\n"
      "  mPtr = vec4(iMouse, 0., 0.);\n"
      "  mPtr.xy = mPtr.xy / canvas - 0.5;\n"
      "  az = 0.25 * pi;\n"
      "  el = -0.05 * pi;\n"
      "  if (mPtr.z > 0.) {\n"
      "    az += 2. * pi * mPtr.x;\n"
      "    el += 0.2 * pi * mPtr.y;\n"
      "  } else {\n"
      "    az -= 0.02 * pi * tCur;\n"
      "    el += 0.04 * pi * cos(0.02 * pi * tCur);\n"
      "  }\n"
      "  el = clamp(el, -0.1 * pi, -0.01 * pi);\n"
      "  ori = vec2(el, az);\n"
      "  ca = cos(ori);\n"
      "  sa = sin(ori);\n"
      "  vuMat = mat3(ca.y, 0., -sa.y, 0., 1., 0., sa.y, 0., ca.y) *\n"
      "          mat3(1., 0., 0., 0., ca.x, -sa.x, 0., sa.x, ca.x);\n"
      "  ro = vuMat * vec3(0., 1., -40.);\n"
      "  rd = vuMat * normalize(vec3(uv, 6.));\n"
      "  dstFar = 100.;\n"
      "  sunDir = normalize(vec3(1., 1., -1.));\n"
      "  gl_FragColor = vec4(ShowScene(ro, rd), 1.);\n"
      "}\n"
      "\n"
      "float PrRoundBox2Df(vec2 p, vec2 b, float r) {\n"
      "  return length(max(abs(p) - b, 0.)) - r;\n"
      "}\n"
      "\n"
      "float PrSphDf(vec3 p, float r) { return length(p) - r; }\n"
      "\n"
      "float PrSphAnDf(vec3 p, float r, float w) {\n"
      "  return abs(length(p) - r) - w;\n"
      "}\n"
      "\n"
      "float PrCylDf(vec3 p, float r, float h) {\n"
      "  return max(length(p.xy) - r, abs(p.z) - h);\n"
      "}\n"
      "\n"
      "float PrTorusDf(vec3 p, float ri, float rc) {\n"
      "  return length(vec2(length(p.xy) - rc, p.z)) - ri;\n"
      "}\n"
      "\n"
      "float SmoothBump(float lo, float hi, float w, float x) {\n"
      "  return (1. - smoothstep(hi - w, hi + w, x)) *\n"
      "               smoothstep(lo - w, lo + w, x);\n"
      "}\n"
      "\n"
      "vec2 Rot2D(vec2 q, float a) {\n"
      "  return q * cos(a) * vec2(1., 1.) + q.yx * sin(a) * vec2(-1., 1.);\n"
      "}\n"
      "\n"
      "const float cHashM = 43758.54;\n"
      "\n"
      "vec2 Hashv2f(float p) {\n"
      "  return fract(sin(p + vec2(0., 1.)) * cHashM);\n"
      "}\n"
      "\n"
      "vec2 Hashv2v2(vec2 p) {\n"
      "  vec2 cHashVA2 = vec2(37., 39.);\n"
      "  return fract(sin(vec2(dot(p, cHashVA2), dot(p + vec2(1., 0.),\n"
      "               cHashVA2))) * cHashM);\n"
      "}\n"
      "\n"
      "float Noiseff(float p) {\n"
      "  vec2 t;\n"
      "  float ip, fp;\n"
      "  ip = floor(p);\n"
      "  fp = fract(p);\n"
      "  fp = fp * fp * (3. - 2. * fp);\n"
      "  t = Hashv2f(ip);\n"
      "  return mix(t.x, t.y, fp);\n"
      "}\n"
      "\n"
      "float Noisefv2(vec2 p) {\n"
      "  vec2 t, ip, fp;\n"
      "  ip = floor(p);\n"
      "  fp = fract(p);\n"
      "  fp = fp * fp * (3. - 2. * fp);\n"
      "  t = mix(Hashv2v2(ip), Hashv2v2(ip + vec2(0., 1.)), fp.y);\n"
      "  return mix(t.x, t.y, fp.x);\n"
      "}\n"
      "\n"
      "float Fbm1(float p) {\n"
      "  float f, a;\n"
      "  f = 0.;\n"
      "  a = 1.;\n"
      "  for (int j = 0; j < 5; j++) {\n"
      "    f += a * Noiseff(p);\n"
      "    a *= 0.5;\n"
      "    p *= 2.;\n"
      "  }\n"
      "  return f * (1. / 1.9375);\n"
      "}\n"
      "\n"
      "float Fbm2(vec2 p) {\n"
      "  float f, a;\n"
      "  f = 0.;\n"
      "  a = 1.;\n"
      "  for (int j = 0; j < 5; j++) {\n"
      "    f += a * Noisefv2(p);\n"
      "    a *= 0.5;\n"
      "    p *= 2.;\n"
      "  }\n"
      "  return f * (1. / 1.9375);\n"
      "}\n"
      "\n"
      "float Fbmn(vec3 p, vec3 n) {\n"
      "  vec3 s;\n"
      "  float a;\n"
      "  s = vec3(0.);\n"
      "  a = 1.;\n"
      "  for (int j = 0; j < 5; j++) {\n"
      "    s += a * vec3(Noisefv2(p.yz), Noisefv2(p.zx), Noisefv2(p.xy));\n"
      "    a *= 0.5;\n"
      "    p *= 2.;\n"
      "  }\n"
      "  return dot(s, abs(n));\n"
      "}\n"
      "\n"
      "vec3 VaryNf(vec3 p, vec3 n, float f) {\n"
      "  vec3 g;\n"
      "  vec2 e = vec2(0.1, 0.);\n"
      "  g = vec3(Fbmn(p + e.xyy, n), Fbmn(p + e.yxy, n),\n"
      "           Fbmn(p + e.yyx, n)) - Fbmn(p, n);\n"
      "  return normalize(n + f * (g - n * dot(n, g)));\n"
      "}\n";

public:
  ShaderMaterialOperaIslandScene(ICanvas* iCanvas);
  ~ShaderMaterialOperaIslandScene() override;

  const char* getName() override;
  void initializeScene(ICanvas* canvas, Scene* scene) override;

private:
  float _time;
  ShaderMaterialPtr _shaderMaterial;

}; // end of class ShaderMaterialOperaIslandScene

} // end of namespace Samples
} // end of namespace BABYLON

#endif // end of
       // BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_OPERA_ISLAND_SCENE_H
