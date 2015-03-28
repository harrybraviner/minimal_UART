#include <stdio.h>

int main(int argc, char *argv[]){
	FILE *UART = fopen("/dev/ttyUSB0", "w");

	fprintf(UART, "abcd\n");

	fclose(UART);

	return 0;
}
