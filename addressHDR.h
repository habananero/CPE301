#ifndef _HEADERFILE2_H
#define _HEADERFILE2_H

//Timer Reg. 1
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80; 
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81; 
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82; 
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;

//Timer Reg. 3
volatile unsigned char *myTCCR3A = (unsigned char *) 0x90; 
volatile unsigned char *myTCCR3B = (unsigned char *) 0x91;  
volatile unsigned char *myTCCR3C = (unsigned char *) 0x92; 
volatile unsigned char *myTIMSK3 = (unsigned char *) 0x71;
volatile unsigned int  *myTCNT3  = (unsigned  int *) 0x94;
volatile unsigned char *myTIFR3 =  (unsigned char *) 0x38;

//Timer Reg. 4
volatile unsigned char *myTCCR4A = (unsigned char *) 0xA0; 
volatile unsigned char *myTCCR4B = (unsigned char *) 0xA1;
volatile unsigned char *myTCCR4C = (unsigned char *) 0xA2;  
volatile unsigned char *myTIMSK4 = (unsigned char *) 0x72;
volatile unsigned int  *myTCNT4  = (unsigned  int *) 0xA4;
volatile unsigned char *myTIFR4 =  (unsigned char *) 0x39;

//Timer Reg. 5 (delay)
volatile unsigned char *myTCCR5A = (unsigned char *) 0x120; 
volatile unsigned char *myTCCR5B = (unsigned char *) 0x121;
volatile unsigned char *myTCCR5C = (unsigned char *) 0x122;  
volatile unsigned char *myTIMSK5 = (unsigned char *) 0x73;
volatile unsigned int  *myTCNT5  = (unsigned  int *) 0x124;
volatile unsigned char *myTIFR5 =  (unsigned char *) 0x3A;

//I/O Registers
volatile unsigned char *ddr_b = (unsigned char*) 0x24;
volatile unsigned char *pin_b = (unsigned char*) 0x23;
volatile unsigned char *port_b = (unsigned char*) 0x22;

volatile unsigned char *ddr_d = (unsigned char*) 0x2A;
volatile unsigned char *port_d = (unsigned char*) 0x2B;
volatile unsigned char *pin_d = (unsigned char*) 0x29;

volatile unsigned char *ddr_e = (unsigned char*) 0x2D;
volatile unsigned char *pin_e = (unsigned char*) 0x2C;
volatile unsigned char *port_e = (unsigned char*) 0x2E;

volatile unsigned char *ddr_h = (unsigned char*) 0x101;
volatile unsigned char *pin_h = (unsigned char*) 0x100;
volatile unsigned char *port_h = (unsigned char*) 0x102;

volatile unsigned char *ddr_j = (unsigned char*) 0x104;
volatile unsigned char *port_j = (unsigned char*) 0x105; 

#endif 
