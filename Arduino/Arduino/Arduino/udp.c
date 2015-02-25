//
//  udp.c
//  Arduino
//
//  Created by Projectgroep TI2.4 on 05-01-15.
//  Copyright (c) 2015 Projectgroep TI2.4. All rights reserved.
//

#include <stdio.h>
#include <util/delay.h>

#include "spi.h"
#include "eth.h"
#include "udp.h"
#include "uart.h"

// Methode om een UDP socket mee te creeren
void udp_create_socket(void) {
    spi_transmit(S0MR, (1 << P1));              // Maakt van socket 0 een UDP socket
    
    uint16_t port = 30000;                      // Luistert naar data op poort 3000
    spi_transmit(S0PORTH, port >> 8);           // Zet hoogste byte
    spi_transmit(S0PORTL, port);                // Zet laagste byte
    
    spi_transmit(S0CR, OPEN);                   // Initialiseert de socket met de instellingen
    uint8_t status = spi_receive(S0SR);         // Haalt de status op
    
    if(status != SOCK_UDP) {                    // Check of de status (SOCK_UDP) klopt met de instellingen
        spi_transmit(S0CR, CLOSE);              // Anders sluit de verbinding
        udp_create_socket();                    // Probeer opnieuw
    }
}

// Methode om de UDP socket mee te sluiten
void udp_close_socket(void) {
    spi_transmit(S0CR, CLOSE);                  // Sluit de verbinding
}

// Methode om mee te broadcasten
void udp_broadcast(udpflag flag, int length) {
    
    // Vraag de vrije ruimte op voor het versturen van pakket
    // Als deze niet groot genoeg wacht op beschikbare ruimte
    uint16_t size = 0;
    do {
        size = spi_receive(S0TXFSRH);
        size <<= 8;
        size |= spi_receive(S0TXFSRL);
        _delay_ms(1);
    }
    while(length > size);
    
    // Zet het ipadres op broadcast
    spi_transmit(S0DIPR + 0, 255);
    spi_transmit(S0DIPR + 1, 255);
    spi_transmit(S0DIPR + 2, 255);
    spi_transmit(S0DIPR + 3, 255);
    
    // Zet de destination poort
    spi_transmit(S0DPORTH, 30000 >> 8);
    spi_transmit(S0DPORTL, 30000 & 0xFF);
    
    // Vraagt de pointer naar het socket geheugen
    uint16_t pointer = spi_receive(S0TXWRH) << 8;
    pointer |= spi_receive(S0TXWRL);
    
    // Berekent het adres met de pointer en mask
    uint16_t address = S0TXBASE + (pointer & 0x07FF);
    
    // Schrijft de flag weg in het socketgeheugen
    spi_transmit(address, flag);
    ++pointer;
    
    // Schrijft het ipadres weg in het socketgeheugen
    int ip_addr[]  = {10, 0, 4, 1};
    for (int i = 0; i < 4; ++i) {
        address = S0TXBASE + (pointer++ & 0x07FF);
        spi_transmit(address, ip_addr[i]);
    }
    
    // Berg de pointer weer op
    spi_transmit(S0TXWRH, pointer >> 8);
    spi_transmit(S0TXWRL, pointer);
    
    // Verstuurt het pakket
    spi_transmit(S0CR, SEND);
    
    // Wacht op het versturen (0x00 = verstuurd)
    while(spi_receive(S0CR));
    
    // Sluit de verbinding
    // SPI_Transmit(S0CR, CLOSE);
}

// Methode om data te sturen naar de monitor
void udp_to_monitor(udpflag flag, uint8_t data[], int length) {
    
    // Vraag de vrije ruimte op voor het versturen van pakket
    // Als deze niet groot genoeg wacht op beschikbare ruimte
    uint16_t size = 0;
    do {
        size = spi_receive(S0TXFSRH);
        size <<= 8;
        size |= spi_receive(S0TXFSRL);
        _delay_ms(1);
    }
    while(length > size);
    
    // Zet het ipadres voor de bestemming
#if school
    spi_transmit(S0DIPR + 0, 10);
    spi_transmit(S0DIPR + 1, 0);
    spi_transmit(S0DIPR + 2, 4);
    spi_transmit(S0DIPR + 3, 0);

#else
    spi_transmit(S0DIPR + 0, 192);
    spi_transmit(S0DIPR + 1, 168);
    spi_transmit(S0DIPR + 2, 178);
    spi_transmit(S0DIPR + 3, 12);
#endif
    
    // Zet de destination poort
    spi_transmit(S0DPORTH, 30000 >> 8);
    spi_transmit(S0DPORTL, 30000 & 0xFF);
    
    // Vraagt de pointer naar het socket geheugen
    uint16_t pointer = spi_receive(S0TXWRH) << 8;
    pointer |= spi_receive(S0TXWRL);
    
    // Berekent het adres met de pointer en mask
    uint16_t address = S0TXBASE + (pointer & 0x07FF);
    
    // Schrijft de flag weg in het socketgeheugen
    spi_transmit(address, flag);
    ++pointer;
    
    // Schrijft het ipadres weg in het socketgeheugen
    char ip_addr[]  = {'1','0','4','1'};
    for (int i = 0; i < 4; ++i) {
        address = S0TXBASE + (pointer++ & 0x07FF);
        spi_transmit(address, ip_addr[i]);
    }
    
    // Schrijft de buffer weg in het socketgeheugen
    for (int i = 0; i < length; ++i) {
        address = S0TXBASE + ((pointer++) & 0x07FF);
        spi_transmit(address, data[i]);
    }
    
    // Berg de pointer weer op
    spi_transmit(S0TXWRH, pointer >> 8);
    spi_transmit(S0TXWRL, pointer);
    
    // Verstuurt het pakket
    spi_transmit(S0CR, SEND);
    
    // Wacht op het versturen (0x00 = verstuurd)
    while(spi_receive(S0CR));
}

// Methode om data mee te ontvangen
void udp_from_monitor(uint8_t buffer[]) {
    // Vraag de grootte van de binnenkomende data op
    uint16_t size = spi_receive(S0RXRSRH) << 8;
    size |= spi_receive(S0RXRSRL);
    
    // Haalt de header van de groote af
    size -= 8;
    
    // Vraagt de pointer naar het socket geheugen
    uint16_t pointer = spi_receive(S0RXRRH) << 8;
    pointer |= spi_receive(S0RXRRL);
    
    // Voegt 8 toe voor de header
    pointer += 8;
    
    // Berekent het adres met de pointer en mask
    // Haalt de data op uit het geheugen minus de eerste 8 header bytes
    uint16_t address;
    for (int i = 0; i < (size ); ++i) {
        address = S0RXBASE + ((pointer++) & 0x07FF);
        buffer[i] = spi_receive(address);
    }
    
    // Harde nul voor string
    buffer[size] = '\0';

    // Berg de pointer weer op
    spi_transmit(S0RXRRH, pointer >> 8);
    spi_transmit(S0RXRRL, pointer);
    
    // Registreer dat het pakket is opgehaald
    spi_transmit(S0CR, RECV);
    
    // Stuur een acknowledge
    // udp_to_monitor(udpack, buffer, size);
}