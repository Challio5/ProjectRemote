//
//  tcp.c
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

// Counter voor het bijhouden van het messagenummer
static int message_number = 0;

// Methode om een TCP socket mee te creeren
void tcp_create_socket(void) {
    spi_transmit(S1MR, (1 << P0));              // Maakt van socket 1 een TCP socket
    
    uint16_t port = 30000;                      // Luistert naar data op poort 3000
    spi_transmit(S1PORTH, port >> 8);           // Zet hoogste byte
    spi_transmit(S1PORTL, port);                // Zet laagste byte
    
    spi_transmit(S1CR, OPEN);                   // Initialiseert de socket met de instellingen
    uint8_t status = spi_receive(S1SR);         // Haalt de status op
    
    if(status != SOCK_INIT) {                   // Check of de status (SOCK_INIT) klopt met de instellingen
        spi_transmit(S1CR, CLOSE);              // Anders sluit de verbinding
        tcp_create_socket();                    // Probeer opnieuw
    }
}

// Methode om een disconnect request te versturen
void tcp_disconnect(void) {
    spi_transmit(S1CR, DISCON);
}

// Methode om de TCP socket mee te sluiten
void tcp_close_socket(void) {
    spi_transmit(S1CR, CLOSE);                  // Sluit de verbinding
}

// Methode naar een TCP request te luisteren
void tcp_listen(void) {
    // Reset het message nummer voor een volgende verbinding
    message_number = 0;
    
    // Laat de socket naar luister naar requests
    spi_transmit(S1CR, LISTEN);
        
    // Haal de status op
    uint8_t status = spi_receive(S1SR);
        
    // Check of de socket goed is ingesteld, verbind anders opnieuw
    if (status != SOCK_LISTEN) {
        spi_transmit(S1CR, CLOSE);
        tcp_listen();
    }
}

// Methode om data te sturen naar de monitor
void tcp_to_monitor(tcpflag flag, uint8_t data[], int length) {
    
    // Verhoogt het messagenummer met 1
    ++message_number;
    
    // Vraag de vrije ruimte op voor het versturen van pakket
    // Als deze niet groot genoeg wacht op beschikbare ruimte
    uint16_t size = 0;
    do {
        size = spi_receive(S1TXFSRH);
        size <<= 8;
        size |= spi_receive(S1TXFSRL);
        _delay_ms(1);
    }
    while(length > size);
    
    // Zet het ipadres in het socket geheugen
    spi_transmit(S1DIPR + 0, 10);
    spi_transmit(S1DIPR + 1, 0);
    spi_transmit(S1DIPR + 2, 4);
    spi_transmit(S1DIPR + 3, 0);
    
    // Zet de destination poort
    spi_transmit(S1DPORTH, 30000 >> 8);
    spi_transmit(S1DPORTL, 30000 & 0xFF);
    
    // Vraagt de pointer naar het socket geheugen
    uint16_t pointer = spi_receive(S1TXWRH) << 8;
    pointer |= spi_receive(S1TXWRL);
    
    // Bereken het start adres
    uint16_t address = S1TXBASE + (pointer & 0x07FF);
    
    // Schrijft de flag weg in het socketgeheugen
    spi_transmit(address, flag);
    ++pointer;
    
    // Schrijf de high byte van het message nummer weg
    address = S1TXBASE + (pointer & 0x07FF);
    spi_transmit(address, message_number >> 8);
    ++pointer;
    
    // Schrijf de low byte van het message nummer weg
    address = S1TXBASE + (pointer & 0x07FF);
    spi_transmit(address, message_number);
    ++pointer;
    
    // Schrijft de data weg in het geheugen
    for (int i = 0; i < length; ++i) {
        address = S1TXBASE + ((pointer++) & 0x07FF);
        spi_transmit(address, data[i]);
    }
    
    // Berg de pointer weer op
    spi_transmit(S1TXWRH, pointer >> 8);
    spi_transmit(S1TXWRL, pointer);
    
    // Verstuurt het pakket
    spi_transmit(S1CR, SEND);
    
    // Wacht op het versturen (0x00 = verstuurd)
    while(spi_receive(S1CR));
}

// Methode om data mee te ontvangen
void tcp_from_monitor(uint8_t buffer[]) {
    // Vraag de grootte van de binnenkomende data op
    uint16_t size = spi_receive(S1RXRSRH) << 8;
    size |= spi_receive(S1RXRSRL);
    
    // Vraagt de pointer naar het socket geheugen
    uint16_t pointer = spi_receive(S1RXRRH) << 8;
    pointer |= spi_receive(S1RXRRL);
    
    // Bereken de offset
    uint16_t offset = pointer & 0x07FF;
    
    // Overflow van de databuffer
    if (offset + size > 2000) {
        // Doe niets
    } else {
        // Berekent het adres met de pointer en mask
        // Haalt de data op uit het geheugen minus de eerste 8 header bytes
        uint16_t address;
        for (int i = 0; i < size; ++i) {
            address = S1RXBASE + ((pointer++) & 0x07FF);
            buffer[i] = spi_receive(address);
        }
        
        // Harde nul voor string
        buffer[size] = '\0';
        
        // Berg de pointer weer op
        spi_transmit(S1RXRRH, pointer >> 8);
        spi_transmit(S1RXRRL, pointer);
        
        // Registreer dat het pakket is opgehaald
        spi_transmit(S1CR, RECV);
    }
}