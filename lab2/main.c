
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "queue.h"
#include "platform.h"
#include "timer.h"
#include "gpio.h"
#include "stdbool.h"


#define BUFF_SIZE 128 //read buffer length

Queue rx_queue; // Queue for storing received characters

void uart_rx_isr(uint8_t rx);
void led_signle_isr(void);
void led_even_isr(void);
void button_isr(void);
void Systick_Handler(void);
bool is_even = true; 

//Button Pressed counter 
uint8_t button_counter = 0;

int main() {
	
	// Variables to help with UART read
	uint8_t rx_char = 0;
	char buff[BUFF_SIZE]; // The UART read string will be stored here
	uint32_t buff_index;
	
	// Set the mode of the used pins
	gpio_set_mode(PC_13, Input);
	gpio_set_mode(PC_1, Output);	
	
	// Initialize the receive queue and UART
	queue_init(&rx_queue, 128);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
	uart_enable(); // Enable UART module
	
  timer_init((CLK_FREQ)/2);
	timer_set_callback(led_signle_isr);
	timer_enable();
	
	
	__enable_irq(); // Enable interrupts
	
	uart_print("\r\n");// Print newline
	
	
	//Initialize timer 
	//timer_init(CLK_FREQ/2);
	//(2000000);
	
	//timer_set_callback(led_even_isr);
	
	
	gpio_set_trigger(PC_13, Rising);
	//gpio_set(PC_1, 1);
	
	gpio_set_callback(PC_13, button_isr);
	
	while(1) {
		// Prompt the user to enter their full name
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
		
		// Check last digit of AEM 
		//TO DO -> check if it a number ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//if single
		if(buff[buff_index-2]%2 == 1){
			is_even = false;
			timer_enable();
		}
		//if even
		else{
			is_even = true;
		}
		
		//sprintf(result_buffer,"%d" ,sum_numbers);
		//uart_print(result_buffer); // print the result in Tera Term 
		//uart_print("\r\n");
		//printf("Result=%d\n\r", sum_numbers); // print the result via printf 
		
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

// To Do: Priorities ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void led_signle_isr(){
	if(is_even == false){
		gpio_toggle(PC_1);
		uart_print("LED status toggled\r\n"); //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}
	else{
		timer_disable();
	}
	
}

void led_even_isr(){
	timer_disable();
}

void button_isr(void){
	if(gpio_get(PC_1) == 0){
		gpio_set(PC_1, 1); //define pin
		uart_print("LED status toggled\r\n");		
	}
	else{
		gpio_set(PC_1, 0); //define pin
		uart_print("LED status toggled\r\n");
	}
	//gpio_set(PC_1, 1);
	char result_buffer[20];
	button_counter += 1;
	sprintf(result_buffer,"%d" ,button_counter);
	uart_print("The button has been pressed= ");
	uart_print(result_buffer); // print the result in Tera Term 
	uart_print("\r\n");
	//printf("The button has been pressed=%d\n\r", sum_numbers); // print the result via printf 
	
}


