/*
 * outputtest.c
 *
 * Created: 11/12/2014 6:51:16 PM
 *  Author: erikhk
 */ 

//#define F_CPU 20000000L
#define F_CPU 8000000
#include "l74hc165.h"
#include "lcd.h"
//#include "lcd.c"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//volatile uint8_t count;
uint8_t sine[] = {128,131,134,137,140,144,147,150,153,156,159,162,165,168,171,174,177,180,182,185,188,191,194,196,199,201,204,206,209,211,214,216,218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,244,246,247,248,249,250,251,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,253,253,252,252,251,250,249,248,247,246,245,244,242,241,240,238,236,235,233,231,229,227,225,223,221,219,217,215,212,210,208,205,203,200,197,195,192,189,187,184,181,178,175,172,169,167,164,160,157,154,151,148,145,142,139,136,133,130,126,123,120,117,114,111,108,105,102,99,96,92,89,87,84,81,78,75,72,69,67,64,61,59,56,53,51,48,46,44,41,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,8,7,6,5,4,4,3,3,2,2,1,1,1,1,1,1,1,2,2,2,3,3,4,5,5,6,7,8,9,10,12,13,14,16,17,19,20,22,24,26,28,30,32,34,36,38,40,42,45,47,50,52,55,57,60,62,65,68,71,74,76,79,82,85,88,91,94,97,100,103,106,109,112,116,119,122,125,128};
uint8_t * data;
uint8_t * adc_value;
uint8_t count;
uint8_t * buffer[2];
uint8_t inc = 8;


void lfsr()
{
	uint16_t lfsr = 0xACE1u;
    unsigned period = 0;
    char s[16];

    do {
          unsigned lsb = lfsr & 1;  /* Get lsb (i.e., the output bit). */
          lfsr >>= 1;               /* Shift register */
          if (lsb == 1)             /* Only apply toggle mask if output bit is 1. */
            lfsr ^= 0xB400u;        /* Apply toggle mask, value has 1 at bits corresponding
                                    /* to taps, 0 elsewhere. */
          ++period;
		  PORTC = lfsr;
          for (int i = 0; i < 16; i++)
          {
             s[15 - i] = (lfsr & (1 << i)) ? '1' : '0';
          }
          //s[16] = '\0';
          //printf("\n%10d: %s", period, s);
    } while(lfsr != 0xACE1u);
	
}


ISR(TIMER0_OVF_vect)
{
	//This is the interrupt service routine for TIMER0 OVERFLOW Interrupt.
	//CPU automatically call this when TIMER0 overflows.

	//Increment our variable
	count += inc;
	
	
	if(data[0]>>1==1)
		inc = 4;
		
	if(data[0]>>2==1)
		inc = 8;
		
	if(data[0]>>3==1)
		inc = 12;
		
	if(data[0] == 0)
		inc = 8;
	
	
	if(data[0] > 0)
		PORTC = sine[count];
	else
		PORTC = 127*PORTC/128;
		
	//PORTC = count;
	
	 //adc_value = ADCW;    //Read the ADC value, really that's just it

	//PORTC = count;
	//if(count > 128)
	//PORTC = 0xff;
	//else
	//PORTC = 0x00;
	//PORTC = 0xFF;
	//if(count == 0)
	//l74hc165_shiftin(data);
}


int main(void)
{
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	int n;

	//ADC	
	/*  
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);                //Avcc(+5v) as voltage reference
	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));    //ADC in free-running mode
	ADCSRA |= (1<<ADATE);                //Signal source, in this case is the free-running
	ADCSRA |= (1<<ADEN);                //Power up the ADC
	ADCSRA |= (1<<ADSC);                //Start converting
	*/
	
	//printf ("%d",buffer);
	
	
	l74hc165_init();
	
	DDRC = 0b11111111;
	PORTC = 0xFF;
	
	DDRD |= 0xFF;
	PORTD = 0b11111111;
	
	// Prescaler = FCPU
	TCCR0B |= (1<<CS00);
	//prescaler FCPU/1024
	//TCCR0B |= (1<<CS00) | (1<<CS02);
	
	 //Enable Overflow Interrupt Enable
	TIMSK0 |=(1<<TOIE0);

	 //Initialize Counter
	 TCNT0=0;
	 
	 count=0;
	 
	 sei();
	 
	 //_delay_ms(1000);
	 //lcd_init(LCD_DISP_ON);
	 //_delay_ms(1000);
	 //lcd_clrscr();
	
    while(1)
    {
		l74hc165_shiftin(&data);
		//n=sprintf (buffer, "%u", data);
		//n=sprintf (buffer, "%u", data);
		//_delay_ms(1000);
		//if(data != 0)
		//lcd_puts(buffer);
		//lcd_puts(buffer);
		//_delay_ms(1000);
		//lcd_clrscr();
		//else
			//lcd_putc('0');
		
		//lfsr();
		/*
        PORTC=0;
        _delay_ms(10);
		PORTC=0xff;
		_delay_ms(10);
		*/
		
		/*
		int i = 0;
		for(i=127;i<255;i+=10)
		{
			PORTC = i;
			_delay_ms(5000);
			
		}
		*/
		
		
		//PORTC = 0x40;
    }
}

