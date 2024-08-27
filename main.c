/**
 * https://stackoverflow.com/questions/1264137/how-to-copy-string-to-clipboard-in-c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define IS_OS_WINDOWS 1
/**
 *  Copies the given string into the clipboard
 */
void copy_to_clipboard(const char *str)
{
	const size_t len = strlen(str) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), str, len);
	GlobalUnlock(hMem);
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		if (SetClipboardData(CF_TEXT, hMem) == NULL)
			printf("Failed to copy to the clipboard.\n");
		else
			printf("Password successfully copied to clipboard.\n");
		CloseClipboard();
	}
	else
	{
		printf("Failed to open the clipboard.\n");
	}
}

#else
#define IS_OS_WINDOWS 0
#endif

int gen_random_number(int min, int max);
void copy_to_clipboard(const char *str);

int main(int argc, char const *argv[])
{

	srand((unsigned int)time(NULL));

	const char ASCII_LOWERCASES[27] = "abcdefghijklmnopqrstuvwxyz";
	const char ASCII_UPPERCASES[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char ASCII_DIGITS[11] = "0123456789";
	const char ASCII_SYMBOLS[33] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

	const int UPPERCASES_COUNT = gen_random_number(3, 5);
	const int DIGITS_COUNT = gen_random_number(3, 5);
	const int SYMBOLS_COUNT = gen_random_number(3, 5);
	const int LOWERCASES_COUNT = 16 - (UPPERCASES_COUNT + DIGITS_COUNT + SYMBOLS_COUNT);

	char *lowercases = (char *)malloc((LOWERCASES_COUNT + 1) * sizeof(char));
	char *uppercases = (char *)malloc((UPPERCASES_COUNT + 1) * sizeof(char));
	char *digits = (char *)malloc((DIGITS_COUNT + 1) * sizeof(char));
	char *symbols = (char *)malloc((SYMBOLS_COUNT + 1) * sizeof(char));
	char *password = (char *)malloc((16 + 1) * sizeof(char));

	if (!lowercases || !uppercases || !digits || !symbols || !password)
	{
		printf("Unable to allocate memory. Exiting...\n");
		exit(1);
	}

	// Adding a null terminator
	lowercases[LOWERCASES_COUNT] = '\0';
	uppercases[UPPERCASES_COUNT] = '\0';
	digits[DIGITS_COUNT] = '\0';
	symbols[SYMBOLS_COUNT] = '\0';
	password[16] = '\0';

	for (int i = 0; i < LOWERCASES_COUNT; i++)
		*(lowercases + i) = ASCII_LOWERCASES[rand() % (sizeof(ASCII_LOWERCASES) - 1)];

	for (int i = 0; i < UPPERCASES_COUNT; i++)
		*(uppercases + i) = ASCII_UPPERCASES[rand() % (sizeof(ASCII_UPPERCASES) - 1)];

	for (int i = 0; i < DIGITS_COUNT; i++)
		*(digits + i) = ASCII_DIGITS[rand() % (sizeof(ASCII_DIGITS) - 1)];

	for (int i = 0; i < SYMBOLS_COUNT; i++)
		*(symbols + i) = ASCII_SYMBOLS[rand() % (sizeof(ASCII_SYMBOLS) - 1)];

	int lowercases_indicator = 0;
	int uppercases_indicator = 0;
	int digits_indicator = 0;
	int symbols_indicator = 0;

	for (int i = 0; i < 16; i++)
	{
		int category = rand() % 4;
		switch (category)
		{
		case 0:
			if (lowercases_indicator < LOWERCASES_COUNT)
			{
				*(password + i) = *(lowercases + lowercases_indicator++);
			}
			else
			{
				i--;
				continue;
			}
			break;
		case 1:
			if (uppercases_indicator < UPPERCASES_COUNT)
			{
				*(password + i) = *(uppercases + uppercases_indicator++);
			}
			else
			{
				i--;
				continue;
			}
			break;
		case 2:
			if (digits_indicator < DIGITS_COUNT)
			{
				*(password + i) = *(digits + digits_indicator++);
			}
			else
			{
				i--;
				continue;
			}
			break;
		case 3:
			if (symbols_indicator < SYMBOLS_COUNT)
			{
				*(password + i) = *(symbols + symbols_indicator++);
			}
			else
			{
				i--;
				continue;
			}
			break;
		}
	}

	/*
	// for debugging
	printf("%d\t%d\t%d\t%d\n", LOWERCASES_COUNT, UPPERCASES_COUNT, DIGITS_COUNT, SYMBOLS_COUNT);
	printf("%s\n", lowercases);
	printf("%s\n", uppercases);
	printf("%s\n", digits);
	printf("%s\n", symbols);
	printf("%s\n", password);
	*/

	printf("Your random generated password is: \n%s\n", password);

	// Copy to clipboard (Windows only)
	if (IS_OS_WINDOWS)
	{
		printf("\nIf you wish to copy the password to the clipboard, type c and press enter.\nOtherwise type any other button to exit.\n");
		int ch = fgetc(stdin);
		if (ch == 'c' || ch == 'C')
			copy_to_clipboard(password);
	}

	free(lowercases);
	free(uppercases);
	free(digits);
	free(symbols);
	free(password);

	return 0;
}

/**
 * Generates a random number in the given range
 */
int gen_random_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
