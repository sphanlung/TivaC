//***************************************************************************************
// Name:   		SevenSegment2.c
//
// Author: 		Sergio PL                                                                 
//         		http://github.com/MrPotati/      http://idielectronica.blogspot.com/                                              
//
// Device: 		LM4F120/TM4C123, Tiva C Launchpad                 
//
// Description: This program uses the Tiva C Launchpad to make a button-press counter 
// with the number shown in a common-anode 4-digit 7-segment display.
// For this implementation we used the 5641BH display. A circuit diagram can be found  
// in the blog listed above.
//***************************************************************************************

#include "tm4c123gh6pm.h"

//Create definitions for Port B registers
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
//Create definitions for Port E registers
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

//Create definitions for Port F registers
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))

//Global variables
int numbers[10] = {0x40,0x79,0x24,0x30,0x19,	// Each value turns on bits needed
									 0x12,0x02,0x78,0x00,0x10}; // to show numbers in display
int digit1, digit2, digit3, digit4;		// Number to be displayed in each digit
unsigned long SW2;										// On-board push button
unsigned long count=0;								// Counter for button presses
int before=1;													// States if button was pressed in previous
																			// loop cycle to enable counter.
																			
//   Function Prototypes
void PortB_Init(void);
void PortE_Init(void);
void PortF_Init(void);
void Delay2(void);
void Display(int digit, int number);
void NumSplit(int counted);


//  Subroutines Section
int main(void){
  PortB_Init();        								// Initialize microcontroller ports
  PortE_Init();	
 	PortF_Init();
	while(1){
		SW2 = GPIO_PORTF_DATA_R&0x01;			// Defines on-board button as SW2

		if(SW2 == 0x00 && before == 0){		// If SW2 is pressed and wasn't pressed in 
			if(count <= 9998){								// previous loop cycle. Prevents counter from
				count++;											// increasing when button is held down.
				before = 1;
			}
			else{														// Reset counter at 9999
				count = 0;
			}
		}
		
		if(SW2 == 0x01){									// If button isn't pressed
			before = 0;
		}
		
		NumSplit(count);									// Split value in counter into 4 numbers
		Display(1,digit1);								// Display number for lowest digit
		Display(2,digit2);			
		Display(4,digit3);
		Display(8,digit4);					
  }
}

// Subroutine to initialize ports B, E and F
void PortB_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000002;     // Port B clock initialized
  GPIO_PORTB_CR_R = 0x7F;           // Allow changes to PB6-PB0       
  GPIO_PORTB_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x7F;          // Set PB6-PB0 outputs   
  GPIO_PORTB_PUR_R = 0x00;          // Enable pullup resistors on PB4,PF0       
  GPIO_PORTB_DEN_R = 0x7F;          // 7) Enable digital pins PB6-PB0        
}
void PortE_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000010;     // Port E clock initialized
  GPIO_PORTE_CR_R = 0x0F;           // Allow changes to PE4-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x0F;          // PE3-PE0 output   
  GPIO_PORTE_PUR_R = 0x00;          // Disable pullup resistors       
  GPIO_PORTE_DEN_R = 0x0F;          // Enable digital pins PE3-PE0
}
void PortF_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000020;     // Port F clock initialized
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // Unlock PortF
  GPIO_PORTF_CR_R = 0x1F;           // Allow changes to PB4-PB0     
  GPIO_PORTF_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // PF4,PF0 input   
  GPIO_PORTF_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTF_PUR_R = 0x11;          // Enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // Enable digital pins PF4-PF0        
}

// Creates 0.1ms delay
void Delay2(void){
	unsigned long volatile time;
	time = 727240*200/91000;  // 0.1 ms
  while(time){
			time--;
  } 
}

// Takes digit and number for LED display
void Display(int digit, int number){
	GPIO_PORTB_DATA_R = 0x00;							// Turns off LEDs
	GPIO_PORTE_DATA_R = digit;						// Selects digit
	GPIO_PORTB_DATA_R = numbers[number];	// Turns on number in selected digit
	Delay2();															// Wait 0.1 ms
	 } 

// Splits number in counter into 4 separate numbers for each digit
void NumSplit(int counted){
	digit1 = counted%10;	//Copies value in counter, divides it by 10 and then keeps remainder
    counted /= 10;	//Dividing value in counter by 10 shifts it by one decimal
    digit2 = counted%10;
    counted /= 10;
    digit3 = counted%10;
    counted /= 10;
    digit4 = counted%10;
    counted /=10;
}
