#include "Turret.h"

Turret::Turret()
{
    // Sets the elevator encoder to the proper distane per pulse. This allows for us to get more useful distance readings.
    m_ElevatorEncoder.SetDistancePerPulse(ElevatorDistancePerPulse);
    m_IndexMotor.SetInverted(true);
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
    }
    else
    {
        m_ElevatorMotor.Set(0);
        m_ElevatorEncoder.Reset();
    }
}

// TODO: Tune function.
double Turret::AngleToEleCounts(double angle)
{
    return angle;
}

// TODO: Tune function.
double Turret::AngleToRPM(double angle)
{
    return angle;
}