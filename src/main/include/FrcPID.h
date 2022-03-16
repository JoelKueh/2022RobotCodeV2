#pragma once

#include <frc2/command/PIDCommand.h>

// INHERITANCE
class FrcPID : public frc2::PIDController {
 public:

  // Defines a struct that contains all of the necessary values to run the PID
  struct PIDValues
  {
    // Sets default values for that struct
    double kP = 6e-5, kI = 1e-6, kD = 0, kMaxOutput = 1.0, kMinOutput = -1.0;
    double setpoint = 0;
    double positionTolerance = 1, velocityTolerance = 1;
  };
  // Creates a struct with the values above. These values will be set in the constructor of the class.
  PIDValues m_PIDValues;

  /**
   * Constructs an FrcPID
   * 
   * @param inputValues Easy input for the many PID tuning values. */
  FrcPID(PIDValues inputValues);
    
  /**
   * Returns the next output of the controller clamped from MinOutput to MaxOutput
   * 
   * @param input The process variable */
  double ClampCalculate(double input);
  /**
   * Sets the setpoint, then returns the next output of the controller clamped from MinOutput to MaxOutput
   * 
   * @param input The process variable
   * @param setpoint The new Setpoint */
  double ClampCalculate(double input, double setpoint);
};