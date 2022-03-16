#include "FrcPID.h"

FrcPID::FrcPID(PIDValues inputValues) : PIDController(m_PIDValues.kP, m_PIDValues.kI, m_PIDValues.kD)
{
    m_PIDValues = inputValues;

    SetPID(m_PIDValues.kP, m_PIDValues.kI, m_PIDValues.kD);
    SetSetpoint(m_PIDValues.setpoint);
    SetTolerance(m_PIDValues.positionTolerance, m_PIDValues.velocityTolerance);
}

double FrcPID::ClampCalculate(double input)
{
    // Runs the inherited Calculate method from the frc2/command/PIDController file then stores it in output.
    double output = Calculate(input);

    // Clamps output to the range between the MinOutput and MaxOutput.
    if(output > m_PIDValues.kMaxOutput) { output = m_PIDValues.kMaxOutput; }
    if(output < m_PIDValues.kMinOutput) { output = m_PIDValues.kMinOutput; }

    return output;
}

// Overloads the fucntion above. In other words, the function above runs if there is only one input, the function below runs if there are two inputs.
double FrcPID::ClampCalculate(double input, double setpoint)
{
    // Runs the inherited Calculate method from the frc2/command/PIDController file then stores it in output.
    m_PIDValues.setpoint = setpoint;

    // Clamps output to the range between the MinOutput and MaxOutput.
    double output = Calculate(input, m_PIDValues.setpoint);
    if(output > m_PIDValues.kMaxOutput) { output = m_PIDValues.kMaxOutput; }
    if(output < m_PIDValues.kMinOutput) { output = m_PIDValues.kMinOutput; }

    return output;
}