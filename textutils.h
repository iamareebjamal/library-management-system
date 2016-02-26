#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <malloc.h>

size_t len(const char* str){
	int len = 0;
	while(*str++)
		len++;
	return len;
}

char* to_upper(const char* s){
	char* up = strdup(s);
	char* to = up;
	while(*to){
		*to=toupper(*to);
		to++;
	}
	return up;
}

int verify_fn(char* fn){
	int n = len(fn);
	if(n==8){
		fn = to_upper(fn);
		regex_t regex;
		char* pattern = \
		"[0-1][0-9][PCLEM]"\
		"[EK][B][0-9][0-9][0-9]";
		int reti = regcomp(&regex, pattern, 0);
		if(reti) {
			printf("Regex couldn't compile");
			return 0;
		}
		reti = regexec(&regex, fn, 0, NULL, 0);
		free(fn);
		regfree(&regex);
		return !reti;
	}
	
	return 0;
}

int verify_pass(char* pass){
	int n = len(pass);
	if(n>=8&&n<16){
		while(*++pass){
			if(*pass==' '||*pass=='\t')
				return 0;
		}
		return 1;
	}
	return 0;
}

void clean(char* s){
	int i,j=0;
	for(i=0; i<len(s); i++){
		char c = s[i];
		if((c>='A'&&c<='Z')||(c>='0'&&c<='9')){
			s[j++] = c;
		}
	}
	s[j]='\0';
}

#endif
