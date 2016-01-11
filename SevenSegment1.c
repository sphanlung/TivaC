//***************************************************************************************
// Name:   		SevenSegment1.c
//
// Author: 		Sergio PL                                                                 
//         		http://github.com/MrPotati/      http://idielectronica.blogspot.com/                                              
//
// Device: 		LM4F120/TM4C123, Tiva C Series microcontroller                 
//
// Description: This code controls a single-digit common-anode seven-segment display  
// using the Tiva C Launchpad board.  The program uses a finite state machine to show 
// a looping number sequence from 0 to 9. Connect LED A to pin B0, LED B to B1, LED C 
// to B2, etc.
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

// State Machine Variables
#define n0 0x40; //0100 0000  Display 0        
#define n1 0x79; //0111 1001  Display 1
#define n2 0x24; //0010 0100  Display 2 
#define n3 0x30; //0011 0000  Display 3
#define n4 0x19; //0001 1001  Display 4
#define n5 0x12; //0001 0010  Display 5
#define n6 0x02; //0000 0010  Display 6
#define n7 0x78; //0111 1000  Display 7
#define n8 0x00; //0000 0000  Display 8
#define n9 0x10; //0001 0000  Display 9

enum states {s0,s1,s2,s3,s4,s5,s6,s7,s8,s9};
enum states state = s0;

//   Function Prototypes
void PortB_Init(void);
void Delay(void);
void Delay2(void);

//  Subroutines Section
int main(void){    
  PortB_Init();        			// Call initialization of port PF0-PF4    
  while(1){
    // State machine
	switch(state) {
			case s0: 
				GPIO_PORTB_DATA_R = n0; // Assign value in n0 to output pins 
				state = s1;		// Set next state
				Delay2();		// Wait 0.5 seconds
				break;
			case s1: 
				GPIO_PORTB_DATA_R = n1;
				state = s2;
				Delay2();
			  break;
			case s2: 
				GPIO_PORTB_DATA_R = n2;
				state = s3;
				Delay2();
				break;
			case s3: 
				GPIO_PORTB_DATA_R = n3;
				state = s4;
				Delay2();
				break;
			case s4: 
				GPIO_PORTB_DATA_R = n4;
				state = s5;
				Delay2();
				break;
			case s5: 
				GPIO_PORTB_DATA_R = n5;
				state = s6;
				Delay2();
				break;
			case s6: 
				GPIO_PORTB_DATA_R = n6;
				state = s7;
				Delay2();
				break;
			case s7: 
				GPIO_PORTB_DATA_R = n7;
				state = s8;
				Delay2();
				break;
			case s8: 
				GPIO_PORTB_DATA_R = n8;
				state = s9;
				Delay2();
				break;
			case s9: 
				GPIO_PORTB_DATA_R = n9;
				state = s0;
				Delay2();
				break;
	 } 
  }
}


// Subroutine to initialize port B pins for input and output
// PB0-PB6 are outputs to the LED
void PortB_Init(void){ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
	delay = 0x00000020;           // reading register adds a delay   
//  delay = SYSCTL_RCGC2_R;           // reading register adds a delay   
  GPIO_PORTB_CR_R = 0x7F;           // allow changes to PF4-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x7F;          // 5) PB6-PB0 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_PUR_R = 0x00;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTB_DEN_R = 0x7F;          // 7) enable digital pins PF4-PF0        
}

void Delay(void){unsigned long volatile time;
  time = 727240*200/91;  // 0.1sec
  while(time){
    time--;
  }
}
void Delay2(void){unsigned long volatile time;
  time = 727240*500/91;  // 0.5sec
  while(time){
    time--;
  } 
}
