#pragma once

#include <frc/DoubleSolenoid.h>
#include <ctre/phoenix/motorcontrol/can/WPI_VictorSPX.h>
#include "WiringDiagram.h"

class Intake {
 private:
  frc::DoubleSolenoid IntakePiston {frc::PneumaticsModuleType::CTREPCM, WiringDiagram::c_IntakePistonA, WiringDiagram::c_IntakePistonB};
  ctre::phoenix::motorcontrol::can::WPI_VictorSPX IntakeMotor {WiringDiagram::c_IntakeMotor};
 public:
  /**
   * Constructs an Intake object. */
  Intake();

  /**
   * Pulls the pistons in. */
  void In();
  /**
   * Pushes the pistons out. */
  void Out();
  /**
   * Toggles the pistons */
  void Toggle();
  /**
   * Pushes out the front pistons while running the front motor to kick us off of the frame. Returns true if done. */
  bool Init(units::time::second_t startTime, units::time::second_t elapsedTime);

  /**
   * Spins the Intake Forward */
  void SpinForward();
  /**
   * Spins the Intake in Reverse */
  void SpinReverse();
  /**
   * Stops the Intake Motor. */
  void Stop();
};
