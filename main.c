#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct node{
 int Linha;
 int Coluna;
 int PixelValue;
 struct node *prev;
 struct node *next;
} Tnode;


char identificador[5]=""; //P2 from file img
int MaxLin=0, MaxCol=0, MaxPixel=0; //from file img

// Insere 1 pixel da imagem no final da lista
// se a lista estiver vazia, cria
// se a lista tiver elementos, coloca no fim....
// Essa é uma lista ligada simples com inserção no fim
Tnode *InsereFimDaListaLigada(Tnode *Imagem, int Linha, int Coluna, int PixelVl ){
    Tnode *tmp, *pt;
    if (Imagem==NULL){
        Imagem=(Tnode*)calloc(1,sizeof(Tnode));
        Imagem->Coluna = Coluna;
        Imagem->Linha = Linha;
        Imagem->PixelValue = PixelVl;
    }

    else{
        tmp = Imagem;
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next = calloc(1,sizeof(Tnode));
        pt=tmp->next;
        pt->Coluna = Coluna;
        pt->Linha = Linha;
        pt->PixelValue = PixelVl;
    }
 return Imagem;
}


// Esse método deve fazer a mesma coisa que o método "InsereFimDaListaLigada" (acima)
// contudo, ele deve implementar um tipo de lista ligada (qualquer outro) que seja mais
// otimizado para o problema que estamos trabalhando.
// Vc pode implementar qualquer tipo de lista, mas essa deve ser melhor em tempo do que aquela
// implementada no método "InsereFimDaListaLigada" (acima)
Tnode *InsereNaListaLigadaNova(Tnode *Imagem, int Linha, int Coluna, int PixelVl ){
    Tnode *tmp, *pt;
    if(Imagem==NULL){
        Imagem=(Tnode*)calloc(1, sizeof(Tnode));
        Imagem->Linha=Linha;
        Imagem->Coluna=Coluna;
        Imagem->PixelValue=PixelVl;
    }
    else{
        if (Imagem->next==NULL){
            Imagem->next=(Tnode*)calloc(1, sizeof(Tnode));
            Imagem->next->Linha=Linha;
            Imagem->next->Coluna=Coluna;
            Imagem->next->PixelValue=PixelVl;
            Imagem->next->next=Imagem;
            Imagem->next->prev=Imagem;
            Imagem->prev=Imagem->next;

        }
        else{
            pt=Imagem->prev;
            tmp=(Tnode*)calloc(1,sizeof(Tnode));
            tmp->Linha=Linha;
            tmp->Coluna=Coluna;
            tmp->PixelValue=PixelVl;
            tmp->next=Imagem;
            tmp->prev=pt;
            pt->next=tmp;
            Imagem->prev=tmp;
        }
    }
 return Imagem;
}

// Libera a Lista da memoria..........
Tnode * LiberaMemoria(Tnode *Imagem){
 Tnode *ptr = Imagem, *tmp;
 while(ptr!=NULL){
 tmp=ptr;
 ptr = ptr->next;
 free(tmp);
 if (ptr==Imagem)
 break;
 }
 return NULL;
}


// Lê o cabeçalho da imagem
// e depois lê cada pixel da imagem, se o pixel não for a cor preta, insere ele na lista
// A lista também armazena a Linha e a Coluna onde o pixel se encontra
Tnode *CarregaImagem(Tnode *Imagem, char File[], int TipoDaLista){
    int data, Lin, Col;
    FILE *fp = fopen(File, "r+");
    fscanf(fp,"%s\n",&identificador);
    fscanf(fp,"%d%d\n",&MaxLin,&MaxCol);
    fscanf(fp,"%d\n",&MaxPixel);
    for(Lin=0;Lin<MaxLin && !feof(fp);Lin++){
        for(Col=0;Col<MaxCol && !feof(fp);Col++){
            fscanf(fp,"%d\n", &data);
            if (data!=0){
                if (TipoDaLista==1) //Lista otimizada para o problema
                    Imagem=InsereNaListaLigadaNova(Imagem, Lin, Col, data);
                else //Lista com problema de desempenho (implementado no último lab)
                    Imagem=InsereFimDaListaLigada(Imagem, Lin, Col, data);
            }
        }
    }
    fclose(fp);
    return Imagem;
}

//Percorre a lista procurando os nós com o OldPixel e substitui por NewPixel
void ProcessaPixel(Tnode *Imagem, int OldPixel, int NewPixel, int TipoDaLista){
    if (TipoDaLista==0){
        while(Imagem!=NULL){
            if(Imagem->PixelValue==OldPixel){
                Imagem->PixelValue=NewPixel;
            }
            Imagem=Imagem->next;
        }
    }
    else{
        Tnode *tmp;
        tmp = Imagem;
        do{
            if(tmp->PixelValue==OldPixel){
                tmp->PixelValue=NewPixel;
            }
            tmp=tmp->next;
        }
        while(tmp!=Imagem);
    }
}


// Monta a imagem com base na lista.
// Lembre-se que a lista tem apenas os pixels não brancos
// Então vc tem que montar a imagem com base na lista
// ou seja onde "não" tiver dado, tem que gravar a cor branca.
// Imagine que a imagem deve uma matriz esparsa como abaixo,
// mas sua lista armazena apenas os elementos maiores que zero
// então para reconstruir a imagem, antes de gravar o 5 no arquivo
// vc tem que gravar o zero seis vezes. O mesmo para os outros elementos
// 0 0 0 0 0 0 5 0 8
// 0 0 1 5 0 0 8 8 0
// 1 2 2 2 5 5 0 0 0
void DescarregaNovaImagem(Tnode *Imagem, char File[], int TipoDaLista){
    int j, i;
    if (TipoDaLista==0){
        FILE *fp = fopen(File,"w+");
        fprintf (fp,"%s\n",identificador);
        fprintf (fp,"%d %d\n",MaxCol, MaxLin);
        fprintf (fp,"%d\n", MaxPixel);
        for (i=0; i<MaxLin; i++){
            for (j=0; j<MaxCol; j++){
                if ((Imagem==NULL) || (Imagem->Linha!=i)||(Imagem->Coluna!=j) ){
                    fprintf (fp, "%d\n", 0);
                }
                else{
                    fprintf (fp, "%d\n", Imagem->PixelValue);
                    Imagem=Imagem->next;
                }
            }
        }
    fclose(fp);
    }
    else{
        Tnode *tmp=Imagem;
        FILE *fp = fopen(File,"w+");
        fprintf (fp,"%s\n",identificador);
        fprintf (fp,"%d %d\n",MaxCol, MaxLin);
        fprintf (fp,"%d\n", MaxPixel);
        for (i=0; i<MaxLin; i++){
            for (j=0; j<MaxCol; j++){
                if ((tmp==NULL) || (tmp->Linha!=i)||(tmp->Coluna!=j) ){
                    fprintf (fp, "%d\n", 0);
                }
                else{
                    if (tmp->next!=Imagem){
                        fprintf (fp, "%d\n", tmp->PixelValue);
                        tmp=tmp->next;
                    }
                }
            }
        }
    fclose(fp);
    }
}


int main(){
 Tnode *Imagem = NULL;
 char ReadFile[30]="", SaveFile[30]="";
 int OldPixel=0, NewPixel=0;
 int TipoDaLista;
 printf("\nDigite 0 para lista simples ou 1 para lista duplamente encadeada e circular:\n   ");
 scanf("%d",&TipoDaLista);
 //TipoDaLista=0; // 0 - Lista ligada simples com inserção no fim
 //TipoDaLista=1; // 1 - sua proposta de lista..........
 printf("Informe o nome do arquivo de imagem para ler: ");
 scanf("%s",ReadFile); //"Img04.pgm"
 printf("No processamento, vc quer substituir qual pixel: ");
 scanf("%d",&OldPixel); // 255 - Branco
 printf("Substituir por qual valor: ");
 scanf("%d",&NewPixel); // 100 - um tom de cinza
 printf("Informe o nome do arquivo NOVO arquivo: ");
 scanf("%s",SaveFile); //"teste.pgm"
 int i;
 //vamos fazer um teste 10 vezes para ver o tempo
 for(i=0;i<10;i++){
 clock_t begin = clock();
 Imagem=CarregaImagem(Imagem,ReadFile, TipoDaLista);
 ProcessaPixel(Imagem, OldPixel, NewPixel, TipoDaLista);
 DescarregaNovaImagem(Imagem,SaveFile,TipoDaLista);
 Imagem=LiberaMemoria(Imagem);
 clock_t end = clock();
 double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
 printf("%lf\n",time_spent); // imprime o tempo
 }
 return 0;
}
