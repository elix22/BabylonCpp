#ifndef BABYLON_PHYSICS_JOINT_SPRING_JOINT_DATA_H
#define BABYLON_PHYSICS_JOINT_SPRING_JOINT_DATA_H

#include <babylon/babylon_api.h>
#include <babylon/physics/joint/physics_joint_data.h>

namespace BABYLON {

struct BABYLON_SHARED_EXPORT SpringJointData : public PhysicsJointData {
  float length;
  float stiffness;
  float damping;
}; // end of struct DistanceJointData

} // end of namespace BABYLON

#endif // end of BABYLON_PHYSICS_JOINT_SPRING_JOINT_DATA_H
