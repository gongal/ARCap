/*
 * MotorHandler.cpp
 *
 *  Created on: 2014-03-03
 *      Author: gongal
 */

#include "MotorHandler.h"
#include "includes.h"
#include "altera_avalon_uart_regs.h"

MotorHandler::MotorHandler() {
	// TODO Auto-generated constructor stub

}

MotorHandler::~MotorHandler() {
	// TODO Auto-generated destructor stub
}
void MotorHandler::sendByteMC(char msg){
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_MC_BASE, msg);
}
/*
 * Move rover forward by activating both motors
 */
void MotorHandler::mc_forward(){
	//motor 1
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR2_FORWARD);
	sendByteMC(MOTOR_CONST_SPEED);
	//motor 2
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR3_FORWARD);
	sendByteMC(MOTOR_CONST_SPEED);
}
/*
 * Move rover backward by activating both motor backwards
 */
void MotorHandler::mc_backward(){
	//motor 1
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR2_BACKWARD);
	sendByteMC(MOTOR_CONST_SPEED);
	//motor 2
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR3_BACKWARD);
	sendByteMC(MOTOR_CONST_SPEED);
}
/*
 * Turn rover left
 */
void MotorHandler::mc_left(){
	//Turn Left by driving the left motor only

	//motor 1
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR2_FORWARD);
	sendByteMC(MOTOR_CONST_SPEED);
}

void MotorHandler::mc_right(){
	//Turn Right by driving right motor only

	//motor 1
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR3_FORWARD);
	sendByteMC(MOTOR_CONST_SPEED);
}
void MotorHandler::mc_stop(){
	//motor1
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR2_FORWARD);
	sendByteMC(MOTOR_STOP_SPEED);
	//motor 2
	sendByteMC(MOTOR_START_BYTE);
	sendByteMC(MOTOR_DEVICE_TYPE);
	sendByteMC(MOTOR_MOTOR3_FORWARD);
	sendByteMC(MOTOR_STOP_SPEED);
}
