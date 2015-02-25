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
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    
    // Letters: j, k, l, m, n, o,
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    
    // Letters: p, q, r, s, t, u,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
    
    // Letters: v, w, x, y, z
    0x76, 0x77, 0x78, 0x79, 0x7A
};

// Control struct voor het versturen van commando's
control lcd_commandos;

// Methode om de lcd en commando's mee te initialiseren
void lcd_init(void)
{
    // Wacht op juiste voltage
    _delay_ms(15);
    
    // Zet de poorten op output
    DDRD |= (1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7) | (1 << E) | (1 << RS);
    
    // Zet de commando's in de struct
    lcd_commandos.clear              = 0b0001;
    lcd_commandos.cursor_home        = 0b0010;
    lcd_commandos.cursor_forward     = 0b0110;     // bit[0] = not shifted
    lcd_commandos.cursor_backwards   = 0b0100;     // bit[0] = not shifted
    lcd_commandos.display_on         = 0b1100;     // bit[1] = cursor off | bit[0] = blink off
    lcd_commandos.display_off        = 0b1000;     // bit[1] = cursor off | bit[0] = blink off
    lcd_commandos.bit4               = 0b0010;     // bit[1] = 4 bits mode

    // Stel de lcd in met de juiste functies
    lcd_4bit_mode();
    _delay_us(50);
    
    // Zet het lcd aan
    lcd_write_command(lcd_commandos.display_on);
    _delay_us(50);
    
    // Clear het display
    lcd_write_command(lcd_commandos.clear);
    _delay_ms(2);
}

void lcd_4bit_mode(void)
{
    // Zet RS laag voor het versturen van een commando
    PORTD &= ~(1 << RS);
    
    // Pull de enable pin omhoog
    PORTD |= (1 << E);
    
    // Zet het lcd in 4 bit mode, (low nibble = 0 = niet aangesloten)
    PORTD &= ~((1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7));
    PORTD |= (lcd_commandos.bit4 << 4);
    
    // Pull de enable pin omlaag, bij falling edge wordt de data weggeschreven
    PORTD &= ~(1 << E);
}

// Methode om de lcd in 4 bit mode te zetten
void lcd_write_command(uint8_t value)
{
    // Zet RS laag voor het versturen van een commando
    PORTD &= ~(1 << RS);
    
    // Low nibble
    
    // Pull de enable pin omhoog
    PORTD |= (1 << E);
    
    // Schrijft high nibble weg (standaard 0), clear operatie
    PORTD &= ~((1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7));
    
    // Pull de enable pin omlaag, bij falling edge wordt de data weggeschreven
    PORTD &= ~(1 << E);
    
    // High nibble
    
    // Pull de enable pin omhoog
    PORTD |= (1 << E);
    
    // Schrijf de low nibble weg, clear oude data eerst
    PORTD &= ~((1 << DB4) | (1 << DB5) | (1 << DB6) | (1 << DB7));
    PORTD |= value << 4;
    
    // Pull de enable pin omlaag, bij falling edge wordt de data weggeschreven
    PORTD &= ~(1 << E);
}

// Methode om een string op het display te zetten
void lcd_print(char *string)
{
    // Cleart het lcd scherm voor een nieuwe string
    lcd_write_command(lcd_commandos.clear);
    _delay_ms(2);
    
    // Print de characters uit de string op het lcd
    for (int i = 0; string[i] != '\0'; ++i) {
        _delay_us(100);
        lcd_send_character(string[i]);
    }
}

// Methode om een character op het display te zetten
void lcd_send_character(char character)
{
    // Int voor het versturen van het character
    uint8_t data;
    
    // Character A is gelijk aan het getal 97 (ASCII)
    data = lcd_letter_characters[character - 97];
    
    // Schrijft de write command weg op de output poorten
    lcd_write_character(data);
    
    // Schuift de cursor een plaats op
    lcd_write_command(lcd_commandos.cursor_forward);
}

// Methode om een nummer op het display te zetten
void lcd_send_number(char number)
{
    // Int voor het versturen van het character
    uint8_t data;
    
    // Character 0 is gelijk aan het getal 48 (ASCII)
    data = lcd_number_characters[number - 48];
    
    // Schrijft de write command weg op de output poorten
    lcd_write_character(data);
    
    // Schuift de cursor een plaats op
    lcd_write_command(lcd_commandos.cursor_forward);
}

// Methode om de juiste waardes op de poorten te zetten
void lcd_write_character(uint8_t value)
{
    // Zet RS hoog voor het versturen van characters
    PORTD |= (1 << RS);
    
    // High nibble
    
    // Pull de enable pin omhoog
    PORTD |= (1 << E);
    
    // Schrijf eerst de high nibble weg
    PORTD &= 0x0F;
    PORTD |= value & 0xF0;
    
    // Pull de enable pin omlaag, bij falling edge wordt de data weggeschreven
    PORTD &= ~(1 << E);
    
    // Low nibble
    
    // Pull de enable pin omhoog
    PORTD |= (1 << E);
    
    // Schrijf daarna de low nibble weg
    PORTD &= 0x0F;
    PORTD |= value << 4;
    
    // Pull de enable pin omlaag, bij falling edge wordt de data weggeschreven
    PORTD &= ~(1 << E);
}