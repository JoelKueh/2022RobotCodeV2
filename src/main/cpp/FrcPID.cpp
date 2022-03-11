#include "FrcPID.h"

/**
 * Constructs an FrcPID
 * 
 * @param inputValues Easy input for the many PID tuning values. */
FrcPID::FrcPID(PIDValues inputValues) : PIDController()
{
    m_PIDValues = inputValues;

    SetPID(m_PIDValues.kP, m_PIDValues.kI, m_PIDValues.kD);
    SetSetpoint(myPIDValues.setpoint);
    SetTolerance(m_PIDValues.positionTolerance, m_PIDValues.velocityTolerance);
}

/**
 * Returns the next output of the controller clamped from MinOutput to MaxOutput
 * 
 * @param input The process variable */
double FrcPID::ClampCalculate(double input)
{
    double output = Calculate(input);
    if(output > myPIDValues.kMaxOutput) { output = myPIDValues.kMaxOutput; }
    if(output < myPIDValues.kMinOutput) { output = myPIDValues.kMinOutput; }

    return output;
}

/**
 * Sets the setpoint, then returns the next output of the controller clamped from MinOutput to MaxOutput
 * 
 * @param input The process variable
 * @param setpoint The new Setpoint */
double FrcPID::ClampCalculate(double input, double setpoint)
{
    myPIDValues.setpoint = inputSetpoint;

    double output = Calculate(input, myPIDValues.setpoint);
    if(output > myPIDValues.kMaxOutput) { output = myPIDValues.kMaxOutput; }
    if(output < myPIDValues.kMinOutput) { output = myPIDValues.kMinOutput; }
    return output;
}