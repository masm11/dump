#include <stdio.h>
#include <sys/types.h>

static void dump(FILE *fp);
static void puthex2(u_long n);
static void puthex8(u_long n);

int main(int argc, char **argv)
{
    int ne = 0;
    if (argc > 1) {
	int i;
	for (i = 1; i < argc; i++) {
	    FILE *fp = NULL;
	    printf("# %s\n", argv[i]);
	    if (argv[i][0] == '-' && argv[i][1] == '\0')
		fp = stdin;
	    else {
		if ((fp = fopen(argv[i], "rb")) == NULL)
		    fp = fopen(argv[i], "r");
	    }
	    if (fp != NULL)
		dump(fp);
	    else {
		perror(argv[i]);
		ne++;
	    }
	}
    } else {
	dump(stdin);
    }
    return ne;
}

static char hexchar[16] = "0123456789abcdef";
static void puthex2(u_long n)
{
    putchar(hexchar[(n >> 4) & 0xf]);
    putchar(hexchar[(n >> 0) & 0xf]);
}
static void puthex8(u_long n)
{
    putchar(hexchar[(n >> 28) & 0xf]);
    putchar(hexchar[(n >> 24) & 0xf]);
    putchar(hexchar[(n >> 20) & 0xf]);
    putchar(hexchar[(n >> 16) & 0xf]);
    putchar(hexchar[(n >> 12) & 0xf]);
    putchar(hexchar[(n >>  8) & 0xf]);
    putchar(hexchar[(n >>  4) & 0xf]);
    putchar(hexchar[(n >>  0) & 0xf]);
}

static void dump(FILE *fp)
{
    u_long pos;
    u_char buf[16];
    int i, len;
    
    pos = 0;
    while (1) {
	len = fread(buf, 1, 16, fp);
	if (len == 0)
	    break;
	puthex8(pos);
	putchar(' ');
	for (i = 0; i < len; i++) {
	    if ((i & 7) == 0)
		putchar(' ');
	    puthex2(buf[i]);
	    putchar(' ');
	}
	for ( ; i < 16; i++) {
	    if ((i & 7) == 0)
		fputs("    ", stdout);
	    else
		fputs("   ", stdout);
	}
	putchar(' ');
	for (i = 0; i < len; i++) {
	    if (buf[i] < 0x20 || buf[i] > 0x7e)
		putchar('.');
	    else
		putchar(buf[i]);
	}
	pos += len;
	putchar('\n');
    }
}

/*EOF*/
