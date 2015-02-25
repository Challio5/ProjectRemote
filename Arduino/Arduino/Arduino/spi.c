//
//  spi.c
//  Arduino
//
//  Created by Projectgroep TI2.4 on 10-12-14.
//  Copyright (c) 2014 Projectgroep TI2.4. All rights reserved.
//

#include <stdio.h>
#include "spi.h"

// Methode voor het opzetten van de SPI verbinding
void spi_init(void) {
    // Zet de mosi, sck en ss lijnen op output voor het aansturen van de SPI verbinding
    DDR_SPI = ((1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS));
    
    // Schrijft naar SPI control register
    // SPE bit voor SPI enable
    // MSTR bit voor het setten als master
    SPCR = ((1 << SPE) | (1 << MSTR));
}

// Methode om met behulp van SPI naar het ethernet shield te transmitten
void spi_transmit(uint16_t address, uint8_t data) {
    SPI_PORT &= ~(1 << SPI_CS);         // Activeert de slave en synchroniseerd
    SPDR = 0xF0;                        // Verstuurt write commando voor transmitten
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPDR = address >> 8;                // Neemt de hoogste byte van het adres en stuurt deze
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPDR = address;                     // Neemt de laagste byte van het adres en stuurt deze
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPDR = data;                        // Verstuurt de data naar het adres
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPI_PORT |= (1<<SPI_CS);            // Deactiveerd de slave
}

// Methode om met behulp van SPI data van het ethernet shield te ontvangen
unsigned char spi_receive(uint16_t address) {
    SPI_PORT &= ~(1<<SPI_CS);           // Activeert de slave en synchroniseerd
    SPDR = 0x0F;                        // Verstuurt read commando voor receiven
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPDR = address >> 8;                // Neemt de hoogste byte van het adres en stuurt deze
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPDR = address;                     // Neemt de laagste byte van het adres en stuurt deze
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPDR = 0x00;                        // Dummy data
    while(!(SPSR & (1 << SPIF)));       // Checkt op SPI interrupt flag zolang data wordt verstuurd
    SPI_PORT |= (1<<SPI_CS);            // Deactiveerd de slave
    
    return SPDR;                        // Returnt de verstuurde data uit het SPI data register
}
