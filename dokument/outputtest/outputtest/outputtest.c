/*
 * outputtest.c
 *
 * Created: 11/12/2014 6:51:16 PM
 *  Author: erikhk
 */ 

#define LCD_DDR		DDRD
#define LCD_PORT	PORTD
#define CLK			PD7
#define	DIN			PD5
#define	DC			PD4
#define CE			PD0
#define	RST			PD1

#define LCD_COMMAND 0
#define LCD_DATA	1

//#define F_CPU 20000000L
#define F_CPU 8000000L
#include "l74hc165.h"
//#include "lcd.h"
//#include "lcd.c"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//NOTE VALUES!
static const uint8_t note_vals[12] = {240,226,214,202,190,180,170,160,151,143,135,127};

//volatile uint8_t count;
const uint8_t sine[] = {128,131,134,137,140,144,147,150,153,156,159,162,165,168,171,174,177,180,182,185,188,191,194,196,199,201,204,206,209,211,214,216,218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,244,246,247,248,249,250,251,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,253,253,252,252,251,250,249,248,247,246,245,244,242,241,240,238,236,235,233,231,229,227,225,223,221,219,217,215,212,210,208,205,203,200,197,195,192,189,187,184,181,178,175,172,169,167,164,160,157,154,151,148,145,142,139,136,133,130,126,123,120,117,114,111,108,105,102,99,96,92,89,87,84,81,78,75,72,69,67,64,61,59,56,53,51,48,46,44,41,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,8,7,6,5,4,4,3,3,2,2,1,1,1,1,1,1,1,2,2,2,3,3,4,5,5,6,7,8,9,10,12,13,14,16,17,19,20,22,24,26,28,30,32,34,36,38,40,42,45,47,50,52,55,57,60,62,65,68,71,74,76,79,82,85,88,91,94,97,100,103,106,109,112,116,119,122,125,128};
const uint8_t square_[] = {254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t triangle[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,254,252,250,248,246,244,242,240,238,236,234,232,230,228,226,224,222,220,218,216,214,212,210,208,206,204,202,200,198,196,194,192,190,188,186,184,182,180,178,176,174,172,170,168,166,164,162,160,158,156,154,152,150,148,146,144,142,140,138,136,134,132,130,128,126,124,122,120,118,116,114,112,110,108,106,104,102,100,98,96,94,92,90,88,86,84,82,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,0};
const uint8_t prutt[] = {126,129,128,127,125,126,129,128,129,130,128,127,127,126,126,124,127,127,125,125,126,126,127,126,127,126,128,128,127,126,127,126,127,126,125,126,126,126,125,125,125,125,127,127,127,126,127,128,128,127,128,126,125,126,127,129,129,129,125,122,126,126,129,128,127,127,127,129,128,125,126,127,127,126,124,126,125,128,128,128,127,127,128,126,127,125,127,126,128,128,127,128,127,128,128,128,128,129,128,128,130,128,127,129,129,128,128,130,131,129,129,129,130,132,133,130,129,128,128,129,130,130,130,130,129,130,128,129,129,130,130,130,129,129,129,130,129,128,127,130,129,127,128,130,129,127,129,130,129,126,126,128,129,130,128,128,129,130,129,126,123,127,128,129,128,130,127,127,128,128,128,129,128,126,126,126,127,129,128,126,127,126,126,125,125,125,127,129,125,126,127,126,125,128,127,125,123,126,127,127,126,126,123,125,125,125,126,128,127,126,127,129,127,125,126,128,129,128,125,125,128,126,127,128,126,126,129,129,127,126,128,95,82,109,104,109,114,117,130,139,141,152,153,142,149,151,159,160,165,159,152,156,142,154,164,191,218,205,140,112,195,55,52,45,45,78,95,106,105,92,88,105,112,124,124,130,139,145,152,151,153,155,151,153,155,163,153,153,152,144,139,136,132,129,127,122,124,124,126,124,118,117,112,109,109,114,115,120,121,126,121,121,125,125,120,121,126,130,134,135,136,136,137,133,133,138,137,140,138,139,140,137,137,132,133,129,124,121,125,132,130,124,123,121,118,119,110,110,117,118,119,118,121,126,125,120,120,123,125,127,131,157,65,93,113,107,86,117,148,116,128,133,148,150,150,155,138,149,165,157,133,143,142,139,193,184,208,202,137,154,90,161,111,72,33,59,118,63,67,76,96,113,129,100,77,101,124,127,127,131,135,142,146,144,145,141,143,148,153,155,157,159,152,150,144,142,140,133,130,131,131,129,131,123,122,115,114,111,109,111,115,119,118,120,118,114,115,115,118,123,125,129,137,139,137,137,144,140,136,138,144,144,143,145,145,143,145,141,138,134,129,128,128,126,129,124,118,118,116,115,113,108,110,116,119,145,118,49,79,124,122,107,126,141,128,86,115,149,145,157,169,166,166,160,148,185,191,163,170,118,136,156,157,129,159,158,192,80,18,165,48,73,62,62,95,96,105,106,104,98,103,113,133,132,131,142,148,151,149,144,149,148,142,142,147,148,137,136,127,125,121,123,127,125,121,124,125,125,124,118,119,114,109,115,122,120,123,126,126,122,123,126,127,125,126,125,127,129,130,131,129,133,129,132,131,134,139,138,140,136,138,133,132,134,132,128,126,131,135,135,129,126,126,122,123,115,121,127,125,125,125,124,126,122,123,127,126,127,69,70,99,106,109,130,119,129,97,141,191,184,148,144,164,152,187,172,135,150,148,150,127,145,128,187,170,120,134,143,100,235,150,39,86,85,102,85,88,100,103,111,106,90,113,133,121,131,137,136,145,146,142,141,139,142,149,151,148,146,138,138,135,130,128,130,123,124,127,129,127,120,116,112,113,110,110,112,113,116,115,114,114,113,109,109,116,114,121,121,130,134,132,137,140,136,134,140,144,142,138,141,140,139,136,135,130,133,127,128,126,119,125,123,125,113,118,108,107,117,119,117,120,127,128,123,124,125,128,128,132,129,128,133,136,134,135,132,132,130,130,130,130,128,135,135,135,135,133,132,129,128,68,61,115,88,81,128,139,125,143,154,154,153,150,216,188,185,163,172,157,175,201,172,191,174,124,102,78,93,36,127,22,86,71,96,96,110,109,99,94,124,127,135,142,139,151,150,156,159,160,151,156,151,153,154,152,143,143,135,127,126,125,119,115,112,109,111,112,107,110,112,106,110,114,115,120,124,125,125,129,131,130,134,129,126,130,133,134,132,135,134,132,135,136,135,135,129,133,132,126,126,126,126,124,123,121,123,120,122,115,112,114,115,114,115,114,113,118,122,122,121,125,127,128,127,131,134,135,136,132,138,137,139,139,141,137,138,140,139,138,135,135,132,129,128,129,127,126,122,120,122,123,124,123,119,122,121,123,125,129,128,128,127,131,130,134,136,136,137,135,135,134,132,133,132,130,128,127,129,128,124,124,122,123,125,122,127,129,128,126,151,86,51,95,137,125,131,123,154,155,172,181,177,181,163,199,158,190,114,106,104,96,167,185,137,59,45,156,107,128,7,13,94,95,133,105,91,87,131,119,125,120,133,127,143,142,134,147,153,146,146,138,143,137,141,124,121,129,126,129,124,113,115,123,121,115,117,114,117,118,120,117,119,123,124,123,122,123,125,125,126,125,126,123,124,125,125,131,131,131,127,132,136,136,134,132,136,134,131,131,127,125,128,131,123,123,120,125,122,117,117,115,122,118,118,110,120,122,117,114,124,129,127,124,125,137,134,138,138,136,137,140,142,138,138,136,139,141,139,136,136,132,132,133,133,131,130,126,126,127,128,124,119,125,125,128,126,126,128,130,131,129,130,132,134,130,132,134,136,133,127,126,127,129,134,128,129,128,127,125,126,129,124,126,129,130,133,130,132,130,131,132,133,134,130,128,132,127,125,124,125,123,122,125,123,121,119,121,123,123,123,120,120,122,122,123,122,125,126,132,129,123,128,132,132,133,131,131,131,131,130,128,130,127,129,128,126,122,121,121,120,116,118,115,118,116,121,121,121,120,124,130,127,130,126,130,132,131,129,129,132,132,131,127,125,127,128,128,128,126,105,110,100,88,100,116,119,112,108,127,162,151,145,135,171,202,198,236,217,172,161,152,139,138,135,114,88,107,104,58,118,116,105,94,91,117,103,108,141,110,121,136,131,150,145,148,152,145,146,157,152,154,156,148,145,145,139,138,133,128,130,129,120,122,119,116,118,118,119,116,116,116,121,124,124,125,126,128,127,128,127,129,132,131,131,132,137,130,131,134,135,129,130,134,132,134,132,134,136,134,134,130,129,128,128,126,137,125,122,120,119,113,116,117,117,121,120,122,119,124,121,122,121,126,126,124,127,126,128,127,130,131,133,129,131,129,131,132,132,132,131,132,130,128,127,128,130,126,127,127,122,125,122,124,122,125,123,124,123,123,125,125,130,130,129,128,129,132,128,125,127,127,124,123,124,124,123,123,124,123,124,126,126,128,129,129,132,132,133,132,134,137,140,141,139,138,137,135,133,131,131,130,130,126,118,106,95,115,106,84,88,116,150,139,145,148,158,162,164,194,165,152,151,147,144,133,140,119,167,159,164,163,123,71,118,136,128,170,101,106,91,85,121,106,92,93,105,110,139,135,149,148,151,155,151,148,148,151,156,150,152,144,145,136,132,131,123,124,122,116,116,123,118,119,119,115,113,114,114,117,117,119,119,125,125,124,121,118,122,123,126,126,130,128,133,138,133,136,131,132,134,135,139,137,135,130,128,127,127,122,123,123,124,129,112,121,120,118,119,120,119,114,131,120,116,125,131,126,130,129,132,126,124,134,131,137,134,135,134,131,134,134,134,133,135,134,132,132,132,127,129,129,125,125,123,126,124,124,124,123,124,124,124,124,124,128,130,132,136,138,135,130,132,131,136,134,134,135,132,132,129,129,126,124,118,77,130,130,80,72,105,133,124,129,123,120,138,159,164,158,192,200,197,189,182,170,158,133,149,137,156,129,107,139,94,83,87,113,101,128,15,120,104,93,107,89,103,149,122,137,112,146,154,138,150,155,144,147,155,150,150,147,148,155,145,129,128,124,122,120,118,115,119,118,118,116,115,112,111,112,108,114,114,116,119,119,125,120,121,124,123,126,122,128,132,135,132,133,130,131,136,138,138,134,141,141,137,135,134,133,130,132,127,127,125,126,122,120,118,118,117,121,112,112,108,120,120,115,120,130,130,125,126,127,126,133,133,134,133,136,136,135,136,137,133,134,133,133,136,133,130,135,130,129,127,127,124,125,122,123,122,124,124,126,125,125,126,129,132,131,131,132,131,133,134,133,131,131,129,125,127,126,125,128,128,126,122,124,124,124,125,129,132,130,135,131,132,138,133,119,68,123,124,95,119,161,118,116,133,128,139,127,173,168,152,169,171,149,142,200,179,124,103,121,127,195,183,99,89,76,68,131,128,126,136,114,103,94,133,121,107,108,113,117,150,148,146,143,154,149,144,139,143,144,142,144,144,131,135,133,123,125,117,116,117,118,117,119,113,114,118,114,110,110,115,117,121,126,126,121,125,131,125,123,124,127,128,131,136,139,137,137,137,131,132,138,135,131,134,130,133,131,132,131,132,126,125,127,128,128,130,121,125,128,121,117,120,119,131,123,124,128,133,128,131,127,132,133,129,134,133,135,137,134,137,136,138,135,137,135,134,134,135,136,135,132,131,128,128,127,124,124,123,123,123,124,124,123,121,123,125,128,126,124,129,130,133,133,129,130,132,132,127,128,129,133,127,127,128,127,126,124,124,125,125,128,127,128,128,126,126,128,132,135,130,130,130,129,130,132,130,127,133,128,125,124,124,123,121,103,92,104,105,97,114,114,108,125,130,140,147,134,152,166,177,182,155,157,175,167,147,134,120,135,131,150,158,123,95,95,99,170,173,49,92,87,96,116,86,77,111,112,142,123,122,160,145,145,152,153,144,150,152,151,148,145,141,145,135,128,128,124,125,119,118,120,120,119,124,119,119,118,119,121,124,125,126,129,125,129,132,127,126,128,125,127,131,132,133,131,137,134,135,133,134,134,137,137,140,138,135,134,134,128,128,128,130,127,129,122,126,127,128,120,124,122,120,118,119,126,127,124,123,126,126,126,130,128,133,135,136,136,133,133,133,133,135,131,131,133,135,136,134,132,131,129,127,125,127,124,125,126,127,125,125,124,122,125,126,127,131,127,130,130,131,132,132,130,128,128,128,127,126,125,123,123,123,124,120,120,116,117,117,117,116,120,120,128,129,124,126,131,134,132,137,136,134,135,136,135,135,132,130,131,130,132,125,126,124,124,123,120,121,121,120,121,120,122,123,123,125,122,103,119,128,115,104,122,124,115,118,97,112,141,157,152,168,185,188,163,171,169,142,116,154,140,124,138,166,143,115,137,99,94,89,61,75,113,125,102,94,126,116,112,114,122,125,140,137,142,155,155,153,155,150,150,154,153,152,153,146,144,143,135,131,125,123,122,123,123,121,115,115,115,113,113,113,112,115,122,116,113,122,124,123,120,127,126,127,133,132,134,133,135,135,133,135,139,136,135,136,139,137,138,133,132,132,128,128,127,126,124,125,127,118,117,119,116,117,118,117,128,124,119,122,129,129,125,123,127,128,129,132,131,131,134,135,134,132,134,131,133,131,132,134,134,131,130,130,126,124,127,125,125,125,124,123,122,121,124,123,121,125,124,122,125,127,127,128,128,125,126,131,130,124,126,130,129,131,131,131,129,129,129,128,127,130,130,130,129,130,129,130,130,132,129,128,130,131,131,129,130,129,128,128,130,129,130,131,130,130,130,129,128,128,129,128,127,127,128,127,126,127,127,127,127,128,127,126,127,127,127,126,128,127,126,128,129,130,128,127,127,126,125,125,125,125,127,127,126,124,125,124,123,123,124,123,122,124,123,122,123,123,122,122,125,124,125,126,127,128,127,129,130,130,130,130,129,129,131,132,131,130,131,132,133,131,131,131,129,130,131,131,131,131,131,131,128,127,127,127,130,129,130,129,129,130,129,130,131,128,128,131,130,131,132,131,128,128,128,128,128,128,128,127,127,126,125,126,126,126,127,126,126,125,124,125,124,125,125,125,127,125,126,126,129,129,128,128,131,129,130,130,129,130,131,133,131,131,130,129,130,131,131,132,131,131,129,130,128,130,131,128,131,131,128,128,129,130,129,128,129,128,129,130,129,128,128,128,129,129,130,130,128,128,130,130,127,128,127,127,125,125,126,129,125,127,127,127,128,130,127,126,126,125,125,126,128,125,126,125,126,127,126,128,128,127,126,126,127,126,127,128,126,126,126,125,127,128,128,126,126,127,125,124,125,125,125,124,127,127,125,124,126,129,128,128,127,127,125,127,130,129,128,126,127,126,129,128,130,129,130,129,127,128,128,128,128,129,129,128,127,129,129,131,129,129,130,131,130,129,130,129,130,128,129,131,132,130,131,131,130,131,128,130,130,129,130,129,128,129,130,128,129,129,128,128,128,130,129,127,128,128,127,127,129,128,128,128,130,129,127,127,127,128,129,130,129,126,130,130,131,129,128,127,129,131,130,129,128,126,127,128,127,129,126,126,127,128,126,125,127,126,127,127,127,125,126,126,127,127,126,127,127,128,129,127,128,129,127,127,127,128,128,128,126,127,128,127,127,127,126,126,127,127,127,125,127,126,126,127,128,127,127,127,128,129,129,129,129,128,129,130,126,127,130,128,128,129,132,129,129,130,127,129,127,128,126,127,127,126,125,126,127,128,129,129,130,128,129,130,129,126,127,128,128,128,127,126,126,127,129,130,128,128,128,128,127,128,127,127,128,130,129,128,129,126,127,129,131,128,128,130,129,126,127,129,129,128,129,129,130,129,129,129,128,129,132,130,128,129,128,128,128,129,128,129,129,131,130,129,128,128,127,128,129,130,130,130,131,129,129,128,129,128,130,130,129,128,129,129,127,128,129,128,127,127,128,128,127,130,128,126,126,128,126,127,128,129,126,127,129,127,129,130,130,128,128,129,128,128,126,127,127,125,129,127,127,126,129,129,128,129,127,126,125,128,128,129,128,129,129,128,127,126,127,128,129,127,127,127,127,125,126,127,128,128,128,128,128,128,128,129,128,130,130,128,126,127,128,128,129,130,130,128,129,130,129,127,128,129,127,128,129,128,127,127,126,127,128,130,129,128,128,129,127,125,127,129,128,128,128,127,127,127,128,128,127,129,128,128,128,127,127,128,127,129,129,129,129,128,129,128,128,128,127,128,128,128,129,131,127,126,128,130,130,128,131,127,127,127,129,128,127,128,129,127,127,129,128,129,128,129,129,128,129,129,128,126,127,126,127,128,128,127,127,127,127,129,127,127,127,126,128,127,127,126,127,128,127,129,128,129,127,129,128,128,128,128,127,126,127,127,127,127,128,126,126,128,128,127,128,128,127,126,128,128,127,127,129,130,128,128,130,130,130,128,129,127,129,129,128,127,128,128,127,129,130,129,128,128,128,128,128,129,128,127,129,130,128,127,127,130,129,129,128,129,127,128,129,128,127,127,129,126,128,128,128,128,130,128,128,127,127,127,127,127,127,126,126,126,127,128,127,129,129,127,127,129,127,127,127,126,127,129,129,127,126,127,128,129,128,129,128,127,127,128,127,128,129,128,127,127,128,128,128,129,129,129,129,129,128,126,128,128,127,127,127,128,129,129,129,128,126,126,126,126,126,128,129,127,128,128,127,125,128,129,130,129,130,128,128,128,128,127,129,128,127,128,129,129,129,130,130,128,126,128,127,127,128,129,129,127,127,128,127,128,129,128,128,126,127,127,127,128};
	
	
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


uint8_t * data[2];
uint8_t * adc_value;
uint8_t count = 0;
uint8_t * buffer[2];
uint8_t inc = 1;
uint8_t buttons[12]; //pressed buttons, 1 is C, 2 is C# etc, 13-16 = 0
uint8_t output = 0;
uint8_t * osc1;


void LCD_Write(uint8_t data_or_command, uint8_t byte)
{
	uint8_t i;
	
	//set CE low (chip enable, inverted input)
	LCD_PORT &= ~(1<<CE);
	
	//tell the display it's a command or data
	//if(data_or_command == LCD_COMMAND)
	if(data_or_command==LCD_DATA)
	LCD_PORT |= (1<<DC);
	else
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
	
	//reset DC
	//if(data_or_command!=LCD_DATA)
	//	LCD_PORT |= (1<<DC);
	//else
	//	LCD_PORT &= ~(1<<DC);
	
	
	//set CE high
	LCD_PORT |= (1<<CE);
}

void LCD_Write_data(uint8_t byte)
{
	uint8_t i;
	
	//set CE low (chip enable, inverted input)
	LCD_PORT &= ~(1<<CE);
	
	LCD_PORT |= (1<<DC);	
	
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
		//_delay_us(10);
		asm("nop");
		LCD_PORT |= (1<<CLK);
	}
	
	//set CE high
	LCD_PORT |= (1<<CE);
}


void LCD_write_char(uint8_t c)
{
	//LCD_Write(LCD_DATA, 0x00);
	uint8_t line;

	for (line=0; line<6; line++)
	{
		LCD_Write(LCD_DATA, font6x8[c-0x20][line]);
	}
	
	//LCD_Write(LCD_DATA, 0x00);
}

void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_Write(0, 0x40 | Y);		// column
	LCD_Write(0, 0x80 | X);          	// row
}

void LCD_write_string(unsigned char X,unsigned char Y,char *s)
{
	//LCD_set_XY(X,Y);
	while (*s)
	{
		LCD_write_char(*s++);
		//s++;
	}
}


void LCD_clear(void)          // clear the LCD
{
	uint16_t i;

	//LCD_Write(0, 0x0C);
	//LCD_Write(0, 0x80);		//set address of RAM, 0x80 is 0,0

	for (i=0; i<504; i++)	//504 = 84*48/8
	{
		LCD_Write(LCD_DATA, 0x00);
	}
}

void Disable_LCD()
{
	//LCD_PORT |= (1<<RST);
	LCD_PORT |= (1<<CE);
}

void Init_LCD()
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
	
	LCD_Write(LCD_COMMAND, 0x21);	//Tell LCD that extended commands follow
	LCD_Write(LCD_COMMAND, 0xC1);	//Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
	LCD_Write(LCD_COMMAND, 0x06);	//Set temp coeff
	LCD_Write(LCD_COMMAND, 0x13);	//LCD bias mode 1:48: Try 0x13 or 0x14
	
	LCD_Write(LCD_COMMAND, 0x20);	//We must send 0x20 before modifying the display control mode
	LCD_clear();
	LCD_Write(LCD_COMMAND, 0x0C);	//Set display control, normal mode. 0x0D for inverse, 0x0C for non-inverse
	//LCD_Write(LCD_COMMAND, 0x09);	//all segments on
	
	LCD_PORT &= ~(1<<CE);
}

void send_bit(uint8_t bit)
{
	
	//if((bit>>7) == 0)
	if((bit & 0x01) == 0)
	//if(bit == 0)
	  LCD_PORT &= ~(1<<DIN); //set DIN to 0
	else
	  LCD_PORT |= (1<<DIN); //set DIN to 1
	  
	
	_delay_us(20);
	LCD_PORT &= ~(1<<CLK); //set CLK low  
	_delay_us(20);
	LCD_PORT |= (1<<CLK); //set CLK high
}


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

ISR(TIMER1_COMPA_vect)
{
	//count += 1;
	populate_buttons();
	//asm("nop");
	//LCD_write_string(0,20,"<3<3<3");
	/*
	LCD_Write(LCD_DATA, 0xaa);
	LCD_Write(LCD_DATA, 0xab);
	LCD_Write(LCD_DATA, 0x10);
	LCD_Write(LCD_DATA, 0x14);
	LCD_Write(LCD_DATA, 0x03);
	*/
}


//ISR(TIMER0_OVF_vect)
ISR(TIMER0_COMPA_vect)
{
	
	count += 4;
	//if(count > 255)
	//  count = 0;
	
	//PORTC = osc1[count];
	
	
	if(buttons[0])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[0];
	}
	
	else if(buttons[1])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[1];
	}
	
	else if(buttons[2])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[2];
	}
	
	else if(buttons[3])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[3];
	}
	
	else if(buttons[4])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[4];
	}
	
	else if(buttons[5])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[5];
	}
	
	
	else if(buttons[6])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[6];
	}
	
	
	else if(buttons[7])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[7];
	}
	
	
	else if(buttons[8])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[8];
	}
	
	else if(buttons[9])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[9];
	}
	
	
	else if(buttons[10])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[10];
	}
	
	
	else if(buttons[11])
	{
		PORTC = osc1[count];
		OCR0A = note_vals[11];
	}
	else
		PORTC = 0;
	
	//TCNT0=0;
}

void setup_timer1()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TCNT1 = 0;
	
	TCCR1A |= (1<<COM1A1) | (1<<COM1A0) | (1<<WGM11);
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10);
	OCR1A = 0xC000;	//set
	//OCR1A = 0xA000;
	//OCR1A = 100;
	ICR1 = 0xF000;	//clear
	TIMSK1 =(1<<OCIE1A);
	//TIFR1 &= ~(1<<OCF1A);

}

void setup_timer0()
{
	
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
	//OCR0A = 5;
	
	TCCR0A |= (1<<WGM01) | (1<<WGM00);
	// Prescaler = FCPU
	TCCR0B |= (1<<CS00) | (1<<WGM02);
	//prescaler FCPU/1024
	//TCCR0B |= (1<<CS00) | (1<<CS02) | (1<<WGM02);
	//prescaler FCPU/8
	//TCCR0B |= (1<<CS01) | (1<<WGM02);
	
	//TCCR0B |= (1<<FOC0B);
	
	
	//Enable Overflow Interrupt Enable
	//TIMSK0 |=(1<<TOIE0);
	TIMSK0 = (1<<OCIE0A);
	//TIMSK0 |=(1<<OCIE0B);
	
}

int main(void)
{
	sei();
	l74hc165_init();
	osc1 = sine;
	//osc1 = square_;
	//osc1 = triangle;
	//osc1 = prutt;
	//int n;
	
	DDRC = 0xff;
	
	Init_LCD();
	LCD_clear();
	LCD_write_string(0,0,"Erik <3 Klara!");
	LCD_write_string(0,0,"Erik <3 Klara!");
	
	LCD_write_string(0,0,"Erik <3 Klara!");
	
	LCD_write_string(0,0,"Erik <3 Klara!");
	LCD_write_string(0,0,"Erik <3 Klara!");
	LCD_write_string(0,0,"Erik <3 Klara!");
	LCD_write_string(0,0,"Erik <3 Klara!");
	
	
	setup_timer1();
	setup_timer0();
	
    while(1)
    {
		//populate_buttons();
		//Init_LCD();
		
		//LCD_clear();
		//_delay_ms(1000);
		//LCD_Write(LCD_DATA, 0xAA);
		//Init_LCD();
		//LCD_clear();
		//LCD_write_string(20,20,"<3");
		
		//_delay_ms(4000);
		//LCD_PORT |= (1<<DC);
		//_delay_ms(4000);
		//LCD_PORT &= ~(1<<DC);
		
		//_delay_ms(1000);
		//LCD_write_string(0,0,"Erik <3 Klara");
		//asm("nop");
		//LCD_write_string(10,10,"HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEJ!!!");
		//LCD_write_char('E');
		
		l74hc165_shiftin(&data);
		//LCD_clear();
		//_delay_ms(1000);
		
		//LCD_Write_data(0xAA);
		//_delay_ms(1000);
		//LCD_Write_data(0x00);
		//asm("nop");
		//LCD_clear();
		//_delay_ms(1000);
    }
}

