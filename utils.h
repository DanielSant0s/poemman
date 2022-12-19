#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *input(char cancel) {
  unsigned int len_max = 128;
  unsigned int current_size = 0;

  char *pStr = malloc(len_max);
  current_size = len_max;

  if (pStr != NULL) {
    int c = EOF;
    unsigned int i = 0;
    // accept user input until hit enter or end of file
    while ((c = getchar()) != cancel && c != EOF) {
      pStr[i++] = (char)c;

      // if i reached maximize size then realloc size
      if (i == current_size) {
        current_size = i + len_max;
        pStr = realloc(pStr, current_size);
      }
    }

    pStr[i] = '\0';
  }
  return pStr;
}

int atoul(char *in, unsigned long *out) {
  char *p;

  for (p = in; *p; p++)
    if (*p > '9' || *p < '0')
      return 1;

  *out = strtoul(in, NULL, 10);
  return 0;
}

size_t fsize(FILE *fp) {
  size_t size;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  return size;
}