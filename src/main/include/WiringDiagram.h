// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

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
  static const int c_IntakePistonIn = 4; // CAN
  static const int c_IntakePistonOut = 5; // CAN

  static const int c_HangerPistonIn = 6; // CAN
  static const int c_HangarPistonOut = 7; // CAN

  static const int c_Xbox = 0; // Xbox
};
