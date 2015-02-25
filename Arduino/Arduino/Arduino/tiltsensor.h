/*
 * 	tiltsensor.h
 *
 *  Dit is de .h file voor de SparkFun Tilt-a-Whirl Breakout - RPI-1031,
 *  Include deze file in uw project om gebruik te kunnen maken van de onderstaande functies.
 *
 *  tilt_analogRead - Met deze functie kun je de waarde van de meegegeven pin opvragen.
 *  tilt_getAlarm - Met deze functie voert hij eerst de check uit om te kijken of het alarm af moet gaan en geeft vervolgens de waarde van het alarm terug. 0 voor geen alarm, 1 voor een alarm.
 *  tilt_reset - Met deze functie zet je het alarm weer op 0 en slaat hij nieuwe start waarden op van de positie waar die nu op staat.
 *  tilt_check - Met deze functie controleer je de nieuwe x en y waarden die je meegeeft ten opzichte van de bij start opgeslane waarden.
 *  tilt_start - Met deze functie slaat hij de eerste waarden van de tiltSensor op, hier kun je later met getAlarm of Check op controleren.
 *
 *  Veel Succes!
 *
 *	Created: 28-11-2014
 * 	Author: Eti2b groep 4
 */

#include <stdio.h>

#ifndef TiltSensor_h
#define TiltSensor_h

    int tilt_analogRead(uint8_t adcPin);
    int tilt_getAlarm(void);
    void tilt_reset(void);
    void tilt_check(int x, int y);
    void tilt_start(void);


#endif
