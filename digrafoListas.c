#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */

//NAMES: HENRIQUE RACHTI ASSUMPÇÃO 
//		BRUNO CESAR ROCHA SALGADO 

typedef struct _TNo{
	int w;
	struct _TNo *prox;	
}TNo;
typedef struct{
	int V; // quantidade de vértices
	int A; // quantidade de arcos
	TNo **adj; //lista de adjacencia
}TDigrafo;

// declaracao de funcoes
TDigrafo * Init( int V ); // ok
void insereA( TDigrafo *D, int v, int w); // reescrever
void removeA(TDigrafo *D, int v, int w); // fazer
void show(TDigrafo *D); //ok
void libera(TDigrafo *D); // liberar a lista
int indeg(TDigrafo *D, int v); // fazer
int outdeg(TDigrafo *D, int v); // fazer

int main( void ){
	
	lerArquivo();
	
	printf("\n\n\nPress ENTER key to Continue\n");  
	getchar(); 
	return 0;
	
}
// Função cria e devolve uma nova estrutura TDigrafo,
// inicializa os atributos da estrutura, 
// cria uma lista de adjacência para V vértices 

int lerArquivo(TDigrafo *D){
	
	char url[]="ordtopo.txt";
	FILE *arq;	
	int n, arcos;
	arq = fopen(url, "r");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		fscanf(arq,"%d", &n);//QUANTIDADE DE VERTICES	
		D = Init(n+1);	
		int i=1;
		char tarefas[n+1][100];
		
		for(;i<n+1;i++){
		fscanf(arq,"%s", tarefas[i]);//LE O NOME DAS TAREFAS
		}
		
		fscanf(arq,"%d", &n);//QUANTIDAD DE ARCOS
		int v,w;
		for(i=0;i<n;i++){
		fscanf(arq,"%d %d", &v, &w);
		insereA(D, v, w);//INSERE OS ARCOS, QUANDO NAO REPETIDOS
		}
		verCiclo(D);//VERIFICA OS CICLOS E REMOVE ELES
		ordenaTopologicamente(D,tarefas);//ORDENA TOPOLOGICAMENTE
	
	}

	fclose(arq);
}

TDigrafo * Init( int V ){
	TDigrafo *D;
	int i;
	// aloca uma estrutura do tipo TDigrafo
	// http://www.cplusplus.com/reference/cstdlib/calloc/
	//printf("tamanho estrutura %d",sizeof(TDigrafo*));
	D = (TDigrafo *) calloc(1, sizeof(TDigrafo) );

	// D = new TDigrafo( V )
	// atribui o numero de vertices para a estrtura
	D->V = V;
	// zero numero arcos
	D->A = 0;
	// aloca as entradas para a lista de adjacencia
	D->adj = (TNo ** )calloc(V,sizeof(TNo*));
	
	return D;
	
}


void insereA( TDigrafo *D, int v, int w){

	TNo *aux = D->adj[v];
	TNo *ant=NULL;
	while( aux ){
		// verifica se já existe o arco v-w
		if( aux->w == w){
			printf("Tarefas (%d,%d) ja inseridas !",v,w);			
			return;
		}
		ant = aux;
		aux = aux->prox;
	}
	// so para testar vou inserir w no inicio de adj[v]
	TNo *novo = (TNo*)calloc(1,sizeof(TNo));
	novo->w = w;
	novo->prox = aux;
	if( ant ) // se existe anterior
		// insere no final
		ant->prox = novo;
	else
		// insere no inicio
		D->adj[v] = novo;
	

	(D->A)++;
}
/*
 Função remove do digrafo o arco que tem ponta inicial v e 
 ponta final w. A função supõe que v e w são distintos, 
 positivos e menores que V. Se não existe arco v-w, 
 método não faz nada.
*/
void removeA(TDigrafo *D, int v, int w){
	// verifica se existe o arco v-w
	/*if( D->adj[v][w]==1){
		D->adj[v][w] = 0;
		(D->A)--; // decrementa o numero de arcos
	}
	*/
	TNo *aux = D->adj[v];
	TNo *ant=NULL;
		// verifica se já existe o arco v-w
		while( aux ){
		if( aux->w == w){
			if(ant)
			ant->prox = aux->prox;
			else
			D->adj[v] = aux->prox;
			return;
		}			
		ant = aux;
		aux = aux->prox;
		
	}
}
/*
Para cada vértice v do digrafo, este método imprime, 
em uma linha, todos os vértices adjacentes a v.
*/ 
void show(TDigrafo *D){
	int v,w;
	
	for( v = 0; v < D->V; v++){ // vetor de listas
		printf("\n%d:",v);
		TNo *aux = D->adj[v];
		while( aux ){ // anda na lista
			printf(" %d",aux->w);
			aux = aux->prox;
		}
	}
}
/*
Função desaloca a memória utilizado pelo dígrafo 
e pela matriz de adjacência.
*/ 
void libera(TDigrafo *D){
	
	int i;
	for(i=0;i<D->V;i++)
		free(D->adj[i]); // desloca linha
	
	// desaloca matriz
	free( D->adj );
	
	// desaloca digrafo
	//free( D );
	
}

/*
Calcula grau de entrada
*/ 
int indeg(TDigrafo *D, int i){
	int v, grau=0;
	
	//for( i=0, grau=0; i < D->V; grau+=D->adj[i][v],i++);
	for(v = 0; v < D->V; v++){ // vetor de listas
		TNo *aux = D->adj[v];
		while( aux ){ // anda na lista
			if(aux->w == i){
				grau++;
			}
			aux = aux->prox;
		}
	}
	
	return grau;
}

/*
Calcula grau de saida
*/ 
int outdeg(TDigrafo *D, int v){
	int i, grau=0;
	
	//for( i=0, grau=0; i < D->V; grau+=D->adj[i][v],i++);
	TNo *aux = D->adj[v];
		while( aux ){ // anda na lista
			grau++;
			
			aux = aux->prox;
		}
	
	return grau;
}
/*
Verifica se existe um caminho entre dois vertices
*/ 
int verCaminho(TDigrafo *D, int v, int t){
	int verificados[D->V];
	int caminho[D->V];
	caminho[0]=v;
	int resultado=0;
	//for( i=0, grau=0; i < D->V; grau+=D->adj[i][v],i++);
	TNo *aux = D->adj[v];
	TNo *verif = NULL;
		while(aux&&resultado==0){ 
			printf("\n: %d",aux->w);
			if(D->adj[aux->w]){
			verif = D->adj[aux->w];	
			verificados[aux->w]=1;	
			caminho[1]=aux->w;		
			resultado=BuscaProf(D, verif,t,verificados,caminho, 2);
			}
			aux=aux->prox;	
		}
	if(resultado==0)
	printf("\nNAO ACHOU");
	return resultado;
}

int BuscaProf(TDigrafo *D, TNo *No, int t, int verificados[], int caminho[], int posicao){
	verificados[No->w]=1;
	caminho[posicao]=No->w;
	posicao++;

	int resultado;
	TNo *verif = NULL;
	
	if(No->w==t){
	printf("\nACHOU");	
	int i;
	printf("\nCAMINHO: ");
	for(i=0;i<posicao;i++){
	printf("%d ",caminho[i]);
	}
	return 1;
	}
	
	while(No&&No->w){
	printf("\nIndex: %d",No->w);
	verif = D->adj[No->w];
	if(verificados[No->w]!=1){	
	resultado=BuscaProf(D, verif,t,verificados,caminho,posicao);
	}
	No=No->prox;
	}

	return 0;
}

int verifCicloSeq(int seq[], int n){
	int i,j;
	if(seq[0]==seq[n-1]){
	printf("\nO %d e o %d sao iguais",0,n-1);
	return 0;	
	}
	for(i=0;i<n;i++)
		for(j=i+1;j<n;j++)
			if(seq[i]==seq[j]){
				printf("\nO %d e o %d sao iguais",i,j);
				return 0;
			}
	
	printf("NAO TEM CICLO");
	
}

int verCiclo(TDigrafo *D){
	int i, j;
	int verificados[D->V];	
	verificados[0]=0;	
		verificados[1]=0;
		verificados[2]=0;
		verificados[3]=0;
		verificados[4]=0;
	verificados[5]=0;
	verificados[6]=0;
	verificados[7]=0;
	verificados[8]=0;
	verificados[9]=0;
	verificados[10]=0;
	verificados[11]=0;
	verificados[12]=0;
	
	for(i=0;i<D->V;i++){
		
	
		
		
		
		//printf("\nINICIO : %d",i);
		
		if(verificados[i]==0)
		BuscaCiclo(D,i, i, verificados,-1);
		
	
	
	}	
	
	return 0;
}



int BuscaCiclo(TDigrafo *D, int i, int inicio, int verificados[], int ant){
	
	 
        verificados[inicio] = 1; 
     //   printf("\n %d verificado",inicio);
        TNo *No = D->adj[inicio];
        while(No)
        {	
        	//printf("\n No de inicio- %d",i);	
			//printf("\n No passado pelo while- %d",No->w);	
			
		//	printf("\n %d foi verificado?",No->w);
		//	printf("\n O valor verificado de %d eh %d",No->w,verificados[No->w]);
            if (verificados[No->w]==0){
		//	printf("\n %d Nao foi verificado",No->w);
			BuscaCiclo(D, i, No->w, verificados, inicio);
		}		
            else if(i==No->w){
        //    	printf("\n %d foi verificado",No->w);
        //    printf("\n%d eh igual a  %d? ",i,No->w);
			printf("\nTarefas (%d,%d) formam ciclos ! ",inicio,i);
			removeA(D,inicio,i);
			return 1;
			}
            else{
          //  	printf("\n %d foi verificado",No->w);
          //  	printf("\n%d n eh igual a  %d? ",i,No->w);	
			}   
            No=No->prox;
          //  printf("\n No incrementado"); 
        }
 
    
    
    return 0;
}

   int ordenaTopologicamente(TDigrafo *D,char tarefas[][100]) {
    int verificados[D->V];    
    int i, continua=1, indegCount=0;
    for(i=0;i<D->V;i++)
    	verificados[i]=0;
    
   
   	while(continua==1){
	  continua=0;
	  
	for(i=1;i<D->V;i++){
		
		//printf("\nINDEG DE %d eh %d",i,indeg(D, i));
		if(verificados[i]==0&&indeg(D, i)==indegCount){
		   continua=1;
		   printf("\n%d: %s",i,tarefas[i]);
		   verificados[i]=1;
		}

	}
 			indegCount++;
 			
     }
}




		

