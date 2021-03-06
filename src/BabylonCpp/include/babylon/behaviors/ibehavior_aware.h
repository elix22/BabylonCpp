#ifndef BABYLON_BEHAVIORS_IBEHAVIOR_AWARE_H
#define BABYLON_BEHAVIORS_IBEHAVIOR_AWARE_H

#include <string>

#include <babylon/babylon_api.h>

namespace BABYLON {

template <class T>
struct Behavior;

/**
 * @brief Interface implemented by classes supporting behaviors.
 */
template <class T>
struct BABYLON_SHARED_EXPORT IBehaviorAware {

  /**
   * @brief Attach a behavior.
   * @param behavior defines the behavior to attach
   * @returns the current host
   */
  virtual T& addBehavior(Behavior<T>* behavior) = 0;
  /**
   * @brief Remove a behavior from the current object.
   * @param behavior defines the behavior to detach
   * @returns the current host
   */
  virtual T& removeBehavior(Behavior<T>* behavior) = 0;
  /**
   * @brief Gets a behavior using its name to search.
   * @param name defines the name to search
   * @returns the behavior or null if not found
   */
  virtual Behavior<T>* getBehaviorByName(const std::string& name) = 0;

}; // end of struct IBehaviorAware<T>

} // end of namespace BABYLON

#endif // end of BABYLON_BEHAVIORS_IBEHAVIOR_AWARE_H
