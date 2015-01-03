/*
 * lcd_5110_menu.h
 *
 * Created: 2014-12-30 20:38:52
 *  Author: erikhk
 */ 


#ifndef LCD_5110_MENU_H_
#define LCD_5110_MENU_H_

const uint8_t font6x8[92][6];
uint8_t lcd_buffer[504];
//struct menu;
//typedef struct menu menu;

/*
struct menu
{
	char * title;
	uint8_t selected = 0;
	void (*command)();
	short num_submenus;
	struct menu * submenu[4];	//array of submenus
};

typedef struct menu menu;
*/

void read_4_buttons();
void setup_4_buttons();
//void navigate_menu(menu * mnu);

void LCD_draw_square(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t inverted);
void LCD_write_char(uint8_t x, uint8_t y, uint8_t c, uint8_t inverted);
void LCD_write_string(uint8_t x, uint8_t y, char *s, uint8_t inverted);
void LCD_write_command(uint8_t byte);
void LCD_write_buffer();
void LCD_draw_menu(uint8_t inverted);
void LCD_invert_row(uint8_t row);
void LCD_fill_buffer();
void LCD_clear_buffer();
void LCD_init();
void LCD_clear();
void LCD_write();

#endif /* LCD_5110_MENU_H_ */