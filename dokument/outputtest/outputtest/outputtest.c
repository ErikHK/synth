/*
 * outputtest.c
 *
 * Created: 11/12/2014 6:51:16 PM
 *  Author: erikhk
 */

//GAMLA!!
/*
#define LCD_DDR		DDRD
#define LCD_PORT	PORTD
#define CLK			PD7
#define	DIN			PD5
#define	DC			PD4
#define CE			PD0
#define	RST			PD1
*/

//NYA!
/*
#define LCD_DDR		DDRD
#define LCD_PORT	PORTD
#define CLK			PD5
#define	DIN			PD7
#define	DC			PD0
#define CE			PD4
#define	RST			PD2

#define LCD_COMMAND 0
#define LCD_DATA	1
*/

#define F_CPU 20000000L
//#define F_CPU 8000000L
#include "l74hc165.h"
#include "lcd_5110_menu.h"
#include "sounds.h"

//#include "lcd.h"
//#include "lcd.c"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

//NOTE VALUES!
static const uint8_t note_vals[12] = {240,226,214,202,190,180,170,160,151,143,135,127};
// 1/2     1/3     1/4     1/5     1/6     1/7     1/8     1/9     1/10     1/11     1/12
static const uint8_t division_values[] = {64, 42, 32, 26, 21, 18, 16, 14, 13, 12, 11};
//20 Mhz/(128*freq) where freq = C0-B0 = 
//{16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87}
//static const uint16_t note_vals[12] = {9557, 9021, 8515, 8033, 7585, 7158, 6758, 6378, 6019, 5682, 5362, 5062};

//C2 - B2, 65.41 - 123.47
//20000000/(1420*128) = 110.035.. => A4 = 440.1408... OK!
//const uint8_t top_values[12] = {215, 203, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114};
//const uint8_t timer_top = 1420;

//uint8_t detune[11] = {7, 14, 21, 26, 32, 38, 43, 47, 52, 56, 60};
//128/detune, i.e. skip every x:th sample
//uint8_t detune1[5] = {18, 9, 6, 5, 4};
//volatile uint8_t count;

uint8_t curr_wave[256];

uint8_t pot_data;
uint16_t data=0;
uint8_t * adc_value;
uint8_t count = 0;
uint8_t count2 = 0;
uint8_t * buffer[2];
uint8_t inc = 1;

uint8_t keys[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; //pressed buttons, 1 is C, 2 is C# etc
//uint8_t deb_buttons[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//debounced buttons

uint8_t pressed[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//pressed confidence
uint8_t released[12] = {0,0,0,0,0,0,0,0,0,0,0,0};	//released confidence
uint8_t keys_playing[3] = {0,0,0};					//Only allow three keys playing (for now)
uint8_t output = 0;
uint8_t * osc1;
//uint8_t * osc2;
uint8_t attack_value = 0;
uint8_t release_value = 255;
//uint8_t num_keys_playing = 0;

uint8_t lcd_buffer[504];

uint16_t freq1;

uint16_t freq_counter[] = {0,0,0,0,0,0,0,0,0,0,0,0};
	
	
uint8_t out[] = {0,0,0,0,0,0,0,0,0,0,0,0};

/*
uint8_t num_keys_playing()
{
	uint8_t playing=0;
	for (uint8_t i=0;i<12;i++)
	{
		if(deb_buttons[i])
			playing++;
	}
	
	return playing;
}
*/

uint8_t fmul(uint8_t frac, uint8_t x)
{
	//disable interrupts, bad things can and will happen otherwise!
	cli();
	uint8_t * result =  0x01; //points to r1 where the value is stored
	uint8_t * ptr = 23;			//point to R23
	*ptr = frac;				//load frac into R23
	ptr = 22;
	*ptr = x;
	asm("fmul r23,r22");
	sei();
	return (uint8_t)*result;
}

void determine_keys_playing()
{
	
}

void lowpass(uint8_t * signal, uint8_t * orig, uint8_t a, uint8_t b)
{
	for (uint8_t i = 3;i<250;i++)
	{
		//uint8_t tmp = signal[i-1];
		signal[i] = fmul(b,orig[i]) + fmul(b,orig[i-1]) + fmul(a,signal[i-1]);
	}
}
/*
void debounce(uint8_t key, uint8_t i)
{
	
	if(key)
	{
		pressed[i]++;
		if(pressed[i] > 20)
		{
			if(deb_buttons[i] == 0)
			{
				deb_buttons[i] = 1;
			}
			
			pressed[i] = 0;
		}
		
	}
	else
	{
		released[i]++;
		if(released[i] > 20)
		{
			deb_buttons[i] = 0;	//set button to zero
			released[i] = 0;		//reset confidence
		}
	}
}
*/
void populate_buttons()
{
	
	if(data & 1)
		keys[11] = 1;
	else
		keys[11] = 0;
		
	if(data & (1<<1))
	keys[10] = 1;
	else
	keys[10] = 0;
	
	if(data & (1<<2))
	keys[9] = 1;
	else
	keys[9] = 0;
	
	if(data & (1<<3))
	keys[8] = 1;
	else
	keys[8] = 0;
	
	if(data & (1<<4))
	keys[7] = 1;
	else
	keys[7] = 0;
	
	if(data & (1<<5))
	keys[6] = 1;
	else
	keys[6] = 0;
	
	if(data & (1<<6))
	keys[5] = 1;
	else
	keys[5] = 0;
	
	if(data & (1<<7))
	keys[4] = 1;
	else
	keys[4] = 0;
	
	if(data & (1<<8))
	keys[3] = 1;
	else
	keys[3] = 0;
	
	if(data & (1<<9))
	keys[2] = 1;
	else
	keys[2] = 0;
	
	if(data & (1<<10))
	keys[1] = 1;
	else
	keys[1] = 0;
	
	if(data & (1<<11))
	keys[0] = 1;
	else
	keys[0] = 0;
	
	
	/*
	if(data & 1) //4:th button
	  //buttons |= (1<<3);
	  buttons[3] = 1;
	else
	  buttons[3] = 0;
	
	if(data & 2)
	  //buttons |= (1<<2);
	  buttons[2] = 1;
	else
	  buttons[2] = 0;
	
	if(data & 4)
	  //buttons |= (1<<1);
	  buttons[1] = 1;
	else
	  buttons[1] = 0;
	
	if(data & 8)
	  //buttons |= (1<<0);
	  buttons[0] = 1;
	else
	  buttons[0] = 0;
	  
	if(data & 256)
	  //buttons |= (1<<11);
	  buttons[11] = 1;
	else
	  buttons[11] = 0;
	  
	if(data & 512)
	  //buttons |= (1<<10);
	  buttons[10] = 1;
	else
	  buttons[10] = 0;
	  
	if(data & 1024)
	  //buttons |= (1<<9);
	  buttons[9] = 1;
	else
	  buttons[9] = 0;
	
	if(data & 2048)
	  //buttons |= (1<<8);
	  buttons[8] = 1;
	else
	  buttons[8] = 0;
	  
	if(data & 4096)
	  //buttons |= (1<<7);
	  buttons[7] = 1;
	else
	  buttons[7] = 0;
	
	if(data & 8192)
	  //buttons |= (1<<6);
	  buttons[6] = 1;
	else
	  buttons[6] = 0;
	
	if(data & 16384)
	  //buttons |= (1<<5);
	  buttons[5] = 1;
	else
	  buttons[5] = 0;
	  
	if(data & 32768) //5:th button
	  //buttons |= (1<<4);
	  buttons[4] = 1;
	else
	  buttons[4] = 0;
	  */
	  
	  
	  //debounce
	  /*
	  for (uint8_t i=0;i<12;i++)
	  {
		  debounce(buttons[i],i);
	  }
	  */
	  
	  
}

/*
void lfsr()
{
	uint16_t lfsr = 0xACE1u;
    unsigned period = 0;
    char s[16];

    do {
          unsigned lsb = lfsr & 1;  // Get lsb (i.e., the output bit).
          lfsr >>= 1;               // Shift register 
          if (lsb == 1)             // Only apply toggle mask if output bit is 1. 
            lfsr ^= 0xB400u;        // Apply toggle mask, value has 1 at bits corresponding
                                    // to taps, 0 elsewhere. 
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
*/

/*
ISR(ADC_vect)
{
	
	//char adc_res[4];
	pot_data = ADCH;
	//itoa(pot_data, adc_res, 10);
	//LCD_write_string(5,5,&adc_res);
	
	//disable ADC again
	ADCSRA &= ~(1<<ADEN);
	
	//ADCSRA |= (1<<ADSC);
}
*/

ISR(TIMER2_COMPA_vect)
{
	shiftin();
	populate_buttons();
	
	/*
	static uint8_t c = 0;
	c++;
	
	
	if(c == 200)
	{
		navigate_menu();
		read_4_buttons();
		LCD_clear_buffer();
		LCD_draw_menu(0);
		LCD_write_buffer();
		c = 0;
	}
	
	
	//button is pressed
	if(deb_buttons[2])
	{
		//increase attack!
		if(attack_value < get_attack_value())
		  attack_value+=1;
		  
		//reset release_value
		release_value = attack_value;
		
	}
	
	//button is released
	if(!deb_buttons[2])
	{
		attack_value = 0;
		if(release_value > get_release_value())
		release_value--;
		
	}
	*/
	//PORTC = osc1[count];
}

ISR(TIMER1_COMPA_vect)
{	
	for (uint8_t i=0;i<12;i++)
	{
		//out[i] = 0;
		if(keys[i])
		out[i] = osc1[freq_counter[i]>>8];
	}
	
	
	//l74hc165_shiftin(&data);
	//shiftin();
	//data = 100;
	//populate_buttons();
	PORTC = (out[0] + out[2] + out[4])>>2;
}


void shiftin()
{
	static uint8_t i = 0;
	static uint16_t dat = 0;
	//parallel load to freeze the state of the data lines
	if(i==0)
	{
		L74HC165_PORT &= ~(1 << L74HC165_LOADPIN);
		asm("nop");
		asm("nop");
		asm("nop");
		//_delay_us(50);
		L74HC165_PORT |= (1 << L74HC165_LOADPIN);
	}
	
	dat |= ((L74HC165_PIN & (1 << L74HC165_DATAPIN))>>L74HC165_DATAPIN)<<(15-i);
	//get next
	L74HC165_PORT |= (1 << L74HC165_CLOCKPIN);
	//_delay_us(5);
	asm("nop");
	asm("nop");
	L74HC165_PORT &= ~(1 << L74HC165_CLOCKPIN);
	i++;
	
	//reset i
	if(i > 15)
	{
		i = 0;
		//copy 2 bytes of data
		memcpy(&data, &dat, 2);
		dat = 0;
		//data = &dat;
	}
	
}



//ISR(TIMER0_OVF_vect)
ISR(TIMER0_COMPA_vect)
{

	//static uint8_t out2 = 0;//square_[freq2_counter>>8]>>3;
	//uint8_t out3 = sawtooth[freq3_counter>>8]>>3;
	//PORTC = out1+out2;
	//PORTC = out[0] + out[1] + out[2] + out[3] + out[4] + out[5] + out[6] + out[7] + out[8] + out[9] + out[10] + out[11];
	//PORTC = out[0] + out[2] + out[4];
	//freq1_counter += freq1;
	freq_counter[0] += 439*2; // 200 Hz
	//freq_counter[1] += 465; // 240 Hz
	freq_counter[2] += 493*2; //etc
	
	//freq_counter[3] += 522; 
	freq_counter[4] += 553*2;
	/*
	freq_counter[5] += 586;
	freq_counter[6] += 621;
	freq_counter[7] += 658;
	freq_counter[8] += 697;
	freq_counter[9] += 738;
	freq_counter[10] += 782;
	freq_counter[11] += 829;
	*/
	
// 	for (uint8_t i=0;i<12;i++)
// 	{
// 		out[i] = 0;
// 		if(keys[i])
// 			out[i] = osc1[freq_counter[i]>>8]>>2;
// 	}
// 	
	//populate_buttons();
	/*
	if(keys[0])
	{
		out[0] = sawtooth[freq_counter[0]>>8]>>2;
	}else
		out[0] = 0;
	
	if(keys[1])
	{
		out[1] = sawtooth[freq_counter[1]>>8]>>2;
	}else
		out[1] = 0;
		
	if(keys[2])
	{
		out[2] = sawtooth[freq_counter[2]>>8]>>2;
	}else
		out[2] = 0;
	
	if(keys[3])
	{
		out[3] = sawtooth[freq_counter[3]>>8]>>2;
	}else
		out[3] = 0;
		*/
}

void setup_timer1()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TCNT1 = 0;
	
	//TCCR1A |= (1<<COM1A1) | (1<<COM1A0) | (1<<WGM11);
	TCCR1A |= (1<<WGM11);
	//TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10);
	//prescaler = 8
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11);
	
	//prescaler = 1024
	//TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10) | (1<<CS12);
	
	//prescaler = 256
	//TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS12);
	
	//OCR1A = 0xC000;	//set
	//OCR1A = 0xA000;
	//OCR1A = 100;
	//ICR1 = 0xF000;	//clear
	ICR1 = 100;	//clear
	OCR1A = 50;
	
	TIMSK1 =(1<<OCIE1A);
	//TIFR1 &= ~(1<<OCF1A);
}

void setup_timer2()
{
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;
	//OCR2A = 5;
	OCR2A = 5000;
	
	TCCR2A |= (1<<WGM21) | (1<<WGM20);
	// Prescaler = FCPU/1024
	TCCR2B |= (1<<CS20) | (1<<CS22) | (1<<WGM22);
	
	TIMSK2 = (1<<OCIE2A);
}

//f = 20000000/512 = 39062.5 Hz
void setup_timer0()
{
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
	//OCR0A = 200;
	OCR0A = 64;
	//OCR0A = 8;
	
	TCCR0A |= (1<<WGM01) | (1<<WGM00);
	// Prescaler = FCPU
	//TCCR0B |= (1<<CS00) | (1<<WGM02);
	//prescaler = x
	//TCCR0B |= (1<<CS00) | (1<<CS01) | (1<<WGM02);
	//prescaler FCPU/1024
	//TCCR0B |= (1<<CS00) | (1<<CS02) | (1<<WGM02);
	//prescaler FCPU/8
	TCCR0B |= (1<<CS01) | (1<<WGM02);
	
	//TCCR0B |= (1<<FOC0B);
	
	//Enable Overflow Interrupt Enable
	//TIMSK0 |=(1<<TOIE0);
	TIMSK0 = (1<<OCIE0A);
	//TIMSK0 |=(1<<OCIE0B);
	
	//uint16_t Fs = 20000000>>9;
	//freq1 = (200<<16)/Fs;
}

void setup_adc()
{
	ADMUX = 0;
	ADCSRA = 0;
	//enable ADC, 1 1 1 = 128 prescaler = 8 MHz/128 = 62.5 kHz
	//ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADIE) | (1<<ADSC);
	//no interrupts!
	ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADSC);
	
	ADMUX |= (1<<ADLAR) | (1<<REFS0);
}

int main(void)
{
	sei();
	l74hc165_init();
	//osc1 = pseudosquare;
	
	osc1 = square_;
	//osc1 = &sine2x;
	
	//0.1
	//lowpass(osc1, square2x, 0b01100000, 0b00010100);
	//0.4
	//lowpass(osc1, square2x, 0b00010000, 0b00110110);
	//0.2
	//lowpass(osc1, square2x, 0b01000000, 0b00100000);
	//0.05
	//lowpass(osc1, square2x, 0b01110000, 0b00001001);
	//0.025
	//lowpass(osc1, square2x, 0b01110111, 0b00000101);
	
	//osc1 = prutt;
	//int n;
	DDRC = 0xff;
	
	//LCD_init();
	//LCD_clear();
	//setup_adc();
	/*
	char test[10];
	memset(test,0,sizeof(test[0])*10); // Clear all to 0 so string properly represented
	
	uint8_t fmulres = fmul(0b00010000, 255);
	//sprintf(test, "hej %i", fmulres);
	itoa(fmulres, test, 10);
	//itoa(36, test, 10);
	//LCD_write_string(0,0,"Erik <3 Klara!");
	LCD_write_string(0,0,test);
	*/
	
	//LCD_draw_menu(0);
	//LCD_write_buffer();
	
	/*
	LCD_set_XY(0,0);
	LCD_clear();
	for(uint8_t i=0;i<84;i++)
	{
		int8_t t = (sine[3*i]-0x80)>>3;
		LCD_set_pixel(i-1,0+osc1[3*i]>>3);
	}
	*/
	setup_timer1();
	setup_timer0();
	setup_timer2();
	//setup_4_buttons();
	//setup_adc();
	//setup_menu();
	
	while(1)
    {
		
		//shiftin();
		
		//populate_buttons();
		/*
		navigate_menu();
		read_4_buttons();
		LCD_clear_buffer();
		LCD_draw_menu(0);
		LCD_write_buffer();
		_delay_ms(200);
		*/
		
		//trigger new adc thingy
		//ADCSRA |= (1<<ADSC);
		
		//l74hc165_shiftin(&data);
		//populate_buttons();
		//_delay_us(10);
		/*
		for (uint8_t i=0;i<12;i++)
		{
			debounce(buttons[i],i);
		}
		*/
		/*
		static uint8_t iii = 0;
		LCD_write_string(iii++, 3, "Erik <3 Klara!", 0);
		LCD_write_buffer();
		if(iii > 14)
			iii = 0;
		_delay_ms(100);
		LCD_clear_buffer();
		*/
		
		/*
		itoa(pot_data, test, 10);
		LCD_write_string(5,5,&test);
		_delay_ms(100);
		LCD_clear();
		*/
    }
}