#pragma once

#include <rev/CANSparkMax.h>

#include "WiringDiagram.h"

class Flywheel : public rev::CANSparkMax
{
 public:
  Flywheel();
};
