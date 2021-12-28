
// ****************************************************************************
//
//                                PWM LED output
//
// ****************************************************************************

#ifndef _PWMLED_H
#define _PWMLED_H

#define LEDFREQ		300	// LED frequency (should be 100..300 Hz)
#define PWMLED_TOP	255	// PRM top (period = PWMLED_TOP + 1 = 256)
#define PWMLED_CLOCK	(LEDFREQ*(PWMLED_TOP+1)) // PWM clock (= 300*256 = 76800 Hz)

// initialize PWM LED output (auto-initialized at startup)
Bool PWMLedInit();

// set PWM LED (0..255)
void SetPWMLed(u8 level);

#endif // _PWMLED_H
