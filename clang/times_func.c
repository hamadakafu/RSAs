#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "change_func.h"
#include "add_func.h"
#include "times_func.h"
#include "util.h"
#include "sub_func.h"


void power(char *a, char *p, char *ans) {
    hexa_array_plus_hexa(ans, '1', 0);
    // printf("ans: %s\n", ans);
    char zero[DIGIT + 1];
    init_hexa(zero);
    char one[DIGIT + 1];
    init_hexa(one);
    hexa_array_plus_hexa(one, '1', 0);
    int debug_count = 0;
    while(1) {
        if (is_left_big(p, zero) == 2) {
            strcpy(ans, one);
            break;
        }
        char tmp[DIGIT+1];
        init_hexa(tmp);
        times(ans, a, tmp);

        char new_p[DIGIT + 1];
        init_hexa(new_p);
        sub(p, one, new_p);

        strcpy(p, new_p);
        strcpy(ans, tmp); 
        debug_count += 1;
    }
}

void hexa_array_times_hexa(char *a, char b, char *ans) {
    // printf("b: %c\n", b);
    for (int i = 0; i < DIGIT; i++) {
        char target[DIGIT+1];
        init_hexa(target);
        hexa_times_hexa(a[i], b, target);
        for (int j = 0; j < DIGIT - i; j++) {
            hexa_array_plus_hexa(ans, target[j], i+j);
        }
    }
} 

void times(char *a, char *b, char *ans) {
    for (int i = 0; i < DIGIT; i++) {
        char tmp[DIGIT+1];
        init_hexa(tmp);
        // printf(tmp);
        // printf("b[%d]: %c\n", i, b[i]);
        hexa_array_times_hexa(a, b[i], tmp);    
        for (int j = 0; j < DIGIT-i; j++) {
            hexa_array_plus_hexa(ans, tmp[j], i+j);
        }
    }
}

void hexa_times_hexa(char a, char b, char *ans) {
    int num_a = hexa2int(a);
    int num_b = hexa2int(b);
    int num_ans = num_a * num_b;
    int2hexa_array(num_ans, ans);
}

void power2hexa_array(int num_a, int num_p, char *ans) {
    char tmp[DIGIT + 1];
    char ten[DIGIT + 1];
    char a[DIGIT + 1];
    char p[DIGIT + 1];
    char one[DIGIT +1];
    init_hexa(tmp);
    init_hexa(ten);
    init_hexa(a);
    init_hexa(p);
    init_hexa(one);
    int2hexa_array(num_a, a);
    int2hexa_array(1, ten);

    while (num_p > 0) {
        int2hexa_array((int)powf(10, 8), p);
        times(ten, p, tmp);
        num_p -= 8;
        strcpy(ten, tmp);
    }
    times(a, ten, ans);
}