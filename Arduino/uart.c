//
//  uart.c
//  Arduino
//
//  Created by Projectgroep TI2.4 on 10-12-14.
//  Copyright (c) 2014 Projectgroep TI2.4. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>

#include "uart.h"

// Methode voor het opzetten van de USART verbinding
// Zet de baudrate, rx/tx en het frame formaat
void usart_init(uint16_t baudRate) {
    uint16_t ubrr = ((F_CPU / 16) / baudRate) - 1;         // Berekent de baudrate voor de usart
    UBRR0H = ubrr >> 8;                                    // Zet de hig byte in het usart baudrate register
    UBRR0L = ubrr;                                         // Zet de low byte in het usart baudrate register
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);                  // Zet zowel tx (transmit) als rx (receive) aan
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);                // Frame formaat(8N1), 8 bits, geen par, 1 stopb
}

// Methode om met behulp van USART data te transmitten
// Wordt gebruikt door de stream om character mee te verzenden
int usart_transmit(char data, FILE *stream) {
    // Als er een newline character binnenkomt wordt hier een carriage return aan toegevoegd
    if (data == '\n') usart_transmit('\r', stream);
    
    // Wachten op een lege buffer door het checken op de empty flag
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Zet de data in het data register om te transmitten
    UDR0 = data;
    
    // Geen errors
    return 0;
}

// Methode om met behulp USART data te ontvangen
// Wordt gebruikt door de stream om character mee te ontvangen
char usart_receive(FILE *stream) {
    // Character om data op te slaan
    char data;
    
    // Wachten op binnenkomende data door te checken op de complete flag
    while (!(UCSR0A & (1 << RXC0)));
    
    // Haal de data op uit het register
    data = UDR0;
    
    // Echo input naar de seriele monitor
    usart_transmit(data, stream);
    
    // Return de data
    return data;
}

// Methode om de buffer te flusen
void usart_flush(void) {
    unsigned char dummy;
    while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}


