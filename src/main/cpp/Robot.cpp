#include "Robot.h"

#include <fmt/core.h>
#include <cmath>

#include <wpi/PortForwarder.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>

void Robot::RobotInit()
{
  m_Timer = new frc::Timer();
  m_Xbox = new frc::XboxController(WiringDiagram::c_Xbox);
  m_Gyro = new frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0);
  m_AutoSwitch = new frc::DigitalInput(WiringDiagram::c_AutoSwitch);
  m_Turret = new Turret();
  m_Intake = new Intake();
  m_Hanger = new Hanger();
  m_Drive = new Drive();
  m_Limelight = new Limelight();

  m_Timer->Start();
  m_Gyro->Calibrate();
  m_Limelight->LEDOn();

  // Port Forwarding the Limelight to work over USB.
  wpi::PortForwarder::GetInstance().Add(5800, "10.21.43.11", 5800);
  wpi::PortForwarder::GetInstance().Add(5801, "10.21.43.11", 5801);
  wpi::PortForwarder::GetInstance().Add(5802, "10.21.43.11", 5802);
  wpi::PortForwarder::GetInstance().Add(5803, "10.21.43.11", 5803);
  wpi::PortForwarder::GetInstance().Add(5804, "10.21.43.11", 5804);
  wpi::PortForwarder::GetInstance().Add(5805, "10.21.43.11", 5805);

  // TESTING SHOT STUFF
  // frc::SmartDashboard::PutNumber("Testing Elevator Setpoint", 0);
  // frc::SmartDashboard::PutNumber("Testing RPM", 0);
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
  m_Gyro->Reset();
  m_Timer->Reset();
  m_Hanger->In();
  m_Intake->Out();

  m_AutoSwitchState = !m_AutoSwitch->Get();
  autoState = 0;
}

void Robot::AutonomousPeriodic()
{
  m_Drive->UpdateMecanumDrive();

  if (!resetDone)
  {
    if(m_Intake->Init(0_s, m_Timer->Get()) && m_Turret->ZeroElevator()) { resetDone = true; m_Timer->Reset(); }
  }
  else
  {
    if(m_AutoSwitchState)
    {
      ThreeBallAuto();
    }
    else
    {
      TwoBallAuto();
    }
  }
}

void Robot::TeleopInit()
{
  StopTargeting();
  resetDone = false;
  m_Timer->Reset();
  m_Hanger->In();
  m_Intake->Out();
}

void Robot::TeleopPeriodic()
{
  if(!resetDone)
  {
    if(m_Intake->Init(0_s, m_Timer->Get()) && m_Turret->ZeroElevator()) { resetDone = true; }
  }
  else
  {
    m_Drive->UpdateMecanumDrive();

    double xboxRB = m_Xbox->GetRightBumperPressed();
    if(m_Xbox->GetLeftBumper() && xboxRB)
    {
      hangerOut = !hangerOut;
      m_Hanger->Toggle();
    }

    if(m_Xbox->GetPOV() == 0 && xboxRB)
    {
      intakeOut = !intakeOut;
      m_Intake->Toggle();
    }

    if(m_Xbox->GetPOV() == 90 && m_Xbox->GetBButtonPressed())
    {
      aimOverridden = true;
    }
    else if(m_Xbox->GetPOV() == 180 && m_Xbox->GetAButtonPressed())
    {
      aimOverridden = false;
    }
    frc::SmartDashboard::PutBoolean("Aim Overridden", aimOverridden);

    if(m_Xbox->GetYButtonPressed())
    {
      targeting = !targeting;
    }
    frc::SmartDashboard::PutBoolean("Targeting?", targeting);

    if(targeting && m_Limelight->TargetFound())
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
    else
    {
      StopTargeting();

      m_Drive->MecanumDrive(m_Xbox->GetLeftY(), -m_Xbox->GetLeftX(), -m_Xbox->GetRightX());

      if(m_Xbox->GetRightTriggerAxis() > .15)
      {
        m_Intake->SpinForward();
        m_Turret->SetIndex(-.15);
      }
      else if(m_Xbox->GetLeftTriggerAxis() > .15)
      {
        m_Intake->SpinReverse();
      }
      else
      {
        m_Intake->Stop();
        m_Turret->SetIndex(0);
      }

      frc::SmartDashboard::PutBoolean("Locked On", lockedOn);
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

  bool inElevatorRange = m_Turret->RunElevator(overriddenElevator);
  bool inFlywheelRange = m_Turret->SpinFlywheel(overriddenRPM);

  if((inElevatorRange && inFlywheelRange) || m_Xbox->GetXButtonPressed())
  {
    m_Turret->SetIndex(1);
    m_Intake->SpinForward();
  }
}

bool Robot::AimedControl()
{
  frc::SmartDashboard::PutBoolean("Locked On", lockedOn);

  double xOffset = m_Limelight->GetX();
  double strafePower = -m_Xbox->GetLeftX();
  double drivePower = m_Xbox->GetLeftY();

  double elevatorSetpoint = m_Turret->AngleToEleCounts(m_Limelight->GetY());
  double flywheelRPM = m_Turret->AngleToRPM(m_Limelight->GetY());

  bool inAngleRange = m_Drive->MecanumDrivePID(xOffset, strafePower, drivePower);
  bool inEleRange = m_Turret->RunElevator(elevatorSetpoint);
  bool inFlywheelRange = m_Turret->SpinFlywheel(flywheelRPM);

  switch (aimingState)
  {
    case 0:
    {
      if((inAngleRange && inEleRange && inFlywheelRange &&
        strafePower < .1 && strafePower > -.1 && drivePower < .1 && strafePower > -.1) ||
        m_Xbox->GetXButtonPressed())
      {
        lockedOn = true;
        aimingState++;

        m_Turret->SetIndex(1);
        m_Intake->SpinForward();
        
        lastShotTime = m_Timer->Get();
      }
      break;
    }
    case 1:
    {
      if(m_Turret->FlywheelKickSeen())
      {
        lastKickTime = m_Timer->Get();
        m_Turret->SetIndex(0);
        aimingState++;
      }
      break;
    }
    case 2:
    {
      if(m_Timer->Get() - lastKickTime > .65_s)
      {
        m_Turret->SetIndex(1);

        aimingState++;
      }
      break;
    }
    case 3:
    {
      if(m_Timer->Get() - lastKickTime > .8_s)
      {
        StopTargeting();
        return true;
      }
      break;
    }
    default: break;
  }

  // REMOVE?
  frc::SmartDashboard::PutBoolean("In Flywheel Range", inFlywheelRange);
  frc::SmartDashboard::PutBoolean("In Drive Range", inAngleRange);
  frc::SmartDashboard::PutBoolean("In Elevator Range", inEleRange);
  
  return false;
}

void Robot::ThreeBallAuto()
{
  switch (autoState)
  {
    case 0:
    {
      // DRIVING NEGATIVE IS DRIVING FORWARDS
      m_Drive->MecanumDrive(-.5, 0, 0);
      m_Intake->SpinForward();
      m_Turret->SetIndex(-.15);

      m_Turret->SpinFlywheel(2750);
      m_Turret->RunElevator(5);

      if(m_Timer->Get() > .8_s) { autoState++; m_Drive->MecanumDrive(0, 0, 0); }
      break;
    }
    case 1:
    {
      m_Turret->SpinFlywheel(2750);
      m_Turret->RunElevator(5);

      if(m_Timer->Get() > 1.4_s) { autoState++; }
      break;
    }
    case 2:
    {
      m_Drive->MecanumDrive(1, 0, 0);

      m_Turret->SpinFlywheel(2750);
      m_Turret->RunElevator(5);

      if(m_Timer->Get() > 1.925_s) { autoState++; m_Drive->MecanumDrive(0, 0, 0); }
      break;
    }
    case 3:
    {
      double gyro = std::fmod(m_Gyro->GetAngle(), 360);
      // frc::SmartDashboard::PutNumber("Gyro", gyro);
      m_Drive->MecanumDrivePID(205 - gyro);

      bool inFlywheelRange = m_Turret->SpinFlywheel(2750);
      bool inDriveRange = m_Turret->RunElevator(5);

      if(m_Drive->AtSetpoint() && inFlywheelRange && inDriveRange)
      {
        lockedOn = true;
      }

      if(lockedOn)
      {
        if(ShootUnaimed())
        {
          autoState++;
        }
      }
      break;
    }
    case 4:
    {
      double gyro = std::fmod(m_Gyro->GetAngle(), 360);
      // frc::SmartDashboard::PutNumber("Gyro", gyro);
      m_Drive->MecanumDrivePID(90 - gyro);

      if(m_Drive->AtSetpoint()) { autoState++; autoSecondTurnTime = m_Timer->Get(); }
      break;
    }
    case 5:
    {
      m_Drive->MecanumDrive(-1, 0, 0);

      m_Turret->SpinFlywheel(3400);
      m_Turret->RunElevator(18);

      if(m_Timer->Get() > autoSecondTurnTime + 1.2_s) { autoState++; m_Drive->MecanumDrive(0, 0, 0); }
      break;
    }
    case 6:
    {
      m_Turret->SpinFlywheel(3400);
      m_Turret->RunElevator(18);

      if(m_Timer->Get() > autoSecondTurnTime + 1.8_s) { autoState++; lockedOn = false; }
      break;
    }
    case 7:
    {
      double gyro = std::fmod(m_Gyro->GetAngle(), 360);
      // frc::SmartDashboard::PutNumber("Gyro", gyro);
      m_Drive->MecanumDrivePID(245 - gyro);

      bool inFlywheelRange = m_Turret->SpinFlywheel(3400);
      bool inDriveRange = m_Turret->RunElevator(14);

      if(m_Drive->AtSetpoint() && inFlywheelRange && inDriveRange)
      {
        lockedOn = true;
      }

      if(lockedOn)
      {
        if(ShootUnaimed())
        {
          aimingState++;
        }
      }
      break;
    }
    default: break;
  }
}

void Robot::TwoBallAuto()
{
  switch (autoState)
  {
    case 0:
    {
      // DRIVING NEGATIVE IS DRIVING FORWARDS
      m_Drive->MecanumDrive(-.5, 0, 0);
      m_Intake->SpinForward();
      m_Turret->SetIndex(-.15);

      m_Turret->SpinFlywheel(2750);
      m_Turret->RunElevator(5);

      if(m_Timer->Get() > 1.4_s) { autoState++; m_Drive->MecanumDrive(0, 0, 0); }
      break;
    }
    case 1:
    {
      m_Turret->SpinFlywheel(2750);
      m_Turret->RunElevator(5);

      if(m_Timer->Get() > 1.5_s) { autoState++; }
      break;
    }
    case 2:
    {
      double gyro = std::fmod(m_Gyro->GetAngle(), 360);
      // frc::SmartDashboard::PutNumber("Gyro", gyro);
      m_Drive->MecanumDrivePID(180 - gyro);

      if(m_Drive->AtSetpoint()) { autoState++; }
      break;
    }
    case 3:
    {
      if(AimedControl())
      {
        autoState++;
      }
      break;
    }
  }
}

void Robot::StopTargeting()
{
  m_Turret->SetIndex(0);
  m_Turret->StopElevator();
  m_Turret->StopFlywheel();
  aimingState = 0;
  targeting = false;
  lockedOn = false;
}

bool Robot::ShootUnaimed()
{
  switch (aimingState)
  {
    case 0:
    {
      lockedOn = true;
      aimingState++;

      m_Turret->SetIndex(1);
      m_Intake->SpinForward();
        
      lastShotTime = m_Timer->Get();
      break;
    }
    case 1:
    {
      if(m_Timer->Get() > lastShotTime + .15_s)
      {
        m_Turret->SetIndex(0);
        aimingState++;
      }
      break;
    }
    case 2:
    {
      if(m_Timer->Get() > lastKickTime + .8_s)
      {
        m_Turret->SetIndex(1);

        aimingState++;
      }
      break;
    }
    case 3:
    {
      if(m_Timer->Get() > lastKickTime + .95_s)
      {
        StopTargeting();
        return true;
      }
      break;
    }
    default: break;
  }
  return false;
}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
