#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

typedef struct no No;

No *criarNo();
void inserirChave(No **raiz, int chave);
void imprimirEspacos(int nivel);
void imprimirEmOrdem(No *raiz, int nivel);
int buscarIndiceChave(No *no, int chave);
void removerChaveFolha(No *no, int indice);
void removerChaveInterna(No *no, int indice);
void mesclarNosFolha(No *esquerdo, No *direito);
void removerChave(No **raiz, int chave);