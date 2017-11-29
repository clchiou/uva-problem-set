#include <stdio.h>
#include <stdlib.h>

int is_self_dividing(int x)
{
	for (int digits = x; digits > 0; digits /= 10) {
		int digit = digits % 10;
		if (digit == 0 || x % digit != 0) {
			return 0;
		}
	}
	return 1;
}

/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *selfDividingNumbers(int left, int right, int* returnSize)
{
	int *self_dividing_numbers = malloc((right - left + 1) * sizeof(int));
	int n = 0;
	for (int x = left; x <= right; x++) {
		if (is_self_dividing(x)) {
			self_dividing_numbers[n++] = x;
		}
	}
	*returnSize = n;
	return self_dividing_numbers;
}

int main()
{
	int n = 0;
	int *actual = selfDividingNumbers(1, 22, &n);
	printf("%d:", n);
	for (int i = 0; i < n; i++) {
		printf(" %d", actual[i]);
	}
	putchar('\n');
	free(actual);
	return 0;
}
