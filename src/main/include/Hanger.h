#pragma once

#include <frc/DoubleSolenoid.h>
#include "WiringDiagram.h"

class Hanger {
 private:
  frc::DoubleSolenoid HangerPiston {frc::PneumaticsModuleType::CTREPCM, WiringDiagram::c_HangerPistonA, WiringDiagram::c_HangarPistonB};
 public:
  /**
   * Constructs a Hanger object. */
  Hanger();

  /**
   * Pulls the piston in. */
  void In();
  /**
   * Pushes the piston out. */
  void Out();
  /**
   * Toggles the piston, call In() or Out() at least once first. */
  void Toggle();
};
