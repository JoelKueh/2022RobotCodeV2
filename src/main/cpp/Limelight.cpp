#include "Limelight.h"

Limelight::Limelight() = default;

double Limelight::GetX()
{
    return nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tx",0.0);
}

double Limelight::GetY()
{
    return nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("ty",0.0);
}

void Limelight::LEDOff()
{
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode",1);
}

void Limelight::LEDOn()
{
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode",3);
}