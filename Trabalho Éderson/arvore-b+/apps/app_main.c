#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
   No *raiz = NULL;

   inserirChave(&raiz, 11);
   
   printf("\nApos inser\n\n");
   imprimirEmOrdem(raiz, 0);
   inserirChave(&raiz, 3);
   printf("\nApos inser\n\n");
   imprimirEmOrdem(raiz, 0);
   inserirChave(&raiz, 5);
   printf("\nApos inser\n\n");
   imprimirEmOrdem(raiz, 0);
   inserirChave(&raiz, 17);
   printf("\nApos inser\n\n");
   imprimirEmOrdem(raiz, 0);
   
   printf("\nApos remocao\n\n");
   removerChave(&raiz, 3);
   imprimirEmOrdem(raiz, 0);

   return 0;
}