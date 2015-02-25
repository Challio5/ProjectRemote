//
//  ethernet.c
//  Arduino
//
//  Created by Projectgroep TI2.4 on 10-12-14.
//  Copyright (c) 2014 Projectgroep TI2.4. All rights reserved.
//

#include <stdio.h>
#include <util/delay.h>

#include "eth.h"
#include "spi.h"
#include "uart.h"

#if 1
uint8_t gtw_addr[] = {10,0,0,1};
uint8_t sub_mask[] = {255,255,0,0};
uint8_t mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
uint8_t ip_addr[]  = {10,0,4,1};

#else
uint8_t gtw_addr[] = {192,168,178,1};
uint8_t sub_mask[] = {255,255,255,0};
uint8_t mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
uint8_t ip_addr[]  = {192,168,178,3};

#endif

// Methode om wiznet mee in te stellen
void initWiznet(void) {
    spi_transmit(MR, (1 << RST));   // Initialiseer registers ethernet shield
    spi_transmit(TMSR, ((1 << S0S0) | (1 << S1S0) | (1 << S2S0) | (1 << S3S0)));    // 2kb grootte
    spi_transmit(RMSR, ((1 << S0S0) | (1 << S1S0) | (1 << S2S0) | (1 << S3S0)));    // 2kb grootte
}

// Methode om de bytes uit het gateway array mee te versturen
void set_gateway(void) {
    for(int i = 0; i < (sizeof(gtw_addr) / sizeof(gtw_addr[0])); ++i) {
        spi_transmit(GAR + i, gtw_addr[i]);
    }
}

// Methode om de bytes uit het subnet array mee te versturen
void set_submask(void) {
    for(int i = 0; i < (sizeof(sub_mask) / sizeof(sub_mask[0])); ++i) {
        spi_transmit(SUBR + i, sub_mask[i]);
    }
}

// Methode om de bytes uit het macaddress array mee te versturen
void set_macaddress(void) {
    for(int i = 0; i < (sizeof(mac_addr) / sizeof(mac_addr[0])); ++i) {
        spi_transmit(SHAR + i, mac_addr[i]);
    }
}

// Methode om de bytes uit het ipadres array mee te versturen
void set_ipaddress(void) {
    for(int i = 0; i < (sizeof(ip_addr) / sizeof(ip_addr[0])); ++i) {
        spi_transmit(SIPR + i, ip_addr[i]);
    }
}

// Methode om de instellingen mee te printen
void print_status(void) {
    printf("Gateway adres: %d.%d.%d.%d\n\n",spi_receive(GAR + 0),spi_receive(GAR + 1),\
           spi_receive(GAR + 2),spi_receive(GAR + 3));
    
    printf("Source hardware adres: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n\n",spi_receive(SHAR + 0),spi_receive(SHAR + 1),\
           spi_receive(SHAR + 2),spi_receive(SHAR + 3),spi_receive(SHAR + 4),spi_receive(SHAR + 5));
    
    printf("Subnet mask: %d.%d.%d.%d\n\n",spi_receive(SUBR + 0),spi_receive(SUBR + 1),\
           spi_receive(SUBR + 2),spi_receive(SUBR + 3));
    
    printf("Ip-adres: %d.%d.%d.%d\n\n",spi_receive(SIPR + 0),spi_receive(SIPR + 1),\
           spi_receive(SIPR + 2),spi_receive(SIPR + 3));
}