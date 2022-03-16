#include "Robot.h"

#include <fmt/core.h>

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>

frc::Timer m_Timer;

void Robot::RobotInit()
{
  m_Timer.Start();

  m_Xbox = new frc::XboxController(WiringDiagram::c_Xbox);
  m_Turret = new Turret();
  m_Intake = new Intake();
  m_Hanger = new Hanger();
  m_Drive = new Drive();
  m_Limelight = new Limelight();

  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit()
{
  resetDone = false;
  m_Timer.Reset();

  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  fmt::print("Auto selected: {}\n", m_autoSelected);

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic()
{
  if (!resetDone)
  {
    if(m_Intake->Init(0_s, m_Timer.Get()) && m_Turret->ZeroElevator()) { resetDone = true; }
  }
  else
  {
    if (m_autoSelected == kAutoNameCustom)
    {
      // Custom Auto goes here
    }
    else
    {
      // Default Auto goes here
    }
  }
}

void Robot::TeleopInit()
{
  resetDone = false;
  m_Timer.Reset();
}

void Robot::TeleopPeriodic()
{
  // TODO: POSSIBLY REMOVE BEFORE COMPETITION, ONLY NEEDS TO BE RESET ONCE.
  if(!resetDone)
  {
    if(m_Intake->Init(0_s, m_Timer.Get()) && m_Turret->ZeroElevator()) { resetDone = true; }
  }
  else
  {
    if(m_Xbox->GetRightBumperPressed())
    {
      intakeOut = !intakeOut;
      m_Intake->Toggle();
    }
    frc::SmartDashboard::PutBoolean("Intake Out?", intakeOut);

    if(m_Xbox->GetLeftBumperPressed())
    {
      intakeOut = !intakeOut;
      m_Intake->Toggle();
    }
    frc::SmartDashboard::PutBoolean("Hanger Out?", hangerOut);

    if(m_Xbox->GetPOV() == 90 && m_Xbox->GetAButton())
    {
      aimOverridden = true;
    }
    else if(m_Xbox->GetPOV() == 180 && m_Xbox->GetBButton())
    {
      aimOverridden = false;
    }
    frc::SmartDashboard::PutBoolean("Aim Overridden", intakeOut);

    if(!targeting)
    {
      m_Drive->MecanumDrive(m_Xbox->GetLeftY(), -m_Xbox->GetLeftX(), -m_Xbox->GetRightX());
    }
    else
    {
      if(!aimOverridden)
      {
        AimedControl();
      }
      else
      {
        AimOverriddenControl();
      }
    }
  }
}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}
void Robot::SimulationPeriodic() {}

void Robot::AimOverriddenControl()
{
  frc::SmartDashboard::PutBoolean("Locked On", lockedOn);

  if(m_Xbox->GetPOV() == 0)
  {
    overriddenRPM = 2500;
    overriddenElevator = 10;
  }
  else if(m_Xbox->GetPOV() == 270)
  {
    overriddenRPM = 4000;
    overriddenElevator = 30;
  }

  if(m_Turret->RunElevator(overriddenElevator) &&
     m_Turret->SpinFlywheel(overriddenRPM))
  {
    lockedOn = true;
  }

  // Repeatedly set the last Shot time to the current time so that when we finally lock on, we approximately now when we started shooting.
  if(!lockedOn)
  {
    lastShotTime = m_Timer.Get();
  }
  else
  {
    m_Turret->SetIndex(1);
    m_Intake->SpinForward();

    // If the elapsed time since the shot is greater than two seconds, stop the motor and stop targeting.
    if(m_Timer.Get() - lastShotTime > 2_s)
    {
      m_Turret->SetIndex(0);
      m_Intake->Stop();
      lockedOn = false;
      targeting = false;
    }
  }

  if(m_Xbox->GetYButtonPressed())
  {
    m_Turret->SetIndex(0);
    m_Intake->Stop();
    lockedOn = false;
    targeting = false;
  }
}

void Robot::AimedControl()
{
  frc::SmartDashboard::PutBoolean("Locked On", lockedOn);

  double xOffset = m_Limelight->GetX();
  double strafePower = -m_Xbox->GetLeftX();
  double drivePower = m_Xbox->GetLeftY();

  double elevatorSetpoint = m_Turret->AngleToEleCounts(m_Limelight->GetY());
  double flywheelRPM = m_Turret->AngleToRPM(m_Limelight->GetY());

  // Set the turret and drive to target the desired values. Passes if all of the pieces are in range.
  if(m_Drive->MecanumDrivePID(xOffset, strafePower, drivePower) &&
      m_Turret->RunElevator(elevatorSetpoint) && 
      m_Turret->SpinFlywheel(flywheelRPM))
  {
    lockedOn = true;
  }

  // Repeatedly set the last Shot time to the current time so that when we finally lock on, we approximately now when we started shooting.
  if(!lockedOn)
  {
    lastShotTime = m_Timer.Get();
  }
  else
  {
    m_Turret->SetIndex(1);
    m_Intake->SpinForward();

    // If the elapsed time since the shot is greater than two seconds, stop the motor and stop targeting.
    if(m_Timer.Get() - lastShotTime > 2_s)
    {
      m_Turret->SetIndex(0);
      m_Intake->Stop();
      lockedOn = false;
      targeting = false;
    }
  }

  if(m_Xbox->GetYButtonPressed())
  {
    m_Turret->SetIndex(0);
    m_Intake->Stop();
    lockedOn = false;
    targeting = false;
  }
}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
