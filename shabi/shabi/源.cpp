#include <stdio.h>
#include <crtdbg.h>
#include <string.h>

char * toUpper(char * src)
{
	char * c;
	int i;
	char * des = new char[strlen(src)];
	for (i = 0, c = src; (*c) != 0; c++, i++) {
		char temp = *c;
		if ((*c) >= 'a' && (*c) <= 'z') {
			temp = *c - 32;
		//	printf("%c\r\n", *c - 32);
		}
		des[i] = temp;
	}
	return des;
}
int main()
{
	char * src = "aafdJfdRJGNjkF12345";
	printf("hello\r\n");
	printf("%s\r\n", toUpper(src));
	return 0;
}

