/*
 * outputtest.c
 *
 * Created: 11/12/2014 6:51:16 PM
 *  Author: erikhk
 */ 

//#define F_CPU 20000000L
#define F_CPU 8000000L
#include "l74hc165.h"
#include "lcd.h"
//#include "lcd.c"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//NOTE VALUES!
uint8_t note_vals[12] = {1,1,1,1,1,1,1,1,1,1,1,1};

//volatile uint8_t count;
uint8_t sine[] = {128,131,134,137,140,144,147,150,153,156,159,162,165,168,171,174,177,180,182,185,188,191,194,196,199,201,204,206,209,211,214,216,218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,244,246,247,248,249,250,251,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,253,253,252,252,251,250,249,248,247,246,245,244,242,241,240,238,236,235,233,231,229,227,225,223,221,219,217,215,212,210,208,205,203,200,197,195,192,189,187,184,181,178,175,172,169,167,164,160,157,154,151,148,145,142,139,136,133,130,126,123,120,117,114,111,108,105,102,99,96,92,89,87,84,81,78,75,72,69,67,64,61,59,56,53,51,48,46,44,41,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,8,7,6,5,4,4,3,3,2,2,1,1,1,1,1,1,1,2,2,2,3,3,4,5,5,6,7,8,9,10,12,13,14,16,17,19,20,22,24,26,28,30,32,34,36,38,40,42,45,47,50,52,55,57,60,62,65,68,71,74,76,79,82,85,88,91,94,97,100,103,106,109,112,116,119,122,125,128};
uint8_t * data[2];
uint8_t * adc_value;
uint8_t count;
uint8_t * buffer[2];
uint8_t inc = 1;
uint8_t buttons[12]; //pressed buttons, 1 is C, 2 is C# etc, 13-16 = 0
uint8_t output = 0;

void populate_buttons()
{
	if((uint16_t)*data & 1) //4:th button
	  //buttons |= (1<<3);
	  buttons[3] = 1;
	else
	  buttons[3] = 0;
	
	if((uint16_t)*data & 2)
	  //buttons |= (1<<2);
	  buttons[2] = 1;
	else
	  buttons[2] = 0;
	
	if((uint16_t)*data & 4)
	  //buttons |= (1<<1);
	  buttons[1] = 1;
	else
	  buttons[1] = 0;
	
	if((uint16_t)*data & 8)
	  //buttons |= (1<<0);
	  buttons[0] = 1;
	else
	  buttons[0] = 0;
	  
	if((uint16_t)*data & 256)
	  //buttons |= (1<<11);
	  buttons[11] = 1;
	else
	  buttons[11] = 0;
	  
	if((uint16_t)*data & 512)
	  //buttons |= (1<<10);
	  buttons[10] = 1;
	else
	  buttons[10] = 0;
	  
	if((uint16_t)*data & 1024)
	  //buttons |= (1<<9);
	  buttons[9] = 1;
	else
	  buttons[9] = 0;
	
	if((uint16_t)*data & 2048)
	  //buttons |= (1<<8);
	  buttons[8] = 1;
	else
	  buttons[8] = 0;
	  
	if((uint16_t)*data & 4096)
	  //buttons |= (1<<7);
	  buttons[7] = 1;
	else
	  buttons[7] = 0;
	
	if((uint16_t)*data & 8192)
	  //buttons |= (1<<6);
	  buttons[6] = 1;
	else
	  buttons[6] = 0;
	
	if((uint16_t)*data & 16384)
	  //buttons |= (1<<5);
	  buttons[5] = 1;
	else
	  buttons[5] = 0;
	  
	if((uint16_t)*data & 32768) //5:th button
	  //buttons |= (1<<4);
	  buttons[4] = 1;
	else
	  buttons[4] = 0;
	  
}

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



//ISR(TIMER0_OVF_vect)
ISR(TIMER0_COMPA_vect)
{
	
	count += inc;
	
	/*
	if(*data > 0)
	  inc = 2;
	  
	if(*data > 1)
	  inc = 4;
	  
	if(*data > 2)
	  inc = 6;
	  
	if(*data > 4)
	  inc = 8;
	  
	if(*data > 8)
	  inc = 10;
	  
	if(*data > 16)
	  inc = 12;
	  
	if(*data > 32)
	  inc = 14;
	
	if(*data > 64)
	  inc = 16;
	  
	if(*data > 128)
	  inc = 18;
	
	if(*data > 256)
	  inc = 20;
	
	if(*data > 512)
	  inc = 22;
	
	if(*data > 1024)
	  inc = 24;
	
	if(*data > 2048)
	  inc = 26;
	
	if(*data > 4096)
	inc = 28;
	
	if(*data > 8192)
	inc = 30;
	
	if(*data > 16384)
	inc = 32;
	*/
	
	//populate_buttons();
	
	/*
	if((uint16_t)*data & 1)
	  inc = 10;
	
	if((uint16_t)*data & 2)
	  inc = 8;
	
	if((uint16_t)*data & 4)
	  inc = 4;
	
	if((uint16_t)*data & 8)
	  inc = 2;
	
	if((uint16_t)*data & 256)
	  inc = 26;
	  
	if((uint16_t)*data & 512)
	  inc = 24;
	  
	if((uint16_t)*data & 1024)
	  inc = 22;
	
	if((uint16_t)*data & 2048)
	  inc = 20;
	  
	if((uint16_t)*data & 4096)
	  inc = 18;
	
	if((uint16_t)*data & 8192)
	  inc = 16;
	
	if((uint16_t)*data & 16384)
	  inc = 14;
	  
	if((uint16_t)*data & 32768)
	  inc = 12;
	  
	*/  
	/*
	if(*data[1] > 4)
		inc = 3;
	else
		inc = 0;
	
	if((*data & 0xFFFD)==1)
		inc = 4;
	
	if(*data & 0xFFFB==1)
		inc = 5;
		
	if((*data & 0xFFF7)==1)
		inc = 6;
	
	
	if(data[1]>>3==1)
		inc = 9;
		
	if(data[0] == 0)
		inc = 8;
	
	*/
	/*
	if(*data > 0)
	//if(*data[0] > 0 || *data[1] > 0)
		PORTC = sine[count];
	else
		PORTC = 127*PORTC/128;
	*/
	/*
	if(buttons[0])
		{
		PORTC = sine[count];
		OCR0A = 250;
		}
		
	else
		PORTC = 127*PORTC/128;
		
	if(buttons[1])
	{
		PORTC = sine[count];
		OCR0A = 1;
	}
	else
		PORTC = 127*PORTC/128;
	*/
	PORTC = sine[count];
	OCR0A = 15;
	/*
	if(*buttons > 0)
	  PORTC = sine[count];
	*/
		
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
	/*
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	lfsr();
	*/
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
	
	
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
	OCR0A = 100;
	
	TCCR0A |= (1<<WGM01);// | (1<<WGM00);
	// Prescaler = FCPU
	TCCR0B |= (1<<CS00);
	//prescaler FCPU/1024
	//TCCR0B |= (1<<CS00) | (1<<CS02);
	
	//TCCR0B |= (1<<FOC0B);
	
	
	 //Enable Overflow Interrupt Enable
	//TIMSK0 |=(1<<TOIE0);
	TIMSK0 =(1<<OCIE0A);
	//TIMSK0 |=(1<<OCIE0B);
	
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

