//
//  lcd.c
//  Arduino
//
//  Created by Projectgroep TI2.4 on 07-01-15.
//  Copyright (c) 2015 Projectgroep TI2.4. All rights reserved.
//

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

// LCD number bytes array
uint8_t lcd_number_characters[] = {
    // Characters 1 tot en met 10
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
};

// LCD character bytes array
uint8_t lcd_letter_characters[] = {
    // Letters: a, b, c, d, e, f, g, h, i,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    
    // Letters: j, k, l, m, n, o,
    0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    
    // Letters: p, q, r, s, t, u,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85,
    
    // Letters: v, w, x, y, z
    0x86, 0x87, 0x88, 0x89, 0x8A
};

// Control struct voor het versturen van commando's
control lcd_commandos;

// Methode om de lcd en commando's mee te initialiseren
void lcd_init(void)
{
    // Zet de poorten op output
    DDRD |= ((1 << DD2) | (1 << DD3) | (1 << DD4) | (1 << DD5));
    
    // Zet de commando's in de struct
    lcd_commandos.clear              = 0b00000001;
    lcd_commandos.cursor_home        = 0b00000010;
    lcd_commandos.cursor_forward     = 0b00000110;     // bit[0] = not shifted
    lcd_commandos.cursor_backwards   = 0b00000100;     // bit[0] = not shifted
    lcd_commandos.display_on         = 0b00001110;     // bit[1] = cursor on | bit[0] = blink off
    lcd_commandos.display_off        = 0b00001010;     // bit[1] = cursor on | bit[0] = blink off
    lcd_commandos.lines_1            = 0b00110000;     // bit[4] = 8 bits    | bit[2] = 7 dots, 0/1 = X
    lcd_commandos.lines_2            = 0b00111000;     // bit[4] = 8 bits    | bit[2] = 7 dots, 0/1 = X

    // Stel de lcd in met de juiste functies
    lcd_write(lcd_commandos.lines_2);
    _delay_us(50);
    
    // Zet het lcd aan
    lcd_write(lcd_commandos.display_on);
    _delay_us(50);
    
    // Clear het display
    lcd_write(lcd_commandos.clear);
    _delay_ms(2);
    
    // Entry mode
    lcd_write(lcd_commandos.cursor_forward);
}

// Methode om de juiste waardes op de poorten te zetten
void lcd_write(uint8_t value)
{
    // Zet de lcd op schrijven
    PORTD |= (1 << RS);
    PORTD &= ~(1 << RW);
    
    // Schrijf weg op de d-poort
    PORTD = value;
}

// Methode om een character op het display te zetten
void lcd_send_character(char character, int is_letter)
{
    // Int voor het versturen van het character
    uint8_t data;
    
    // Schrijft het character weg in de low byte
    if (is_letter) {
        // Character A is gelijk aan het getal 97 (ASCII)
        data = lcd_letter_characters[character - 97];
    } else {
        // Character 0 is gelijk aan het getal 48 (ASCII)
        data = lcd_number_characters[character - 48];
    }
    
    // Schrijft de write command weg op de output poorten
    lcd_write(data);
    
    // Schuift de cursor een plaats op
    lcd_write(lcd_commandos.cursor_forward);
}