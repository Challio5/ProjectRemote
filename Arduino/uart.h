//
//  uart.h
//  Arduino
//
//  Created by Projectgroep TI2.4 on 10-12-14.
//  Copyright (c) 2014 Projectgroep TI2.4. All rights reserved.
//

#ifndef Arduino_uart_h
#define Arduino_uart_h

void usart_init(uint16_t baudRate);
int usart_transmit(char data, FILE *stream);
char usart_receive(FILE *stream);
void usart_flush(void);

#endif
