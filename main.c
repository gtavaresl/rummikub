#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    char valor,naipe;
    int usado;
}peca;

typedef struct{
    int *mao,*table;
}player;

void Inicializa(peca *carta){
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
        carta[i].usado=0;
        //printf("%c%c\n",carta[i].valor,carta[i].naipe);
    }
    for(int i=104;i<106;i++){
        carta[i].valor='*';
        carta[i].naipe='*';
        carta[i].usado=0;
        //printf("%c%c\n",carta[i].valor,carta[i].naipe);
    }
}

void distribuiAleat(peca *carta, player *jogador, int nplayers){
    int pcarta;
    for(int i=0;i<nplayers;i++) jogador[i].mao=(int *)malloc(14*sizeof(int));
    for(int i=0,p=0,mod=0;i<14*nplayers;i++,p++,mod++){
        pcarta=(rand()%106);
        while(carta[pcarta].usado==2){
            pcarta=((rand()+55)%106);
        }
        carta[pcarta].usado++;
        if(i%nplayers==0) p=0;
        if(mod==14) mod=0;
        jogador[p].mao[mod]=pcarta;
        printf("Jogador %d: %c%c\n",p+1,carta[pcarta].valor,carta[pcarta].naipe);
    }
}

void vizualizaMao(peca *carta, player *jogador, int pplayer){
    printf("Mao do jogador %d: ",pplayer);
    pplayer--;
    int pcarta;
    for(int i=0;i<14;i++){
        pcarta=jogador[pplayer].mao[i];
        printf(" %c%c",carta[pcarta].valor,carta[pcarta].naipe);
    }
}

int main(){
    srand(time(NULL));
    peca *carta;
    carta=(peca *)malloc(106*sizeof(peca));
    Inicializa(carta);
    printf("Entre com o numero de jogadores (1 a 4): ");
    int nplayers,pcarta;
    int i=0,p=0,mod=0;
    scanf("%d",&nplayers);
    player *jogador;
    jogador=(player *)malloc(nplayers*sizeof(jogador));
    distribuiAleat(carta,jogador,nplayers);
    printf("Exibir mao do jogador: ");
    scanf("%d",&p);
    vizualizaMao(carta,jogador,p);
}
