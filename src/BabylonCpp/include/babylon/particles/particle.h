#ifndef BABYLON_PARTICLES_PARTICLE_H
#define BABYLON_PARTICLES_PARTICLE_H

#include <babylon/babylon_api.h>
#include <babylon/math/color4.h>
#include <babylon/math/vector2.h>
#include <babylon/math/vector3.h>
#include <babylon/tools/color_gradient.h>
#include <babylon/tools/factor_gradient.h>

namespace BABYLON {

class ParticleSystem;

/**
 * @brief A particle represents one of the element emitted by a particle system.
 * This is mainly define by its coordinates, direction, velocity and age.
 */
class BABYLON_SHARED_EXPORT Particle {

public:
  /**
   * @brief Creates a new instance Particle.
   * @param particleSystem the particle system the particle belongs to
   */
  Particle(ParticleSystem* particleSystem);
  Particle(const Particle& other);            // Copy constructor
  Particle(Particle&& other);                 // Move constructor
  Particle& operator=(const Particle& other); // Copy assignment operator
  Particle& operator=(Particle&& other);      // Move assignment operator
  ~Particle();                                // Destructor
  Particle* clone() const;

  /**
   * @brief Defines how the sprite cell index is updated for the particle.
   */
  void updateCellIndex();

  /**
   * @brief Copy the properties of particle to another one.
   * @param other the particle to copy the information to.
   */
  void copyTo(Particle& other);

private:
  void updateCellInfoFromSystem();

public:
  /**
   * The world position of the particle in the scene.
   */
  Vector3 position;

  /**
   * The world direction of the particle in the scene.
   */
  Vector3 direction;

  /**
   * The color of the particle.
   */
  Color4 color;

  /**
   * The color change of the particle per step.
   */
  Color4 colorStep;

  /**
   * Defines how long will the life of the particle be.
   */
  float lifeTime;

  /**
   * The current age of the particle.
   */
  float age;

  /**
   * The current size of the particle.
   */
  float size;

  /**
   * The current scale of the particle.
   */
  Vector2 scale;

  /**
   * The current angle of the particle.
   */
  float angle;

  /**
   * Defines how fast is the angle changing.
   */
  float angularSpeed;

  /**
   * Defines the cell index used by the particle to be rendered from a sprite.
   */
  unsigned int cellIndex;

  /** Hidden */
  std::optional<Vector3> _initialDirection;

  /** Hidden */
  unsigned int _initialStartSpriteCellID;
  unsigned int _initialEndSpriteCellID;

  /** Hidden */
  std::optional<ColorGradient> _currentColorGradient;
  /** Hidden */
  Color4 _currentColor1;
  /** Hidden */
  Color4 _currentColor2;

  /** Hidden */
  std::optional<FactorGradient> _currentSizeGradient;
  /** Hidden */
  float _currentSize1;
  /** Hidden */
  float _currentSize2;

  /** Hidden */
  std::optional<FactorGradient> _currentAngularSpeedGradient;
  /** Hidden */
  float _currentAngularSpeed1;
  /** Hidden */
  float _currentAngularSpeed2;

  /** Hidden */
  std::optional<FactorGradient> _currentVelocityGradient;
  /** Hidden */
  float _currentVelocity1;
  /** Hidden */
  float _currentVelocity2;

  /** Hidden */
  std::optional<FactorGradient> _currentLimitVelocityGradient;
  /** Hidden */
  float _currentLimitVelocity1;
  /** Hidden */
  float _currentLimitVelocity2;

  /** Hidden */
  std::optional<FactorGradient> _currentDragGradient;
  /** Hidden */
  float _currentDrag1;
  /** Hidden */
  float _currentDrag2;

  /**
   * The particle system the particle belongs to.
   */
  ParticleSystem* particleSystem;

private:
  unsigned int _currentFrameCounter;

}; // end of class Particle

} // end of namespace BABYLON

#endif // end of BABYLON_PARTICLES_PARTICLE_H
