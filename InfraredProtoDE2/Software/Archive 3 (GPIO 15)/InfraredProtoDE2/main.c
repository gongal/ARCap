/*************************************************************************
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
 * All rights reserved. All use of this software and documentation is     *
 * subject to the License Agreement located at the end of this file below.*
 **************************************************************************
 * Description:                                                           *
 * The following is a simple hello world program running MicroC/OS-II.The *
 * purpose of the design is to be a very simple application that just     *
 * demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
 * for issues such as checking system call return codes. etc.             *
 *                                                                        *
 * Requirements:                                                          *
 *   -Supported Example Hardware Platforms                                *
 *     Standard                                                           *
 *     Full Featured                                                      *
 *     Low Cost                                                           *
 *   -Supported Development Boards                                        *
 *     Nios II Development Board, Stratix II Edition                      *
 *     Nios Development Board, Stratix Professional Edition               *
 *     Nios Development Board, Stratix Edition                            *
 *     Nios Development Board, Cyclone Edition                            *
 *   -System Library Settings                                             *
 *     RTOS Type - MicroC/OS-II                                           *
 *     Periodic System Timer                                              *
 *   -Know Issues                                                         *
 *     If this design is run on the ISS, terminal output will take several*
 *     minutes per iteration.                                             *
 **************************************************************************/


#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_character_lcd.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    lcd_task_stk[TASK_STACKSIZE];
OS_STK    ir_task_stk[TASK_STACKSIZE];
OS_STK	  task3_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define LCD_TASK_PRIORITY	1
#define IR_TASK_PRIORITY	2
#define TASK3_PRIORITY		3

/* LCD */
#define LCD_UPPER 0
#define LCD_LOWER 1
#define LCD_LEFT 0
#define LCD_RIGHT 15
#define LCD_MESSAGE_SIZE 32
alt_up_character_lcd_dev *lcd;

/* Message queue */
#define MAX_MESSAGES 4
#define WAIT_FOREVER 0
int messages[MAX_MESSAGES];
OS_EVENT *queue;

/* Messages */
#define IR_PUSHBUTTON_MESSAGE	1
#define IR_PUSHBUTTON_OFF		(IR_PUSHBUTTON_MESSAGE + 0)
#define IR_PUSHBUTTON_ON		(IR_PUSHBUTTON_MESSAGE + 1)

/* Error status */
#define OK 0

static void getMessageFromStatus(int status, char *message) {
	switch (status) {
	case IR_PUSHBUTTON_OFF:
		snprintf(message, LCD_MESSAGE_SIZE, "off");
	case IR_PUSHBUTTON_ON:
		snprintf(message, LCD_MESSAGE_SIZE, "on");
	}
}

/* Interrupt service routine triggered whenever the status of the IR emitter pushbutton changes. */
static void isr_on_ir_pushbutton( void * context) {
	// Read the state of the pushbutton and post it to the queue.
	int state = IR_PUSHBUTTON_MESSAGE + IORD_ALTERA_AVALON_PIO_DATA(IR_PUSHBUTTON_BASE);
	OSQPost(queue, (void*)state);
	// Mask to mark the end of the ISR?
	IOWR_ALTERA_AVALON_PIO_CLEAR_BITS(IR_PUSHBUTTON_BIT_CLEARING_EDGE_REGISTER, 0);
}

/* Controllers the IR emitter based on the value of the pushbutton. */
void ir_task(void* pdata)
{
//	INT8U err;
	int status = 0;
	while (1)
	{
		status = !status;
		printf("Emitter: %d", status);
		IOWR_ALTERA_AVALON_PIO_DATA(IR_EMITTER_BASE, status);
		OSTimeDlyHMSM(0, 0, 2, 0);
	}
}

/* Clear a row of the LCD. */
void lcd_clear(int row) {
	int col;
	for (col = LCD_LEFT; col < LCD_RIGHT; col++) {
			alt_up_character_lcd_erase_pos(lcd, col, row);
	}
}


/* Writes to the LCD based on the value of the switch. */
void lcd_task(void* pdata)
{
	INT8U err;
	while (1)
	{
		char message[LCD_MESSAGE_SIZE];
		int status = (int)OSQPend(queue, WAIT_FOREVER, &err);
		if (err == OS_NO_ERR) {
			lcd_clear(LCD_UPPER);
			getMessageFromStatus(status, message);
			alt_up_character_lcd_set_cursor_pos(lcd, LCD_LEFT, LCD_UPPER);
			alt_up_character_lcd_string(lcd, message);
		}
	}
}

/* Initialize the LCD. */
void lcd_init() {
	lcd = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);
	if (lcd == NULL) {
		printf("Error: cannot open device %s", CHARACTER_LCD_0_NAME);
	} else {
		printf("Opened device %s", CHARACTER_LCD_0_NAME);
		alt_up_character_lcd_init(lcd);
	}
}

void queue_init() {
	queue = OSQCreate((void**)&messages, MAX_MESSAGES);
}

/* The main function creates the LCD task, registers the edge counter polling interrupt,
 * and starts the OS. */
int main(void)
{
	int status;

	// Initialize components.
	lcd_init();
	queue_init();

	// Create the LCD task.
	OSTaskCreateExt(lcd_task,
			NULL,
			(void *)&lcd_task_stk[TASK_STACKSIZE - 1],
			LCD_TASK_PRIORITY,
			LCD_TASK_PRIORITY,
			lcd_task_stk,
			TASK_STACKSIZE,
			NULL,
			0);

	// Create the LCD task.
	OSTaskCreateExt(ir_task,
			NULL,
			(void *)&ir_task_stk[TASK_STACKSIZE - 1],
			IR_TASK_PRIORITY,
			IR_TASK_PRIORITY,
			ir_task_stk,
			TASK_STACKSIZE,
			NULL,
			0);

	// Register the IR pushbutton interrupt.
	status = alt_ic_isr_register(IR_PUSHBUTTON_IRQ_INTERRUPT_CONTROLLER_ID,
				IR_PUSHBUTTON_IRQ,
				isr_on_ir_pushbutton,
				NULL,
				NULL);

	// Start.
	if (status == OK) {
		OSStart();
	}

	return 0;
}

/******************************************************************************
 *                                                                             *
 * License Agreement                                                           *
 *                                                                             *
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
 * All rights reserved.                                                        *
 *                                                                             *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * and/or sell copies of the Software, and to permit persons to whom the       *
 * Software is furnished to do so, subject to the following conditions:        *
 *                                                                             *
 * The above copyright notice and this permission notice shall be included in  *
 * all copies or substantial portions of the Software.                         *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
 * DEALINGS IN THE SOFTWARE.                                                   *
 *                                                                             *
 * This agreement shall be governed in all respects by the laws of the State   *
 * of California and by the laws of the United States of America.              *
 * Altera does not recommend, suggest or require that this reference design    *
 * file be used in conjunction or combination with any other product.          *
 ******************************************************************************/
