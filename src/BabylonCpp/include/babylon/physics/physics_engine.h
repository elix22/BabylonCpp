#ifndef BABYLON_PHYSICS_PHYSICS_ENGINE_H
#define BABYLON_PHYSICS_PHYSICS_ENGINE_H

#include <babylon/babylon_api.h>
#include <babylon/math/vector3.h>

namespace BABYLON {

struct IPhysicsBody;
struct IPhysicsEnabledObject;
struct IPhysicsEnginePlugin;
class PhysicsImpostor;
struct PhysicsImpostorJoint;
class PhysicsJoint;

class BABYLON_SHARED_EXPORT PhysicsEngine {

public:
  PhysicsEngine(const Vector3& gravity = Vector3(0.f, -9.807f, 0.f),
                IPhysicsEnginePlugin* physicsPlugin = nullptr);
  virtual ~PhysicsEngine();

  void setGravity(const Vector3& gravity);

  /**
   * @brief Set the time step of the physics engine.
   * default is 1/60.
   * To slow it down, enter 1/600 for example.
   * To speed it up, 1/30
   * @param {number} newTimeStep the new timestep to apply to this world.
   */
  void setTimeStep(float newTimeStep = 1.f / 60.f);

  /**
   * @brief Get the time step of the physics engine.
   */
  float getTimeStep() const;

  void dispose();
  std::string getPhysicsPluginName() const;

  /**
   * @brief Adding a new impostor for the impostor tracking.
   * This will be done by the impostor itself.
   * @param {PhysicsImpostor} impostor the impostor to add
   */
  void addImpostor(PhysicsImpostor* impostor);

  /**
   * @brief Remove an impostor from the engine.
   * This impostor and its mesh will not longer be updated by the physics
   * engine.
   * @param {PhysicsImpostor} impostor the impostor to remove
   */
  void removeImpostor(PhysicsImpostor* impostor);

  /**
   * @brief Add a joint to the physics engine
   * @param {PhysicsImpostor} mainImpostor the main impostor to which the joint
   * is added.
   * @param {PhysicsImpostor} connectedImpostor the impostor that is connected
   * to the main impostor using this joint
   * @param {PhysicsJoint} the joint that will connect both impostors.
   */
  void addJoint(PhysicsImpostor* mainImpostor,
                PhysicsImpostor* connectedImpostor,
                const std::shared_ptr<PhysicsJoint>& joint);

  void removeJoint(PhysicsImpostor* mainImpostor,
                   PhysicsImpostor* connectedImpostor, PhysicsJoint* joint);

  /**
   * @brief Called by the scene. no need to call it.
   * Hidden
   */
  void _step(float delta);

  IPhysicsEnginePlugin* getPhysicsPlugin();

  std::vector<std::shared_ptr<PhysicsImpostor>>& getImpostors();

  PhysicsImpostor* getImpostorForPhysicsObject(IPhysicsEnabledObject* object);

  PhysicsImpostor* getImpostorWithPhysicsBody(IPhysicsBody* body);

  bool isInitialized() const;

public:
  Vector3 gravity;

private:
  bool _initialized;
  IPhysicsEnginePlugin* _physicsPlugin;
  std::vector<std::shared_ptr<PhysicsImpostor>> _impostors;
  std::vector<std::shared_ptr<PhysicsImpostorJoint>> _joints;

}; // end of class PhysicsEngine

} // end of namespace BABYLON

#endif // end of BABYLON_PHYSICS_PHYSICS_ENGINE_H
