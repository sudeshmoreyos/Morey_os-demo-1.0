#include "avr_mega_analog_input.h"
#include "../../../../os/util/delay_blocking.h"


static void begin(mos_uint8_t speed_mode, mos_uint8_t ref_type)
{
	ADMUX = (ref_type<<REFS0) | (0<<ADLAR);
	#if defined(ANALOGINPUT_TYPE0)
		ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | speed_mode;
		SFIOR &= ~(1<<ACME);
	#elif defined(ANALOGINPUT_TYPE1)
		ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | speed_mode;		
	#endif
}

mos_uint16_t read(mos_uint8_t channel)
{
	ADMUX &= ~0x1F;
	ADMUX |= channel;
	delay_us_blocking(10);
	ADCSRA|=(1<<ADSC);
	while (!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	return ADCW;
}

#ifdef ANALOGINPUT_SUPPORT_DIFFERNTIAL

mos_uint16_t (*readDifferntial)(mos_uint8_t channel, mos_uint8_t gain)
{
	if(channel == 0)
	{
		
		
	}
	
}

#endif

const struct analog_input_driver avr_mega_analog_input_driver = {
	begin,
	read
};
