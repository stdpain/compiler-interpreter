#include <cstdio>
#include <cstdlib>
int yyparse(void);
void yyerror(const char* s) {
    printf("Error Error Error here %s\n", s);
}

int main(int argc, char*argv[]) {
    if (yyparse()) // 语法分析
    {
        /* BUGBUG */
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
    return 0;
}