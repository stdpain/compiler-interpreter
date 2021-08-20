#include <stdio.h>

#include "MEM.h"

extern int yyparse(void);

void yyerror(char* s) {
    printf("Error Error Error here %s\n", s);
}
void Interpreter_printRoot();
void execute();
void initProcs();

int main(int argc, char* argv[]) {
    extern FILE* yyin;
    FILE* fp;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }
    yyin = fp;
    if (yyparse()) // 语法分析
    {
        /* BUGBUG */
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
    initProcs();
    Interpreter_printRoot();
    execute();
    return 0;
}