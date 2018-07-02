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
    int deck,grupo; // TENHO QUE AJEITAR PRA FAZER FUNCIONAR O SISTEMA DE COMBINAÇÕES
}rhcp;

typedef struct{
    rhcp *jogada;
    int quantidadejogada,trucos;
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

int distribuiAlternado(pilha *carta, player *jogador, int nplayers, int ncartas){
    int i=0,p=0,mod=0;
    for(i=0;i<nplayers;i++){
        jogador[i].manga=14;
        jogador[i].turno=0;
        jogador[i].mao=(int *)realloc(jogador[i].mao,jogador[i].manga*sizeof(int));
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


void vizualizaMao(pilha *carta, player *jogador, int posicao, int *usada){
    printf("\n\nMao do jogador %d:\n\n|",posicao);
    int i;
    for(i=0;i<jogador[posicao].manga;i++)   printf(" %d- %c%c |",i,carta[jogador[posicao].mao[i]].valor,carta[jogador[posicao].mao[i]].naipe);
    printf("\n\n1. Alterar posicao\n2. Ordenar por valor\n3. Ordenar por naipe\n4.Sair\n<?> ");
    int h;
    scanf("%d",&h);
    setbuf(stdin,NULL);
    while(h<1||h>4){
        printf("Opcao invalida!\n<?> ");
        scanf("%d",&h);
        setbuf(stdin,NULL);
    }
    int aux,j,menor,xua;
    while(1){
        if(h==1){
            int a,b;
            printf("\nAltera posicao: ");
            printf("\nPosicao 1: ");
            scanf("%d",&a);
            setbuf(stdin,NULL);
            while(a<0||a>jogador[posicao].manga){
                printf("Opcao invalida!\n<?> ");
                scanf("%d",&a);
                setbuf(stdin,NULL);
            }
            printf("%d: %c%c",a,carta[jogador[posicao].mao[a]].valor,carta[jogador[posicao].mao[a]].naipe);
            printf("\nPosicao 2: ");
            scanf("%d",&b);
            setbuf(stdin,NULL);
            while(b<0||b>jogador[posicao].manga){
                printf("Opcao invalida!\n<?> ");
                scanf("%d",&b);
                setbuf(stdin,NULL);
            }
            printf("%d: %c%c\n\n",b,carta[jogador[posicao].mao[b]].valor,carta[jogador[posicao].mao[b]].naipe);
            aux=jogador[posicao].mao[a];
            xua=usada[a];
            jogador[posicao].mao[a]=jogador[posicao].mao[b];
            usada[a]=usada[b];
            jogador[posicao].mao[b]=aux;
            usada[b]=xua;
        }else if(h==2){
            printf("\n\nOrdenado por valor:\n\n|");
            for(i=0;i<jogador[posicao].manga;i++){
                menor=i;
                for(j=i+1;j<jogador[posicao].manga;j++){
                    if(carta[jogador[posicao].mao[j]].valor<carta[jogador[posicao].mao[menor]].valor||(carta[jogador[posicao].mao[j]].valor==carta[jogador[posicao].mao[menor]].valor&&carta[jogador[posicao].mao[j]].naipe<carta[jogador[posicao].mao[menor]].naipe)) menor=j;
                }
                aux=jogador[posicao].mao[menor];
                xua=usada[menor];
                jogador[posicao].mao[menor]=jogador[posicao].mao[i];
                usada[menor]=usada[i];
                jogador[posicao].mao[i]=aux;
                usada[i]=xua;
            }
        }else if(h==3){
            printf("\n\nOrdenado por naipe:\n\n|");
            for(i=0;i<jogador[posicao].manga;i++){
                menor=i;
                for(j=i+1;j<jogador[posicao].manga;j++){
                    if(carta[jogador[posicao].mao[j]].naipe<carta[jogador[posicao].mao[menor]].naipe||(carta[jogador[posicao].mao[j]].naipe==carta[jogador[posicao].mao[menor]].naipe&&carta[jogador[posicao].mao[j]].valor<carta[jogador[posicao].mao[menor]].valor)) menor=j;
                }
                aux=jogador[posicao].mao[menor];
                xua=usada[menor];
                jogador[posicao].mao[menor]=jogador[posicao].mao[i];
                usada[menor]=usada[i];
                jogador[posicao].mao[i]=aux;
                usada[i]=xua;
            }
        }else if(h==4) break;
        for(i=0;i<jogador[posicao].manga;i++)   printf(" %d- %c%c |",i,carta[jogador[posicao].mao[i]].valor,carta[jogador[posicao].mao[i]].naipe);
        printf("\n\n1. Alterar posicao\n2. Ordenar por valor\n3. Ordenar por naipe\n4.Sair\n<?> ");
        scanf("%d",&h);
        setbuf(stdin,NULL);
        while(h<1||h>4){
            printf("Opcao invalida!\n<?> ");
            scanf("%d",&h);
            setbuf(stdin,NULL);
        }
    }
}

void vizualizaMesa(pilha *carta,tabela *mesa, int ncartas){
    printf("\nTotal de cartas na mesa: %d\n\n",mesa->quantidadejogada);
    printf("Mesa:\n");
    if(mesa->quantidadejogada==0) printf("Vazia!\n");
    else{
        printf("|");
        int *tamanhoCombo=(int *)calloc(1,sizeof(int));
        int combo=1;
        for(int i=0;i<mesa->quantidadejogada;i++){
            if(combo!=mesa->jogada[i].grupo){
                combo++;
                tamanhoCombo=(int *)realloc(tamanhoCombo,combo*sizeof(int));
                tamanhoCombo[combo-1]=0;
                printf("\n|");
            }
            printf(" %d- %c%c |",i,carta[mesa->jogada[i].deck].valor,carta[mesa->jogada[i].deck].naipe);
            tamanhoCombo[combo-1]++;
        }
        free(tamanhoCombo);
    }
    printf("\n\nTotal de cartas na pilha: %d\n",ncartas);
}

int pedeCarta(pilha *carta, player *jogador, int posicao, int ncartas){
    jogador[posicao].manga++;
    jogador[posicao].mao = (int *)realloc(jogador[posicao].mao,jogador[posicao].manga*sizeof(int));
    jogador[posicao].mao[jogador[posicao].manga-1]=106-ncartas;
    ncartas--;
    return ncartas;
}

int validaJogada(pilha *carta, rhcp *transfere, int transferencia, int turno){
    if(transferencia<3) return 0;
    else{
        int ok=0;
        if(transferencia==3||transferencia==4){
            for(int i=0;i<transferencia;i++){
                for(int p=i+1;p<transferencia;p++){
                    if((carta[transfere[i].deck].valor==carta[transfere[p].deck].valor&&carta[transfere[i].deck].naipe!=carta[transfere[p].deck].naipe)||carta[transfere[i].deck].valor=='*'||carta[transfere[p].deck].valor=='*') ok++;
                }
            }
            if(ok==(((transferencia-1)*transferencia)/2)){
                if(turno==0){
                    int pontos=0;
                    int i=0;
                    while(i<3){
                        if(carta[transfere[i].deck].valor!='*') break;
                        else i++;
                    }
                    if(carta[transfere[i].deck].valor<58) pontos=transferencia*((int)carta[transfere[i].deck].valor-48);
                    else pontos=transferencia*((int)carta[transfere[i].deck].valor-55);
                    if(pontos<30){
                        printf("\nA primeira jogada deve somar 30+ pontos!");
                        return 0;
                    }else return 1;
                }else return 1;
            }
            else{
                int a,b;
                int p=0;
                ok=0;
                // Escolher a carta base, início da sequência
                while(p<3){
                    if(carta[transfere[p].deck].naipe!='*'){
                        break;
                    }else p++;
                }
                a=(int)carta[transfere[p].deck].valor;
                if(a>48&&a<58) a-=48;
                else a-=55;
                if(turno==0){
                    int pontos=0;
                    pontos=(transferencia*(2*a-2*p+transferencia-1))/2;
                    if(pontos<30){
                        printf("\nA primeira jogada deve somar 30+ pontos!");
                        return 0;
                    }
                }
                for(int i=(p+1);i<transferencia;i++){
                    b=0;
                    if(carta[transfere[i].deck].naipe==carta[transfere[p].deck].naipe){
                        if(carta[transfere[i].deck].valor>48&&carta[transfere[i].deck].valor<58) b=(int)carta[transfere[i].deck].valor-48;
                        else if(carta[transfere[i].deck].valor>64) b=(int)carta[transfere[i].deck].valor-55;
                        if((b-a)!=(i-p)) return 0;
                    }else if(carta[transfere[i].deck].naipe!='*') return 0;
                }
                return 1;
            }
        }else{
            int a,b;
            int p=0;
            ok=0;
            // Escolher a carta base, início da sequência
            while(p<3){
                if(carta[transfere[p].deck].naipe!='*'){
                    break;
                }else p++;
            }
            a=(int)carta[transfere[p].deck].valor;
            if(a>48&&a<58) a-=48;
            else a-=55;
            if(turno==0){
                int pontos=0;
                pontos=(transferencia*(2*a-2*p+transferencia-1))/2;
                if(pontos<30){
                    printf("\nA primeira jogada deve somar 30+ pontos!");
                    return 0;
                }
            }
            for(int i=(p+1);i<transferencia;i++){
                b=0;
                if(carta[transfere[i].deck].naipe==carta[transfere[p].deck].naipe){
                    if(carta[transfere[i].deck].valor>48&&carta[transfere[i].deck].valor<58) b=(int)carta[transfere[i].deck].valor-48;
                    else if(carta[transfere[i].deck].valor>64) b=(int)carta[transfere[i].deck].valor-55;
                    if((b-a)!=(i-p)) return 0;
                }else if(carta[transfere[i].deck].naipe!='*') return 0;
            }
            return 1;
        }
    }
}

int validaMesa(tabela *mesa, pilha *carta, int validturno){
    if(mesa->jogada!=NULL){
        rhcp *verifica=NULL;
        int *tamanhoCombo=(int *)calloc(1,sizeof(int));
        int combo=1;
        for(int i=0;i<mesa->quantidadejogada;i++){
            if(combo!=mesa->jogada[i].grupo){
                combo++;
                //printf("Combo %d:\n",combo);
                tamanhoCombo=(int *)realloc(tamanhoCombo,combo*sizeof(int));
                tamanhoCombo[combo-1]=0;
            }
            //printf("%d- %c%c\n",i,carta[mesa->jogada[i].deck].valor,carta[mesa->jogada[i].deck].naipe);
            tamanhoCombo[combo-1]++;
        }
        int k=0,jump=0;
        while(k<combo){
            //printf("Tamanho do combo %d = %d\n",k+1,tamanhoCombo[k]);
            verifica=(rhcp *)realloc(verifica,tamanhoCombo[k]*sizeof(rhcp));
            for(int i=jump;i<jump+tamanhoCombo[k];i++){
                verifica[i-jump]=mesa->jogada[i];
                //printf("Verifica[%d]=%c%c\n",i-jump,carta[verifica[i-jump].deck].valor,carta[verifica[i-jump].deck].naipe);
            }
            if(tamanhoCombo[k]!=0){
                if(!validaJogada(carta,verifica,tamanhoCombo[k],validturno)){
                    free(tamanhoCombo);
                    free(verifica);
                    return 0;
                }
            }
            jump+=tamanhoCombo[k];
            k++;
        }
        free(tamanhoCombo);
        free(verifica);
        return 1;
    }else return 1;
}

void menuTurn(){
    printf("\n1. Ver mao\n");
    printf("2. Ver mesa\n");
    printf("3. Pedir carta da pilha\n");
    printf("4. Combina carta da mao\n");
    printf("5. Combina carta da mesa\n");
    printf("6. Joga\n");
    printf("7. Desfazer combinacao\n");
    printf("8. Desfazer jogada\n");
    printf("9. Encerrar jogada\n");
    printf("<?> ");
}

int turn(pilha *carta, player *jogador,tabela *mesa, int nplayers, int posicao, int ncartas){
    printf("\nTurno %d do jogador %d",jogador[posicao].turno,posicao);
    menuTurn();
    int op,p=-1,transferencia=0,transferenciamesa=0;
    int backup_manga=jogador[posicao].manga;
    int validturno=jogador[posicao].turno;
    int *backup_mao=(int *)malloc(backup_manga*sizeof(int)); // BACKUP DA MAO DO JOGADOR
    for(int i=0;i<backup_manga;i++) backup_mao[i]=jogador[posicao].mao[i]; // CÓPIA DO VALOR DE CADA CARTA DA MÃO P/ O BACKUP
    int backup_qtdmesa=mesa->quantidadejogada;
    int backup_trucos=mesa->trucos;
    rhcp *backup_mesa=(rhcp *)malloc(backup_qtdmesa*sizeof(rhcp));
    for(int i=0;i<backup_qtdmesa;i++){
        backup_mesa[i].deck=mesa->jogada[i].deck;
        backup_mesa[i].grupo=mesa->jogada[i].grupo;
    }
    int *usada=(int*)calloc((jogador[posicao].manga+mesa->quantidadejogada),sizeof(int));
    rhcp *transfere=NULL;
    scanf("%d",&op);
    setbuf(stdin,NULL);
    while(op<1||op>8){
        printf("Opcao invalida!\n<?> ");
        scanf("%d",&op);
        setbuf(stdin,NULL);
    }
    while(1){
        if(op==1) vizualizaMao(carta,jogador,posicao,usada);
        if(op==2) vizualizaMesa(carta,mesa,ncartas);
        if(op==3){
            if(validaMesa(mesa,carta,validturno)){
                ncartas=pedeCarta(carta,jogador,posicao,ncartas);
                printf("\nNova mao do jogador %d:\n\n|",posicao);
                for(int i=0;i<jogador[posicao].manga;i++)   printf(" %d- %c%c |",i,carta[jogador[posicao].mao[i]].valor,carta[jogador[posicao].mao[i]].naipe);
                break;
            }else printf("Mesa invalida!\n");
        }
        if(op==4){
            printf("\nMao do jogador %d:\n\n|",posicao);
            for(int i=0;i<jogador[posicao].manga;i++)   printf(" %d- %c%c |",i,carta[jogador[posicao].mao[i]].valor,carta[jogador[posicao].mao[i]].naipe);
            printf("\n\nEscolha uma carta: ");
            scanf("%d",&p);
            setbuf(stdin,NULL);
            while(p<0||p>=(jogador[posicao].manga)){
                printf("Entrada invalida!\n");
                printf("Escolha uma carta: ");
                scanf("%d",&p);
                setbuf(stdin,NULL);
            }
            if(usada[p]) printf("Carta ja transferida!\n");
            else{
                transferencia++;
                transfere=(rhcp *)realloc(transfere,transferencia*sizeof(rhcp));
                if(transfere!=NULL){
                    transfere[transferencia-1].deck=jogador[posicao].mao[p];
                    transfere[transferencia-1].grupo=1;
                    usada[p]++;
                    printf("\nArea de transferencia:\n\n|");
                    for(int i=0;i<transferencia;i++) printf(" %d - %c%c |",i,carta[transfere[i].deck].valor,carta[transfere[i].deck].naipe);
                    printf("\n");
                }else printf("ERRO!\n");
            }
        }
        if(op==5){
            if(!jogador[posicao].turno) printf("Primeira jogada!\n");
            else if(mesa->quantidadejogada==0) printf("Mesa vazia!\n");
            else{
                printf("\nMesa: \n\n");
                for(int i=0;i<mesa->quantidadejogada;i++) printf(" %d- %c%c |",i,carta[mesa->jogada[i].deck].valor,carta[mesa->jogada[i].deck].naipe);
                printf("\n\nEscolha uma carta: ");
                scanf("%d",&p);
                setbuf(stdin,NULL);
                while(p<0||p>=(mesa->quantidadejogada)){
                    printf("Entrada invalida!\n");
                    printf("Escolha uma carta: ");
                    scanf("%d",&p);
                    setbuf(stdin,NULL);
                }
                p+=jogador[posicao].manga;
                if(usada[p]) printf("Carta ja transferida!\n");
                else{
                    transferencia++;
                    transferenciamesa++;
                    transfere=(rhcp *)realloc(transfere,transferencia*sizeof(rhcp));
                    if(transfere!=NULL){
                        transfere[transferencia-1].deck=mesa->jogada[p-jogador[posicao].manga].deck;
                        transfere[transferencia-1].grupo=0;
                        usada[p]++;
                        printf("\nArea de transferencia:\n\n|");
                        for(int i=0;i<transferencia;i++) printf(" %d - %c%c |",i,carta[transfere[i].deck].valor,carta[transfere[i].deck].naipe);
                        printf("\n");
                    }else printf("ERRO!\n");
                }
            }
        }
        if(op==6){
            if(transfere!=NULL){
                printf("\nArea de transferencia:\n\n|");
                for(int i=0;i<transferencia;i++) printf(" %d - %c%c |",i,carta[transfere[i].deck].valor,carta[transfere[i].deck].naipe);
                if(validaJogada(carta,transfere,transferencia,validturno)){
                    // TENHO QUE RETIRAR OS QUE ESTAVAM NA MESA P/ JOGAR NOVAMENTE
                    // MAS PRECISO MUDAR A COMBINAÇÃO DAS QUE JÁ ESTAVAM NA MESA
                    if(transferenciamesa>0){
                        for(int i=0;i<transferencia;i++){
                            for(int busca=0;busca<mesa->quantidadejogada;busca++){
                                if(transfere[i].deck==mesa->jogada[busca].deck){
                                    mesa->jogada[busca].deck=-1;
                                    break;
                                }
                            }
                        }
                        rhcp *hand=(rhcp *)malloc(mesa->quantidadejogada*sizeof(rhcp));
                        for(int i=0;i<mesa->quantidadejogada;i++) hand[i]=mesa->jogada[i];
                        mesa->jogada=(rhcp *)realloc(mesa->jogada,(mesa->quantidadejogada-transferenciamesa)*sizeof(rhcp));
                        for(int i=0,j=0;i<mesa->quantidadejogada;i++){
                            if(hand[i].deck!=-1){
                                mesa->jogada[j]=hand[i];
                                j++;
                            }
                        }
                        free(hand);
                        mesa->quantidadejogada-=transferenciamesa;
                    }
                    mesa->jogada=(rhcp *)realloc(mesa->jogada,(mesa->quantidadejogada+transferencia)*sizeof(rhcp));
                    mesa->trucos++;
                    for(int i=mesa->quantidadejogada;i<(mesa->quantidadejogada+transferencia);i++){
                        mesa->jogada[i].deck=transfere[i-mesa->quantidadejogada].deck;
                        mesa->jogada[i].grupo=mesa->trucos;
                    }
                    mesa->quantidadejogada+=transferencia;
                    vizualizaMesa(carta,mesa,ncartas);
                    int j=0;
                    for(int i=0;i<jogador[posicao].manga;i++){
                        if(!usada[i]){
                            jogador[posicao].mao[j]=jogador[posicao].mao[i];
                            j++;
                        }
                    }
                    jogador[posicao].manga=j;
                    jogador[posicao].mao=(int *)realloc(jogador[posicao].mao,jogador[posicao].manga*sizeof(int));
                    free(transfere);
                    transfere=NULL;
                    transferencia=0;
                    transferenciamesa=0;
                    free(usada);
                    usada=(int*)calloc((jogador[posicao].manga+mesa->quantidadejogada),sizeof(int));
                    validturno++;
                }else{
                    printf("\nJogada invalida!\n");
                }
            }else printf("Area de transferencia vazia!\n");
        }
        if(op==7){
            if(transfere!=NULL){
                free(transfere);
                transfere=NULL;
                transferencia=0;
                transferenciamesa=0;
                free(usada);
                usada=(int*)calloc((jogador[posicao].manga+mesa->quantidadejogada),sizeof(int));
                printf("Area de transferencia limpa!\n");
            }else printf("Area de transferencia vazia!\n");
        }
        if(op==8){
            printf("Desfazer jogada!\n");
            jogador[posicao].manga=backup_manga;
            jogador[posicao].turno=validturno;
            jogador[posicao].mao=(int *)realloc(jogador[posicao].mao,backup_manga*sizeof(int)); // MÃO DO JOGADOR RETORNA PARA O TAMANHO ORIGINAL
            for(int i=0;i<backup_manga;i++) jogador[posicao].mao[i]=backup_mao[i]; // CÓPIA DO VALOR DE CADA CARTA DO BACKUP P/ A MÃO
            mesa->quantidadejogada=backup_qtdmesa;
            mesa->trucos=backup_trucos;
            mesa->jogada=(rhcp *)realloc(mesa->jogada,backup_qtdmesa*sizeof(rhcp));
            for(int i=0;i<backup_qtdmesa;i++){
                mesa->jogada[i].deck=backup_mesa[i].deck;
                mesa->jogada[i].grupo=backup_mesa[i].grupo;
            }
            free(transfere);
            transfere=NULL;
            transferencia=0;
            transferenciamesa=0;
            free(usada);
            usada=(int*)calloc((jogador[posicao].manga+mesa->quantidadejogada),sizeof(int));
            printf("Undo!\n");
            vizualizaMesa(carta,mesa,ncartas);
        }
        if(op==9){
            if(backup_manga==jogador[posicao].manga) printf("O jogador ainda nao realizou nenhum movimento!\n");
            else if(validaMesa(mesa,carta,validturno)){
                break;
            }else printf("Mesa invalida!\n");
        }
        menuTurn();
        scanf("%d",&op);
        setbuf(stdin,NULL);
        while(op<1||op>9){
            printf("Opcao invalida!\n<?> ");
            scanf("%d",&op);
            setbuf(stdin,NULL);
        }
    }
    printf("\nFim de turno!\n\n");
    jogador[posicao].turno=validturno;
    free(backup_mao);
    free(backup_mesa);
    free(usada);
    free(transfere);
    return ncartas;
}

int main(){
    srand(time(NULL));
    pilha *carta = (pilha *)malloc(106*sizeof(pilha));
    if(!carta) return 0;
    tabela *mesa = (tabela *)malloc(sizeof(tabela));
    if(!mesa) return 0;
    mesa->jogada=NULL;
    mesa->quantidadejogada=0;
    mesa->trucos=0;
    FILE *arquivo;
    int nplayers,ncartas=106;
    printf("Entre com o numero de jogadores (1 a 5): ");
    scanf("%d",&nplayers);
    setbuf(stdin,NULL);
    while(nplayers<1||nplayers>5){
        printf("Opcao invalida!\n<?> ");
        scanf("%d",&nplayers);
        setbuf(stdin,NULL);
    }
    player *jogador = (player *)malloc(nplayers*sizeof(player));
    if(!jogador) return 0;
    for(int i=0;i<nplayers;i++) jogador[i].mao=NULL;
    printf("Escolha o modo de jogo(1. Ler baralho / 2. Inicializar baralho): ");
    int p;
    scanf("%d",&p);
    setbuf(stdin,NULL);
    while(p<1||p>2){
        printf("Opcao invalida!\n<?> ");
        scanf("%d",&p);
        setbuf(stdin,NULL);
    }
    if(p==1){
        char baralho[15];
        printf("Entre com o nome do arquivo: ");
        fgets(baralho,15,stdin);
        setbuf(stdin,NULL);
        baralho[strlen(baralho)-1]='\0';
        strcat(baralho,".txt");
        arquivo=fopen(baralho,"rt");
        if(arquivo){
            printf("\nLendo baralho!\n");
            lePilha(arquivo,carta);
        }else{
            printf("\nErro ao ler baralho!\n");
            return 0;
        }
    }else if(p==2){
        inicializaPilha(carta);
    }
    ncartas=distribuiAlternado(carta,jogador,nplayers,ncartas);
    p=0;
    while(1){
        ncartas=turn(carta,jogador,mesa,nplayers,p,ncartas);
        if(jogador[p].manga==0){
            printf("Jogador %d ganhou!",p);
            return 0;
        }else if(!ncartas){
            printf("Fim de jogo!");
            int *finalpontos=(int *)calloc(nplayers,sizeof(int));
            int *posicao=(int *)calloc(nplayers,sizeof(int));
            for(int i=0;i<nplayers;i++){
                for(p=0;p<jogador[i].manga;p++){
                    if(carta[jogador[i].mao[p]].valor=='*')   finalpontos[i]+=20;
                    else if(carta[jogador[i].mao[p]].valor<58) finalpontos[i]+=((int)carta[jogador[i].mao[p]].valor-48);
                    else finalpontos[i]+=(int)carta[jogador[i].mao[p]].valor-55;
                }
                posicao[i]=i;
                printf("\nPontos do jogador %d: %d\n",posicao[i],finalpontos[i]);
            }
            int menor,auxiliar,j,poker;
            for(int i=0;i<nplayers;i++){
                menor=i;
                for(j=i+1;j<nplayers;j++){
                    if(finalpontos[j]<finalpontos[menor]) menor=j;
                }
                auxiliar=finalpontos[menor];
                poker=posicao[menor];
                finalpontos[menor]=finalpontos[i];
                posicao[menor]=posicao[i];
                finalpontos[i]=auxiliar;
                posicao[i]=poker;
            }
            if(finalpontos[0]==finalpontos[1]) printf("\nEmpate!!!\n");
            else printf("\nCAMPEAO: Jogador %d !!!\n",posicao[0]);
            free(finalpontos);
            free(posicao);
            free(carta);
            free(mesa);
            free(jogador);
            return 42;
        }
        p++;
        if(p==nplayers) p=0;
    }
    free(carta);
    free(mesa);
    free(jogador);
    return 42;
}
