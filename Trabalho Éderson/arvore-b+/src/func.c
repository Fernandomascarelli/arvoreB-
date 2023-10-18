#include "func.h" 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// estrutura nó arvore b+
struct no
{
    int *chaves;           // chaves
    struct no **ponteiros; // ponteiros
    struct no *prox;       // nós próximos
    int teste_folha;       // verificação de folha
    int num_chaves;        // quantidade atual de chaves no nó
};

// criar nó
No *criarNo()
{
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->chaves = (int *)malloc(4 * sizeof(int));     // máximo de chaves = ordem
    novoNo->ponteiros = (No **)malloc(4 * sizeof(No *)); // ordem de 4
    memset(novoNo->ponteiros, 0, 4 * sizeof(No *));      // inicializar ponteiros com NULL
    novoNo->prox = NULL;
    novoNo->teste_folha = 0;
    novoNo->num_chaves = 0;
    return novoNo;
}

// função inserção de chave
void inserirChave(No **raiz, int chave)
{
    // se árvore vazia
    if (*raiz == NULL)
    {
        *raiz = criarNo();
        (*raiz)->chaves[0] = chave;
        (*raiz)->num_chaves++;
        (*raiz)->teste_folha = 1;
    }
    else
    {
        // indica na folha onde colocar a nova chave
        No *noFolha = *raiz;
        while (!noFolha->teste_folha)
        {
            int i = 0;
            while (i < noFolha->num_chaves)
            {
                if (chave < noFolha->chaves[i])
                    break;
                i++;
            }
            noFolha = noFolha->ponteiros[i];
        }

        // verifica espaço na folha
        if (noFolha->num_chaves < 3)
        { // Alterado para 3 (máximo de chaves em um nó folha)
            int i = 0;
            while (i < noFolha->num_chaves && chave > noFolha->chaves[i])
                i++;

            // mover para a direita
            for (int j = noFolha->num_chaves; j > i; j--)
            {
                noFolha->chaves[j] = noFolha->chaves[j - 1];
            }

            // reorganizar chave
            noFolha->chaves[i] = chave;
            noFolha->num_chaves++;
        }
        else
        {
            // folha cheia = split
            No *novoNoFolha = criarNo();
            int chaveAux[4];

            // copiar para o auxiliar
            for (int i = 0; i < 3; i++)
            { // Alterado para 3 (máximo de chaves em um nó folha)
                chaveAux[i] = noFolha->chaves[i];
            }

            int i = 0;
            while (i < 3 && chave > chaveAux[i])
                i++;

            // mover para a direita
            for (int j = 4; j > i; j--)
            { // Alterado para 3 (máximo de chaves em um nó folha)
                chaveAux[j] = chaveAux[j - 1];
            }

            // reorganizar chave
            chaveAux[i] = chave;

            // atualizar chaves nos nós
            noFolha->num_chaves = 2;
            novoNoFolha->num_chaves = 2;

            // atualizar chaves e ponteiros
            for (int i = 0; i < 3; i++)
            {
                noFolha->chaves[i] = chaveAux[i];
                novoNoFolha->chaves[i] = chaveAux[i + 2];
                noFolha->ponteiros[i + 1] = novoNoFolha;
            }
            noFolha->ponteiros[noFolha->num_chaves] = novoNoFolha;

            // se nó atual = raiz
            if (noFolha == *raiz)
            {
                No *novaRaiz = criarNo();
                novaRaiz->chaves[0] = chaveAux[2];
                novaRaiz->ponteiros[0] = noFolha;
                novaRaiz->ponteiros[1] = novoNoFolha;
                novaRaiz->num_chaves++;
                novaRaiz->teste_folha = 0;
                *raiz = novaRaiz;
            }
            else
            {
                // inserir chave para o pai
                inserirChave(&(noFolha->prox), chaveAux[2]);
            }
        }
    }
}

// so de bonito
void imprimirEspacos(int nivel)
{
    for (int i = 0; i < nivel; i++)
    {
        printf("  ");
    }
}

// imprimir em ordem com nível
void imprimirEmOrdem(No *raiz, int nivel)
{
    if (raiz != NULL)
    {
        if (raiz->teste_folha)
        {
            for (int i = 0; i < raiz->num_chaves; i++)
            {
                imprimirEspacos(nivel);
                printf("| %d - NIVEL %d |\n", raiz->chaves[i], nivel);
            }
        }
        else
        {
            for (int i = 0; i < raiz->num_chaves; i++)
            {
                imprimirEmOrdem(raiz->ponteiros[i], nivel + 1);
                imprimirEspacos(nivel);
                printf("| %d - NIVEL %d |\n", raiz->chaves[i], nivel);
            }
            imprimirEmOrdem(raiz->ponteiros[raiz->num_chaves], nivel + 1);
        }
    }
}

// func aux buscar indice
int buscarIndiceChave(No *no, int chave)
{
    int indice = 0;
    while (indice < no->num_chaves && chave > no->chaves[indice])
    {
        indice++;
    }
    return indice;
}

// func aux remocao chave no folha
void removerChaveFolha(No *no, int indice)
{
    for (int i = indice; i < no->num_chaves - 1; i++)
    {
        no->chaves[i] = no->chaves[i + 1];
    }
    no->num_chaves--;
}

// func aux remocao chave no interno
void removerChaveInterna(No *no, int indice)
{
    for (int i = indice; i < no->num_chaves - 1; i++)
    {
        no->chaves[i] = no->chaves[i + 1];
        no->ponteiros[i + 1] = no->ponteiros[i + 2];
    }
    no->num_chaves--;
}

// func aux juntar
void mesclarNosFolha(No *esquerdo, No *direito)
{
    for (int i = 0; i < direito->num_chaves; i++)
    {
        esquerdo->chaves[esquerdo->num_chaves + i] = direito->chaves[i];
    }
    esquerdo->num_chaves += direito->num_chaves;
    esquerdo->prox = direito->prox;
    free(direito->chaves);
    free(direito->ponteiros);
    free(direito);
}


// func remover
void removerChave(No **raiz, int chave)
{
    if (*raiz == NULL)
    {
        printf("A arvore esta vazia.\n");
        return;
    }

    // buscar
    No *noFolha = *raiz;
    No *pai = NULL;

    int indice;

    while (!noFolha->teste_folha)
    {
        indice = buscarIndiceChave(noFolha, chave);
        if (indice == noFolha->num_chaves || chave != noFolha->chaves[indice])
        {
            pai = noFolha;
            noFolha = noFolha->ponteiros[indice];
        }
        else
        {
            noFolha = noFolha->ponteiros[indice + 1];
        }
    }

    // verify
    int indiceChave = buscarIndiceChave(noFolha, chave);
    if (indiceChave == noFolha->num_chaves || chave != noFolha->chaves[indiceChave])
    {
        printf("A chave %d nao existe na arvore.\n", chave);
        return;
    }

    // remove
    removerChaveFolha(noFolha, indiceChave);

    // verify condiçao
    if (noFolha == *raiz)
    {
        if (noFolha->num_chaves == 0)
        {
            free(noFolha->chaves);
            free(noFolha->ponteiros);
            free(noFolha);
            *raiz = NULL;
        }
        return;
    }

    // verifica e equilibra
    if (noFolha->num_chaves < (4 - 1) / 2)
    {
        // busca irmaos
        No *irmaoEsquerdo = NULL;
        No *irmaoDireito = NULL;
        int indicePai = buscarIndiceChave(pai, chave);
        if (indicePai > 0)
        {
            irmaoEsquerdo = pai->ponteiros[indicePai - 1];
        }
        if (indicePai < 0)
        {
            irmaoDireito = pai->ponteiros[indicePai + 1];
   
        }

        // tenta pega irmao esquerdo
        if (irmaoEsquerdo != NULL && irmaoEsquerdo->num_chaves > (4 - 1) / 2)
        {
            // mover para no folha
            for (int i = noFolha->num_chaves; i > 0; i--)
            {
                noFolha->chaves[i] = noFolha->chaves[i - 1];
            }
            noFolha->chaves[0] = irmaoEsquerdo->chaves[irmaoEsquerdo->num_chaves - 1];
            noFolha->num_chaves++;

            // att daddy
            pai->chaves[indicePai - 1] = noFolha->chaves[0];

            // tira esquerdo brother
            removerChaveFolha(irmaoEsquerdo, irmaoEsquerdo->num_chaves - 1);
        }
        // tenta pega o direito
        else if (irmaoDireito != NULL && irmaoDireito->num_chaves > (4 - 1) / 2)
        {
            for (int i = noFolha->num_chaves; i > 0; i--)
            {
                noFolha->chaves[i] = noFolha->chaves[i - 1];
            }
            noFolha->chaves[noFolha->num_chaves] = irmaoDireito->chaves[0];
            noFolha->num_chaves++;

            // att daddy
            pai->chaves[indicePai - 1] = noFolha->chaves[0];

            // tira esquerdo brother
            removerChaveFolha(irmaoDireito, irmaoDireito->num_chaves + 1);
        }
        // junta com esquerdo ou direito
        else if (irmaoEsquerdo != NULL)
        {
            mesclarNosFolha(irmaoEsquerdo, noFolha);
            removerChaveInterna(pai, indicePai - 1);
            noFolha = irmaoEsquerdo;
        }
        else if (irmaoDireito != NULL)
        {
            mesclarNosFolha(noFolha, irmaoDireito);
            removerChaveInterna(pai, indicePai);
        }
    }

    // verify e equilibra
    if (noFolha->num_chaves > 4 - 1)
    {
        int chavePromovida = noFolha->chaves[(4 - 1) / 2];

        // cria novo
        No *novoNoFolha = criarNo();

        // move metade
        for (int i = (4 - 1) / 2 + 1; i < noFolha->num_chaves; i++)
        {
            novoNoFolha->chaves[i - (4 - 1) / 2 - 1] = noFolha->chaves[i];
        }
        novoNoFolha->num_chaves = noFolha->num_chaves - ((4 - 1) / 2 + 1);
        noFolha->num_chaves = (4 - 1) / 2 + 1;

        // att pointers
        novoNoFolha->prox = noFolha->prox;
        noFolha->prox = novoNoFolha;

        // chave que sobe pro pai
        inserirChave(&(pai->prox), chavePromovida);
    }
}