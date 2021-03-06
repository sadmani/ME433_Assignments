#include<xc.h> // processor SFR definitions
#include<sys/attribs.h>

#include "i2c_display.h" // __ISR macro

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
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // controlled by USB module

int readADC(void);

int main() {

    // startup
	__builtin_disable_interrupts();
    
//    i2c_master_setup();

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

	ANSELAbits.ANSA0 = 0;
    
    // set up USER pin as input
	ANSELBbits.ANSB13 = 0;
	TRISBbits.TRISB13 = 1;

    // set up LED1 (Yellow LED on my board) pin as a digital output
	//ANSELBbits.ANSB7 = 0; --> not needed because B7 is not an analog pin
	RPB7Rbits.RPB7R = 0b0001;
    TRISBbits.TRISB7 = 0;
	LATBbits.LATB7 = 1;
    
    // set up LED2 (Green LED on my board) as OC1 using Timer2 at 1kHz
    ANSELBbits.ANSB15 = 0; // 0 for digital, 1 for analog
    RPB15Rbits.RPB15R = 0b0101; //set B15 as output compare 1
    
    //This is to set up PWM frequency for OC pin- this will be altered using potentiometer
	T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1 (1:4)
	PR2 = 39999;                 // period = (PR2+1) * N * 12.5 ns = 1 ms, 1 kHz
	TMR2 = 0;     
 	OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
	T2CONbits.ON = 1;        // turn on Timer2
	OC1CONbits.ON = 1;       // turn on OC1

    OC1RS = 0;               //duty cycle = OC1RS/(PR2+1)
    OC1R = 0;                //initialize before turning OC1 on;    
    
    // set up A0 as AN0
    ANSELAbits.ANSA0 = 1;
    AD1CON3bits.ADCS = 3;
    AD1CHSbits.CH0SA = 0;
    AD1CON1bits.ADON = 1;
    
    display_init();
    
    int val = 0;
    
    //Initialize the OLED display and clear it
    display_init();
    display_clear();

    //Set up buffer for char
    char message[20];
    int val2 = 1337;
    sprintf(message,"Hello, World %d!",val2);
    
    //Write the message to the display
    oled_write(15,30,message);
    //Need to draw the pixels because the pixels are set prior to this, but not written
    display_draw();
    
	while (1) {
        // invert pin every 0.5s, set PWM duty cycle % to the pot voltage output 
        //Use the core timer to double check your CPU clock settings
        _CP0_SET_COUNT(0); // set core timer to 0, remember it counts at half the CPU clock
        LATBINV = 0x0080; // invert a pin
//        display_pixel_set(5,5,1);
//        display_draw();
        // wait for half a second, setting LED brightness to pot angle while waiting
        
        
        while (_CP0_GET_COUNT() < 10000000) {
            val = readADC();
            OC1RS = val * (PR2/1024);
            if (PORTBbits.RB13 == 1) {
                // nothing
            } else {
                LATBINV = 0x0080;
            }
        }
    }
}

int readADC(void) {
    int elapsed = 0;
    int finishtime = 0;
    int sampletime = 20;
    int a = 0;

    AD1CON1bits.SAMP = 1;
    elapsed = _CP0_GET_COUNT();
    finishtime = elapsed + sampletime;
    while (_CP0_GET_COUNT() < finishtime) {
    }
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE) {
    }
    a = ADC1BUF0;
    return a;
}
