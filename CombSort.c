#include <stdio.h>
#include <stdlib.h>

typedef struct sElemento{
    struct sElemento *next;
    struct sElemento *prev;
    int dado; //Depende do tipo de dados que se deseja trabalhar
} Elemento;


//Representação de Lista encadeada Simples em C
typedef struct sLista{
    struct sElemento *head;
    struct sElemento *tail;
    int size;
} Lista;




int estaOrdenado(Lista* list){
    for(Elemento *item = list->tail; item != NULL; item = item->prev){
      if(item->prev != NULL){
        if(item->prev->dado > item->dado){
          return 0;
        }
      }
    }
    return 1;
}












Lista* criaLista();
Elemento* criaElemento(int);
void insereElemento(Lista*, int, Elemento*);
int removeElemento(Lista*, Elemento*);
void escrveLista(Lista*);
void liberaLista(Lista*);
void buscarLista(Lista*, int);
void buscarListaBP(Lista*, int);
//novos
Elemento* retPos(Lista*, int);
void trocaPos(Lista*, int, int);
//do comb sort
int getNextGap(int);
void combSort(Lista*);



int main(void) {
  Lista* listaA = criaLista();
  insereElemento(listaA, 15, NULL);
  insereElemento(listaA, 10, listaA->head);
  insereElemento(listaA, 3, listaA->tail);
  insereElemento(listaA, 5, listaA->tail);
  insereElemento(listaA, 4, listaA->head);
  insereElemento(listaA, 15, NULL);
  
  escrveLista(listaA);
  
  if(estaOrdenado(listaA) == 1){
    printf("Tudo Certo!\n");
  }else{
    printf("Tudo Desordenado!\n");
  }



  combSort(listaA);

  escrveLista(listaA);

  if(estaOrdenado(listaA) == 1){
    printf("Tudo Certo!\n");
  }else{
    printf("Tudo Desordenado!\n");
  }

  
  liberaLista(listaA);

  getch();

  return 0;
}

Lista* criaLista(){
  Lista* lista = (Lista*) malloc(sizeof(Lista));
  lista->head = NULL;
  lista->tail = NULL;
  lista->size = 0;

  return lista;
}

Elemento* criaElemento(int dado){
  Elemento *novo = (Elemento*) malloc(sizeof(Elemento));
  novo->next = NULL;
  novo->prev = NULL;
  novo->dado = dado;
  return novo;
}

void insereElemento(Lista* lista, int dado, Elemento* pivo){
    Elemento* novo = criaElemento(dado);
  if (pivo == NULL && lista->size > 0){
      printf("Erro, somente insere a partir de NULL se for o primeiro!\n");
      return;
  } 
  if(lista->size == 0){
    lista->head = novo;
    lista->tail = novo;
  }else {
    novo->next = pivo->next;
    novo->prev = pivo;
    //lista->head = novo;
    if(pivo->next == NULL){
        lista->tail = novo;
    }else{
        pivo->next->prev = novo;
    }
    pivo->next = novo;
  }
  lista->size++;
  printf("Inceriu!\n");
}



int removeElemento(Lista* lista, Elemento* removendo){
    if(removendo != NULL && lista->size > 0){
        if(removendo == lista->head){
            lista->head = removendo->next;
            if(lista->head == NULL){
                lista->tail = NULL;
            }else{
                removendo->next->prev = NULL;
            }
        }else{
            removendo->prev->next = removendo->next;
            if(removendo->next == NULL){
                lista->tail = removendo->prev;
            }else{
                removendo->next->prev = removendo->prev;
            }
        }
        free(removendo);
        lista->size--;
        //printf("Removeu!\n");
        return 0;
    }else{
        if(removendo == NULL){
            printf("Erro na remoção -1\n");
            return -1;
        }else{
            printf("Erro na remoção -2\n");
            return -2;
        }
    }
}

void escrveLista(Lista* lista){
    for(Elemento *item = lista->head; item != NULL; item = item->next){
      int elemento = item->dado;
      printf("%d \n",elemento);
    }
}
void buscarLista(Lista* lista, int dado){
    int posicao = 0;
    for(Elemento *item = lista->head; item != NULL; item = item->next){
      int elemento = item->dado;
      if(elemento == dado)
        printf("elemento %d na posicao %d \n",elemento,posicao);
      posicao++;
    }
}
void buscarListaBP(Lista* lista, int dado){
    int posicao = (lista->size -1);
    for(Elemento *item = lista->tail; item != NULL; item = item->prev){
      int elemento = item->dado;
      if(elemento == dado)
        printf("elemento %d na posicao %d \n",elemento,posicao);
      posicao--;
    }
}
void liberaLista(Lista* lista){
  while(lista->size > 0){
    removeElemento(lista, lista->head);
  }
  free(lista);
  printf("Lista liberada!");
}


Elemento* retPos(Lista* lista, int index){
  if(index >= 0 && index < lista->size){
    Elemento* aux = lista->head;
    int i;
    for(i=0; i < index; i++){
      aux = aux->next;
    }
    return aux;
  }else if(index<0){
    return NULL;

  }else if(index >= lista->size){
    return NULL;

  }
}

void trocaPos(Lista* lista, int primeiro, int segundo){
  if(primeiro == segundo)
    return;

  
  Elemento* elemento1 = retPos(lista,primeiro);
  Elemento* elemento2 = retPos(lista,segundo);
  
  //printf("elemento %d na posicao %d \n",elemento1->dado,primeiro);
  //printf("elemento %d na posicao %d \n",elemento2->dado,segundo);
  Elemento* aux = criaElemento(0);
  
  aux->dado = elemento1->dado;
  elemento1->dado = elemento2->dado;
  elemento2->dado = aux->dado;
  free(aux);
  printf("Troca!!\n");

}

int getNextGap(int gap){
    //O fator de redução foi empiricamente encontrado em 1,3 (testando Combsort em mais de 200.000 listas aleatórias) 
    gap = (gap*10)/13;
    //pega o valor menor inteiro
    if (gap < 1)
        return 1;
    return gap;
}


void combSort(Lista* list){
    // Inicia gap
    int gap = list->size;
 
    // Inicializa o troca como 1 para ter certeza q vai começar o loop
    int troca = 1;
 
    while (gap != 1 || troca == 1){
        gap = getNextGap(gap);
 
        // Inicia troca como zero para checar se houve troca ou não
        troca = 0;
 
        for (int i=0; i<(list->size)-gap; i++){
          Elemento* elementoa = retPos(list,i);
          Elemento* elementob = retPos(list,i+gap);
            if (elementoa->dado > elementob->dado){
                trocaPos(list,i,(i+gap));
                troca = 1;
            }
        }
    }
}
