
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "queue.h"
#include "platform.h"
#include "timer.h"
#include "gpio.h"
#include "stdbool.h"
#include "delay.h"


#define BUFF_SIZE 128 //read buffer length

Queue rx_queue; // Queue for storing received characters

float temperature = 0;

void uart_rx_isr(uint8_t rx);
void get_temperature_isr(void);
float get_temperature(void);
void higher_than_25_isr(void);
void lower_than_20_isr(void);
void is_between_20_and_25(void);
void change_period_isr(void);

bool is_higher_than_25 = false;
bool is_lower_than_20 = false;
bool is_touching = false;
unsigned long period = 2;

int sum_of_last_two_digits = 0;
int button_counter=0;

int main() {
	
	// Variables to help with UART read
	uint8_t rx_char = 0;
	char buff[BUFF_SIZE]; // The UART read string will be stored here
	uint32_t buff_index;
	
	// Set the mode of the used pins
	gpio_set_mode(PA_5, Input);
	gpio_set_mode(PC_1, Output);	
	
	// Initialize the receive queue and UART
	queue_init(&rx_queue, 128);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
	uart_enable(); // Enable UART module
	
	// Initialize timer interrupt
  timer_init((CLK_FREQ)*period); // interrupt every 2sec
	timer_set_callback(get_temperature_isr); // callback function
	timer_enable();	
	
	__enable_irq(); // Enable interrupts
	
	uart_print("\r\n");// Print newline
	
	//Definitions for GPIO interrupt
	gpio_set_trigger(PC_3, Rising); // PC_13 corresponds to the button and the trigger condition is to be pressed
	gpio_set_callback(PC_1, change_period_isr);// callback function
	
	uart_print("Enter your AEM:");
	buff_index = 0; // Reset buffer index
	
	do {
		// Wait until a character is received in the queue
		while (!queue_dequeue(&rx_queue, &rx_char))
			__WFI(); // Wait for Interrupt

		if (rx_char == 0x7F) { // Handle backspace character
			if (buff_index > 0) {
				buff_index--; // Move buffer index back
				uart_tx(rx_char); // Send backspace character to erase on terminal
			}
		} else {
			// Store and echo the received character back
			buff[buff_index++] = (char)rx_char; // Store character in buffer
			uart_tx(rx_char); // Echo character back to terminal
		}
	} while (rx_char != '\r' && buff_index < BUFF_SIZE); // Continue until Enter key or buffer full
	
	// Replace the last character with null terminator to make it a valid C string
	buff[buff_index - 1] = '\0';
	uart_print("\r\n"); // Print newline
	
	// Check if buffer overflow occurred
	if (buff_index > BUFF_SIZE) {
		uart_print("Stop trying to overflow my buffer! I resent that!\r\n");
	}
	
	sum_of_last_two_digits = buff[buff_index-2] + buff[buff_index-3];
	
	while(1) {
		// Prompt the user to enter their full name
		
		
		if(temperature>25){
			is_higher_than_25 = true;
			
		}
		else{
			is_higher_than_25 = false;
		}
		
		if(temperature<20){
			is_lower_than_20 = true; 			
		}
		else{
			is_lower_than_20 = false; 
		}
		
	}
}

// Interrupt Service Routine for UART receive
void uart_rx_isr(uint8_t rx) {
	// Check if the received character is a printable ASCII character
	if (rx >= 0x0 && rx <= 0x7F ) {
		// Store the received character
		queue_enqueue(&rx_queue, rx);
	}
}

void get_temperature_isr(){
	
	temperature = get_temperature();
	char temperature_buffer[20]; 
	sprintf(temperature_buffer,"%f" ,(double)temperature);
	uart_print("The temperature is = ");
	uart_print(temperature_buffer); // print the result in Tera Term  
	uart_print("\r\n");
	
	char period_buffer[20]; 
	sprintf(period_buffer,"%d" ,(int)period);
	uart_print("The temperature is = ");
	uart_print(period_buffer); // print the result in Tera Term  
	uart_print("\r\n");
}

float get_temperature(){
	//fill later ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return temperature;
}

void higher_than_25_isr(void){
	if(is_higher_than_25){
		gpio_set(PA_5,1);
	}
}

void lower_than_20_isr(void){
	if(is_lower_than_20){
		gpio_set(PA_5,0);
	}
}


void is_between_20_and_25(void){
		if(!(is_lower_than_20 && is_higher_than_25)){
			gpio_toggle(PA_5);
			delay_ms(1000);
		}
		
}



void change_period_isr(void){
	button_counter++;
	if(button_counter==1){
		period = (unsigned long)sum_of_last_two_digits; 
	}
	else{
		if(button_counter%2==1){
			period=3;
		}
		else{
			//print humidity with the temperature together
		}
		
	}
		timer_init((CLK_FREQ)*period); // interrupt every 2sec
		timer_set_callback(get_temperature_isr); // callback function
		timer_enable();

}
