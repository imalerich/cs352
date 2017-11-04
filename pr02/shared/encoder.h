#include <stdio.h>
#include <string.h>
#include <math.h>

//inplace_reverse takes string as an input and reverses the contents of the string inplace.
void inplace_reverse(char * str);

//binaryToDecimal will return the decimal number for the corresponding binary form. for eg. decimal [5] for binary [101].
int binaryToDecimal(long n);

//decimalToBinary will return the binary form for the corresponding decimal form. for eg. binary [101] for decimal [5] 
long decimalToBinary(long n);

//stringToAscii will print ASCII representation of a given string
void stringToAscii(char *s);

//stringToReverseAscii will print the reverse ASCII representation of the given string
void stringToReverseAscii(char *s);

//stringToEncodedAscii prints the encoded representaion of the ASCII representaion of a given string(Encoding is based on encoding array at the top of this file)
void stringToEncodedAscii(char *s);
