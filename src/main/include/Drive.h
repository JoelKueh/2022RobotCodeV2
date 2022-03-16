#pragma once

#include <frc/drive/MecanumDrive.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonSRX.h>
#include "FrcPID.h"
#include "WiringDiagram.h"

class Drive {
 private:
  
  // Creates the struct that we defined in our FrcPID class and sets it's values
  FrcPID::PIDValues m_DrivePIDValues
  {
    m_DrivePIDValues.kP = .05, m_DrivePIDValues.kI = 0, m_DrivePIDValues.kD = 0, m_DrivePIDValues.kMaxOutput = 1.0, m_DrivePIDValues.kMinOutput = -1.0,
    m_DrivePIDValues.setpoint = 0,
    m_DrivePIDValues.positionTolerance = 1, m_DrivePIDValues.velocityTolerance = 5
  };
  // Constructs an object of the FrcPID class that we made. Uses the values from the struct that we made above as an input.
  FrcPID m_DrivePID {m_DrivePIDValues};

  // Constructs an object that contains each of the drive motors. NOTICE THE USE OF WPI_TalonSRX rather than just TalonSRX.
  // The WPI part allows for propper passing of the motor controllers into the drive train.
  ctre::phoenix::motorcontrol::can::WPI_TalonSRX m_frontLeft {WiringDiagram::c_FrontLeftMotor};
  ctre::phoenix::motorcontrol::can::WPI_TalonSRX m_frontRight {WiringDiagram::c_FrontRightMotor};
  ctre::phoenix::motorcontrol::can::WPI_TalonSRX m_backLeft {WiringDiagram::c_BackLeftMotor};
  ctre::phoenix::motorcontrol::can::WPI_TalonSRX m_backRight {WiringDiagram::c_BackRightMotor};

/*POINTERS (Ask Joel or try to read this)
    Rather than constructing the object, this creates a space in memory that POINTS towards another space in memory.
    Say you want to store some data, your first option is to just make a variable (maybe this variable will go in memory slot four).
    However, you have a second option. You could skip making a variable altogether and just directly put the data in memory slot four,
    the only problem is, you now have no way to access it. To access it, you make a different type of variable, called a pointer.
    Rather than storing the value, the pointer POINTS to the spot in memory where it is stored.
    This might seem roudnabout and useless, but there are sometimes cases where this has to be used.
    The simplest example is this, say you have a clock variable running on your program, and you want to use that clock across different classes.
    Without pointers, you would have to make a different clock variable for each class and update them individually.
    This is slow and complicated, and the clocks might get desynced.
    The better solution is to make one clock in one class, then make pointers to that clock across all of your other classes.
    
    The pointer we are making here is used for something called delayed construction.
    A full construction and declaration looks like this
      ctre::phoenix::motorcontrol::can::WPI_TalonSRX m_backRight {WiringDiagram::c_BackRightMotor};
    Essentially, you create a space in memory and fill it all in one pass.

    Here, all that we do is create a pointer. It doesn't allocate any space, it doesn't make any frc::MecanumDrive objects, all it does is make something that is the right
    size to point to the right location in memory.
      frc::MecanumDrive* m_MecanumDrive;
    Later (in the cpp file) we can fill up a spot in memory and tell the pointer where it needs to point.
      m_MecanumDrive = new frc::MecanumDrive(m_frontLeft, m_backLeft, m_frontRight, m_backRight);
    Translation:
      new frc::MecanumDrive
        Create a NEW space in memory, and make it the size of an frc::MecanumDrive.
      (m_frontLeft, m_backLeft, m_frontRight, m_backRight);
        Fill that space in memory with a mecanum drive object by running the constructor with these inputs.
      m_MecanumDrive = 
        Point to this NEW spot in memory.

    If this needs more explanation, talk to me.
  */
  frc::MecanumDrive* m_MecanumDrive;

 public:
  /**
   * Constructs a Drive object. */
  Drive();

  /**
   * Runs the mecanum drive with a deadzone of .15
   * @param drivePower The percent power dedicated to driving.
   * @param strafePower The percent power dedicated to strafing.
   * @param turnPower The percent power dedicated to turning. */
  void MecanumDrive(double drivePower, double strafePower, double turnPower);
  /**
   * Sets the MecanumDrive to eliminate an angle error
   * @param angleError The current angle offset from 0 */
  bool MecanumDrivePID(double angleError);
  /**
   * @overload Sets the MecanumDrive to eliminate an angle error
   * @param angleError The current angle offset from 0
   * @param strafePower The percent power dedicated to strafing */
  bool MecanumDrivePID(double angleError, double strafePower);
  /**
   * @overload Sets the MecanumDrive to eliminate an angle error
   * @param angleError The current angle offset from 0
   * @param strafePower The percent power dedicated to strafing
   * @param drivePower The percent power dedicated to driving */
  bool MecanumDrivePID(double angleError, double strafePower, double drivePower);

};