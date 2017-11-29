#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *keyboard_rows[] = {
	"qwertyuiop",
	"asdfghjkl",
	"zxcvbnm",
	NULL,
};

char **findWords(char **words, int words_size, int* return_size)
{
	char **output = malloc(words_size * sizeof(char*));
	int n = 0;
	for (int i = 0; i < words_size; i++) {
		int used_rows = 0;
		for (char *p = words[i]; *p; p++) {
			for (int row = 0; keyboard_rows[row]; row++) {
				if (strchr(keyboard_rows[row], tolower(*p))) {
					used_rows |= (1 << row);
				}
			}
		}
		if (used_rows == 1 || used_rows == 2 || used_rows == 4) {
			output[n++] = words[i];
		}
	}
	*return_size = n;
	return output;
}

int main()
{
	char *input[] = {"Hello", "Alaska", "Dad", "Peace"};
	int n;
	char **output = findWords(input, 4, &n);
	printf("%d:", n);
	for (int i = 0; i < n; i++) {
		printf(" \"%s\"", output[i]);
	}
	putchar('\n');
	free(output);
	return 0;
}
