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

task main()
{


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

		if(vexRT[Btn7UXmtr2]==1 || vexRT[Btn7U]==1){
			motor[LeftMobile] = -127;
			motor[RightMobile] = -127;
		}else if(vexRT[Btn7DXmtr2]==1 || vexRT[Btn7D]==1){
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
