#include "stdlib.h"

#define isdigit(c) (c >= '0' && c <= '9')

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

int atoi(char * ascii_data)
{
	mos_uint8_t is_negative=0;
	int value=0;
	
	if ((*ascii_data) == '-')
	{
		is_negative = 1;
		ascii_data++;
	}
	
	while(*ascii_data)
	{
		if( ((*ascii_data) >= '0') && ((*ascii_data) <= '9') )
		{
			value = 10*value + (*ascii_data) - '0';
		}
		else
			return 0;
		
		ascii_data++;
	}
	
	if(is_negative)
		value = value * (-1);
	
	return value;
}

float atof(const char *s)
{
  // This function stolen from either Rolf Neugebauer or Andrew Tolmach. 
  // Probably Rolf.
  float a = 0.0;
  int e = 0;
  int c;
  while ((c = *s++) != '\0' && isdigit(c)) 
  {
    a = a*10.0 + (c - '0');
  }
  
  if (c == '.') 
  {
    while ((c = *s++) != '\0' && isdigit(c)) {
      a = a*10.0 + (c - '0');
      e = e-1;
    }
  }
  if (c == 'e' || c == 'E') {
    int sign = 1;
    int i = 0;
    c = *s++;
    if (c == '+')
      c = *s++;
    else if (c == '-') {
      c = *s++;
      sign = -1;
    }
    while (isdigit(c)) {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }
  while (e > 0) {
    a *= 10.0;
    e--;
  }
  while (e < 0) {
    a *= 0.1;
    e++;
  }
  return a;
}