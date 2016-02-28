#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <malloc.h>

size_t len(const char* str) {
	int len = 0;
	while (*str++)
		len++;
	return len;
}

//Fix increment
char* to_upper(const char* s) {
	char* up = strdup(s);
	char* to = up;
	while (*to) {
		*to = toupper(*to);
		to++;
	}
	return up;
}

int is_fac_year(char *year) {
	if ((year[0] >= '0' && year[0] <= '1') && (year[1] >= '0' && year[1] <= '9') && len(year) == 2) {
		return 1;
	} else
		return 0;
}

int is_fac_branch(char *branch) {
	const char *br[6] = {"MEB", "PEB", "KEB", "LEB", "EEB", "PKB"};
	int i = 0;
	for (i; i <= 5; i++) {
		if (strcmp(br[i], branch) == 0) {
			return 1;
		}
		//printf("%s\n",br[i] );

	}
	return 0;
}

int is_serial_number(char *serial) {
	int i = 0;

	if (len(serial) != 3)
		return 0;
	for (i; i <= 2; i++) {
		if (!(serial[i] >= '0' && serial[i] <= '9'))
			return 0;
	}
	return 1;

}

int verify_fn(char* fn) {
	//Not supported in windows. Change to cross
	//compilant code
	char year[2], branch[3], serial[3];

	int n = len(fn);
	if (n == 8) {

		strncpy(year, fn, 2); year[2] = 0;
		strncpy(branch, &fn[2], 3); branch[3] = 0;
		strncpy(serial, &fn[5], 3); serial[3] = 0;

		if (is_fac_year(year) && is_fac_branch(branch) && is_serial_number(serial)){
			return 1;
		}
	}

	return 0;
}

int verify_pass(char* pass) {
	int n = len(pass);
	if (n >= 8 && n < 16) {
		while (*++pass) {
			if (*pass == ' ' || *pass == '\t')
				return 0;
		}
		return 1;
	}
	return 0;
}

void clean(char* s) {
	int i, j = 0;
	for (i = 0; i < len(s); i++) {
		char c = s[i];
		if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
			s[j++] = c;
		}
	}
	s[j] = '\0';
}

#endif
