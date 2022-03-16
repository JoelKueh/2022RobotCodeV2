#include "Intake.h"

// Uses the default constructor for Intake.
Intake::Intake() = default;

void Intake::In()
{
    // Sets the solonoid to its reverse output.
    IntakePiston.Set(frc::DoubleSolenoid::kReverse);
}

void Intake::Out()
{
    // Sets the solonoid to its forward output.
    IntakePiston.Set(frc::DoubleSolenoid::kForward);
}

void Intake::Toggle()
{
    // Toggles the solonoid.
    IntakePiston.Toggle();
}

bool Intake::Init(units::time::second_t startTime, units::time::second_t elapsedTime)
{
    if(elapsedTime - startTime < 1_s)
    {
        IntakeMotor.Set(1);
        Out();
    }
    else
    {
        IntakeMotor.Set(0);
        return true;
    }
}

void Intake::SpinForward()
{
    IntakeMotor.Set(.9);
}

void Intake::SpinReverse()
{
    IntakeMotor.Set(-.9);
}

void Intake::Stop()
{
    IntakeMotor.StopMotor();
}