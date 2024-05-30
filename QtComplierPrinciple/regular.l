%{
#include<stdio.h>
FILE* out;
FILE* in;

%}

digit [0-9]

%%
void[ ] {fprintf(out,"%s 26\n",yytext);}
int[ ] {fprintf(out,"%s 27\n",yytext);}
char[ ] {fprintf(out,"%s 29\n",yytext);}
if {fprintf(out,"%s 30\n",yytext);}
else {fprintf(out,"%s 31\n",yytext);}
while {fprintf(out,"%s 32\n",yytext);}
do {fprintf(out,"%s 33\n",yytext);}
include[ ] {fprintf(out,"%s 50\n",yytext);}
using[ ] {fprintf(out,"%s 51\n",yytext);}
namespace[ ] {fprintf(out,"%s 52\n",yytext);}
\+ {fprintf(out,"%s 3\n",yytext);}
- {fprintf(out,"%s 4\n",yytext);}
\* {fprintf(out,"%s 5\n",yytext);}
\/ {fprintf(out,"%s 6\n",yytext);}
\< {fprintf(out,"%s 7\n",yytext);}
\<\= {fprintf(out,"%s 8\n",yytext);}
> {fprintf(out,"%s 9\n",yytext);}
>= {fprintf(out,"%s 10\n",yytext);}
== {fprintf(out,"%s 11\n",yytext);}
!= {fprintf(out,"%s 12\n",yytext);}
& {fprintf(out,"%s 13\n",yytext);}
&& {fprintf(out,"%s 14\n",yytext);}
\|\| {fprintf(out,"%s 15\n",yytext);}
\= {fprintf(out,"%s 16\n",yytext);}
\( {fprintf(out,"%s 17\n",yytext);}
\) {fprintf(out,"%s 18\n",yytext);}
\[ {fprintf(out,"%s 19\n",yytext);}
\] {fprintf(out,"%s 20\n",yytext);}
\{ {fprintf(out,"%s 21\n",yytext);}
\} {fprintf(out,"%s 22\n",yytext);}
: {fprintf(out,"%s 23\n",yytext);}
; {fprintf(out,"%s 24\n",yytext);}
, {fprintf(out,"%s 25\n",yytext);}
\" {fprintf(out,"%s 26\n",yytext);}
# {fprintf(out,"%s 40\n",yytext);}
[1-9]{digit}* {fprintf(out,"%s 2\n",yytext);}
[_]*[a-zA-Z][a-zA-Z|0-9]* {fprintf(out,"%s 1\n",yytext);}
[ \t\n] { /* ignore whitespace */ }
. ;
%%

int main(int argc, char* argv[])
{
    out = fopen("Lexoutput.txt","w");
    if(argc < 2) {
        printf("¶ÁÈ¡ÎÄ¼þLexinput.txt¡£\n");
        yyin = fopen("Lexinput.txt","r");
        yylex();
    }else {
        for(int i = 1; i < argc; i++) {
            yy_scan_string(argv[i]);
            yylex();
        }
    }
    fclose(out);
    return 0;
}

int yywrap()
{
	return 1;
}

