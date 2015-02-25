//
//  ethernet.h
//  Arduino
//
//  Created by Projectgroep TI2.4 on 10-12-14.
//  Copyright (c) 2014 Projectgroep TI2.4. All rights reserved.
//

#ifndef Arduino_ethernet_h
#define Arduino_ethernet_h

/* Mode registers */
#define MR      0x0000          // Mode Register voor reset, pppoe, ping block en ind-bus

#define AI      1               // Increment het adres automatisch in I/F mode
#define PPPoE   3               // Set PPPoE mode
#define PB      4               // Set pinblock mode
#define RST     7               // Reset en initialiseert de registers

/* Socket grootte register */
#define RMSR    0x001A          // Register voor het RX socket geheugen grootte
#define TMSR    0x001B          // Register voor het TX socket geheugen grootte

#define S0S0    0               // Bit voor het zetten van de socket 0 grootte
#define S0S1    1               // Bit voor het zetten van de socket 0 grootte
#define S1S0    2               // Bit voor het zetten van de socket 1 grootte
#define S1S1    3               // Bit voor het zetten van de socket 1 grootte
#define S2S0    4               // Bit voor het zetten van de socket 2 grootte
#define S2S1    5               // Bit voor het zetten van de socket 2 grootte
#define S3S0    6               // Bit voor het zetten van de socket 3 grootte
#define S3S1    7               // Bit voor het zetten van de socket 3 grootte

/* Ethernet configuratie registers */
#define GAR     0x0001          // Register voor het gateway ipadres, 1 t/m 4 voor elke byte
#define SUBR    0x0005          // Register voor het subnet mask, 5 t/m 8 voor elke byte
#define SHAR    0x0009          // Register voor het source hardware address (mac), 9 t/m E per byte
#define SIPR    0x000F          // Register voor het source ipadres, F t/m 12 voor elke byte

/* Socket mode registers */
#define S0MR    0x0400          // Mode register voor socket 0
#define S1MR    0x0500          // Mode register voor socket 1
#define S2MR    0x0600          // Mode register voor socket 2
#define S3MR    0x0700          // Mode register voor socket 3

#define P0      0               // Protocol bit 0
#define P1      1               // Protocol bit 1
#define P2      2               // Protocol bit 2
#define P3      3               // Protocol bit 3
#define ND      5               // Geen delay ACK bit (multicast: IGMPv2 = 0, IGMPv1 = 1)
#define MULTI   7               // Multicast bit

/* Socket control registers */
#define S0CR    0x0401          // Control register voor socket 0
#define S1CR    0x0501          // Control register voor socket 1
#define S2CR    0x0601          // Control register voor socket 2
#define S3CR    0x0701          // Control register voor socket 3

#define OPEN        0x01        // Register waarde voor het initialiseren van een socket
#define LISTEN      0x02        // Register waarde voor het luisteren naar connectie aanvraag (TCP)
#define CONNECT     0x04        // Register waarde voor het versturen van een connectie aanvraag (TCP)
#define DISCON      0x08        // Register waarde voor het sluiten van de verbinding (TCP)
#define CLOSE       0x10        // Register waarde voor het sluiten van de socket
#define SEND        0x20        // Register waarde voor het versturen van data
#define SEND_MAC    0x21        // Register waarde voor het versturen van het macadres
#define SEND_KEEP   0x22        // Register waarde voor het checken van de connectie status (TCP)
#define RECV        0x40        // Register waarde voor het versturen van een receive?

/* Socket interrupt registers */
#define S0IR    0x0402          // Interrupt register voor socket 0
#define S1IR    0x0502          // Interrupt register voor socket 1
#define S2IR    0x0602          // Interrupt register voor socket 2
#define S3IR    0x0702          // Interrupt register voor socket 3

#define SEND_OK 4               // Flag voor als een send operatie klaar is
#define TIMEOUT 3               // Flag voor als een timeout optreed
#define RECEIVE 2               // Flag voor als er data binnenkomt
#define DISCONI 1               // Flag voor als er geen of verzoek om de verbinding te eindigen is
#define CON     0               // Flag voor als er een verbinding is

/* Socket status registers */
#define S0SR    0x0403          // Status register voor socket 0
#define S1SR    0x0503          // Status register voor socket 1
#define S2SR    0x0603          // Status register voor socket 2
#define S3SR    0x0703          // Status register voor socket 3

#define SOCK_CLOSED         0x00        // Het socket control register bevat een close command
#define SOCK_INIT           0x13        // Het socket control register bevat een open command, MR is TCP
#define SOCK_LISTEN         0x14        // Het socket control register bevat een listen command
#define SOCK_ESTABLISHED    0x17        // Er is een TCP verbinding
#define SOCK_CLOSE_WAIT     0x1C        // Het socket control register bevat een close command
#define SOCK_UDP            0x22        // Het socket control register bevat een open command, MR is UDP
#define SOCK_IPRAW          0x32        // Het socket control register bevat een open command, MR is IPRAW
#define SOCK_MACRAW         0x42        // Het socket control register 0 bevat een open command, 0 is MACRAW
#define SOCK_PPPOE          0x5F        // Het socket control register 0 bevat een open command, 0 is PPPOE

/* Waardes voor een veranderende status */
#define SOCK_SYNSENT    0x15            // Het socket control register bevat een connect command (init stat)
#define SOCK_SYNRECV    0x16            // Er is een binnenkomend verzoek voor een verbinding
#define SOCK_FIN_WAIT   0x18            // Proces voor het sluiten van verbinding
#define SOCK_CLOSING    0x1A            // Proces voor het sluiten van verbinding
#define SOCK_TIME_WAIT  0x1B            // Proces voor het sluiten van verbinding
#define SOCK_LAST_ACK   0x1D            // Proces voor het sluiten van verbinding
#define SOCK_ARP1       0x11            // Er is een ARP request
#define SOCK_ARP2       0x21            // Er is een ARP request
#define SOCK_ARP3       0x31            // Er is een ARP request

/* Socket port registers */
#define S0PORTH     0x0404          // Port register voor socket 0 high byte
#define S0PORTL     0x0405          // Port register voor socket 0 low byte
#define S1PORTH     0x0504          // Port register voor socket 1 high byte
#define S1PORTL     0x0505          // Port register voor socket 1 low byte
#define S2PORTH     0x0604          // Port register voor socket 2 high byte
#define S2PORTL     0x0605          // Port register voor socket 2 low byte
#define S3PORTH     0x0704          // Port register voor socket 3 high byte
#define S3PORTL     0x0705          // Port register voor socket 3 low byte

/* Socket destination MAC registers (Passive mode, geset via ARP) */
#define S0DHAR  0x0406      // Destination mac adres voor socket 0, loopt van 6 t/m b per byte
#define S1DHAR  0x0506      // Destination mac adres voor socket 1, loopt van 6 t/m b per byte
#define S2DHAR  0x0606      // Destination mac adres voor socket 2, loopt van 6 t/m b per byte
#define S3DHAR  0x0706      // Destination mac adres voor socket 3, loopt van 6 t/m b per byte

/* Socket destination IP registers (Passive mode, automatisch geset) */
#define S0DIPR  0x040C      // Destination ip adres voor socket 0, loopt van c t/m f per byte
#define S1DIPR  0x050C      // Destination ip adres voor socket 1, loopt van c t/m f per byte
#define S2DIPR  0x060C      // Destination ip adres voor socket 2, loopt van c t/m f per byte
#define S3DIPR  0x070C      // Destination ip adres voor socket 3, loopt van c t/m f per byte

/* Socket destination Port registers (Passive mode, automatisch geset) */
#define S0DPORTH    0x0410      // Destination port voor socket 0 high byte
#define S0DPORTL    0x0411      // Destination port voor socket 0 low byte
#define S1DPORTH    0x0510      // Destination port voor socket 1 high byte
#define S1DPORTL    0x0511      // Destination port voor socket 1 low byte
#define S2DPORTH    0x0610      // Destination port voor socket 2 high byte
#define S2DPORTL    0x0611      // Destination port voor socket 2 low byte
#define S3DPORTH    0x0710      // Destination port voor socket 3 high byte
#define S3DPORTL    0x0711      // Destination port voor socket 3 low byte

/* Socket maximum segment size registers (Passive mode, geset door andere partij) */
#define S0MSSH      0x0412      // Port register voor socket 0 high byte
#define S0MSSL      0x0413      // Port register voor socket 0 low byte
#define S1MSSH      0x0512      // Port register voor socket 1 high byte
#define S1MSSL      0x0513      // Port register voor socket 1 low byte
#define S2MSSH      0x0612      // Port register voor socket 2 high byte
#define S2MSSL      0x0613      // Port register voor socket 2 low byte
#define S3MSSH      0x0712      // Port register voor socket 3 high byte
#define S3MSSL      0x0713      // Port register voor socket 3 low byte

/* Socket Transmit Read Pointer Registers */
#define S0TXRRH     0x0422      // Locatie informatie voor socket 0 high byte
#define S0TXRRL     0x0423      // Locatie informatie voor socket 0 low byte
#define S1TXRRH     0x0522      // Locatie informatie voor socket 1 high byte
#define S1TXRRL     0x0523      // Locatie informatie voor socket 1 low byte
#define S2TXRRH     0x0622      // Locatie informatie voor socket 2 high byte
#define S2TXRRL     0x0623      // Locatie informatie voor socket 2 low byte
#define S3TXRRH     0x0722      // Locatie informatie voor socket 3 high byte
#define S3TXRRL     0x0723      // Locatie informatie voor socket 3 low byte

/* Socket Tranmit Write Pointer Registers */
#define S0TXWRH     0x0424      // Locatie informatie voor socket 0 high byte
#define S0TXWRL     0x0425      // Locatie informatie voor socket 0 low byte
#define S1TXWRH     0x0524      // Locatie informatie voor socket 1 high byte
#define S1TXWRL     0x0525      // Locatie informatie voor socket 1 low byte
#define S2TXWRH     0x0624      // Locatie informatie voor socket 2 high byte
#define S2TXWRL     0x0625      // Locatie informatie voor socket 2 low byte
#define S3TXWRH     0x0724      // Locatie informatie voor socket 3 high byte
#define S3TXWRL     0x0725      // Locatie informatie voor socket 3 low byte

/* Socket Receive Read Pointer Registers */
#define S0RXRRH     0x0428      // Locatie informatie voor socket 0 high byte
#define S0RXRRL     0x0429      // Locatie informatie voor socket 0 low byte
#define S1RXRRH     0x0528      // Locatie informatie voor socket 1 high byte
#define S1RXRRL     0x0529      // Locatie informatie voor socket 1 low byte
#define S2RXRRH     0x0628      // Locatie informatie voor socket 2 high byte
#define S2RXRRL     0x0629      // Locatie informatie voor socket 2 low byte
#define S3RXRRH     0x0728      // Locatie informatie voor socket 3 high byte
#define S3RXRRL     0x0729      // Locatie informatie voor socket 3 low byte

/* Base adressen voor de sockets */
#define S0TXBASE    0x4000
#define S1TXBASE    0x4800
#define S2TXBASE    0x5000
#define S3TXBASE    0x5800

#define S0RXBASE    0x6000
#define S1RXBASE    0x6800
#define S2RXBASE    0x7000
#define S3RXBASE    0x7800

/* Socket Transmit Size Registers */
#define S0RXRSRH    0x0426      // Geeft de grootte van het versturen van een pakket
#define S0RXRSRL    0x0427
#define S1RXRSRH    0x0526
#define S1RXRSRL    0x0527
#define S2RXRSRH    0x0626
#define S2RXRSRL    0x0627
#define S3RXRSRH    0x0726
#define S3RXRSRL    0x0727

/* Socket Receive Size Registers */
#define S0TXFSRH    0x0420      // Geeft de grootte van het binnenkomende pakket
#define S0TXFSRL    0x0421
#define S1TXFSRH    0x0520
#define S1TXFSRL    0x0521
#define S2TXFSRH    0x0620
#define S2TXFSRL    0x0621
#define S3TXFSRH    0x0720
#define S3TXFSRL    0x0721

// UDP flags
typedef enum {
    udpres = 0, udpack = 1, reg = 3, unr = 4, alive = 5, alarm = 6
} udpflag;

// TCP flags
typedef enum {
    tcpres = 0, login = 1, buzz = 2, lamp = 3, arm = 4, reset = 5,
    ping = 6, tcpnak = 7, tcpack = 8, receive = 10
} tcpflag;

void initWiznet(void);

void set_gateway(void);
void set_submask(void);
void set_macaddress(void);
void set_ipaddress(void);

void print_status(void);

#endif
