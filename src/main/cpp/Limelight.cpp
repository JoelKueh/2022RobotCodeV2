#include "Limelight.h"

Limelight::Limelight() = default;

double Limelight::GetX()
{
    double tx = nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tx",0.0);
    tx = tx + 1;
    // frc::SmartDashboard::PutNumber("Limelight X", tx);
    return tx;
}

double Limelight::GetY()
{
    double ty = nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("ty",0.0);
    // frc::SmartDashboard::PutNumber("Limelight Y", ty);
    return ty;
}

bool Limelight::TargetFound()
{
    bool tv = nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumber("tv",false);
    frc::SmartDashboard::PutBoolean("Target Found?", tv);
    return tv;
}

void Limelight::LEDOff()
{
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode",1);
}

void Limelight::LEDOn()
{
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->PutNumber("ledMode",3);
}