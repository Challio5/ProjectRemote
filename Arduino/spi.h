//
//  spi.h
//  Arduino
//
//  Created by Projectgroep TI2.4 on 10-12-14.
//  Copyright (c) 2014 Projectgroep TI2.4. All rights reserved.
//

#include <avr/io.h>

#ifndef Arduino_spi_h
#define Arduino_spi_h

#define DDR_SPI DDRB                    // Stuurt het ethernetshield aan met de B pinnen
#define DD_SCK  DDB5                    // SCK Master clock input 13
#define DD_MISO DDB4                    // MISO Master input slave output pin 12
#define DD_MOSI DDB3                    // MOSI Master output slave input pin 11
#define DD_SS   DDB2                    // SS slave select

#define SPI_PORT PORTB                  // IO register voor het aansturen van de SPI
#define SPI_CS   PORTB2                 // SS pin voor het selecteren van de slave

void spi_init(void);

void spi_transmit(uint16_t address, uint8_t data);
uint8_t spi_receive(uint16_t address);



#endif
