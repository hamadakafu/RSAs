#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "change_func.h"
#include "add_func.h"
#include "times_func.h"
#include "util.h"
#include "sub_func.h"

// return ans address
char *e_mod(char *a, char *p, char *div, char *ans, char *a2_mod) {
    char target1[DIGIT + 1];
    char two[DIGIT + 1];
    char one[DIGIT + 1];
    char next_p[DIGIT +1];
    char tmp[DIGIT + 1];

    init_hexa(target1);
    init_hexa(two);
    init_hexa(one);
    init_hexa(next_p);
    init_hexa(tmp);

    int2hexa_array(2, two);
    int2hexa_array(1, one);

    switch (is_left_big(p, two)) {
        case 2:
            ; // == 2
            strcpy(ans, a2_mod);
            break;
        case 0:
            ; // < 2 and p must be 1...
            if (p[0] == '0') {
                fprintf(stdout, "exist p[0] == 0 sorry\n");
                strcpy(ans, one);
            } else {
                fprintf(stdout, "p[0] == %d\n", p[0]);
                strcpy(ans, a);
            }
            break;
        case 1:
            ; // > 2
            char left_ans[DIGIT + 1];
            init_hexa(left_ans);
            // judge odd or even
            if (hexa2int(p[0]) % 2 == 0) {  // even
                mydiv(p, two, next_p, tmp);
                init_hexa(tmp);
                e_mod(a, next_p, div, left_ans, a2_mod);
                times(left_ans, left_ans, target1);
                mydiv(target1, div, tmp, ans);
            } else {                        // odd
                sub(p, one, next_p);
                times(e_mod(a, next_p, div, left_ans, a2_mod), a, target1);
                mydiv(target1, div, tmp, ans);
            }
            break;
        default:
            fprintf(stderr, "error: logic error in e_mod.\n");
            exit(1);
    }
    return ans;
}

void mod(char *a, char *p, char *div, char *ans) {
    char target1[DIGIT + 1];
    init_hexa(target1);
    times(a, a, target1);
    char tmp[DIGIT + 1];
    init_hexa(tmp);
    char left[DIGIT + 1];

    char one[DIGIT + 1];
    init_hexa(one);
    hexa_array_plus_hexa(one, '1', 0);
    char new_p[DIGIT + 1];
    init_hexa(new_p);
    sub(p, one, new_p);
    strcpy(p, new_p);
    init_hexa(new_p);
    sub(p, one, new_p);
    strcpy(p, new_p);
    char zero[DIGIT + 1];
    init_hexa(zero);

    init_hexa(left);
    mydiv(target1, div, tmp, left);
    int debug_count = 0;

    while (1) {
        if (is_left_big(p, zero) == 2) { break;} 

        init_hexa(target1);
        times(left, a, target1);

        init_hexa(tmp);
        init_hexa(left);
        mydiv(target1, div, tmp, left);

        init_hexa(new_p);
        sub(p, one, new_p);
        strcpy(p, new_p);
        debug_count += 1;
    }
    strcpy(ans, left);
}


void mydiv(char *a, char *b, char *ans, char *left) {
    char divided[DIGIT+1];
    init_hexa(divided);
    for (int i = 0; i < DIGIT; i++) {
        push_hexa(left, a[DIGIT-i-1]);
        if (is_left_big(left, b) != 0) {
            int ans_num = old_mydiv(left, b, divided);
            if (ans_num >= 16) { fprintf(stderr, "wtf! in mydiv\n"); exit(1);}
            push_hexa(ans, int2hexa(ans_num));
            strcpy(left, divided);
        } else {
            push_hexa(ans, '0');
        }
    }
}
int old_mydiv(char *a, char *b, char *ans) {
    int count = 0;
    char tmp[DIGIT+1];
    init_hexa(tmp);
    strcpy(ans, a);
    while (sub(ans, b, tmp) != 0) {
        strcpy(ans, tmp);
        init_hexa(tmp);
        count += 1;
    }
    return count;
}
// return carry over 
int hexa_sub_hexa(char a, char b, char *ans) {
    int num_a = hexa2int(a);
    int num_b = hexa2int(b);
    if (a < b) {
        *ans = int2hexa(num_a + 16 - num_b);
        return 1;
    } else {
        *ans = int2hexa(num_a - num_b);
        return 0;
    }
}

// success 1, fail 0
int sub(char *a, char *b, char *ans) {
    int is_big = is_left_big(a, b);
    if (is_big == 0) {
        return 0;
    } else if (is_big == 2) {
        // do nothing
    } else {
        int carry_over = 0;
        for (int i = 0; i < DIGIT; i++) {
            if (carry_over != 0) {
                // maybe '1' 
                char tmp;
                char co = int2hexa(carry_over);
                int ad_co = hexa_sub_hexa(a[i], co, &tmp); 
                ans[i] = tmp;
                ad_co += hexa_sub_hexa(ans[i], b[i], &tmp);
                ans[i] = tmp;
                carry_over = ad_co;
            } else {
                char tmp;
                carry_over = hexa_sub_hexa(a[i], b[i], &tmp);
                ans[i] = tmp;
            }
        }
    }
    return 1;
}