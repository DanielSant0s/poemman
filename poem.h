#include "utils.h"

typedef struct {
  char title[256];  // title -> string(fixed size - 256 bytes)
  char author[128]; // author -> string(fixed size - 128 bytes)
  struct tm date;   // date -> tm(datetime structure)
  size_t len;       // len -> size_t
  char *poem;       // poem -> string(variable size, based on len)
} Poem;

FILE *open_registry(const char *path) {
  size_t def = 0;
  FILE *fp = fopen(path, "rb+");

  if (fp == NULL)
    fp = fopen(path, "wb+");

  return fp;
}

size_t get_poem_quantity(FILE *db) {
  size_t num_poem = 0;

  size_t reg_size = fsize(db);
  size_t pos = 0;

  size_t tmp_seek;

  if (reg_size > 0) {
    while (reg_size > pos) {
      fseek(db, sizeof(Poem) - (sizeof(size_t) + sizeof(char *)), SEEK_CUR);
      fread(&tmp_seek, sizeof(size_t), 1, db);
      fseek(db, tmp_seek, SEEK_CUR);
      pos = ftell(db);
      num_poem++;
    }

    fseek(db, 0, SEEK_SET);
  }

  return num_poem;
}

Poem *read_poems(FILE *db, size_t num) {
  Poem *poems = malloc(num * sizeof(Poem));
  for (size_t i = 0; i < num; i++) {
    fread(&poems[i].title, sizeof(char), 256, db);
    fread(&poems[i].author, sizeof(char), 128, db);
    fread(&poems[i].date, sizeof(struct tm), 1, db);
    fread(&poems[i].len, sizeof(size_t), 1, db);

    char *tmp_str = malloc(poems[i].len);
    fread(tmp_str, sizeof(char), poems[i].len, db);
    poems[i].poem = tmp_str;
  }

  fseek(db, 0, SEEK_SET);

  return poems;
}

Poem *add_poem(Poem *poems, size_t *num) {
  char *tmp_date;
  size_t old_size = (*num * sizeof(Poem));
  Poem *new_poems = realloc(poems, old_size + sizeof(Poem));

  memset(new_poems[*num].title, 0, 256);
  memset(new_poems[*num].author, 0, 128);

  getchar();
  printf("Insira o título do poema: \n");
  strcpy(new_poems[*num].title, input('\n'));

  printf("Insira o autor do poema: \n");
  strcpy(new_poems[*num].author, input('\n'));

  printf("Insira a data de publicação do poema(Ex.: 01/01/1900): \n");
  tmp_date = input('\n');
  strptime(tmp_date, "%d/%m/%Y", &new_poems[*num].date);
  tmp_date = "01/01/1900";

  printf("Digite o poema a ser adicionado(você pode pular linhas, com "
         "'return/enter', finalize com ;): \n");
  new_poems[*num].poem = input(';');

  new_poems[*num].len = strlen(new_poems[*num].poem);

  *num = *num + 1;

  return new_poems;
}

void list_poems(Poem *poems, size_t num) {
  char tmp_datum[80];

  printf("Poemas registrados:\n");
  for (size_t i = 0; i < num; i++) {
    strftime(tmp_datum, 80, "%d/%m/%Y", &poems[i].date);
    printf("%d - %s | %s | %s\n", i + 1, poems[i].title, poems[i].author,
           tmp_datum);
  }
}

void write_poems(Poem *poems, size_t num, FILE *db) {
  for (size_t i = 0; i < num; i++) {
    fwrite(poems[i].title, sizeof(char), 256, db);
    fwrite(poems[i].author, sizeof(char), 128, db);
    fwrite(&poems[i].date, sizeof(struct tm), 1, db);
    fwrite(&poems[i].len, sizeof(size_t), 1, db);
    fwrite(poems[i].poem, sizeof(char), poems[i].len, db);
  }
}

void edit_poem(Poem *poem, size_t i) {
  char option, tmp_buffer[80];

  do {
    printf("Selecione o atributo a ser editado:\n");
    printf("1 - Título\n");
    printf("2 - Autor\n");
    printf("3 - Data de publicação\n");
    printf("4 - Texto do poema\n");
    printf("0 - Voltar\n\n");

    printf("Opção: ");
    scanf(" %c", &option);
    getchar();

    switch (option) {
    case '1':
      printf("Título atual: %s\n\n", poem[i].title);
      printf("Insira o novo título: \n");
      memset(poem[i].title, 0, 256);
      strcpy(poem[i].title, input('\n'));
      break;

    case '2':
      printf("Autor atual: %s\n\n", poem[i].author);
      printf("Insira o novo autor: \n");
      memset(poem[i].author, 0, 128);
      strcpy(poem[i].author, input('\n'));
      break;

    case '3':
      strftime(tmp_buffer, 80, "%d/%m/%Y", &poem[i].date);
      printf("Data atual: %s\n\n", tmp_buffer);
      printf("Insira a nova data(Ex.: 01/01/1900): \n");
      strptime(input('\n'), "%d/%m/%Y", &poem[i].date);
      break;

    case '4':
      printf("Texto do poema: \n\n%s\n\n", poem[i].poem);
      printf("Digite o novo texto do poema(você pode pular linhas, com "
             "'return/enter', finalize com ;): \n");
      poem[i].poem = input(';');
      poem[i].len = strlen(poem[i].poem);
      break;
    }

  } while (option != '0');
}

size_t find_poem(Poem *poems, size_t num, const char *title) {
  for (size_t i = 0; i < num; i++) {
    if (strcmp(title, poems[i].title) == 0) {
      return i;
    }
  }

  return NULL;
}

Poem *remove_poem(Poem *poems, size_t i, size_t *num) {
  Poem *new_poems = malloc(sizeof(Poem) * (*num - 1));

  size_t poem_ds = (i * sizeof(Poem));
  memcpy(new_poems, poems, poem_ds);
  memcpy(new_poems + poem_ds, poems + poem_ds, (*num - i) * sizeof(Poem));
  free(poems);

  return new_poems;
}