#include <babylon/actions/valueactions/interpolate_value_action.h>

#include <babylon/actions/action_manager.h>
#include <babylon/animations/animation.h>
#include <babylon/animations/animation_value.h>
#include <babylon/animations/ianimatable.h>
#include <babylon/animations/ianimation_key.h>
#include <babylon/audio/sound.h>
#include <babylon/core/json.h>
#include <babylon/core/logging.h>
#include <babylon/engine/scene.h>

namespace BABYLON {

InterpolateValueAction::InterpolateValueAction(
  unsigned int triggerOptions, const IAnimatablePtr& target,
  const std::string& iPropertyPath, AnimationValue* iValue, int iDuration,
  Condition* condition, bool iStopOtherAnimations,
  const std::function<void()>& iOnInterpolationDone)
    : Action(triggerOptions, condition)
    , propertyPath{iPropertyPath}
    , value{iValue}
    , duration{iDuration}
    , stopOtherAnimations{iStopOtherAnimations}
    , onInterpolationDone{iOnInterpolationDone}
    , _target{target}
    , _effectiveTarget{target}
{
}

InterpolateValueAction::~InterpolateValueAction()
{
}

void InterpolateValueAction::_prepare()
{
  _effectiveTarget = _getEffectiveTarget(_effectiveTarget, propertyPath);
  _property        = _getProperty(propertyPath);
}

void InterpolateValueAction::execute(const ActionEvent& /*evt*/)
{
  auto scene = _actionManager->getScene();
  std::vector<IAnimationKey> keys{
    IAnimationKey(0, (*_effectiveTarget)[_property]), //
    IAnimationKey(100, *value)};

  int dataType = value->dataType;
  if (dataType == -1) {
    BABYLON_LOG_WARN("InterpolateValueAction",
                     "InterpolateValueAction: Unsupported type");
  }

  AnimationPtr animation
    = Animation::New("InterpolateValueAction", _property,
                     static_cast<size_t>(100 * (1000.0 / duration)), dataType,
                     Animation::ANIMATIONLOOPMODE_CONSTANT());
  animation->setKeys(keys);

  if (stopOtherAnimations) {
    scene->stopAnimation(_effectiveTarget);
  }

  const auto wrapper = [this]() {
    onInterpolationDoneObservable.notifyObservers(this);
    if (onInterpolationDone) {
      onInterpolationDone();
    }
  };

  scene->beginDirectAnimation(_effectiveTarget, {animation}, 0, 100, false, 1,
                              wrapper);
}

Json::object InterpolateValueAction::serialize(Json::object& parent) const
{
  return Action::_serialize(
    Json::object(
      {Json::Pair("name", "InterpolateValueAction"),
       Json::Pair("properties", Json::array({{/*
Action::_GetTargetProperty(_target),
Json::NameValuePair("propertyPath", propertyPath),
Json::NameValuePair("value", Action::_SerializeValueAsString(value)),
Json::NameValuePair("duration", Action::_SerializeValueAsString(
                                  AnimationValue(duration))),
Json::NameValuePair("stopOtherAnimations",
                    Action::_SerializeValueAsString(
                      AnimationValue(stopOtherAnimations))),

*/}}))}),
    parent);
}

} // end of namespace BABYLON
