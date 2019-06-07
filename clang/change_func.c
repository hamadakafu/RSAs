#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "change_func.h"
#include "add_func.h"
#include "times_func.h"
#include "util.h"
#include "sub_func.h"


void init_hexa(char *a) {
    for (int i = 0; i < DIGIT; i++) {
        a[i] = '0';
    }
    a[DIGIT] = '\0';
}

int hexa2int(char a) {
    if ((int)a >= 65) { return (int)a-55; } else { return (int)a-48; }
}

char int2hexa(int n) {
    switch (n % 16) {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            fprintf(stderr, "wtf!!!%d / 16 = %d\n", n, n % 16);
            exit(1);
    }    
}

void int2hexa_array(int n, char *hexa) {
    for (int i = 0; i < DIGIT; i++) {
        hexa[i] = int2hexa(n%16);
        n /= 16;
    }
}

void print_hexa_array(char *a) {
    for (int i = 0; i < DIGIT; i++) {
        char c = a[DIGIT-i-1];
        printf("%c", c);
    }
    printf("\n");
}

void print_hexa_array_ten(char *a) {
    // can't do this
}