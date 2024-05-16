/*
Exercicio 1: Contador de letras
Igor Correa Domingues de Almeida
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define o tipo de dado a ser colocado na lista
typedef struct {
  char letra; // Letra usada, "Outros" será representado com '0'
  int apps;   // Número de aparições da letra
} Tdata;

// tipo de dado para lista encadeada
typedef struct TNode {
  Tdata info;         // dado armazenado no nó
  struct TNode *prev; // ponteiro para o nó anterior
  struct TNode *next; // ponteiro para o próximo nó
} TNode;

typedef struct TListDE { //Descritor da Lista DE
  TNode *first;
  TNode *last;
  short length;
} TListDE;

//Funcao para iniciar a lista
void initList(TListDE *L) {
  L->first = NULL;
  L->last = NULL;
  L->length = 0;
}

//Funcao para deletar Lista
void deleteList(TListDE *L) {
  TNode *p = L->first;
  while (p) {
    L->first = L->first->next;
    free(p);
    p = L->first;
  }
  L->last = NULL;
  L->length = 0;
}

// Inserção pela direita
short insertRight(Tdata x, TListDE *L) {
  TNode *aux = (TNode *)malloc(sizeof(TNode));
  if (aux == NULL) {
    return 1;
  } else {
    aux->info = x;
    aux->next = NULL;    // Não tem próximo
    aux->prev = L->last; // O anterior é o antigo último

    if (L->last == NULL) {      // Lista estava vazia
      L->first = L->last = aux; // atualiza primeiro e último
    } else {                    // atualiza só o último
      L->last->next = aux;      // liga o novo último nó
      L->last = aux;            // atualiza o ponteiro para o novo último
    }
    L->length++; // atualiza o comprimento
    return 0;
  }
}

int main(void) {
  FILE *fIn, *fOut; //Arquivos de entrada e saida
  char filenameIn[31], letra; //Nome do arquivo de entrada e char para operacoes
  int total = 0; //total de caracteres
  TListDE L; //Lista DE
  Tdata st; //Struct para ser inserida na lista
  TNode *aux; //Auxiliar para buscas na lista

  initList(&L); //Inicializar a lista

  //Preencher a lista de A a Z
  for (char c = 'A'; c <= 'Z'; c++) {
    st.letra = c;
    st.apps = 0;
    insertRight(st, &L); //Inserir a direita na lista
  }
  st.letra = '0'; //Preencher o sinal escolhido para Outros
  insertRight(st, &L);

  printf("Escreva o nome do arquivo de entrada:\n"); 
  scanf("%s", filenameIn); //Pedir o nome do arquivo de entrada

  fIn = fopen(filenameIn, "r"); //Abrir o arquivo de entrada em r
  fOut = fopen("out.txt", "w"); //Abrir o arquivo de saida em w

  //Testes de erros
  if (!fIn) {
    printf("Erro: Arquivo %s não pode ser aberto.\n", filenameIn);
    return 1;
  }
  if (!fOut) {
    printf("Erro: Arquivo out.txt não pode ser aberto.\n");
    return 1;
  }

  //Enquanto estiver lendo caracteres, continuar no laco
  while (fscanf(fIn, "%c", &letra) == 1) {
    total++; //Contador do total de letras
    //Se for uma letra
    if ((letra >= 'A' && letra <= 'Z') || (letra >= 'a' && letra <= 'z')) {
      if (letra >= 'a' && letra <= 'z') {
        letra -= 32; // converter minúscula para maiúscula
      }
      aux = L.first;
      int index = letra - 'A'; //Achar a posicao dele na lista sem ter que fazer comparacoes
      for (int i = 0; i < index; i++) {
        aux = aux->next;
      }
      aux->info.apps++; //Incrementar o total de aparicoes do caracter
    } else { //Senao, operar no Outros
      aux = L.last;
      aux->info.apps++;
    }
  }
  //Cabeçalho a ser impresso no arquivo de saida
  fprintf(fOut, "Programa: Contador de letras\n");
  fprintf(fOut, "Arquivo: %s\n\n", filenameIn);
  fprintf(fOut, "Contagem de letras:\n\n");
  fprintf(fOut, "%-7s | %-11s | %-10s\n", "Letras", "Ocorrências",
          "Frequência");
  fprintf(fOut, "--------+-------------+------------\n");

  aux = L.first; //Colocar o auxiliar no inicio da lista
  while (aux) { //Enquando aux é diferente de NULL
    if (aux == L.last) { //No caso do ultimo, usar o Outros
      //Preencher os espaçamento alinhado a esquerda
      fprintf(fOut, "%-7s | %-11d | %6.2lf%%    \n", "Outros", aux->info.apps, (double)aux->info.apps / total * 100);
    } else {
      fprintf(fOut, "%-7c | %-11d | %6.2lf%%    \n", aux->info.letra, aux->info.apps, (double)aux->info.apps / total * 100);
    }
    aux = aux->next; //Ir para o próximo nó
  }
  //Print do final
  fprintf(fOut, "--------+-------------+------------\nTotal   | %-6d car. | 100.00%%\n", total);

  //Fechar os arquivos e excluir a lista
  fclose(fIn);
  fclose(fOut);
  deleteList(&L);

  //Indicador do arquivo da saída
  printf("A saída está em out.txt\n"); 
  return 0;
}

