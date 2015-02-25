//
//  udp.h
//  Arduino
//
//  Created by Projectgroep TI2.4 on 05-01-15.
//  Copyright (c) 2015 Projectgroep TI2.4. All rights reserved.
//

#ifndef __Arduino__udp__
#define __Arduino__udp__

void udp_create_socket(void);
void udp_close_socket(void);
void udp_broadcast(udpflag flag, int length);
void udp_to_monitor(udpflag flag, uint8_t data[], int length);
void udp_from_monitor(uint8_t buffer[]);

#endif /* defined(__Arduino__udp__) */
