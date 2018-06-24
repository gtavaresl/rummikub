#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

typedef struct{
    char valor,naipe;
}pilha;

typedef struct{
    int *mao;
    int manga,turno;
}player;

typedef struct{
    int *jogada;
    int quantidadejogada;
}tabela;

void lePilha(FILE *arquivo, pilha *carta){
    char pega[3];
    for(int i=0;i<106;i++){
        fscanf(arquivo,"%s",pega);
        carta[i].valor=pega[0];
        carta[i].naipe=pega[1];
        //printf("%c%c\n",carta[i].valor,carta[i].naipe);
    }
}

void inicializaPilha(pilha *carta){
    int mod;
    for(int i=0,p=0;i<104;i++){
        mod=(i%13);
        if(mod<9) carta[i].valor = mod+49;
        else if(mod>=9) carta[i].valor = mod+56;
        if(p==0) carta[i].naipe='!';
        else if(p==1) carta[i].naipe='@';
        else if(p==2) carta[i].naipe='#';
        else if(p==3)carta[i].naipe='$';
        if(mod==12) p++;
        if(p==4) p=0;
        //printf("%c%c\n",carta[i].valor,carta[i].naipe);
    }
    for(int i=104;i<106;i++){
        carta[i].valor='*';
        carta[i].naipe='*';
        //printf("%c%c\n",carta[i].valor,carta[i].naipe);
    }
    //Algoritmo de Fisher-Yates (solução de Durstenfeld) p/ embaralhar as cartas
    int falta = 106;
    while (falta>0){
        int k = rand() % (falta);
        pilha embaralha = carta[k];
        carta[k] = carta[falta-1];
        carta[falta-1] = embaralha;
        falta--;
    }
    /*printf("\n\n Embaralhando!\n");
    for(int i=0;i<106;i++){
        printf("%c%c\n",carta[i].valor,carta[i].naipe);
    }*/
}

int distribuiAlternate(pilha *carta, player *jogador, int nplayers, int ncartas){
    int i=0,p=0,mod=0;
    for(i=0;i<nplayers;i++){
        jogador[i].manga=14;
        jogador[i].turno=0;
        jogador[i].mao=(int *)malloc(jogador[i].manga*sizeof(int));
    }
    for(i=0,p=0,mod=0;i<14*nplayers;i++,ncartas--){
        if(nplayers>1 && i!=0 && i%nplayers==0) p=0,mod++;
        jogador[p].mao[mod]=i;
        printf("Mao %d do jogador %d: %c%c\n",mod,p,carta[jogador[p].mao[mod]].valor,carta[jogador[p].mao[mod]].naipe);
        if(nplayers==1) mod++;
        else p++;
    }
    printf("\nCartas na pilha: %d\n",ncartas);
    return ncartas;
}


void vizualizaMao(pilha *carta, player *jogador, int posicao){
    printf("\nMao do jogador %d:\n",posicao);
    for(int i=0;i<jogador[posicao].manga;i++)   printf("Carta[%d]: %c%c\n",i,carta[jogador[posicao].mao[i]].valor,carta[jogador[posicao].mao[i]].naipe);
}

void vizualizaMesa(pilha *carta,tabela mesa, int ncartas){
    printf("Mesa:\n");
    if(mesa.jogada==NULL) printf("Vazia!\n");
    else for(int i=0;i<mesa.quantidadejogada;i++) printf("%d- %c%c\n",i,carta[mesa.jogada[i]].valor,carta[mesa.jogada[i]].naipe);
    printf("Total de cartas na mesa: %d\n",mesa.quantidadejogada);
    printf("Total de cartas na pilha: %d\n",ncartas);
}

int pedeCarta(pilha *carta, player *jogador, int posicao, int ncartas){
    //printf("\nPuxei carta da stack\n");
    int *hand = (int *)malloc(jogador[posicao].manga*sizeof(int));
    int i=0;
    for(i=0;i<jogador[posicao].manga;i++) hand[i]=jogador[posicao].mao[i];
    free(jogador[posicao].mao);
    jogador[posicao].manga++;
    jogador[posicao].mao = (int *)malloc(jogador[posicao].manga*sizeof(int));
    for(i=0;i<((jogador[posicao].manga)-1);i++) jogador[posicao].mao[i]=hand[i];
    jogador[posicao].mao[jogador[posicao].manga-1]=106-ncartas;
    ncartas--;
    return ncartas;
    //for(i=0;i<((jogador[posicao].manga));i++) printf("Carta %d: %c%c\n",i+1,carta[jogador[posicao].mao[i]].valor,carta[jogador[posicao].mao[i]].naipe);
}

void menuTurn(){
    printf("\n1. Ver mao\n");
    printf("2. Ver mesa\n");
    printf("3. Pedir carta da pilha\n");
    printf("4. Combina carta da mao\n");
    printf("5. Combina carta da mesa\n");
    printf("6. Joga\n");
    printf("7. Desfazer combinacao\n");
    printf("8. Encerrar jogada\n");
    printf("<?> ");
}

int turn(pilha *carta, player *jogador,tabela mesa, int posicao, int ncartas){
    printf("\nTurno %d do jogador %d",jogador[posicao].turno,posicao);
    menuTurn();
    int op,p=-1,transferencia=0;
    //int qtdinicial=jogador[posicao].manga;
    int *usada=(int*)calloc((jogador[posicao].manga+mesa.quantidadejogada),sizeof(int));
    int *transfere=NULL;
    scanf("%d",&op);
    while(op!=8){
        if(op==1) vizualizaMao(carta,jogador,posicao);
        if(op==2) vizualizaMesa(carta,mesa,ncartas);
        if(op==3){
            ncartas=pedeCarta(carta,jogador,posicao,ncartas);
            vizualizaMao(carta,jogador,posicao);
            printf("Fim de turno!\n");
            return ncartas;
        }
        if(op==4){
            vizualizaMao(carta,jogador,posicao);
            printf("Escolha uma carta: ");
            scanf("%d",&p);
            while(p<0||p>=(jogador[posicao].manga)){
                printf("Entrada invalida!\n");
                printf("Escolha uma carta: ");
                scanf("%d",&p);
            }
            if(usada[p]) printf("Carta ja transferida!\n");
            else{
                transferencia++;
                transfere=(int *)realloc(transfere,transferencia*sizeof(int));
                if(transfere!=NULL){
                    transfere[transferencia-1]=jogador[posicao].mao[p];
                    usada[p]++;
                    printf("Area de transferencia: ");
                    for(int i=0;i<transferencia;i++) printf("%d - %c%c\n",i,carta[transfere[i]].valor,carta[transfere[i]].naipe);
                }else printf("ERRO!\n");
            }
        }
        if(op==5){
            if(!jogador[posicao].turno) printf("Primeira jogada!\n");
            else if(mesa.quantidadejogada==0) printf("Mesa vazia!\n");
            else{
                vizualizaMesa(carta,mesa,ncartas);
                printf("Escolha uma carta: ");
                scanf("%d",&p);
                while(p<0||p>=(mesa.quantidadejogada)){
                    printf("Entrada invalida!\n");
                    printf("Escolha uma carta: ");
                    scanf("%d",&p);
                }
                p=p+jogador[posicao].manga;
                if(usada[p]) printf("Carta ja transferida!\n");
                else {
                    transferencia++;
                    transfere=(int *)realloc(transfere,transferencia*sizeof(int));
                    if(transfere!=NULL){
                        transfere[transferencia-1]=jogador[posicao].mao[p];
                        usada[p]++;
                        printf("Area de transferencia:\n");
                        for(int i=0;i<transferencia;i++) printf("%d - %c%c\n",i,carta[transfere[i]].valor,carta[transfere[i]].naipe);
                    }else printf("ERRO!\n");
                }
            }
        }
        if(op==6){
            if(transfere!=NULL){
                printf("Area de transferencia: ");
                for(int i=0;i<transferencia;i++) printf("%d - %c%c\n",i,carta[transfere[i]].valor,carta[transfere[i]].naipe);
                mesa.jogada=(int *)realloc(mesa.jogada,(mesa.quantidadejogada+transferencia)*sizeof(int));
                if(mesa.jogada==NULL) printf("ERRO!");
                else{
                    //PRECISO VALIDAR JOGADA!
                    for(int i=mesa.quantidadejogada;i<mesa.quantidadejogada+transferencia;i++) mesa.jogada[i]=transfere[i-mesa.quantidadejogada];
                    printf("Mesa:\n");
                    for(int i=0;i<(mesa.quantidadejogada+transferencia);i++) printf("%d- %c%c\n",i,carta[mesa.jogada[i]].valor,carta[mesa.jogada[i]].naipe);
                    mesa.quantidadejogada+=transferencia;
                    int j=0;
                    for(int i=0;i<jogador[posicao].manga;i++){
                        if(!usada[i]){ // não usado
                            jogador[posicao].mao[j]=jogador[posicao].mao[i];
                            j++;
                        }
                    }
                    jogador[posicao].manga=j;
                    jogador[posicao].mao=(int *)realloc(jogador[posicao].mao,jogador[posicao].manga*sizeof(int));
                    free(transfere);
                    transfere=NULL;
                    transferencia=0;
                    free(usada);
                    usada=(int*)calloc((jogador[posicao].manga+mesa.quantidadejogada),sizeof(int));
                    //jogador[posicao].turno++;
                }
            }else printf("Area de transferencia vazia!\n");
        }
        if(op==7){
            if(transfere!=NULL){
                free(transfere);
                transfere=NULL;
                transferencia=0;
                free(usada);
                usada=(int*)calloc((jogador[posicao].manga+mesa.quantidadejogada),sizeof(int));
                printf("Area de transferencia limpa!\n");
            }else printf("Area de transferencia vazia!\n");
        }
        menuTurn();
        scanf("%d",&op);
    }
    /*if(qtdinicial==jogador[posicao].manga){
        printf("Nao mexeu!");
    }*/
    printf("Fim de turno!\n");
    free(usada);
    free(transfere);
    return ncartas;
}

int main(){
    srand(time(NULL));
    pilha *carta = (pilha *)malloc(106*sizeof(pilha));
    tabela mesa;
    mesa.jogada=NULL;
    mesa.quantidadejogada=0;
    FILE *arquivo;
    printf("Entre com o numero de jogadores (1 a 4): ");
    int nplayers,ncartas=106;
    scanf("%d",&nplayers);
    player *jogador = (player *)malloc(nplayers*sizeof(jogador));
    arquivo=fopen("baralho.txt","rt");
    printf("Escolha o modo de jogo(1. Ler baralho / 2. Incializar baralho): ");
    int p;
    scanf("%d",&p);
    if(p==1){
        if(arquivo){
            printf("\nLendo baralho!\n");
            lePilha(arquivo,carta);
            //printf("Segunda carta que o jogador 2 recebeu: %c%c",carta[jogador[1].mao[1]].valor,carta[jogador[1].mao[1]].naipe);
        }else{
            printf("\nErro ao ler baralho!\n");
            return 0;
        }
    }else if(p==2){
        inicializaPilha(carta);
    }
    ncartas=distribuiAlternate(carta,jogador,nplayers,ncartas);
    p=0;
    while(1){
        ncartas=turn(carta,jogador,mesa,p,ncartas);
        if(jogador[p].manga==0){
        printf("Jogador %d ganhou!",p);
            return 0;
        }else if(!ncartas){
            printf("Fim de jogo!");
            return 0;
        }
        p++;
        if(p==nplayers) p=0;
    }
    return 0;
}
