#ifndef TIMES_FUNC_H
#define TIMES_FUNC_H

// 'F' times 'F' == ['1', 'E']
void hexa_times_hexa(char a, char b, char *ans);
void times(char *a, char *b, char *ans);
void hexa_array_times_hexa(char *a, char b, char *ans);

void power(char *a, char *p, char *ans);
void power2hexa_array(int num_a, int num_p, char *ans);

#endif