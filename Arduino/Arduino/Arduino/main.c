// Tilt bibliotheek
#include "tiltsensor.h"

// c bibliotheken
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// avr bibliotheken
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// gebruikers bibliotheken
#include "uart.h"
#include "lcd.h"
#include "spi.h"
#include "eth.h"

// udp/tcp bibliotheken
#include "udp.h"
#include "tcp.h"

#define DEBUG   1               // Debug mode, print naar de monitor

#define BUZZER  PORTB0          // Poort waar de buzzer op is aangesloten
#define LED     PORTB1          // Poort waar de led op is aangesloten

// Opent een stream voor seriele communicatie
FILE uart = FDEV_SETUP_STREAM(usart_transmit, usart_receive, _FDEV_SETUP_RW);

// Wachtwoord voor tcp, verandert bij elke nieuwe verbinding
char * password[] = {"6Ic1_894", "hkj0K&l5", "7M|I#p96", "82Q018!h", "X96!$M35"
                     "9K16&p52", ">-$}u85Q", "93C213H1", "769!cK0O", "1H355Q9}"};
int password_index = 0;
int counter = 0;

// Geeft aan met nul en 1 of iemand geauthoriseerd is
uint8_t authorised = 0;

// Geeft aan met nul en 1 of de node gearmed is
uint8_t armed = 1;

// De main methode voor het uitvoeren van de acties
int main(void) {
    // Geen interrupts voor het instellen van de node
    cli();

    // Zet USART op
    usart_init(9600);
    
     // Stdin/stdout lezen en schrijven via uart
    stdout = stdin = &uart;
	
    // Kalibreer de tiltsensor
    tilt_start();
    
    // Zet SPI op
    spi_init();
    
    // Zet en configureer het lcd display
    // lcd_init();
    
     // Configureert de wiznet
    initWiznet();
    
    // Configureert de netwerk instellingen
    set_gateway();
    set_submask();
    set_macaddress();
    set_ipaddress();
	
    // Print de instellingen
    print_status();

    // Buffer voor het ontvangen en versturen van data
    uint8_t receiveddata[512];
    
    // Zend een register bij startup
    udp_broadcast(reg, 8);
    
    // Led poort op output
    DDRB |= (1 << LED);
    
    // Buzzer poort op output
    DDRB |= (1 << BUZZER);
    
    // Schakel timer overflow interrupts in
    TIMSK1 |= (1 << TOIE1);
    
    // Stel de timer in met prescaler 1024
    TCCR1B |= ((1 << CS12) | (1 << CS10));
    
    // Zet interrupts aan voor het alarm
    sei();
    
    while(1) {
        // Lees de status van zowel de udp als tcp socket
        uint8_t udpstatus = spi_receive(S0SR);
        uint8_t tcpstatus = spi_receive(S1SR);
        
        switch (udpstatus) {
            // Gesloten socket
            case SOCK_CLOSED:
#if DEBUG
                printf("UDP socket gesloten\n");
#endif
                // Maak een udp socket aan
                udp_create_socket();
                break;
            
            // Werkende UDP socket
            case SOCK_UDP:
#if DEBUG
                printf("UDP socket geopend\n");
#endif
                /* MONITOR VERSTUURD NIETS VIA UDP
                // Checkt op binenkomende data
                if(spi_receive(S0RXRSRH) || spi_receive(S0RXRSRL)){
                    // Schrijft de binnenkomende data weg in de meegegeven buffer
                    udp_from_monitor(receiveddata);
                    
#if DEBUG           // Print de data in de buffer
                    printf("\nUDP data ontvangen: %s\n", receiveddata);
#endif                
                }
                */
                 
                break;
        }
        
        switch (tcpstatus) {
            // Gesloten socket
            case SOCK_CLOSED:
#if DEBUG
                printf("TCP socket gesloten\n");
#endif
                // Maak een tcp socket aan
                tcp_create_socket();
                
                break;
       
            // TCP socket geinitialiseerd
            case SOCK_INIT:
#if DEBUG
                printf("TCP socket geopend\n");
#endif
                // Luister naar binnenkomende request
                tcp_listen();
                
                break;
            
            // TCP serversocket listen
            case SOCK_LISTEN:
#if DEBUG
                printf("TCPServer luisteren\n");
#endif
                break;
            
            // TCP verbinding opgezet
            case SOCK_ESTABLISHED:
#if DEBUG
                printf("TCP verbinding opgezet\n");
#endif
                // Checkt op binenkomende data
                if(spi_receive(S1RXRSRH) || spi_receive(S1RXRSRL)) {
                    // Schrijft de binnenkomende data weg in de meegegeven buffer
                    tcp_from_monitor(receiveddata);
                    
#if DEBUG           // Print de data in de buffer
                    printf("\nTCP data ontvangen: %s\n", receiveddata);
#endif
                    // Stuur een acknowledge
                    uint8_t flag = receiveddata[0];
                    uint16_t messagenumber = (receiveddata[1] << 8) | receiveddata[2];
                    
                    switch (flag) {
                        case tcpres:
                            // ??
                            if(authorised) {
                                
                            }
                            break;
                        case login:
                            // Telt het aantal matchende characters
                            while (password[password_index % 10][counter] == receiveddata[counter + 3]) {
                                ++counter;
                            }
                            
                            // Checkt of er 9 characters zijn geteld
                            // 8 van het wachtwoord + 1 niet matchend character
                            if (counter == 9) {
                                authorised = 1;
#if DEBUG
                                printf("Gebruiker ingelogd\n");
#endif
                                // Verhoogt de index voor een volgende wachtwoord
                                ++password_index;
                            } else {
                                // Laat de buzzer afgaan met 3 korte beeps
                                for (int i = 0; i < 3; ++i) {
                                    printf("Incorrect wachtwoord\n");
                                    PORTB ^= (1 << BUZZER);
                                    _delay_ms(500);
                                }
                            }
                            
                            break;
                        case buzz:
                            if(authorised) {
                                // Lees de timer waarde uit
                                uint32_t time = ((uint32_t) receiveddata[3]) << 24;
                                time |= ((uint32_t) receiveddata[4]) << 16;
                                time |= receiveddata[5] << 8;
                                time |= receiveddata[6];
                                
                                // Zet de buzzer aan
                                PORTB |= (1 << BUZZER);
                                
                                // Voert een delay uit met de timer waarde
                                for (uint32_t i = 0; i < time; ++i) {
                                    _delay_ms(1);
                                }
                                
                                // Zet de buzzer uit
                                PORTB &= ~(1 << BUZZER);
                            }
                            break;
                        case lamp:
                            if(authorised) {
                                printf("Lamp");
                                uint8_t on = receiveddata[3];
                                if (on) {
                                    // Zet de led aan
                                    PORTB |= (1 << LED);
                                } else {
                                    // Zet de led uit
                                    PORTB &= ~(1 << LED);
                                }
                            }
                            break;
                        case arm:
                            if(authorised) {
                                uint8_t on = receiveddata[3];
                                if (on) {
                                    printf("Armed");
                                    // Armed de node en schakelt het alarm in
                                    armed = 1;
                                } else {
                                    printf("Disarmed");
                                    // Disarmed de node en schakelt het alarm uit
                                    armed = 0;
                                }
                            }
                            break;
                        case reset:
                            if(authorised) {
                                printf("Reset");
                                
                                // Unregister de node
                                udp_broadcast(unr, 10);
                                
                                // Reset de node, instellingen worden hersteld
                                WDTCSR |= (1 << WDE);
                            }
                            break;
                        case ping:
                            if(authorised) {
                                // Broadcast een acknowledge
                                udp_broadcast(udpack, 3);
                            }
                            break;
                        case tcpack:
                            // Bericht door monitor goed ontvangen
                            // DEPRECATED Ingebakken in TCP
                            break;
                        case tcpnak:
                            // bericht door monitor niet goed ontvangen
                            // DEPRECATED Ingebakken in TCP
                            break;
                        case receive:
                            if(authorised) {
                                // stuur status
                                // DEPRECATED
                            }
                            break;
                    }
                }
                // Anders stuur eventueel test data
                else {
                    // Stuur hallo monitor
                    // tcp_to_monitor(tcpack, "Hallo Monitor", 13);
                }
                
                break;
            
            // TCP sluit request
            case SOCK_CLOSE_WAIT:
#if DEBUG
                printf("TCP close request ontvangen\n");
#endif
                // Monitor wordt uitgelogd
                authorised = 0;
                
                // Socket luistert naar nieuwe requests
                tcp_listen();
                
                break;
        }
#if DEBUG
        // Delay voor het uitlezen van de seriele communicatie
        _delay_ms(1000);
#endif
        // Verwerkingstijd
        // _delay_ms(1);
    }
    
	return 0;
}

// Timer interrupt, checkt of er op basis van de tiltsensor een alarm moet worden gegenereerd
ISR(TIMER1_OVF_vect) {
    // Leest de tiltsensor uit
    if(tilt_getAlarm() && armed) {
#if DEBUG
        printf("ALARM\n");
#endif
        // Broadcast een alarm
        udp_broadcast(alarm, 3);
        
        // Zet de led aan
        PORTB |= (1 << LED);
        
        // Zet de buzzer aan
        PORTB |= (1 << BUZZER);
    } else {
#if DEBUG
        printf("BROADCAST\n");
#endif
        // Broadcast een alive
        udp_broadcast(alive, 3);
    }
}
