#include "Turret.h"

#include <math.h>

Turret::Turret()
{
    // Sets the elevator encoder to the proper distane per pulse. This allows for us to get more useful distance readings.
    m_ElevatorEncoder.SetDistancePerPulse(ElevatorDistancePerPulse);
    m_IndexMotor.SetInverted(true);
    m_IndexMotor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}

bool Turret::SpinFlywheel(double RPM)
{
    m_Flywheel.SetReference(RPM);
    return m_Flywheel.InVelocityRange();
}

void Turret::StopFlywheel()
{
    m_Flywheel.StopMotor();
}

void Turret::SetIndex(double power)
{
    m_IndexMotor.Set(power);
}

bool Turret::RunElevator(double targetCounts)
{
    // Runs the Elevator PID and calculates an output.
    double distance = m_ElevatorEncoder.GetDistance();
    double elevatorPower = m_ElevatorPID.Calculate(distance, targetCounts);

    // Makes sure the elevator doesn't overrun itself.
    if(!m_ElevatorLimit.Get() && elevatorPower < 0)
    {
        m_ElevatorMotor.Set(0);
    }
    else if(distance > ElevatorMaxDistance && elevatorPower > 0)
    {
        m_ElevatorMotor.Set(0);
    }
    else
    {
        m_ElevatorMotor.Set(elevatorPower);
    }

    return m_ElevatorPID.AtSetpoint();
}

void Turret::StopElevator()
{
    m_ElevatorMotor.Set(0);
}

bool Turret::ZeroElevator()
{
    if(m_ElevatorLimit.Get())
    {
        m_ElevatorMotor.Set(-1);
        return false;
    }
    else
    {
        m_ElevatorMotor.Set(0);
        m_ElevatorEncoder.Reset();
        return true;
    }
}

bool Turret::FlywheelKickSeen()
{
    return m_Flywheel.KickSeen();
}

double Turret::AngleToEleCounts(double angle)
{
    // OLD return pow(1.125, -angle + 1.7) + 1.8;
    // OLD return 0.01647 * pow(angle, 2) - .5202 * angle + 9.079;
    
    // FINAL
    return 0.01647 * pow(angle, 2) - .5202 * angle + 9.079;
}

double Turret::AngleToRPM(double angle)
{
    // OLD return angle * -40.0 + 3350.0;
    // OLD return 1.985 * pow(angle, 2) - 58.8 * angle + 2904.4;

    // TODO: CHECK THIS, MAY NEED A RETUNE
    // return .9238 * pow(angle, 2) - 42 * angle + 3175;

    // TODO: REVERT BACK TO THE EQUATION
    // return 2000;

    // FINAL?
    // return 2.56 * pow(angle - 2, 2) - 65.405 * (angle - 2) + 3082.8;

    if(angle < -9)
    {
        return -160 * (angle + 9) + 3627.828;
    }
    else
    {
        return .9238 * pow(angle, 2) - 42 * angle + 3175;
    }
}