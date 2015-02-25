//
//  tcp.h
//  Arduino
//
//  Created by Projectgroep TI2.4 on 05-01-15.
//  Copyright (c) 2015 Projectgroep TI2.4. All rights reserved.
//

#ifndef __Arduino__tcp__
#define __Arduino__tcp__

void tcp_create_socket(void);
void tcp_disconnect(void);
void tcp_close_socket(void);
void tcp_listen(void);
void tcp_to_monitor(tcpflag flag, uint8_t data[], int length);
void tcp_from_monitor(uint8_t buffer[]);

#endif /* defined(__Arduino__tcp__) */
