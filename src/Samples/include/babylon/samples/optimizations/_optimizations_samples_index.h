#ifndef BABYLON_SAMPLES_OPTIMIZATIONS_OPTIMIZATIONS_SAMPLES_INDEX_H
#define BABYLON_SAMPLES_OPTIMIZATIONS_OPTIMIZATIONS_SAMPLES_INDEX_H

#include <babylon/interfaces/irenderable_scene.h>
#include <babylon/samples/samples_common.h>

namespace BABYLON {
namespace Samples {

class BABYLON_SHARED_EXPORT _OptimizationsSamplesIndex : public _ISamplesIndex {

private:
  static constexpr const char* _CategoryName = "Optimizations";

public:
  /**
   * @brief Returns the category name.
   * @return the category name
   */
  static const string_t CategoryName()
  {
    return _OptimizationsSamplesIndex::_CategoryName;
  }

public:
  _OptimizationsSamplesIndex();
  virtual ~_OptimizationsSamplesIndex();

}; // end of class _OptimizationsSamplesIndex

} // end of namespace Samples
} // end of namespace BABYLON

#endif // end of BABYLON_SAMPLES_OPTIMIZATIONS_OPTIMIZATIONS_SAMPLES_INDEX_H