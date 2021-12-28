
// ****************************************************************************
//
//                                PWM LED output
//
// ****************************************************************************

#include "include.h"

// initialize PWM LED output (auto-initialized at startup)
Bool PWMLedInit()
{
#if USE_PWMLED // use PWM LED support
	// set GPIO function to PWM
	gpio_set_function(PWMLED_GPIO, GPIO_FUNC_PWM);

	// get PWM default config
	pwm_config cfg = pwm_get_default_config();

	// set clock divider (INT = 0..255, FRAC = 1/16..15/16)
	//  126 MHz: 126000000/76800 = 1 640.625, INT=1640, FRAC=6,
	//     real sample rate = 126000000/(1640+6/16)/256 = 300.05
	pwm_config_set_clkdiv(&cfg, (float)clock_get_hz(clk_sys)/PWMLED_CLOCK + 0.03f); // 0.03f = rounding 0.5/16

	// set period to 256 cycles
	pwm_config_set_wrap(&cfg, PWMLED_TOP);

	// start PWM
	pwm_init(PWMLED_SLICE, &cfg, True);

	SetPWMLed(200);
#endif // USE_PWMLED

	return True;
}

// set PWM LED (0..255)
void SetPWMLed(u8 level)
{
	((u16*)&pwm_hw->slice[PWMLED_SLICE].cc)[PWMLED_CHAN] = level;
}

// auto initialize PWM LED
#if USE_PWMLED // use PWM LED support
Bool PWMLedInitOK = PWMLedInit();
#endif // USE_PWMLED
