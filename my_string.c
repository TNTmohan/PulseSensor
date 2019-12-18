#include "my_string.h"

char* strcpy(char *dst, const char *src) {
	char *tmp = dst;
	while ((*dst++ = *src++) != '\0');  
	return tmp;
}


char * strcat(char * dest, const char * src) {
    char *tmp = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++) != '\0');
    return tmp;
}

char* itoa(int num,char *str) {
    int i = 0;
    if(num<0) {
        num = -num;
        str[i++] = '-';
    } 
    do {
        str[i++] = num%10+48;
        num /= 10;   
    } while(num);
    
    str[i] = '\0';
    
    
    int j = 0;
    if(str[0]=='-') {
        j = 1;
        ++i;
    }
    for(;j<i/2;j++) {
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;
}
