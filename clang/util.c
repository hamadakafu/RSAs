#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "change_func.h"
#include "add_func.h"
#include "times_func.h"
#include "util.h"
#include "sub_func.h"


// = 2, little 0, big, 1 
int is_left_big(char *a, char *b) {
    for (int i = 0; i < DIGIT+1; i++) {
        if (a[DIGIT - i] > b[DIGIT - i]) {
            return 1;
        } else if (a[DIGIT - i] < b[DIGIT - i]) {
            return 0;
        }
    }
    return 2;
}

void find_d(char *e, char *phi, char *d) {
    char target[DIGIT + 1];
    init_hexa(target);
    char zero[DIGIT + 1];
    init_hexa(zero);
    hexa_array_plus_hexa(target, '1', 0);
    char left[DIGIT + 1];
    char target2[DIGIT + 1];
    while (1) {
        init_hexa(left);
        mydiv(target, e, d, left);
        if (is_left_big(left, zero) == 2) {
            break;
        }
        init_hexa(target2);
        add(target, phi, target2);
        strcpy(target, target2);
    }
}
void shift(char *a) {
    for (int i = 0; i < DIGIT-1; i++) {
        a[DIGIT-1-i] = a[DIGIT-2-i];
    }
}

void push_hexa(char *a, char b) {
    shift(a);
    a[0] = b;
}

void parse2hexa_array(char *ans) {
    char buf[256];
    fgets(buf, sizeof(buf), stdin);
    char str[256];
    sscanf(buf, "%s\n", str);
    char tmp_str[9];
    tmp_str[8] = '\0';
    int count = 0;
    
    char one[DIGIT + 1];
    char zero[DIGIT + 1];
    char tmp[DIGIT + 1];
    
    init_hexa(one);
    init_hexa(zero);
    init_hexa(tmp);
    hexa_array_plus_hexa(one, '1', 0);
    
    while (1) {
        int length = strlen(str);
        if (length > 8) {
            for (int i = 0; i < 8; i++) {
                tmp_str[i] = str[length - 8 + i];
            }
            str[length-8] = '\0';
            int tmp_num = atoi(tmp_str);

            char target[DIGIT + 1];
            init_hexa(target);
            power2hexa_array(tmp_num, count, target);
            add(ans, target, tmp);
            strcpy(ans, tmp);
        } else {
            strncpy(tmp_str, str, 8);
            int tmp_num = atoi(tmp_str);
            char target[DIGIT + 1];
            init_hexa(target);
            power2hexa_array(tmp_num, count, target);
            add(ans, target, tmp);
            strcpy(ans, tmp);
            break;
        }
        count += 8;
    }
}