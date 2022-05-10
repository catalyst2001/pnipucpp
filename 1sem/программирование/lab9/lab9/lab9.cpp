//lab9.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char text[] =
R"(Computer science is the study of algorithmic processes, computational machines and computation itself.
As a discipline, computer science spans a range of topics from theoretical studies of algorithms, computation and information to
the practical issues of implementing computational systems in hardware and software.
Its fields can be divided into theoretical and practical disciplines. For example, the theory of computation concerns abstract
models of computation and general classes of problems that can be solved using them, while computer graphics or computational
geometry emphasize more specific applications. Algorithms and data structures have been called the heart of computer science.
Programming language theory considers approaches to the description of computational processes, while computer programming involves
the use of them to create complex systems. Computer architecture describes construction of computer components and computer-operated
equipment. Artificial intelligence aims to synthesize goal-orientated processes such as problem-solving, decision-making,
environmental adaptation, planning and learning found in humans and animals. A digital computer is capable of simulating various
information processes. The fundamental concern of computer science is determining what can and cannot be automated.
Computer scientists usually focus on academic research. The Turing Award is generally recognized as the highest distinction in
computer sciences.)";

void create_file_with_text(const char *filename)
{
	FILE *fp = fopen(filename, "wt");
	if (!fp) {
		printf("Failed to open file!\n");
		return;
	}
	fputs(text, fp);
	fclose(fp);
}

int wordlen(const char *str)
{
	int len = 0;
	while (*str) {
		if (!ispunct(*str))
			len++;

		str++;
	}
	return len;
}

int main()
{
	create_file_with_text("F1.txt");

	FILE *fpfrom, *fpto;
	if (!(fpfrom = fopen("F1.txt", "rt"))) {
		printf("Failed open file F1.txt!\n");
		return 1;
	}

	if(!(fpto = fopen("F2.txt", "w+"))) {
		printf("Failed open file F2.txt!\n");
		return 1;
	}

	int lineidx = 1;
	char buffer[512];
	while (!feof(fpfrom) && !ferror(fpfrom)) {
		fgets(buffer, sizeof(buffer), fpfrom);
		if (lineidx >= 4)
			fputs(buffer, fpto);

		lineidx++;
	}
	fclose(fpfrom);

	rewind(fpto);
	while (!feof(fpto) && !ferror(fpto))
		fgets(buffer, sizeof(buffer), fpto);

	int lastword_len;
	char *ptok = strtok(buffer, " ");
	while (ptok) {
		lastword_len = wordlen(ptok);
		ptok = strtok(NULL, " ");
	}

	printf("%d\n", lastword_len);
	fclose(fpto);
	return 0;
}