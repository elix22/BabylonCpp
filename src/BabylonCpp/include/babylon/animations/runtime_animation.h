#ifndef BABYLON_ANIMATIONS_RUNTIME_ANIMATION_H
#define BABYLON_ANIMATIONS_RUNTIME_ANIMATION_H

#include <unordered_map>

#include <babylon/animations/animation_value.h>
#include <babylon/babylon_api.h>

namespace BABYLON {

class Animatable;
class Animation;
class AnimationEvent;
class IAnimatable;
class RuntimeAnimation;
class Scene;
using AnimationPtr        = std::shared_ptr<Animation>;
using IAnimatablePtr      = std::shared_ptr<IAnimatable>;
using RuntimeAnimationPtr = std::shared_ptr<RuntimeAnimation>;

/**
 * @brief Defines a runtime animation.
 */
class BABYLON_SHARED_EXPORT RuntimeAnimation {

public:
  template <typename... Ts>
  static RuntimeAnimationPtr New(Ts&&... args)
  {
    auto animation = std::shared_ptr<RuntimeAnimation>(
      new RuntimeAnimation(std::forward<Ts>(args)...));
    animation->addToRuntimeAnimations(animation);

    return animation;
  }
  ~RuntimeAnimation();

  void addToRuntimeAnimations(const RuntimeAnimationPtr& animation);

  /**
   * @brief Gets the animation from the runtime animation.
   */
  AnimationPtr& animation();

  /**
   * @brief Resets the runtime animation to the beginning.
   * @param restoreOriginal defines whether to restore the target property to
   * the original value
   */
  void reset(bool restoreOriginal = false);

  /**
   * @brief Specifies if the runtime animation is stopped.
   * @returns Boolean specifying if the runtime animation is stopped
   */
  bool isStopped() const;

  /**
   * @brief Disposes of the runtime animation.
   */
  void dispose();

  /**
   * @brief Apply the interpolated value to the target.
   * @param currentValue defines the value computed by the animation
   * @param weight defines the weight to apply to this value (Defaults to 1.0)
   */
  void setValue(const AnimationValue& currentValue, float weight = 1.f);

  /**
   * @brief Gets the loop pmode of the runtime animation
   * @returns Loop Mode
   */
  std::optional<unsigned int> _getCorrectLoopMode() const;

  /**
   * @brief Move the current animation to a given frame.
   * @param frame defines the frame to move to
   */
  void goToFrame(int frame);

  /**
   * @brief Hidden Internal use only
   */
  void _prepareForSpeedRatioChange(float newSpeedRatio);

  /**
   * @brief Execute the current animation.
   * @param delay defines the delay to add to the current frame
   * @param from defines the lower bound of the animation range
   * @param to defines the upper bound of the animation range
   * @param loop defines if the current animation must loop
   * @param speedRatio defines the current speed ratio
   * @param weight defines the weight of the animation (default is -1 so no
   * weight)
   * @returns a boolean indicating if the animation is running
   */
  bool animate(millisecond_t delay, float from, float to, bool loop,
               float speedRatio, float weight = -1.f);

protected:
  /**
   * @brief Create a new RuntimeAnimation object.
   * @param target defines the target of the animation
   * @param animation defines the source animation object
   * @param scene defines the hosting scene
   * @param host defines the initiating Animatable
   */
  RuntimeAnimation(const IAnimatablePtr& target, const AnimationPtr& animation,
                   Scene* scene, Animatable* host);

  /**
   * @brief Gets the current frame of the runtime animation.
   */
  float get_currentFrame() const;

  /**
   * @brief Gets the weight of the runtime animation.
   */
  float get_weight() const;

  /**
   * @brief Gets the current value of the runtime animation.
   */
  std::optional<AnimationValue>& get_currentValue();

  /**
   * @brief Gets the target path of the runtime animation.
   */
  std::string get_targetPath() const;

  /**
   * @brief Gets the actual target of the runtime animation.
   */
  IAnimatable*& get_target();

private:
  /**
   * @brief Interpolates the animation from the current frame.
   * @param currentFrame The frame to interpolate the animation to
   * @param repeatCount The number of times that the animation should loop
   * @param loopMode The type of looping mode to use
   * @param offsetValue Animation offset value
   * @param highLimitValue The high limit value
   * @returns The interpolated value
   */
  AnimationValue
  _interpolate(float currentFrame, int repeatCount, unsigned int loopMode,
               const AnimationValue& offsetValue    = AnimationValue(),
               const AnimationValue& highLimitValue = AnimationValue());

  void _setValue(const IAnimatablePtr& target,
                 const AnimationValue& currentValue, float weight,
                 unsigned int targetIndex = 0);

public:
  /**
   * Current frame of the runtime animation
   */
  ReadOnlyProperty<RuntimeAnimation, float> currentFrame;

  /**
   * Weight of the runtime animation
   */
  ReadOnlyProperty<RuntimeAnimation, float> weight;

  /**
   * Current value of the runtime animation
   */
  ReadOnlyProperty<RuntimeAnimation, std::optional<AnimationValue>>
    currentValue;

  /**
   * Hidden
   */
  std::optional<AnimationValue> _workValue;

  /**
   * Target path of the runtime animation
   */
  ReadOnlyProperty<RuntimeAnimation, std::string> targetPath;

  /**
   * Actual target of the runtime animation
   */
  ReadOnlyProperty<RuntimeAnimation, IAnimatable*> target;

private:
  std::vector<AnimationEvent> _events;

  /**
   * The current frame of the runtime animation
   */
  float _currentFrame;

  /**
   * The animation used by the runtime animation
   */
  AnimationPtr _animation;

  /**
   * The target of the runtime animation
   */
  IAnimatablePtr _target;

  /**
   * The initiating animatable
   */
  Animatable* _host;

  /**
   * The original value of the runtime animation
   */
  std::vector<std::optional<AnimationValue>> _originalValue;

  /**
   * The original blend value of the runtime animation
   */
  AnimationValue _originalBlendValue;

  /**
   * The offsets cache of the runtime animation
   */
  std::unordered_map<std::string, AnimationValue> _offsetsCache;

  /**
   * The high limits cache of the runtime animation
   */
  std::unordered_map<std::string, AnimationValue> _highLimitsCache;

  /**
   * Specifies if the runtime animation has been stopped
   */
  bool _stopped;

  /**
   * The blending factor of the runtime animation
   */
  float _blendingFactor;

  /**
   * The BabylonJS scene
   */
  Scene* _scene;

  /**
   * The current value of the runtime animation
   */
  std::optional<AnimationValue> _currentValue;

  /**
   * The active target of the runtime animation
   */
  IAnimatable* _activeTarget;

  /**
   * The target path of the runtime animation
   */
  std::string _targetPath;

  /**
   * The weight of the runtime animation
   */
  float _weight;

  /**
   * The ratio offset of the runtime animation
   */
  float _ratioOffset;

  /**
   * The previous delay of the runtime animation
   */
  millisecond_t _previousDelay;

  /**
   * The previous ratio of the runtime animation
   */
  float _previousRatio;

}; // end of class RuntimeAnimation

} // end of namespace BABYLON

#endif // end of BABYLON_ANIMATIONS_RUNTIME_ANIMATION_H
