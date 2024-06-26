#include "platform.h"
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "queue.h"

extern int sum_natural_numbers(int a);
extern int hash_calc(char* array);
extern int hash_calc_single_value(int a);

#define BUFF_SIZE 128 //read buffer length

Queue rx_queue; // Queue for storing received characters

void uart_rx_isr(uint8_t rx);

int main() {
	char result_buffer[20];
	char hash_buffer[20];
	int sum_numbers = 0;
	int hash = 0;
	int hash_value = 0;
	
	// Variables to help with UART read
	uint8_t rx_char = 0;
	char buff[BUFF_SIZE]; // The UART read string will be stored here
	uint32_t buff_index;
	
	// Initialize the receive queue and UART
	queue_init(&rx_queue, 128);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
	uart_enable(); // Enable UART module
	
	__enable_irq(); // Enable interrupts
	
	uart_print("\r\n");// Print newline
	
	while(1) {
		// Prompt the user to enter their full name
		uart_print("Enter your string:");
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
		
		hash = hash_calc(buff); // buff is the char array of the input that the user types on Tera Term
		printf("Hash= %d\n\r", hash);
		uart_print("Hash = ");
		sprintf(hash_buffer,"%d" ,hash);
		uart_print(hash_buffer); // print the hash in Tera Term 
		uart_print("\r\n");
		hash_value = hash_calc_single_value(hash);
		sum_numbers = sum_natural_numbers(hash_value);
		sprintf(result_buffer,"%d" ,sum_numbers);
		uart_print("Final Result = ");
		uart_print(result_buffer); // print the result in Tera Term 
		uart_print("\r\n");
		printf("Result=%d\n\r", sum_numbers); // print the result via printf 
		
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

