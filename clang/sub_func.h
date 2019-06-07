#ifndef SUB_FUNC_H
#define SUB_FUNC_H

// return carry over
int hexa_sub_hexa(char a, char b, char *ans);
// return success or fail
int sub(char *a, char *b, char *ans);

void mydiv(char *a, char *b, char *ans, char *left);
int old_mydiv(char *a, char *b, char *ans);
void mod(char *a, char *p, char *div, char *ans);
char *e_mod(char *a, char *p, char *div, char *ans, char *a2_mod);

#endif