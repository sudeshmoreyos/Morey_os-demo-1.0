#include "stdlib.h"

/* A utility function to reverse a string  */
static void reverse(char * string, int size)
{
    int start = 0, end = size -1;
	char x;
	
    while (start < end)
    {
		x = *(string + start);
		*(string + start) = *(string + end);
		*(string + end) = x;
        start++;
        end--;
    }
}

void itoa ( int int_data, char * ascii_data)
{
    mos_uint8_t i = 0, negative = 0;

    if (int_data == 0)
    {
        ascii_data[i] = '0';
		i++;
        ascii_data[i] = '\0';
        return;
    }
 
    if (int_data < 0)
    {
        negative = 1;
        int_data = - int_data;
    }
 
    while (int_data != 0)
    {
        ascii_data[i] = (int_data % 10) + '0';
        int_data = int_data/10;
		i++;
    }
 
    if (negative)
	{
        ascii_data[i] = '-';
		i++;
	}
 
    ascii_data[i] = '\0';
    reverse(ascii_data, i);
}

void ltoa ( long int int_data, char * ascii_data)
{
    mos_uint8_t i = 0, negative = 0;

    if (int_data == 0)
    {
        ascii_data[i] = '0';
		i++;
        ascii_data[i] = '\0';
        return;
    }
 
    if (int_data < 0)
    {
        negative = 1;
        int_data = - int_data;
    }
 
    while (int_data != 0)
    {
        ascii_data[i] = (int_data % 10) + '0';
        int_data = int_data/10;
		i++;
    }
 
    if (negative)
	{
        ascii_data[i] = '-';
		i++;
	}
 
    ascii_data[i] = '\0';
    reverse(ascii_data, i);
}

/*
void ftoa ( float float_data, mos_uint8_t decimal_point, char * ascii_data)
{
	long int temp1=0, temp2=0;
	float ten_power=1;
	mos_uint8_t i = 0;
	
	if(float_data < 0)
	{
		*ascii_data = '-';
		ascii_data++;
		float_data = - float_data;
	}
	
	for(i=0;i<decimal_point;i++)
	{
		ten_power = ten_power * 10;
	}
	
	temp1 = (long int)float_data;
	temp2 = (long int)(( float_data - (float)temp1 ) * ten_power);
	
	ltoa(temp1,ascii_data);
	
	while(*ascii_data)
		ascii_data++;
	
	*ascii_data = '.';
	ascii_data++;
	
	ltoa(temp2,ascii_data);
}
*/

#define MAX_PRECISION	(10)
static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};

//char * ftoa(float f, int precision, char * buf)
void ftoa ( float f, mos_uint8_t precision, char * buf)
{
	char * ptr = buf;
	char * p = ptr;
	char * pointer1;
	char c;
	long intPart;

	// check precision bounds
	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (precision < 0)  // negative precision == automatic precision guess
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	// round value according the precision
	if (precision)
		f += rounders[precision];

	// integer part...
	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		pointer1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = pointer1;
	}

	// decimal part
	if (precision)
	{
		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// terminating zero
	*ptr = 0;

	//return buf;
	return;
}