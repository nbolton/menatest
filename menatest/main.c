#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pause(void) {

	// HACK: so screen doesn't disappear when finished debugging :)
	system("pause");
}

int main(int argc, char **argv) {
	int exit = 0, askedn = 0, validn = 0, i = 0;

	atexit(pause);

	printf(
		"Mental Arithmetic Tester in C\n\n"
		"Tips:\n"
		"- A calculator might be helpful.\n"
		"- Remember operator precedence and truncation.\n"
		"- If you find a bug, please report or patch it.\n"
		"- Good luck, have fun! :)\n\n");

	while (!exit) {
		
		char in[10];
		char eq[100];
		double rate = 0;
		char ans[100];
		unsigned int isfloat, isvalid, isclose = 0, ret;
		float fans;

		geneq(eq, 2);
		printf("New question!\n\n%s\n\n", eq);

		printf("Value of x to 2 dp? ");
		scanf("%s", in);

		askedn++;

		printf("\n--------------------\n");
		if (ret = calcans(eq, in, ans, &isfloat) != 0) {
			printf("error: checkans fail: %i\n", ret);
			printf("\nHmm, couldn't figure that one out.\n");

		} else {

			// shorten to 2 decimal places so answer isn't absurdly difficult
			// maybe this is a hack...? maybe calcans should do this.
			if (isfloat) {
				sscanf(ans, "%f", &fans);
				sprintf(ans, "%.2f", fans);
			}

			// is our answer *exactly* the same as the user's?
			isvalid = strcmp(ans, in) == 0;

			// maybe the user was close? i.e. didn't end with .00 or .n0
			if (isfloat && !isvalid) {
				if (strstr(ans, ".00")) {
					// check the answer, excluding the ".00" at the end
					isclose = strncmp(in, ans, strlen(ans) - 3) == 0;
				} else if (ans[strlen(ans) - 1] == '0') {
					// check the answer, excluding the "0" at the end
					isclose = strncmp(in, ans, strlen(ans) - 2) == 0;
				}
			}

			printf("\n--------------------\n");
			if (isvalid || isclose) {
				validn++;
				printf("\n%s is correct", in);
				if (isclose) {
					// is this really necessary? ;)
					printf(", however %s is more accurate.", ans);
				} else {
					printf("!\n");
				}
			} else {
				printf("\nSorry, %s is wrong. The answer is actually %s\n", in, ans);
			}
			
			rate = ((float)validn / (float)askedn) * 100.0;
			printf("Accuracy so far: %.2f%\n", rate);
		}

		printf("\n====================\n\n");
	}

	return 0;
}