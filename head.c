#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


void write_chars(FILE *, int);
void write_lines(FILE *, int);

int main(int argc, char *argv[])
{
	int result;
	char *c_arg, *n_arg;
	int i;
	int err_flag;
	int c_flag, n_flag, v_flag, help_flag, version_flag, index;
	int nlines, nbytes;
	FILE *f; 

	const struct option options[] = {

		{"bytes", required_argument, NULL, 'c'},
		{"lines", required_argument, NULL, 'n'},
		{"verbose", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 1},
		{"version", no_argument, NULL, 2},
		{0, 0, 0, 0}
	};

	c_flag = n_flag = v_flag = help_flag = version_flag = 0;
	opterr = 0;
	err_flag = 0;

	while ((result = getopt_long(argc, argv, "vc:n:", options, &index)) != -1)
	{
		switch (result)
		{
		case 'c':
		{
			c_flag = 1;
			c_arg = optarg;
			break;
		}

		case 'n':
		{
			n_flag = 1;
			n_arg = optarg;
			break;
		}

		case 'v':
		{
			v_flag = 1;
			break;
		}
		case  1:
		{
			help_flag = 1;
			break;
		}
		case 2:
		{
			version_flag = 1;
			break;
		}
		case '?':
		{
			if (optopt == 'c')
				fprintf(stderr, "-c or --bytes option must have an argument!..\n");
			else if (optopt == 'n')
				fprintf(stderr, "-n or --lines option must have an argument..\n");
			else if (optopt != 0)
				fprintf(stderr, "invalid short option: -%c\n", optopt);
			else
				fprintf(stderr, "invalid long option!..\n");
			err_flag = 1;
			break;
		}
		default:
			break;
		}
	}

	if (err_flag)
		exit(EXIT_FAILURE);


	if (help_flag)
	{
		if (n_flag || v_flag || c_flag || version_flag == 1)
		{
			fprintf(stderr, "help option must not be used with other options!..\n");
			exit(EXIT_FAILURE);
		}
		printf("Usage: head [OPTION]... [FILE]...\n");
		exit(EXIT_FAILURE);
	}
	
	if (version_flag)
	{
		if (n_flag || v_flag || c_flag || help_flag == 1)
		{
			fprintf(stderr, "version option must not be used with other options!..\n");
			exit(EXIT_FAILURE);
		}
		printf("1.0..\n");
		exit(EXIT_FAILURE);
	}

	if (n_flag & c_flag)
	{
		fprintf(stderr, "-n (--lines) and -c (--bytes) options must not be used together!..\n");
		exit(EXIT_FAILURE);
	}


	if (n_flag)
	{
		nlines = (int)strtol(n_arg, NULL, 10);
		if (!nlines)
		{
			fprintf(stderr, "invalid number of lines!..\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (c_flag)
	{
		nbytes = (int)strtol(c_arg, NULL, 10);
		if (!nbytes)
		{
			fprintf(stderr, "invalid number of chars!..\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		n_flag = 1;
		nlines = 10;  // since just file name is typed. Ex: ./sample test.txt
	}

	if (optind == argc)
	{
		fprintf(stderr, "at least one file is necessary!..\n");
		exit(EXIT_FAILURE);
	}

	if (argc - optind > 1)
	{
		v_flag = 1;
	}




	for (i = optind; i<argc; i++)
	{
		if ((f = fopen(argv[i], "r")) == NULL)
		{
			fprintf(stderr, "%s file cannot be opened file!..\n", argv[i]);
			continue;
		}
		if (v_flag)
		{
			printf("==> %s <==\n", argv[i]);
		}

		if (n_flag)
		{
			write_lines(f, nlines);
		}
		if (c_flag)
		{
			write_chars(f, nbytes);
		}
		if (i != argc-1)
			printf("\n");

	}

	return 0;
}

void write_lines(FILE *f, int nlines)
{
	char c;
	int n = 0;

	while((c= fgetc(f)) != EOF && n < nlines)
	{
		putchar(c);
		if (c == '\n')
		{
			n++;
		}
	}

	if (ferror(f))
	{
		fprintf(stderr, "cannot read file!..\n");
	}
}

void write_chars(FILE *f, int nchars)
{
	char c;
	int n = 0;

	while((c= fgetc(f)) != EOF && n < nchars)
	{
		putchar(c);
		n++;
	}

	if (ferror(f))
	{
		fprintf(stderr, "cannot read file!..\n");
	}
}
