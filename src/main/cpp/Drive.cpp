#include "Drive.h"

Drive::Drive()
{
    // Sets the three moters that we need to be inverted (not all of them do)
    m_frontLeft.SetInverted(true);
    m_frontRight.SetInverted(true);
    m_backLeft.SetInverted(true);

    m_DrivePID.EnableContinuousInput(0, 360);

    // The memory allocation thing referecned in line 49 of Drive.h
    m_MecanumDrive = new frc::MecanumDrive(m_frontLeft, m_backLeft, m_frontRight, m_backRight);
}

void Drive::UpdateMecanumDrive()
{
    m_MecanumDrive->DriveCartesian(currentDrivePower, currentStrafePower, currentTurnPower);
}

void Drive::MecanumDrive(double drivePower, double strafePower, double turnPower)
{
    // Checks to see if variables are between -.15 and .15, if any of them are, they get set down to 0.
    if(drivePower < .15 && drivePower > -.15) { drivePower = 0; }
    if(strafePower < .15 && strafePower > -.15) { strafePower = 0; }
    if(turnPower < .15 && turnPower > -.15) { turnPower = 0; }

    currentDrivePower = drivePower;
    currentStrafePower = strafePower;
    currentTurnPower = turnPower;
}

bool Drive::MecanumDrivePID(double angleError)
{
    // Takes the output from ClampCalculate and gives it a little bit of a boost.
    // If the values outputed by ClampCalculate are lwo enough, then we can't actually turn the motors that we need to turn. For example:
    // .01 isn't enough to turn the motors at all, but .06 is, so we just give it a little bit of a boost so that it can actually turn the wheels.
    double output = m_DrivePID.ClampCalculate(angleError);
    if(output > 0)
    {
        output = output + .05;
    }
    else if(output < 0)
    {
        output = output - .05;
    }
    currentTurnPower = output;
    currentDrivePower = 0;
    currentStrafePower = 0;
    return m_DrivePID.AtSetpoint();
}

// An overload of the function above. Talked about in FrcPID.cpp line 24
bool Drive::MecanumDrivePID(double angleError, double strafePower)
{
    double output = m_DrivePID.ClampCalculate(angleError);
    if(output > 0)
    {
        output = output + .05;
    }
    else if(output < 0)
    {
        output = output - .05;
    }
    currentTurnPower = output;
    currentDrivePower = 0;
    currentStrafePower = strafePower;
    return m_DrivePID.AtSetpoint();
}

// A second overload of the MecanumDrivePID function. (Yes, you can do that).
bool Drive::MecanumDrivePID(double angleError, double strafePower, double drivePower)
{
    double output = m_DrivePID.ClampCalculate(angleError);
    if(output > 0)
    {
        output = output + .075;
    }
    else if(output < 0)
    {
        output = output - .075;
    }
    currentTurnPower = output;
    currentDrivePower = drivePower;
    currentStrafePower = strafePower;
    return m_DrivePID.AtSetpoint();
}

bool Drive::AtSetpoint()
{
    return m_DrivePID.AtSetpoint();
}