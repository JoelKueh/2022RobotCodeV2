#include "Flywheel.h"

// Uses the default constructor for the Flywheel object, in other words, we don't need to run any special code or take any inputs.
Flywheel::Flywheel() = default;

void Flywheel::SetReference(double setpoint)
{
    m_PID.SetReference(setpoint, rev::CANSparkMax::ControlType::kVelocity, 0);
    m_PIDValues.setpoint = setpoint;
}

void Flywheel::StopMotor()
{
    m_Flywheel.StopMotor();
}

bool Flywheel::InVelocityRange()
{
    if(m_Encoder.GetVelocity() < m_PIDValues.setpoint + m_PIDValues.velocityTolerance && m_Encoder.GetVelocity() > m_PIDValues.setpoint - m_PIDValues.velocityTolerance)
    {
        return true;
    }
    else
    {
        return false;
    }
}