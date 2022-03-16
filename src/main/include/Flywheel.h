#pragma once

#include <rev/CANSparkMax.h>

#include "WiringDiagram.h"

class Flywheel
{
 private:
  rev::CANSparkMax m_Flywheel{WiringDiagram::c_Flywheel, rev::CANSparkMax::MotorType::kBrushless};
  rev::SparkMaxPIDController m_PID = m_Flywheel.GetPIDController();
  rev::SparkMaxRelativeEncoder m_Encoder = m_Flywheel.GetEncoder();

  struct PIDValues
  {
    double kP = 0.00015, kI = 0.000001, kD = 0.00005, kIz = 100, kFF = 0.000175, kMaxOutput = 1.0, kMinOutput = -1.0;

    double setpoint = 0;
    double velocityTolerance = 100;
  };
  PIDValues m_PIDValues;

 public:
  /**
   * Contructs a Flywheel object.*/
  Flywheel();

  /**
   * Sets a new setpoint for the Flywheel.
   * @param setpoint The new setpoint */
  void SetReference(double setpoint);
  /**
   * Stops the motor, call SetReference again to start it back up at a new reference, or StartMotor to start it back up at the old reference. */
  void StopMotor();

  /**
   * Returns true if the Velocity of the motor is within the value defined in PIDValues.velocityTolerance. */
  bool InVelocityRange();
};
