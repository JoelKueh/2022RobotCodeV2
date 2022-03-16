#pragma once

#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/XboxController.h>

#include "Drive.h"
#include "Turret.h"
#include "Hanger.h"
#include "Intake.h"
#include "Limelight.h"
#include "WiringDiagram.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void DisabledInit() override;
  void DisabledPeriodic() override;
  void TestInit() override;
  void TestPeriodic() override;
  void SimulationInit() override;
  void SimulationPeriodic() override;

 private:
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;

  units::time::second_t lastShotTime;

  double overriddenRPM = 2500;
  double overriddenElevator = 10;

  bool resetDone = false;
  bool aimOverridden = false;

  bool intakeOut = true;
  bool hangerOut = false;

  bool targeting = false;
  bool lockedOn = false;

  frc::XboxController* m_Xbox;
  Drive* m_Drive;
  Turret* m_Turret;
  Hanger* m_Hanger;
  Intake* m_Intake;
  Limelight* m_Limelight;

  void AimOverriddenControl();
  void AimedControl();
};
