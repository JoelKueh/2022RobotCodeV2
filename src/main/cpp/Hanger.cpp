#include "Hanger.h"

// Uses the default constructor for Hanger.
Hanger::Hanger() = default;

void Hanger::In()
{
    // Set the solonoid to itss Reverse output. In this case that is in.
    HangerPiston.Set(frc::DoubleSolenoid::kReverse);
}

void Hanger::Out()
{
    // Set the solonoid to its Forward output. In this case that is out.
    HangerPiston.Set(frc::DoubleSolenoid::kForward);
}

void Hanger::Toggle()
{
    // Toggle the solonoid output. Can only be called after In or Out has been called once.
    HangerPiston.Toggle();
}