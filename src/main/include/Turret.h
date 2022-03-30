#pragma once

#include <rev/CANSparkMax.h>
#include <frc/Encoder.h>
#include <frc/DigitalInput.h>
#include <ctre/phoenix/motorcontrol/can/WPI_VictorSPX.h>

#include "Flywheel.h"
#include "FrcPID.h"
#include "WiringDiagram.h"

class Turret {
 private:
  Flywheel m_Flywheel;
  ctre::phoenix::motorcontrol::can::WPI_VictorSPX m_IndexMotor {WiringDiagram::c_IndexMotor};

  FrcPID::PIDValues ElevatorValues
  { 
    ElevatorValues.kP = .5, ElevatorValues.kI = 0, ElevatorValues.kD = 0, ElevatorValues.kMaxOutput = 1.0, ElevatorValues.kMinOutput = -1.0,
    ElevatorValues.setpoint = 0,
    ElevatorValues.positionTolerance = 1, ElevatorValues.velocityTolerance = 1
  };
  FrcPID m_ElevatorPID {ElevatorValues};

  rev::CANSparkMax m_ElevatorMotor {WiringDiagram::c_Elevator, rev::CANSparkMax::MotorType::kBrushed};
  frc::Encoder m_ElevatorEncoder {WiringDiagram::c_ElevatorA, WiringDiagram::c_ElevatorB};
  frc::DigitalInput m_ElevatorLimit {WiringDiagram::c_ElevatorLimit};
  double ElevatorDistancePerPulse = -1.0/22.0;
  double ElevatorMaxDistance = 30.0;

 public:
  /**
   * Constructs a Turret object */
  Turret();

  /**
   * Initializes the Elevator, running it down to the limit switch and setting it to zero. Returns true if done.*/
  bool ZeroElevator();

  /**
   * Turns an angle from the limelight into a useful value like RPM for the flywheel. */
  double AngleToRPM(double angle);
  /**
   * Spins the Flywheel at a set RPM
   * @param RPM the setpoint RPM of the motor */
  bool SpinFlywheel(double RPM);
  /**
   * Stops the Flywheel motor */
  void StopFlywheel();
  /**
   * Converts a limelight angle to counts on the Elevator shaft.
   * @param angle The limelight angle to be converted */
  double AngleToEleCounts(double angle);
  /**
   * Sets the elevator to run from an inputted limelight angle
   * @param setpoint The target number of counts on the elevator encoder. */
  bool RunElevator(double setpoint);
  /**
   * Sets the Elevator speed to 0 */
  void StopElevator();
  /**
   * Sets the Index Motor's percent power
   * @param power The percent power of the index motor */
  void SetIndex(double power);

  bool FlywheelKickSeen();

  /**
   * Returns true if the Flywheel is in the desired range */
  bool FlywheelInRange();
  /**
   * Returns true if the Elevator is in the desired range */
  bool ElevatorInRange();
};
