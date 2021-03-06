#include <babylon/gamepad/controllers/web_vr_controller.h>

#include <babylon/babylon_stl_util.h>
#include <babylon/gamepad/controllers/extended_gamepad_button.h>
#include <babylon/interfaces/ibrowser_gamepad.h>

namespace BABYLON {

WebVRController::WebVRController(const std::shared_ptr<IBrowserGamepad>& vrGamepad)
    : PoseEnabledController(vrGamepad)
    , pad{StickValues(0.f, 0.f)}
    , hand{vrGamepad->hand}
    , defaultModel{this, &WebVRController::get_defaultModel}
    , _changes{GamepadButtonChanges{
        // changed
        false,
        // pressChanged
        false,
        // touchChanged
        false,
        // valueChanged
        false,
      }}
{
  _buttons = std::vector<ExtendedGamepadButton>(vrGamepad->buttons.size());
}

WebVRController::~WebVRController()
{
}

AbstractMeshPtr& WebVRController::get_defaultModel()
{
  return _defaultModel;
}

void WebVRController::setOnButtonStateChange(
  const std::function<void(int controlledIndex, unsigned int buttonIndex,
                             const ExtendedGamepadButton& state)>& callback)
{
  _onButtonStateChange = callback;
}

void WebVRController::update()
{
  PoseEnabledController::update();
  for (unsigned int index = 0; index < _buttons.size(); ++index) {
    // _setButtonValue(_browserGamepad->buttons[index], _buttons[index], index);
  };
  const auto& pLeftStick = *leftStick();
  if (!stl_util::almost_equal(pLeftStick.x, pad.x)
      || !stl_util::almost_equal(pLeftStick.y, pad.y)) {
    pad.x = pLeftStick.x;
    pad.y = pLeftStick.y;
    onPadValuesChangedObservable.notifyObservers(&pad);
  }
}

void WebVRController::_setButtonValue(
  std::optional<ExtendedGamepadButton> newState,
  const std::optional<ExtendedGamepadButton>& currentState,
  unsigned int buttonIndex)
{
  if (!newState) {
    newState = ExtendedGamepadButton(0, false, false);
  }
  if (!currentState) {
    _buttons[buttonIndex] = ExtendedGamepadButton(
      (*newState).value(), (*newState).touched(), (*newState).pressed());
    return;
  }
  _checkChanges(*newState, *currentState);
  if (_changes.changed) {
    if (_onButtonStateChange) {
      _onButtonStateChange(index, buttonIndex, *newState);
    }

    _handleButtonChange(buttonIndex, *newState, _changes);
  }
  _buttons[buttonIndex].setPressed((*newState).pressed());
  _buttons[buttonIndex].setTouched((*newState).touched());
  // oculus triggers are never 0, thou not touched.
  _buttons[buttonIndex].setValue(
    (*newState).value() < 0.00000001f ? 0.f : (*newState).value());
}

GamepadButtonChanges&
WebVRController::_checkChanges(const ExtendedGamepadButton& newState,
                               const ExtendedGamepadButton& currentState)
{
  _changes.pressChanged = newState.pressed() != currentState.pressed();
  _changes.touchChanged = newState.touched() != currentState.touched();
  _changes.valueChanged = newState.value() != currentState.value();
  _changes.changed
    = _changes.pressChanged || _changes.touchChanged || _changes.valueChanged;
  return _changes;
}

void WebVRController::dispose()
{
  PoseEnabledController::dispose();

  onTriggerStateChangedObservable.clear();
  onMainButtonStateChangedObservable.clear();
  onSecondaryButtonStateChangedObservable.clear();
  onPadStateChangedObservable.clear();
  onPadValuesChangedObservable.clear();
}

} // end of namespace BABYLON
