#pragma config(Sensor, in2,    ClawLift1,      sensorPotentiometer)
#pragma config(Sensor, in3,    LiftRight,      sensorPotentiometer)
#pragma config(Sensor, in5,    LiftLeft,       sensorPotentiometer)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port2,           LeftDrive,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           LeftMobile,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           Claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LeftLift,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           RightLift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           ClawLift,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           RightMobile,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           RightDrive,    tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float wheelRadius = 2;
float turnRadius = 6.7;
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

void openLift() {
	while(SensorValue[LiftRight] < 940 && SensorValue[LiftLeft] < 1410){
		motor[RightLift] = 70;
		motor[LeftLift] = 70;
	}
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
}

void closeLift() {
	while(SensorValue[LiftRight] > 7 && SensorValue[LiftLeft] > 45){
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

	while(SensorValue[LiftRight] > 3400 && SensorValue[LiftLeft] > 3500){
		motor[RightLift] = -70;
		motor[LeftLift] = -70;
	}
	motor[RightLift] = 0;
	motor[LeftLift] = 0;
}

void raiseClawLift() {
	while( SensorValue[ClawLift1] > 1900){
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
		wait(0.9);
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

	int leftDirection;
	int rightDirection;

	if (turnDegrees > 0) {
		leftDirection = 1;
		rightDirection = -1;
		} else {
		leftDirection = -1;
		rightDirection = 1;
	}

	// TODO: Test which turn method is more accurate

	/*
	// Normal Turn Method
	while (abs(SensorValue[LeftEncoder]) < rotDegrees || abs(SensorValue[RightEncoder]) < rotDegrees) {
		if (SensorValue[LeftEncoder] < rotDegrees) {
				motor[LeftDrive] = turnSpeed * leftDirection;
		} else {
				motor[LeftDrive] = 0;
		}

		if (SensorValue[RightEncoder] < rotDegrees) {
			motor[RightDrive] = turnSpeed *	rightDirection;
		} else {
			motor[RightDrive] = 0;
		}
	}
	*/

	// Turning with self-correction
	while (abs(SensorValue[LeftEncoder]) < rotDegrees) {
		// If the left side is farther ahead, speed up the right side
		if (SensorValue[LeftEncoder] > SensorValue[RightEncoder]) {
			motor[RightDrive] = (turnSpeed + 10) * rightDirection;
			motor[LeftDrive] = turnSpeed * leftDirection;
		}

		// If the right side is farther ahead, speed up the left side
		if (SensorValue[LeftEncoder] < SensorValue[RightEncoder]) {
			motor[RightDrive] = turnSpeed * rightDirection;
			motor[LeftDrive] = (turnSpeed + 10) * leftDirection;
		}

		// If they are both at equal distances, drive straight ahead.
		if (SensorValue[LeftEncoder] == SensorValue[RightEncoder]) {
			motor[RightDrive] = turnSpeed * rightDirection;
			motor[LeftDrive] = turnSpeed * leftDirection;
		}
	}

	motor[LeftDrive] = 0;
	motor[RightDrive] = 0;
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
		if (SensorValue[LeftEncoder] > SensorValue[RightEncoder]) {
			motor[RightDrive] = (moveSpeed + 10) * direction;
			motor[LeftDrive] = moveSpeed * direction;
		}

		// If the right side is farther ahead, speed up the left side
		if (SensorValue[LeftEncoder] < SensorValue[RightEncoder]) {
			motor[RightDrive] = moveSpeed * direction;
			motor[LeftDrive] = (moveSpeed + 10) * direction;
		}

		// If they are both at equal distances, drive straight ahead.
		if (SensorValue[LeftEncoder] == SensorValue[RightEncoder]) {
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

task main(){

	// Program Start
	// Raise Lift, Open Up Claw and Mobile
	openLift();
	raiseClawLift();
	openMobile(false);

	// Move to capture mobile goal
	shouldWait = true;
	move(45, 90); // <- Asynchronous
	while(shouldWait) {EndTimeSlice();}
	wait(0.5);

	// Turn
	shouldWait = true;
	turn(-160, 90); // <- Asynchronous
	closeMobile(true);
	while(shouldWait) {EndTimeSlice();}

	// Move back to goal and stack cone onto mobile goal
	shouldWait = true;
	move(52,90); // <- Asynchronous
	lowerLiftToStack();
	wait(0.5);
	releaseCone();
	while(shouldWait) {EndTimeSlice();}

	// Place mobile goal on ground
	openMobile(true);
	wait(0.25);
	// Move backwards and close the mobile to release the cone
	shouldWait = true;
	move(-15,90); // <- Asynchronous
	closeMobile(true);
	while(shouldWait) {EndTimeSlice();}

}
