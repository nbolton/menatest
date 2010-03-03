#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

enum {
	TYPE_INT,
	TYPE_FLOAT
};

// operators in order of precedence
char ops1[] = { '*', '/', '%' }; // priority 3 (left to right grouping)
char ops2[] = { '+', '-' }; // priority 4 (left to right grouping)

// string print random type
void sprintrt(char *c, uint t, uint frange, uint drange) {

	switch (t) {
		case TYPE_INT:
			sprintf(c, "%i", frange - rand() % (frange * 2));
			break;

		case TYPE_FLOAT:
			sprintf(c, "%i.%i", frange - rand() % (frange * 2), rand() % drange);
			break;
	}
}

// generate equation
void geneq(char *eq, uint opn) {

	uint i;
	time_t t;
	char part[10];
	uint type = 2, frange = 5, drange = 4;

	srand((uint)time(&t));

	// create a random type for x
	sprintf(eq, "%s x = ", rand() % 2 ? "int" : "float");

	for (i = 0; i < opn; i++) {

		char op; // operator

		// get a random operator from either group
		if (rand() % 2) {
			op = ops1[rand() % sizeof(ops1)];
		} else {
			op = ops2[rand() % sizeof(ops2)];
		}

		// string print first digit
		sprintrt(part, rand() % type, frange, drange);

		// append this part of the equation
		strcat(eq, part);

		// append the operator
		sprintf(part, " %c ", op);
		strcat(eq, part);
	}

	// append another int or float
	sprintrt(part, rand() % type, frange, drange);
	strcat(eq, part);
}

void freearr(char ***arr, uint len) {

	uint i;
	for (i = 0; i < len; i++) {
		free((*arr)[i]);
	}

	free(*arr);
}

// returns pointer to char if . found, otherwise null
const char *strfloat(const char *c) {

	return strchr(c, '.');
}

int procops(const char *ops, uint oplen, char **tokarr, uint tokidx, uint tokc, uint *ftokc, uint ftokt) {

	uint i, j = tokidx;

	for (i = 0; i < oplen; i++) {

		const char op = ops[i];

		if (*ftokc == ftokt) {
			// nothing left to eval, finished
			break;
		}

		// only evaluate 1-char strings which are operators
		if (strlen(tokarr[j]) == 1 && strchr(tokarr[j], op)) {
			
			char res[100]; // char result (we don't know how big this will be!)
			float fres = 0; // float result
			int ires = 0; // int result
			char *lv, *rv; // left/right operand strings
			int lfloat = 0, rfloat = 0; // is left/right float?
			float lfv = 0, rfv = 0; // left/right operand float values
			int liv = 0, riv = 0; // left/right operand int values
			uint k; // loop counters
			uint lvidx = j, rvidx = j; // right/left operand indices

			// ensure operator isn't at start or end of tokens
			if (j == 0 || j == tokc - 1) {
				printf("error: operator at start or end of tokens\n");
				return 1;
			}

			// get the left operand (skipping already-evaluated tokens)
			do {
				if (lvidx - 1 >= 0) {
					lv = tokarr[--lvidx];
				} else {
					printf("error: cannot find left operand\n");
					return 1;
				}
			} while (strlen(lv) == 0);
			
			// get the right operand (skipping already-evaluated tokens)
			do {
				if (rvidx + 1 < tokc) {
					rv = tokarr[++rvidx];
				} else {
					printf("error: cannot find right operand\n");
					return 1;
				}
			} while (strlen(rv) == 0);

			// non-zero if left/right operands are float
			lfloat = strfloat(lv) != NULL;
			rfloat = strfloat(rv) != NULL;

			// parse left operand as float or int
			if (lfloat) {
				sscanf(lv, "%f", &lfv);
			} else {
				sscanf(lv, "%i", &liv);
			}

			// parse right operand as float or int
			if (rfloat) {
				sscanf(rv, "%f", &rfv);
			} else {
				sscanf(rv, "%i", &riv);
			}

			switch (op) {
				case '*':
					if (lfloat && rfloat) {
						fres = lfv * rfv; // float = float * float
					}
					else if (lfloat && !rfloat) {
						fres = lfv * riv; // float = float * int
					}
					else if (!lfloat && rfloat) {
						fres = liv * rfv; // float = int * float
					}
					else {
						ires = liv * riv; // int = int * int
					}
					break;

				case '/':
					if (lfloat && rfloat) {
						if (rfv == 0) {
							printf("error: divide by zero\n");
							return 1;
						}
						fres = lfv / rfv; // float = float / float
					}
					else if (lfloat && !rfloat) {
						if (riv == 0) {
							printf("error: divide by zero\n");
							return 1;
						}
						fres = lfv / riv; // float = float / int
					}
					else if (!lfloat && rfloat) {
						if (rfv == 0) {
							printf("error: divide by zero\n");
							return 1;
						}
						fres = liv / rfv; // float = int / float
					}
					else {
						if (riv == 0) {
							printf("error: divide by zero\n");
							return 1;
						}
						ires = liv / riv; // int = int / int
					}
					break;

				case '%':

					if (!lfloat && !rfloat) {
						if (riv == 0) {
							printf("error: divide by zero? oh dear!\n");
							return 1;
						}
						ires = liv % riv; // int = int % int
					} 
					else {
						printf("error: operands of modulo (%%) must be int\n");
						return 1;
					}
					break;

				case '+':
					if (lfloat && rfloat) {
						fres = lfv + rfv; // float = float + float
					}
					else if (lfloat && !rfloat) {
						fres = lfv + riv; // float = float + int
					}
					else if (!lfloat && rfloat) {
						fres = liv + rfv; // float = int + float
					}
					else {
						ires = liv + riv; // int = int + int
					}
					break;

				case '-':
					if (lfloat && rfloat) {
						fres = lfv - rfv; // float = float - float
					}
					else if (lfloat && !rfloat) {
						fres = lfv - riv; // float = float - int
					}
					else if (!lfloat && rfloat) {
						fres = liv - rfv; // float = int - float
					}
					else {
						ires = liv - riv; // int = int - int
					}
					break;
			}

			// the only case when result is int, is when both 
			// operands are int
			if (!lfloat && !rfloat) {
				sprintf(res, "%i", ires);
			} else {
				sprintf(res, "%f", fres);
			}

			// replace the left operand with the evaluation
			free(lv);
			lv = _strdup(res);

			// replace operator and right operand with valid 
			// empty string
			sprintf(tokarr[j], "");
			sprintf(rv, "");

			// decrement *final* toc count each time we evaluate
			// an operator
			*ftokc -= 2;

			// after each elimination, print the new equation
			printf("\nsimplified equation:\n ");
			for (k = 3; k < tokc; k++) {
				if (strlen(tokarr[k]) != 0) {
					printf("%s ", tokarr[k]);
				}
			}
			printf("\n");
		}
	}

	return 0;
}

int calcans(const char *eq, char *ans, uint *isfloat) {
	
	const uint ftokt = 4; // final toc target is 3 (t x = n)
	uint tokc = 0, i = 0, j, ftokc;
	char **tokarr = 0;
	char *tok;
	int ret;

	printf("\ncalculating...\n\n");

	do {

		// first time pass eq, subisiquent times pass NULL
		tok = strtok(tokc == 0 ? (char *)eq : NULL, " ");
		
		if (tok) {

			// increase the size of the pointer array
			// `char *` (pointer to char) is usually 4 bytes
			tokarr = realloc(tokarr, (tokc + 1) * sizeof(char *));
			if (!tokarr) {
				printf("error: realloc fail\n");
				return 1;
			}
			
			// copy the string and return the memory
			tokarr[tokc++] = _strdup(tok);
		}

	} while (tok);

	// final tok count starts as original tok count
	ftokc = tokc;

	printf("tokens:\n");
	for (i = 0; i < tokc; i++) {
		printf(" %s\n", tokarr[i]);
	}

	// print original equation again for comparison
	printf("\noriginal equation:\n ");
	for (i = 3; i < tokc; i++) {
		if (strlen(tokarr[i]) != 0) {
			printf("%s ", tokarr[i]);
		}
	}
	printf("\n");

	// this loop evaluates each operator until we only have the
	// simplified equation left (t x = n) which is 4 tokens long
	while (ftokc != ftokt) {

		for (j = 0; j < tokc; j++) {

			if (ftokc == ftokt) {
				// nothing left to eval, finished
				break;
			}
			
			ret = procops(ops1, sizeof(ops1), tokarr, j, tokc, &ftokc, ftokt);
			if (ret != 0) {
				freearr(&tokarr, tokc);
				printf("error: procops ops1 fail: %i\n", ret);
				return 1;
			}
		}

		for (j = 0; j < tokc; j++) {

			if (ftokc == ftokt) {
				// nothing left to eval, finished
				break;
			}
			
			ret = procops(ops2, sizeof(ops2), tokarr, j, tokc, &ftokc, ftokt);
			if (ret != 0) {
				freearr(&tokarr, tokc);
				printf("error: procops ops2 fail: %i\n", ret);
				return 1;
			}
		}
	}

	// if the return type is int, then truncate the final value
	// - the 1st token (index 0), which is the type
	// - the 4th token (ftokt - 1), is always the final value
	if (strstr(tokarr[0], "int")) {
		
		float f;
		uint i;

		*isfloat = 0;

		sscanf(tokarr[ftokt - 1], "%f", &f);
		sprintf(tokarr[ftokt - 1], "%i", (int)f);

		printf("\ntruncated answer:\n ");
		for (i = 3; i < tokc; i++) {
			if (strlen(tokarr[i]) != 0) {
				printf("%s ", tokarr[i]);
			}
		}
		printf("\n");

	} else {

		// if not int, assume float
		*isfloat = 1;
	}

	// copy last token to answer buffer
	sprintf(ans, tokarr[ftokt - 1]);

	freearr(&tokarr, tokc);
	return 0;
}