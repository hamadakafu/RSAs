#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "change_func.h"
#include "add_func.h"
#include "times_func.h"
#include "util.h"
#include "sub_func.h"



void add(char *a, char *b, char *ans) {
    int carry_over = 0;
    for (int i = 0; i < DIGIT; i++) {
        char target;
        if (carry_over != 0) {
            char co = int2hexa(carry_over);
            carry_over = hexa_plus_hexa(a[i], b[i], &target);
            char b;
            carry_over += hexa_plus_hexa(target, co, &b);
            target = b;
        } else {
            carry_over = hexa_plus_hexa(a[i], b[i], &target);
        }
        ans[i] = target;
    } 
}

int hexa_plus_hexa(char a, char b, char *ans) {
    int num_a = hexa2int(a);
    int num_b = hexa2int(b);
    if (num_a + num_b >= 16) {
        *ans = int2hexa(num_a + num_b - 16);
        return (num_a + num_b) / 16;
    } else {
        *ans = int2hexa(num_a + num_b);
        return 0;
    }
}

void hexa_array_plus_hexa(char *a, char b, int index) {
    int carry_over = 0;
    do {
        if (index > DIGIT) {
            fprintf(stderr, "buffer overflow in hexa_array_plus_hexa\n");
            exit(1);
        }
        if (carry_over != 0) {
            char target;
            char co = int2hexa(carry_over);
            carry_over = hexa_plus_hexa(a[index], co, &target);
            a[index] = target;
        } else {
            char target;
            carry_over = hexa_plus_hexa(a[index], b, &target);
            a[index] = target;
        }
        index += 1;
    } while (carry_over != 0);
}