#pragma config(Sensor, in2,    ClawLift1,      sensorPotentiometer)
#pragma config(Sensor, in3,    LiftRight,      sensorPotentiometer)
#pragma config(Sensor, in5,    LiftLeft,       sensorPotentiometer)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port2,           LeftDrive,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           LeftMobile,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           Claw,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           LeftLift,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           RightLift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           ClawLift,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           RightMobile,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           RightDrive,    tmotorVex393_MC29, openLoop)
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
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


float wheelRadius = 2;
float turnRadius = 5.4;
bool shouldWait = false;

// -------- CALCULATIONS ---------

float rotsFor(float degrees) {
	float wheelCircumference = 2 * PI * wheelRadius;
	float turnCircumference = 2 * PI * turnRadius;

	float degRatio = abs(degrees) / 360;
	float turnDist = degRatio * turnCircumference;
	return turnDist / wheelCircumference;
}

float inchesToRots(float inches) {
	float wheelCircumference = 2 * PI * wheelRadius;
	return abs(inches) / wheelCircumference;
}

// -------- ARM CONTROLS ---------

task openLift() {
	while(SensorValue[LiftRight] < 900 && SensorValue[LiftLeft] < 1340){
		motor[RightLift] = 70;
		motor[LeftLift] = 70;
	}
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
	shouldWait = false;
	EndTimeSlice();
}

void closeLift() {
	while(SensorValue[LiftRight] > 3350 && SensorValue[LiftLeft] > 3425){
		motor[RightLift] = -70;
		motor[LeftLift] = -70;
	}
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
}

void lowerLiftToStack() {
	/*
	// Lower Claw Lift to Stack Position
	while(SensorValue[ClawLift1]<1300){
	motor[ClawLift] = 30;
	}
	motor[ClawLift] = 0;

	wait(0.5);
	*/

	while(SensorValue[LiftRight] > 310 && SensorValue[LiftLeft] > 710){
		motor[RightLift] = -70;
		motor[LeftLift] = -70;
	}
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
}

void raiseClawLift() {
	while( SensorValue[ClawLift1] > 1940){
		motor[ClawLift] = -60;
	}
	motor[ClawLift] = 0;
}

void releaseCone() {
	motor[Claw] = -60;
	wait(0.5);
	raiseClawLift();
	motor[Claw] = 0;
}

void openMobile(bool hasCone) {
	motor[LeftMobile] = 80;
	motor[RightMobile] = 80;
	if (hasCone) {
		wait(1.5);
	} else {
		wait(1);
	}
	motor[LeftMobile] = 0;
	motor[RightMobile] = 0;
}

void closeMobile(bool hasCone) {
	motor[LeftMobile] = -80;
	motor[RightMobile] = -80;
	if (hasCone) {
		wait(1.5);
		} else {
		wait(1.2);
	}
	motor[LeftMobile] = 0;
	motor[RightMobile] = 0;
}



// -------- TURNING ---------

float turnDegrees = 0;
float turnSpeed = 0;

task turnME() {
	float rots = rotsFor(turnDegrees);
	float rotDegrees = rots * 360;

	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;

	if (turnDegrees > 0) {
		motor[LeftDrive] = turnSpeed;
		motor[RightDrive] = -turnSpeed;
	} else {
		motor[LeftDrive] = -turnSpeed;
		motor[RightDrive] = turnSpeed;
	}


	// TODO: Test which turn method is more accurate


	// Normal Turn Method
	while (abs(SensorValue[LeftEncoder]) < rotDegrees || abs(SensorValue[RightEncoder]) < rotDegrees) {
		if (abs(SensorValue[LeftEncoder]) >= rotDegrees && abs(SensorValue[RightEncoder]) >= rotDegrees){
			motor[LeftDrive] = 0;
			motor[RightDrive] = 0;
		}
	}



	/*
	// Turning with self-correction
	while (abs(SensorValue[LeftEncoder]) < rotDegrees) {
		// If the left side is farther ahead, speed up the right side
		if (abs(SensorValue[LeftEncoder]) > abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = (turnSpeed + 10) * rightDirection;
			motor[LeftDrive] = turnSpeed * leftDirection;
		}

		// If the right side is farther ahead, speed up the left side
		if (abs(SensorValue[LeftEncoder]) < abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = turnSpeed * rightDirection;
			motor[LeftDrive] = (turnSpeed + 10) * leftDirection;
		}

		// If they are both at equal distances, drive straight ahead.
		if (abs(SensorValue[LeftEncoder]) == abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = turnSpeed * rightDirection;
			motor[LeftDrive] = turnSpeed * leftDirection;
		}
	}
	*/

	shouldWait = false;
	EndTimeSlice();
}

void turn(float deg, float speed){
	turnDegrees = deg;
	turnSpeed = speed;
	startTask(turnME);
}

// -------- MOVING ---------
// Note: DO NOT RUN AT MAX SPEED, RUNNING AT MAX SPEED WILL NOT ALLOW THE ROBOT TO STRAIGHTEN ITSELF.
// You should run this at a maximum of 110.

float moveInches = 0;
float moveSpeed = 0;

task moveME() {
	float rots = inchesToRots(moveInches);
	float adjustment = 1;
	float rotDegrees = rots * 360* adjustment;

	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;

	// Either 1 or -1 depending on given inches
	int direction;
	if (moveInches > 0) {
		direction = 1;
		} else {
		direction = -1;
	}

	// Auto-Straigtening
	while (abs(SensorValue[LeftEncoder]) < rotDegrees) {
		// If the left side is farther ahead, speed up the right side
		if (abs(SensorValue[LeftEncoder]) > abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = (moveSpeed + 10) * direction;
			motor[LeftDrive] = moveSpeed * direction;
		}

		// If the right side is farther ahead, speed up the left side
		if (abs(SensorValue[LeftEncoder]) < abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = moveSpeed * direction;
			motor[LeftDrive] = (moveSpeed + 10) * direction;
		}

		// If they are both at equal distances, drive straight ahead.
		if (abs(SensorValue[LeftEncoder]) == abs(SensorValue[RightEncoder])) {
			motor[RightDrive] = moveSpeed * direction;
			motor[LeftDrive] = moveSpeed * direction;
		}
	}

	// Stop Motors
	motor[LeftDrive] = 0;
	motor[RightDrive] = 0;
	// Let the main task know that we can move ahead
	shouldWait = false;
	EndTimeSlice();
}

void move(float inches, float speed) {
	moveInches = inches;
	moveSpeed = speed;
	startTask(moveME);
}

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................
	// Program Start
	// Raise Lift, Open Up Claw and Mobile

	shouldWait = true;
	startTask(openLift);
	raiseClawLift();
	while (shouldWait) {}

	// Move to capture mobile goal
	shouldWait = true;
	move(45, 110); // <- Asynchronous
	openMobile(false);
	while(shouldWait) {}

	wait(0.5);
	closeMobile(true);

	shouldWait = true;
	move(-35, 110);
	lowerLiftToStack();
	wait(0.5);
	releaseCone();
	while(shouldWait) {}

	// Turn
	shouldWait = true;
	turn(-185, 80); // <- Asynchronous
	while(shouldWait) {}

	shouldWait = true;
	move(22,110);
	while(shouldWait) {}

	motor[RightDrive] = 110;
	wait(1);
	motor[RightDrive] = 0;
	openMobile(true);
	wait(0.2);
	// Move backwards and close the mobile to release the cone
	shouldWait = true;
	move(-15,110); // <- Asynchronous
	closeMobile(true);
	while(shouldWait) {}
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
  // User control code here, inside the loop

  while(1){

		int c1 = -(vexRT[Ch3]);
		int c3 = vexRT[Ch1];

		if(((c1>60||c1<-60)||(c3>60||c3<-60))){
			motor[RightDrive] = -(c3 + c1);
			motor[LeftDrive] = -(c1 - c3);
		}else{
			motor[LeftDrive] = 0;
			motor[RightDrive] = 0;
		}

		if(vexRT[Btn8UXmtr2]==1){
			motor[RightLift] = 127;
			motor[LeftLift] = 127;
		}else if(vexRT[Btn8DXmtr2]==1){
			motor[RightLift] = -127;
			motor[LeftLift] = -127;
		}else{
			motor[RightLift] = 0;
			motor[LeftLift] = 0;
		}

		if(vexRT[Btn7UXmtr2]==1 || vexRT[Btn5D]==1){
			motor[LeftMobile] = -127;
			motor[RightMobile] = -127;
		}else if(vexRT[Btn7DXmtr2]==1 || vexRT[Btn5U]==1){
			motor[LeftMobile] = 127;
			motor[RightMobile] = 127;
		}else{
			motor[LeftMobile] = 0;
			motor[RightMobile] = 0;
		}

		if(vexRT[Btn5UXmtr2]==1){
			motor[ClawLift] = -75;
		}else if(vexRT[Btn5DXmtr2]==1){
			motor[ClawLift] = 75;
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
