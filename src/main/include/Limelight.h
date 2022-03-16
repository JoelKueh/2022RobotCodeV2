#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

class Limelight {
 public:
  /** Constructs a Limelight objec. */
  Limelight();

  /** Turns on the Limelight LED. */
  void LEDOn();
  /** Turns off the Limelight LED. */
  void LEDOff();
  /** Returns the X angle offset of the Limelight Camera. */
  double GetX();
  /** Returns the Y angle offset of the Limelight Camera. */
  double GetY();
};
