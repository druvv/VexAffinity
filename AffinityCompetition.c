#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    Claw1,          sensorPotentiometer)
#pragma config(Sensor, in2,    ClawLift1,      sensorPotentiometer)
#pragma config(Sensor, in3,    LiftRight,      sensorPotentiometer)
#pragma config(Sensor, in4,    LiftLeft,       sensorPotentiometer)
#pragma config(Sensor, in5,    Mobile,         sensorPotentiometer)
#pragma config(Sensor, in6,    Auto,           sensorPotentiometer)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           LeftDrive,     tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           LeftMobile,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           Claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LeftLift,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           RightLift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           ClawLift,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           RightMobile,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           RightDrive,    tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: The Only Winning Robot                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Remove this function call once you have "real" code.
  // Claw Close
	//(SensorValue[Claw1]<250){
		//motor[Claw] = 60;
		//wait(0.5);
	//}
	//motor[Claw] = 0;

	// Claw Lift Opens
//	while(SensorValue[ClawLift1]>250){
	//	motor[ClawLift] = 60;
	//}
	//motor[ClawLift] = 0;

	// Lift Extends
	while(SensorValue[LiftRight]<4085&&SensorValue[LiftLeft]>10){
		motor[RightLift] = 127;
		motor[LeftLift] = -127;
	}
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
	// Mobile Extends
	motor[LeftMobile] = 60;
	motor[RightMobile] = -60;
	wait(2);
	motor[LeftMobile] = 0;
	motor[RightMobile] = 0;
	// Drive to cone
	resetMotorEncoder(LeftDrive);
	resetMotorEncoder(RightDrive);
	float q = (4 * PI) / 627;
	//while(getMotorEncoder(LeftDrive)*q<28&&getMotorEncoder(RightDrive)*q<28){
		motor[RightDrive] = -80;
		motor[LeftDrive] = 80;
		wait(3);
	//}
	motor[RightDrive] = 0;
	motor[LeftDrive] = 0;
	// Pick up cone
	motor[LeftMobile] = -50;
	motor[RightMobile] = 50;
	wait(2.25);
	// Move straight backward
	resetMotorEncoder(LeftDrive);
	resetMotorEncoder(RightDrive);
	//while(getMotorEncoder(LeftDrive)*q<5&&getMotorEncoder(RightDrive)*q<5){
	motor[RightDrive] = 127;
	motor[LeftDrive] = -127;
	wait(1.25);
	//}
	motor[RightDrive] = 0;
	motor[LeftDrive] = 0;
	wait();
	if (SensorValue[Auto] > 2000) {
		// Turn Left
		motor[RightDrive] = 80;
		motor[LeftDrive] = 80;
		wait(1.4);
		motor[RightDrive] = 0;
		motor[LeftDrive] = 0;
		wait(0.1);
		// Drive forward
		motor[RightDrive] = -127;
		motor[LeftDrive] = 127;
		wait(0.25);
		motor[RightDrive] = 0;//222220 //pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp
		motor[LeftDrive] = 0;
		wait(0.2);
		// Right Forward to align center
		motor[RightDrive] = -127;
		motor[LeftDrive] = 0;
		wait(0.125);
		} else {
		// Turn Right
		motor[RightDrive] = -80;
		motor[LeftDrive] = -80;
		wait(1.4);
		motor[RightDrive] = 0;
		motor[LeftDrive] = 0;
		wait(0.1);
		// Drive forward
		motor[RightDrive] = -127;
		motor[LeftDrive] = 127;
		wait(0.25);
		motor[RightDrive] = 0;
		motor[LeftDrive] = 0;
		wait(0.2);
		// Right Forward to align center
		motor[RightDrive] = -127;
		motor[LeftDrive] = 0;
		wait(0.125);
	}
	// Lift down to balance center of mass
	motor[RightLift] = -127;
	motor[LeftLift] = 127;
	wait(0.25);
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
	// Full speed over goal zone
	motor[RightDrive] = -127;
	motor[LeftDrive] = 127;
	wait(3);
	motor[RightDrive] = 0;
	motor[LeftDrive] = 0;
	wait(0.1);
	// Pushes Mobile Goal
	motor[LeftMobile] = 127;
	motor[RightMobile] = -0;
	wait(1.5);
	motor[LeftMobile] = 0;
	motor[RightMobile] = 0;
	wait(0.1);
	// Mobile Goal in and backwards driving
	motor[LeftMobile] = -50;
	motor[RightMobile] = 50;
	motor[RightDrive] = 127;
	motor[LeftDrive] = -127;
	wait(2.25);
	stopAllMotors();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
  int direction = 0;

	while(1){

		int c1 = -(vexRT[Ch1]);
		int c3 = vexRT[Ch3];

		if(vexRT[Btn5D]==1){
			if(direction==0){
				direction=1;
			}else{
			direction=0;
			}
		}

		if(((c1>60||c1<-60)||(c3>60||c3<-60))&&direction==0){
			motor[RightDrive] = -(c3 + c1);
			motor[LeftDrive] = -(c1 - c3);
		}else if(((c1>60||c1<-60)||(c3>60||c3<-60))&&direction==1){
			motor[LeftDrive] = -(c3 + c1);
			motor[RightDrive] = -(c1 - c3);
		}else{
			motor[LeftDrive] = 0;
			motor[RightDrive] = 0;
		}

		if(vexRT[Btn8UXmtr2]==1){
			motor[RightLift] = 127;
			motor[LeftLift] = -127;
		}else if(vexRT[Btn8DXmtr2]==1){
			motor[RightLift] = -127;
			motor[LeftLift] = 127;
		}else{
			motor[RightLift] = 0;
			motor[LeftLift] = 0;
		}

		if(vexRT[Btn7UXmtr2]==1 || vexRT[Btn7U]==1){
			motor[LeftMobile] = 127;
			motor[RightMobile] = -127;
		}else if(vexRT[Btn7DXmtr2]==1 || vexRT[Btn7D]==1){
			motor[LeftMobile] = -127;
			motor[RightMobile] = 127;
		}else{
			motor[LeftMobile] = 0;
			motor[RightMobile] = 0;
		}

		if(vexRT[Btn5UXmtr2]==1){
			motor[ClawLift] = 127;
		}else if(vexRT[Btn5DXmtr2]==1){
			motor[ClawLift] = -127;
		}else{
			motor[ClawLift] = 0;
		}

		if(vexRT[Btn6UXmtr2]==1){
			motor[Claw] = 60;
		}else if(vexRT[Btn6DXmtr2]==1){
			motor[Claw] = -60;
		}else{
			motor[Claw] = 0;
		}

	}
}
