/*
 * SerialHandler.cpp
 *
 *  Created on: 2014-03-01
 *      Author: gongal
 */

#include "SerialHandler.h"
#include "includes.h"
#include "altera_avalon_uart_regs.h"

SerialHandler::SerialHandler() {
	// TODO Auto-generated constructor stub

}

SerialHandler::~SerialHandler() {
	// TODO Auto-generated destructor stub
}

void SerialHandler::sendByteMC(char msg){
		IOWR_ALTERA_AVALON_UART_TXDATA(UART_MC_BASE, msg);
}

void SerialHandler::sendDataWifi(char * msg, int length){
	int i;
		for(i = 0; i<length; i++){
			IOWR_ALTERA_AVALON_UART_TXDATA(UART_WIFI_BASE, msg[i]);
		}
}

