/*
 *  tiltsensor.c
 *
 *  Dit is de .c file voor de SparkFun Tilt-a-Whirl Breakout - RPI-1031,
 *  Dit is een TiltSensor die reageert op de plaats van het balletje wat zich er binnenin bevindt.
 *  Eigenlijk is het bij deze tiltsensor enkel mogelijk om te kijken tegen welke zijde het balletje ligt.
 *  Dit omdat wanneer je hem horizontaal legt hij altijd tegen 1 kant zal gaan liggen (de kans is zeer zeer klein dat hij in het midden blijft liggen).
 *  Ik raad u daarom aan de Tilt-a-Whirl verticaal te plaatsen, zo kun je het alarm af laten gaan wanneer deze zich niet meer tegen de eerste opgeslagen zijde bevindt (de zijde waarin hij start)
 *
 *  Veel Succes!
 *
 *	Created: 28-11-2014
 * 	Author: Eti2b groep 4
 */

#include "TiltSensor.h"
#include <avr/io.h>
#include <util/delay.h>

int xAs;
int yAs;
int alarmOn = 0;

int tilt_analogRead(uint8_t adcPin)
{
    int ADCval;
    //ADMUX is het ADC multiplexer selection register, hier wordt de pin  gezet die wordt meegegeven als parameter.
    // Dit kan gedaan worden met enkel "=", omdat dit het eerste is wat gezet wordt.
    ADMUX = adcPin;
    // De AVcc met een externe condensator wordt gezet op de AREF pin
    ADMUX |= (1 << REFS0 | 0 << REFS1);
    //  maak het register weer vrij
    ADMUX &= ~(1 << ADLAR);

    // De prescaler wordt hier op 128 gezet voor de 8Mhz
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // De ADC wordt ge-enabled
    ADCSRA |= (1 << ADEN);

    //De ADSC bit wordt gezet, daarmee wordt de de conversie gestart.
    ADCSRA |= (1 << ADSC);

    // Door deze regel blijft hij wachten tot de conversie klaar is.
    // zolang de bit op 1 staat wacht hij.
    while(ADCSRA & (1 << ADSC));

    // Op de plek van de ADCval wordt ADCL opgeslagen.
    // Vervolgens wordt de ADCH gelezen en opgeslagen hierdoor kan de ADC weer opnieuw gebruikt worden.
    ADCval = ADCL;
    ADCval = (ADCH << 8) + ADCval;

    return ADCval;
}

void tilt_start(void){
	xAs = tilt_analogRead(0);
 	yAs = tilt_analogRead(1);
}

int tilt_getAlarm(void) {
   	int x = tilt_analogRead(0);				//Lees x waarde uit
  	int y = tilt_analogRead(1);				//Lees x waarde uit
   	tilt_check(x,y);					//Kijk of er een verandering is opgetreden ten opzichte van de beginstand.
	return alarmOn;
}

void tilt_check(int x, int y) {
    int tempX = (x)-xAs;
    int tempY = (y)-yAs;
    if(tempX >50 || tempY > 50) {
	alarmOn = 1;
    }
}

void tilt_reset(void) {
	alarmOn = 0;
	xAs = tilt_analogRead(0);
 	yAs = tilt_analogRead(1);
}



