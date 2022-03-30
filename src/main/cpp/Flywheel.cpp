#include "Flywheel.h"

// TODO: REMOVE TEMP SMARTDASHBOARD STUFF
#include <frc/smartdashboard/SmartDashboard.h>

// Uses the default constructor for the Flywheel object, in other words, we don't need to run any special code or take any inputs.
Flywheel::Flywheel()
{
    m_PID.SetP(m_PIDValues.kP);
    m_PID.SetI(m_PIDValues.kI);
    m_PID.SetD(m_PIDValues.kD);
    m_PID.SetIZone(m_PIDValues.kIz);
    m_PID.SetFF(m_PIDValues.kFF);
    m_PID.SetOutputRange(m_PIDValues.kMinOutput, m_PIDValues.kMaxOutput);
}

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
    // TODO: REMOVE TEMP SMARTDASHBOARD STUFF
    frc::SmartDashboard::PutNumber("RPM", m_Encoder.GetVelocity());
    frc::SmartDashboard::PutNumber("Flywheel Setpoint", m_PIDValues.setpoint);

    if(m_Encoder.GetVelocity() < m_PIDValues.setpoint + m_PIDValues.velocityTolerance && m_Encoder.GetVelocity() > m_PIDValues.setpoint - m_PIDValues.velocityTolerance)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Flywheel::KickSeen()
{
    return m_Encoder.GetVelocity() < m_PIDValues.setpoint - 100;
}