#include "poem.h"

int main(void) {
  char option, *tmp_input;
  size_t idx;

  FILE *poemdb = open_registry("poem_db.podb");

  size_t poem_qt = get_poem_quantity(poemdb);

  Poem *poems = read_poems(poemdb, poem_qt);

  do {
    printf("Bem-vindo ao banco de dados de poemas!\n");
    printf("%ld poemas foram encontrados no banco de dados.\n\n", poem_qt);
    printf("Selecione uma opção abaixo para prosseguir:\n");
    printf("1 - Registrar poema\n");
    printf("2 - Listar poemas\n");
    printf("3 - Editar poema\n");
    printf("4 - Deletar poema\n");
    printf("0 - Sair\n\n");

    printf("Opção: ");

    scanf(" %c", &option);

    printf("\n");

    switch (option) {
    case '1':
      poems = add_poem(poems, &poem_qt);
      break;
    case '2':
      list_poems(poems, poem_qt);
      break;
    case '3':
      printf("Selecione o poema pelo índice ou título: ");
      getchar();
      tmp_input = input('\n');

      if (atoul(tmp_input, &idx) == 1) {
        idx = find_poem(poems, poem_qt, tmp_input) + 1;
      }

      if (idx <= poem_qt && idx > 0) {
        edit_poem(poems, idx - 1);
      } else {
        printf("Erro! O poema fornecido não existe.\n");
      }
      break;
    case '4':
      printf("Selecione o poema pelo índice ou título: ");
      getchar();
      tmp_input = input('\n');

      if (atoul(tmp_input, &idx) == 1) {
        idx = find_poem(poems, poem_qt, tmp_input) + 1;
      }

      if (idx <= poem_qt && idx > 0) {
        poems = remove_poem(poems, idx, &poem_qt);
      } else {
        printf("Erro! O poema fornecido não existe.\n");
      }
      break;
    }

    printf("\n");

  } while (option != '0');

  write_poems(poems, poem_qt, poemdb);

  fclose(poemdb);

  return 0;
}