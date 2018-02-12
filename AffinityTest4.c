#pragma config(Sensor, in2,    clawLiftPot,    sensorPotentiometer)
#pragma config(Sensor, in3,    leftLiftPot,    sensorPotentiometer)
#pragma config(Sensor, in4,    rightLiftPot,   sensorPotentiometer)
#pragma config(Sensor, in5,    leftMobilePot,  sensorPotentiometer)
#pragma config(Sensor, in6,    rightMobilePot, sensorPotentiometer)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port2,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightMobile,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftLift,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightLift,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           leftMobile,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           clawLift,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           rightDrive,    tmotorVex393_MC29, openLoop)

task main()
{
	while(true) {
		if (abs(vexRT[Ch1]) > 20) {
				motor[port8] = vexRT[Ch1];
		} else {
			motor[port8] = 0;
		}
	}



}
