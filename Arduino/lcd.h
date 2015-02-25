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
    uint8_t clear               :4;
    uint8_t cursor_home         :4;
    uint8_t cursor_forward      :4;
    uint8_t cursor_backwards    :4;
    uint8_t display_on          :4;
    uint8_t display_off         :4;
    uint8_t bit4                :4;
} control;

void lcd_init(void);
void lcd_4bit_mode(void);
void lcd_write_command(uint8_t);
void lcd_write_character(uint8_t);
void lcd_send_character(char);
void lcd_send_number(char nummer);
void lcd_print(char *string);

// Poorten waarop de lcd is aangesloten
#define RS  PORTD2
#define E   PORTD3

#define DB4 PORTD4
#define DB5 PORTD5
#define DB6 PORTD6
#define DB7 PORTD7


#endif /* defined(__Arduino__lcd__) */
