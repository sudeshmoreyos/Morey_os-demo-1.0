#include "calculate_baudrate.h"

float power_of_two( int power)
{
	float result = 1;
	
	if( power < 0 )
	{
		while(power < 0)
		{
			result *= 0.5;
			power++;
		}
	}
	else
	{
		while(power > 0 )
		{
			result *= 2.0;
			power--;
		}
	}
	
	return result;
}

float xmega_baudrate(mos_uint32_t freq, mos_uint32_t baudrate, mos_uint8_t speed, \
mos_uint16_t * bsel_value, int * bscale_value)
{
	float error_min = 100, error_now=0, bsel_float = 0;
	mos_uint16_t bsel_int;
	int i; 
	
	*bsel_value = 0;
	*bscale_value = 0;
	
	for(i=-7;i<8;i++)
	{	
		if(i<0)
			bsel_float = (1 / power_of_two(i)) * ( (float)freq / ( (16/speed) * (float)baudrate) - 1);
		else
			bsel_float = (float)freq / ( power_of_two(i) * (16/speed) * (float)baudrate) - 1;
		
		bsel_int = (int)bsel_float;
		
		if ((bsel_int > 0) && (bsel_int < 4096))
		{				
			error_now = ((bsel_float - (float)bsel_int) / bsel_float) * 100.0;
			
			if (error_now < 0)
			{
				error_now *= -1;
			}
			
			if (error_now < error_min)
			{
				error_min = error_now;
				*bsel_value = bsel_int;
				*bscale_value = i;
			}
		}
	}

	return error_min;
}