#pragma once

#include <frc2/command/PIDCommand.h>

class FrcPID : public frc2::PIDController {
 private:

  struct PIDValues
  {
    double kP = 6e-5, kI = 1e-6, kD = 0, kMaxOutput = 1.0, kMinOutput = -1.0;
    double setpoint = 0;
    double positionTolerance = 1, velocityTolerance = 1;
  };
  PIDValues m_PIDValues;

 public:
  FrcPID(PIDValues inputValues);
  
  double ClampCalculate(double input);
  double ClampCalculate(double input, double setpoint);
}