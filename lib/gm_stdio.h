#pragma once

#include <stddef.h>

typedef struct {
  size_t fid;
} FILE;

struct __sFstub {
  long _stub;
};
typedef long off_t;

extern struct __sFstub __stdin[];
extern struct __sFstub __stdout[];
extern struct __sFstub __stderr[];

#define stdin ((struct __sFILE *)__stdin)
#define stdout ((struct __sFILE *)__stdout)
#define stderr ((struct __sFILE *)__stderr)

#define BUFSIZ 1024

#define EOF (-1)

/*
 * FOPEN_MAX is a minimum maximum, and should be the number of descriptors
 * that the kernel can provide without allocation of a resource that can
 * fail without the process sleeping.  Do not use this for anything.
 */
#define FOPEN_MAX 20
#define FILENAME_MAX 1024

#ifndef SEEK_SET
#define SEEK_SET 0 /* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1 /* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* set file offset to EOF plus offset */
#endif

typedef long int fpos_t;

int dprintf(int n, const char *__restrict s, ...)
// __attribute__((__format__(printf, 2, 3))) __attribute__((__nonnull__(2)))
{
  return 0;
}
int fclose(FILE *f) { return 0; }
int feof(FILE *f) { return 0; }
int ferror(FILE *f) { return 0; }
int fflush(FILE *f) { return 0; }
int fgetc(FILE *f) { return 0; }
int fgetpos(FILE *f, fpos_t *p) { return 0; }
char *fgets(char *c, int n, FILE *f)
    __attribute__((__bounded__(__string__, 1, 2))) {
  return NULL;
}
FILE *fopen(const char *p, const char *m) { return NULL; }
int fprintf(FILE *f, const char *__restrict s, ...) { return 0; }
int fputc(int n, FILE *f) { return 0; }
int fputs(const char *s, FILE *f) { return 0; }
size_t fread(void *v, size_t s, size_t s2, FILE *f)
    __attribute__((__bounded__(__size__, 1, 3, 2))) {
  return 0;
}
FILE *freopen(const char *p, const char *m, FILE *f) { return f; }
int fscanf(FILE *f, const char *s, ...) { return 0; }
int fseek(FILE *f, long n, int i) { return 0; }
int fseeko(FILE *f, off_t o, int n) { return 0; }
int fsetpos(FILE *f, const fpos_t *p) { return 0; }
long ftell(FILE *f) { return 0; }
off_t ftello(FILE *f) { return 0; }
size_t fwrite(const void *v, size_t s, size_t s2, FILE *f)
    __attribute__((__bounded__(__size__, 1, 3, 2))) {
  return 0;
}
int getc(FILE *f) { return '\0'; }
int getchar() { return '\0'; }

int printf(const char *__restrict c, ...) { return 0; }
int putc(int c, FILE *f) { return 0; }
int putchar(int c) { return 0; }
int puts(const char *s) { return 0; }
int remove(const char *s) { return 0; }
int rename(const char *s, const char *t) { return 0; }
void rewind(FILE *f) {};
int scanf(const char *s, ...) { return 0; }
int sprintf(char *__restrict s, const char *__restrict t, ...) { return 0; };

int snprintf(char *__restrict s, size_t n, const char *__restrict f, ...)
    // __attribute__((__format__(printf, 3, 4))) __attribute__((__nonnull__(3)))
    __attribute__((__bounded__(__string__, 1, 2))) {
  return 0;
};

int fileno(FILE *f) { return 0; };

#define getchar() getc(stdin)
#define putchar(x) putc(x, stdout)
#define getchar_unlocked() getc_unlocked(stdin)
#define putchar_unlocked(c) putc_unlocked(c, stdout)
