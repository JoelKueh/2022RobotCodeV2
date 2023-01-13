
#pragma once

// This is a nice little class which allows us to keep all o four motor port numbers in the same spot.
// It's just a little quality of life class which makes everything easier to read.
class WiringDiagram {
 public:
  static const int c_Flywheel = 1; // CAN
  static const int c_Elevator = 2; // CAN
  static const int c_ElevatorA = 0; // DIO
  static const int c_ElevatorB = 1; // DIO
  static const int c_ElevatorLimit = 2; // DIO
  static const int c_IndexMotor = 1; // CAN

  static const int c_FrontLeftMotor = 1; // CAN
  static const int c_FrontRightMotor = 0; // CAN
  static const int c_BackLeftMotor = 3; // CAN
  static const int c_BackRightMotor = 2; // CAN

  static const int c_IntakeMotor = 0; // CAN
  static const int c_PCMID = 0; // CAN
  static const int c_IntakePistonA = 5; // CAN
  static const int c_IntakePistonB = 4; // CAN

  static const int c_HangerPistonA = 6; // CAN
  static const int c_HangerPistonB = 7; // CAN

  static const int c_AutoSwitch = 3; // DIO
  static const int c_Xbox = 0; // Xbox
};
