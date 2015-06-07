#include<xc.h> // processor SFR definitions
#include<sys/attribs.h>

// DEVCFGs here

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // not boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free up secondary osc pins by turning sosc off
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 40MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_20 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL
#pragma config UPLLIDIV = DIV_2 // divide 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0x1234 // some 16 bit userid
#pragma config PMDL1WAY = ON // not multiple reconfiguration, check this
#pragma config IOL1WAY = ON // not multiple reconfiguration, check this
#pragma config FUSBIDIO = OFF // USB pins controlled by USB module
#pragma config FVBUSONIO = OFF // controlled by USB module

int main(){
    // startup
	__builtin_disable_interrupts();
	// set the CP0 CONFIG register to indicate that
	// kseg0 is cacheable (0x3) or uncacheable (0x2)
	// see Chapter 2 "CPU for Devices with M4K Core"
	// of the PIC32 reference manual
	__builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

	// no cache on this chip!

	// 0 data RAM access wait states
	BMXCONbits.BMXWSDRM = 0x0;

	// enable multi vector interrupts
	INTCONbits.MVEC = 0x1;

	// disable JTAG to be able to use TDI, TDO, TCK, TMS as digital
	DDPCONbits.JTAGEN = 0;

	__builtin_enable_interrupts();
    
    // set up USER pin as input
	ANSELBbits.ANSB13 = 0;
	TRISBbits.TRISB13 = 1;

    //Set phase pins
    //   PH1--> B7
//    RPB7Rbits.RPB7R = 0b001; //Peripheral requires output pin
    TRISBbits.TRISB7 = 0; //Make it an output
    LATBbits.LATB7 = 1; //Set it high- this initializes as the forward direction

    //   PH2-->B14
    ANSELBbits.ANSB14 = 0; //Make it digital
    RPB14Rbits.RPB14R = 0b001;
    TRISBbits.TRISB14 = 0; //Make it an output
    LATBbits.LATB14 = 1; //Set it high- this initializes as the forward direction    

    //Set enable pins
    //   EN1--> B5 
    //ANSELBbits.ANSB5 = 0; //Make it digital
    RPB5Rbits.RPB5R = 0b0101; //Set to OC2
    TRISBbits.TRISB5 = 0; //Make it an output

    //   EN2-->B15    
    ANSELBbits.ANSB15 = 0; //Make it digital
    RPB15Rbits.RPB15R = 0b0101; //Set to OC1
//    TRISBbits.TRISB15 = 0; //Make it an output

    //Set timers:
    //This is to set up PWM frequency for OC pin- this will be altered using potentiometer
	T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1 (1:4)
	PR2 = 1999;                 // period = (PR2+1) * N * 12.5 ns = 1 ms, 1 kHz 
    TMR2 = 0;
    
 	OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1CONbits.OCTSEL = 0; //Using Timer 2 for OC1
    OC1RS = 1000;               //duty cycle = OC1RS/(PR2+1) 25% duty cycle to begin with
    OC1R = 1000;                //initialize before turning OC1 on;        
	T2CONbits.ON = 1;        // turn on Timer2
	OC1CONbits.ON = 1;       // turn on OC1

    //This is to set up PWM frequency for OC pin- this will be altered using potentiometer

 	OC2CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC2CONbits.OCTSEL = 0;     //Using Timer2 for OC2
    OC2RS = 1000;               //duty cycle = OC1RS/(PR2+1)
    OC2R = 1000;                //initialize before turning OC1 on;        
	OC2CONbits.ON = 1;       // turn on OC2
    
    __builtin_enable_interrupts();
    
    _CP0_SET_COUNT(0);
    
    
    while(_CP0_GET_COUNT()<80000000){
        //motor 1 go backwards
        LATBbits.LATB7 = 0;
        LATBbits.LATB14 = 1;
        OC1RS = 50*(PR2+1)/100;
        OC2RS = 50*(PR2+1)/100;        
    }
    _CP0_SET_COUNT(0);
    
    while(_CP0_GET_COUNT()<80000000){
        //motor 1 go forwards
        LATBbits.LATB7 = 1;
        LATBbits.LATB14 = 0;
        OC1RS = 50*(PR2+1)/100;
        OC2RS = 50*(PR2+1)/100;
    }
//    _CP0_SET_COUNT(0);        
}
