/*
 * lcd_5110_menu.c
 *
 * Created: 2014-12-30 20:39:09
 *  Author: erikhk
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define WIDTH 84
#define HEIGHT 48
//#define F__CPU 8000000L

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

#define LCD_DDR		DDRD
#define LCD_PORT	PORTD
#define CLK			PD5
#define	DIN			PD7
#define	DC			PD0
#define CE			PD4
#define	RST			PD2

#define LCD_COMMAND 0
#define LCD_DATA	1

//4 buttons
#define DDRLEFT		DDRD
#define PINLEFT		PIND
#define PORTLEFT	PORTD
#define LEFT		PD6

#define DDRPORT3	DDRB
#define PORT3		PORTB
#define PIN3		PINB
#define RIGHT		PB1
#define UP			PB2
#define DOWN		PB3


const uint8_t font6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

//menus
#define MAIN 0;
#define ADSR 1;
#define OSC 2;

const char *main_menu[] = {"Oscillators", "ADSR", "Filters", "Sounds"};
const char *adsr_menu[] = {"Attack", "Decay", "Sustain", "Release"};
const char *oscs_menu[] = {"Osc 1", "Osc 2", "Osc 3", "LFO"};

//const char osc_menu[4][12] = {"Waveform", "Oscillator 2", "Oscillator 3", "LFO"};
const char waveforms[4][12] = {"squ", "sin", "saw", "tri"};

const char filters[2][12] = {"low", "high"};

uint8_t lcd_buffer[504];
uint8_t buttons[] = {0, 0, 0, 0};
uint8_t menuchoice = 0;
uint8_t ** current_menu = main_menu;

uint8_t attack_top = 240;
uint8_t decay_top = 0;
uint8_t sustain_top = 0;
uint8_t release_top = 200;

uint8_t * pot_data;
uint8_t prev_pot_data = 0;

struct menu
{
	char * title;
	uint8_t selected;
	uint8_t * value;
	void (*command)();
	short num_submenus;
	struct menu * submenu[6];	//array of submenus
	struct menu * parent;
} menu_default = {NULL, 0, NULL, 0, NULL, NULL};

typedef struct menu menu;

menu attack = {NULL, 0, NULL, 0, NULL, NULL};
menu decay = {NULL, 0, NULL, 0, NULL, NULL};
menu sustain = {NULL, 0, NULL, 0, NULL, NULL};
menu release = {NULL, 0, NULL, 0, NULL, NULL};
menu adsrmenu = {NULL, 0, NULL, 0, NULL, NULL};
menu mainmenu = {NULL, 0, NULL, 0, NULL, NULL};
menu oscmenu = {NULL, 0, NULL, 0, NULL, NULL};
menu osc1menu = {NULL, 0, NULL, 0, NULL, NULL};
menu waveformmenu = {NULL, 0, NULL, 0, NULL, NULL};
menu levelmenu = {NULL, 0, NULL, 0, NULL, NULL};
menu osc2menu = {NULL, 0, NULL, 0, NULL, NULL};
menu lfomenu = {NULL, 0, NULL, 0, NULL, NULL};
menu filtersmenu = {NULL, 0, NULL, 0, NULL, NULL};

/*
ISR(ADC_vect)
{
	*pot_data = ADCH;
	//ADCSRA |= (1<<ADSC);
	
	//disable ADC again
	ADCSRA &= ~(1<<ADEN);
}
*/

void attack_command()
{
	//LCD_write_string(0,0,"BAAAAJS",0);
	//LCD_write_buffer();
	//if(*pot_data != prev_pot_data)
	pot_data = &attack_top;
	
	//enable ADC and start conversion
	//ADCSRA |= (1<<ADEN) | (1<<ADSC);
}

void decay_command()
{
	//LCD_write_string(0,0,"BAAAAJS",0);
	//LCD_write_buffer();
	//if(*pot_data != prev_pot_data)
	pot_data = &decay_top;
	
	//enable ADC and start conversion
	//ADCSRA |= (1<<ADEN) | (1<<ADSC);
}

void sustain_command()
{
	//LCD_write_string(0,0,"BAAAAJS",0);
	//LCD_write_buffer();
	//if(*pot_data != prev_pot_data)
	pot_data = &sustain_top;
	
	//enable ADC and start conversion
	//ADCSRA |= (1<<ADEN) | (1<<ADSC);
}

void release_command()
{
	//if(*pot_data != prev_pot_data)
	pot_data = &release_top;
	
	//enable ADC
	//ADCSRA |= (1<<ADEN) | (1<<ADSC);
}

uint8_t get_attack_value()
{
	return attack_top;
}

uint8_t get_release_value()
{
	return release_top;
}

menu * currentmenu;

void setup_4_buttons()
{	
	//set inputs
	DDRLEFT &= ~(1 << LEFT);
	DDRPORT3 &= ~(1 << RIGHT);
	DDRPORT3 &= ~(1 << UP);
	DDRPORT3 &= ~(1 << DOWN);
	
	//enable pull-up
	PORTLEFT |= (1 << LEFT);
	PORT3 |= (1 << RIGHT) | (1 << DOWN) | (1 << UP);
}

void navigate_menu()
{
	if(buttons[2] && currentmenu->selected < 3)
		currentmenu->selected++;
	if(buttons[1] && currentmenu->selected > 0)
		currentmenu->selected--;
		
	if(buttons[3])
	{
		//set submenu's parent
		currentmenu->submenu[currentmenu->selected]->parent = currentmenu;
		//set current menu as selected submenu
		currentmenu = currentmenu->submenu[currentmenu->selected];
	}
	
	//if the selected menu item has a command
	if(currentmenu->submenu[currentmenu->selected]->command != NULL)
		currentmenu->submenu[currentmenu->selected]->command();
	//currentmenu->submenu[currentmenu->selected]->command();
	
	if(buttons[0])
		currentmenu = currentmenu->parent;
}


void setup_menu()
{
	filtersmenu.title = "Filters";
	
	lfomenu.title = "LFO";
	lfomenu.submenu[0] = &waveformmenu;
	lfomenu.submenu[1] = &levelmenu;
	
	waveformmenu.title = "Waveform";
	waveformmenu.command = &attack_command;
	levelmenu.title = "Level";
	
	osc1menu.title = "Oscillator 1";
	osc1menu.submenu[0] = &waveformmenu;
	osc1menu.submenu[1] = &levelmenu;
	
	osc2menu.title = "Oscillator 2";
	osc2menu.submenu[0] = &waveformmenu;
	osc2menu.submenu[1] = &levelmenu;
	
	oscmenu.title = "Oscillators";
	oscmenu.submenu[0] = &osc1menu;
	oscmenu.submenu[1] = &osc2menu;
	oscmenu.submenu[2] = &lfomenu;
	
	//strcpy(attack->title, "attack");
	attack.title = "Attack";
	attack.command = *attack_command;
	//attack.submenu = NULL;
	
	decay.title = "Decay";
	decay.command = *decay_command;
	
	sustain.title = "Sustain";
	sustain.command = *sustain_command;
	
	
	release.title = "Release";
	release.command = *release_command;
	
	//release.submenu = NULL;
	
	adsrmenu.title = "ADSR";
	//adsrmenu.command = *attack_command;
	adsrmenu.submenu[0] = &attack;
	adsrmenu.submenu[1] = &decay;
	adsrmenu.submenu[2] = &sustain;
	adsrmenu.submenu[3] = &release;
	//adsrmenu.submenu[4] = NULL;		//NULL-terminate, might work without
	
	mainmenu.title = NULL;
	mainmenu.parent = &mainmenu;
	mainmenu.submenu[0] = &oscmenu;
	mainmenu.submenu[1] = &adsrmenu;
	mainmenu.submenu[2] = &filtersmenu;
	
	currentmenu = &mainmenu;
}


void read_4_buttons()
{
	//left is pressed
	if(PINLEFT & (1 << LEFT))
		buttons[0] = 0;
	else
		buttons[0] = 1;
		
	if(PIN3 & (1 << RIGHT))
		buttons[1] = 0;
	else
		buttons[1] = 1;

	if(PIN3 & (1 << UP))
		buttons[2] = 0;
	else
		buttons[2] = 1;

	if(PIN3 & (1 << DOWN))
		buttons[3] = 0;
	else
		buttons[3] = 1;
		
	//if(buttons[0])
	//	current_menu = main_menu;
	//if(buttons[3])
	//    current_menu = adsr_menu;
		//currentmenu.command();
		
	/*
	//up
	if(buttons[1] && menuchoice > 0)
	  menuchoice--;
	
	//down
	if(buttons[2] && menuchoice < 3)
	  menuchoice++;
	*/
}

void LCD_init()
{
	//try some shit
	LCD_DDR = 0xFF;
	LCD_PORT &= ~(1<<RST);
	_delay_ms(100);
	LCD_PORT |= (1<<RST);
	LCD_PORT &= ~(1<<CE);
	_delay_ms(5);
	LCD_PORT &= ~(1<<RST);
	asm("nop");
	LCD_PORT |= (1<<RST);
	
	//set output
	//LCD_DDR = (1<<CLK)|(1<<DIN)|(1<<DC)|(1<<CE)|(1<<RST);
	//DDRD |= 0xff;
	
	//LCD_PORT = 0;
	
	//reset
	LCD_PORT &= ~(1<<RST);
	_delay_us(1);
	LCD_PORT |= (1<<RST);
	
	LCD_PORT &= ~(1<<CE);
	_delay_us(1);
	LCD_PORT |= (1<<CE);
	_delay_us(1);
	
	LCD_PORT |= (1<<RST);
	
	LCD_write_command(0x21);	//Tell LCD that extended commands follow
	//LCD_Write(LCD_COMMAND, 0xC1);	//Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark, funkar p� f�rsta!
	LCD_write_command(0xC1);	//Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
	LCD_write_command(0x06);	//Set temp coeff
	LCD_write_command(0x13);	//LCD bias mode 1:48: Try 0x13 or 0x14
	
	LCD_write_command(0x20);	//We must send 0x20 before modifying the display control mode
	LCD_clear();
	LCD_write_command(0x0C);	//Set display control, normal mode. 0x0D for inverse, 0x0C for non-inverse
	//LCD_Write(LCD_COMMAND, 0x09);	//all segments on
	LCD_PORT &= ~(1<<CE);
	
	/*
	for (uint8_t i=0;i<504;i++)
	{
		lcd_buffer[i] = 0x00;
	}
	*/
}

/*
void LCD_write_string(uint8_t x, uint8_t y, char *s, uint8_t inverted)
{
	while (*s)
	{
		LCD_write_char(x, y, *s++, inverted);
	}
}
*/

void LCD_write_command(uint8_t byte)
{
	uint8_t i;
	
	//set CE low (chip enable, inverted input)
	LCD_PORT &= ~(1<<CE);
	
	//tell the display it's a command or data
	LCD_PORT &= ~(1<<DC);
	
	for (i = 0; i < 8; i++)
	{

		// consider leftmost bit
		// set line high if bit is 1, low if bit is 0
		if (byte & 0x80)
		LCD_PORT |= (1<<DIN);
		else
		LCD_PORT &= ~(1<<DIN);
		
		// pulse clock to indicate that bit value should be read
		LCD_PORT &= ~(1<<CLK);
		// shift byte left so next bit will be leftmost
		byte <<= 1;
		LCD_PORT |= (1<<CLK);
	}
	
	//set CE high
	LCD_PORT |= (1<<CE);
}

void LCD_write_string(uint8_t x, uint8_t y, char *s, uint8_t inverted)
{
	uint8_t i = 0;
	while (*s)
	{
		LCD_write_char(x + i++, y, *s++, inverted);
	}
}

void LCD_write_char(uint8_t x, uint8_t y, uint8_t c, uint8_t inverted)
{
	uint8_t line;

	for (line=0; line<6; line++)
	{
		//LCD_Write_data_at(x,y,font6x8[c-0x20][line], inverted);
		if(inverted)
		  lcd_buffer[x*6+y*WIDTH+line] = ~font6x8[c-0x20][line];
		else
		  lcd_buffer[x*6+y*WIDTH+line] = font6x8[c-0x20][line];
	}
	
}


void LCD_Write_data(uint8_t byte, uint8_t inverted)
{
	static uint16_t ii = 0;
	
	if(inverted)
	lcd_buffer[ii] = ~byte;
	else
	lcd_buffer[ii] = byte;
	
	if(ii > 504)
	ii = 0;
	else
	ii++;
}


void LCD_Write_data_at(uint8_t x, uint8_t y, uint8_t byte, uint8_t inverted)
{
	static uint16_t ii=0;
	
	if(inverted)
		lcd_buffer[x+y*WIDTH+ii] = ~byte;
	else
		lcd_buffer[x+y*WIDTH+ii] = byte;
		
	if(ii > 504)
		ii = 0;
	else
		ii++;
}



void LCD_set_pixel(uint8_t X, uint8_t Y, uint8_t inverted)
{
	uint8_t byte = 0;
	//determine which byte
	uint8_t row = Y>>3;
	
	uint8_t shift = Y-row*8;
	
	if(inverted)
	lcd_buffer[X+row] = (lcd_buffer[X+row] & ~(0x01>>shift));
	else
	lcd_buffer[X+row] = (lcd_buffer[X+row] | 0x01>>shift);
}

void LCD_draw_square(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t inverted)
{
	
	for (uint8_t i=y1;i<y2;i++)
	{
		for (uint8_t j=x1;j<x2;j++)
		{
			LCD_set_pixel(j,i, inverted);
		}
		
	}
	
}

void LCD_clear()          // clear the LCD
{
	LCD_clear_buffer();
	LCD_write_buffer();
}

void LCD_write_buffer()
{
	uint8_t i;
	uint16_t j;
	
	//set CE low (chip enable, inverted input)
	LCD_PORT &= ~(1<<CE);
	
	LCD_PORT |= (1<<DC);
	for (j=0;j<504;j++)
	{
		for (i = 0; i < 8; i++)
		{
			// consider leftmost bit
			// set line high if bit is 1, low if bit is 0
			if (lcd_buffer[j] & 0x80)
			LCD_PORT |= (1<<DIN);
			else
			LCD_PORT &= ~(1<<DIN);
			
			// pulse clock to indicate that bit value should be read
			LCD_PORT &= ~(1<<CLK);
			// shift byte left so next bit will be leftmost
			lcd_buffer[j] <<= 1;
			//_delay_us(10);
			asm("nop");
			LCD_PORT |= (1<<CLK);
		}
	}
	//set CE high
	LCD_PORT |= (1<<CE);
}

void LCD_fill_buffer()
{
	for (uint16_t i=0;i<504;i++)
	{
		lcd_buffer[i] = 0xff;
	}
}

void LCD_clear_buffer()
{
	for (uint16_t i=0;i<504;i++)
	{
		lcd_buffer[i] = 0x00;
	}
}


void LCD_invert_row(uint8_t row)
{
	uint8_t i;
	for (i=0;i<WIDTH;i++)
	{
		lcd_buffer[i+WIDTH*row] = ~lcd_buffer[i+WIDTH*row];
	}
}

/*
void LCD_draw_menu(uint8_t inverted)
{
	for (uint8_t i=0;i<4;i++)
	{
		LCD_write_string(0, i, current_menu[i], inverted);
		if(i == menuchoice)
		  LCD_invert_row(i);
	}
}
*/

void LCD_draw_menu(uint8_t inverted)
{
	//struct menu * currmenu = currentmenu;
	//menu * currmenuitems = *currentmenu->submenu;
	
	if(ADCSRA | ADIF)
	{
		//attack_top = ADCH;
		prev_pot_data = *pot_data;
		*pot_data = ADCH;
	
		//trigger new conversion
		ADCSRA |= (1<<ADSC);
	}
	
	//attack_top = 12;
	char adc_res[4];
	itoa(&pot_data, adc_res, 10);
	//LCD_write_string(5,5,&adc_res);
	
	uint8_t i=0;
	//for (uint8_t i=0;i<3;i++)
	while(currentmenu->submenu[i])
	{
		LCD_write_string(0, i, currentmenu->submenu[i]->title, 0);
		
		if(currentmenu->selected == i)
			LCD_invert_row(i);
		
		//if it has a value, print it!
		if(currentmenu->submenu[i]->value)
		{
			itoa(*currentmenu->submenu[i]->value, adc_res, 10);
			LCD_write_string(11, i, adc_res, 0);
			//LCD_write_string(11, i, currentmenu->submenu[i]->value, 0);
		}
		
		if(currentmenu->submenu[i]->command == attack_command)
		{
			itoa(fmul(50, attack_top), adc_res, 10);
			//attack_top = adc_res;
			*currentmenu->submenu[i]->value = attack_top;
			LCD_write_string(11, i, adc_res, 0);
		}
		
		if(currentmenu->submenu[i]->command == release_command)
		{
			itoa(fmul(50, release_top), adc_res, 10);
			//release_top = adc_res;
			*currentmenu->submenu[i]->value = release_top;
			LCD_write_string(11, i, adc_res, 0);
		}
		
		if(currentmenu->submenu[i]->command == decay_command)
		{
			itoa(fmul(50, decay_top), adc_res, 10);
			//release_top = adc_res;
			*currentmenu->submenu[i]->value = decay_top;
			LCD_write_string(11, i, adc_res, 0);
		}
		
		if(currentmenu->submenu[i]->command == sustain_command)
		{
			itoa(fmul(50, sustain_top), adc_res, 10);
			//release_top = adc_res;
			*currentmenu->submenu[i]->value = sustain_top;
			LCD_write_string(11, i, adc_res, 0);
		}
		
		//LCD_write_string(0, i, currmenuitems->title, 0);
		i++;
		//currmenuitems++;
	}
	
	//currmenu = currmenu->submenu;
}
