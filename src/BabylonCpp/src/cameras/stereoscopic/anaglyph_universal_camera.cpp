#include <babylon/cameras/stereoscopic/anaglyph_universal_camera.h>

#include <babylon/core/json.h>

namespace BABYLON {

bool AnaglyphUniversalCamera::NodeConstructorAdded = false;

void AnaglyphUniversalCamera::AddNodeConstructor()
{
  Node::AddNodeConstructor(
    "AnaglyphUniversalCamera", [](const std::string& name, Scene* scene,
                                  const std::optional<Json::value>& options) {
      float interaxialDistance = 0.f;
      if (options) {
        interaxialDistance
          = Json::GetNumber<float>(*options, "interaxial_distance", 0.f);
      }
      return AnaglyphUniversalCamera::New(name, Vector3::Zero(),
                                          interaxialDistance, scene);
    });
  AnaglyphUniversalCamera::NodeConstructorAdded = true;
}

AnaglyphUniversalCamera::AnaglyphUniversalCamera(const std::string& name,
                                                 const Vector3& position,
                                                 float iInteraxialDistance,
                                                 Scene* scene)
    : UniversalCamera{name, position, scene}
{
  interaxialDistance = iInteraxialDistance;
  Json::object rigParams;
  rigParams["interaxialDistance"]
    = picojson::value(static_cast<double>(interaxialDistance));
  setCameraRigMode(Camera::RIG_MODE_STEREOSCOPIC_ANAGLYPH(),
                   Json::value(rigParams));
}

AnaglyphUniversalCamera::~AnaglyphUniversalCamera()
{
}

const std::string AnaglyphUniversalCamera::getClassName() const
{
  return "AnaglyphUniversalCamera";
}

} // end of namespace BABYLON
