#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "1.0.3"

void pause(void) {

	// HACK: so screen doesn't disappear when finished debugging :)
	//system("pause");
}

int main(int argc, char **argv) {

	int exit = 0, askedn = 0, validn = 0, i = 0;

	atexit(pause);

	printf(
		"Mental Arithmetic Tester in C - " VERSION "\n\n"
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
		printf(
			"New question!\n\n"
			"If it's invalid, type: i\n\n"
			"  %s\n\n", eq);

		printf("Value of x to 2 dp? ");
		scanf("%s", in);

		askedn++;

		printf("\n--------------------\n");
		ret = calcans(eq, ans, &isfloat);

		if (ret != 0) {

			printf("error: calcans fail: %i\n\n", ret);

			if (strchr(in, 'i')) {
				validn++;
				printf("Correct, this equation is invalid!\n");
			} else {
				printf("Sorry, you typed %s but this equation is invalid.\n", in);
			}

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
				printf("\n%s is correct!\n", in);
			} else {
				printf("\nSorry, %s is wrong. The answer is actually %s\n", in, ans);
			}
		}
			
		rate = ((float)validn / (float)askedn) * 100.0;
		printf("Accuracy so far: %.2f%%\n\n", rate);

		// TODO: use something other than pause
		// this is a nasty little hack to replace getchar/getch as they 
		// arent working correctly :(
		system("pause");

		printf("\n====================\n\n");
	}

	return 0;
}