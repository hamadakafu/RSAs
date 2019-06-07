#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "change_func.h"
#include "add_func.h"
#include "times_func.h"
#include "util.h"
#include "sub_func.h"


int main(int argc, char **argv) {
    char test[DIGIT + 1];
    char tmp[DIGIT + 1];
    char tmp2[DIGIT + 1];
    char some2[DIGIT + 1];

    char ans[DIGIT+1];
    char p[DIGIT+1];
    char q[DIGIT+1];
    char e[DIGIT + 1];
    char d[DIGIT + 1];
    char phi[DIGIT + 1];
    char m[DIGIT + 1];
    char c[DIGIT + 1];
    char n[DIGIT + 1];
    char hat_m[DIGIT + 1];
    char p_[DIGIT+1];
    char q_[DIGIT+1];
    char one[DIGIT + 1];

    init_hexa(test);
    init_hexa(tmp);
    init_hexa(tmp2);
    init_hexa(some2);

    init_hexa(ans);
    init_hexa(p);
    init_hexa(q);
    init_hexa(e);
    init_hexa(d);
    init_hexa(phi);
    init_hexa(m);
    init_hexa(c);
    init_hexa(n);
    init_hexa(hat_m);
    init_hexa(p_);
    init_hexa(q_);
    init_hexa(one);
    
    char buf[256];
    int num_p, num_q, num_e, num_m;

    printf("input p:\n");
    parse2hexa_array(p);
    printf("input q:\n");
    parse2hexa_array(q);
    printf("input e:\n");
    parse2hexa_array(e);

    printf("input message: ");
    parse2hexa_array(m);

    printf("m: ");
    print_hexa_array(m);

    hexa_array_plus_hexa(one, '1', 0);

    sub(p, one, p_);
    sub(q, one, q_);
    times(p_, q_, phi);

    times(p, q, n);

    printf("n: %s\n", n);
    printf("e: %s\n", e);

    find_d(e, phi, d);

    printf("d: %s\n", d);
    times(m, m, some2);
    mydiv(some2, n, tmp, tmp2);
    e_mod(m, e, n, c, tmp2);
    printf("c: %s\n", c);

    init_hexa(some2);
    init_hexa(tmp);
    init_hexa(tmp2);
    times(c, c, some2);
    mydiv(some2, n, tmp, tmp2);
    e_mod(c, d, n, hat_m, tmp2);
    printf("speculated m: ");
    print_hexa_array(m);
    return 0;
}
