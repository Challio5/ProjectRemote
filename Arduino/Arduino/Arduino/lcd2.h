//
//  lcd.h
//  Arduino
//
//  Created by Projectgroep TI2.4 on 07-01-15.
//  Copyright (c) 2015 Projectgroep TI2.4. All rights reserved.
//

#ifndef __Arduino__lcd__
#define __Arduino__lcd__

#include <stdio.h>

typedef struct control {
    uint8_t clear;
    uint8_t cursor_home;
    uint8_t cursor_forward;
    uint8_t cursor_backwards;
    uint8_t display_on;
    uint8_t display_off;
    uint8_t lines_1;
    uint8_t lines_2;
} control;

void lcd_init(void);
void lcd_write(uint8_t);
void lcd_send_command(uint8_t);
void lcd_send_character(char, int);

// Poorten waarop de lcd is aangesloten
#define RS  2
#define RW  1
#define DB7 7
#define DB6 6
#define DB5 5
#define DB4 4
#define DB3 3
#define DB2 2
#define DB1 1
#define DB0 0

#endif /* defined(__Arduino__lcd__) */
