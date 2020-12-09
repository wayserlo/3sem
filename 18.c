#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <function(x)>\n", argv[0]);
		return 1;
	}
	// Prepare source file
	FILE * f = fopen("tmp.c", "w");
	fprintf(f,
		"#include <math.h>\n"
		double tmpfun(double x) {\n"
		"	return %s;\n"
		"}\n",
		argv[1]
	);
	fclose(f);
	\\ Call gcc
	pid_t my_id;
	if ((my_id = fork()) == 0) {
		execlp(
			//executable
			"gcc",
			//argv
			"gcc", "-Wall", "-Wextra", 
