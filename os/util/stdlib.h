#ifndef STDLIB_H
#define STDLIB_H

#include "../../platform.h"

void itoa ( int int_data, char * ascii_data);
void ltoa ( long int int_data, char * ascii_data);
void ftoa ( float float_data, mos_uint8_t decimal_point, char * ascii_data);
int atoi(char * ascii_data);
float atof(const char *s);

#endif