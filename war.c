#include <stdio.h>
#include <stdlib.h> // Necessária para malloc, calloc e free
#include <string.h> // Necessária para manipular textos (strcpy, strcmp)
#include <time.h>   // Necessária para gerar números aleatórios baseados no tempo

// Definição da "Ficha" de cada território
typedef struct {
    char nome[30];
    char cor[20];
    int tropas;
} Territorio;

// --- PROTÓTIPOS DAS FUNÇÕES ---
// Avisamos o programa que estas funções existem abaixo da main
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao); 
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missao);

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    int numTerritorios = 4;
    char* corDoJogador = "Verde";
    
    // Vetor de strings com as descrições das missões
    char* bancoDeMissoes[] = {
        "Conquistar 3 territorios",
        "Eliminar as tropas da cor Vermelha",
        "Conquistar o territorio de Madagascar",
        "Ter mais de 10 tropas em um unico territorio",
        "Dominar territorios de duas cores diferentes"
    };

    // ALOCAÇÃO DINÂMICA: Reservando memória RAM manualmente
    // calloc reserva espaço para 4 territórios e limpa a memória
    Territorio* mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    char* missaoJogador = NULL; // Ponteiro que guardará o texto da missão

    // Preenchendo os dados iniciais do mapa
    strcpy(mapa[0].nome, "Brasil");     strcpy(mapa[0].cor, "Verde");    mapa[0].tropas = 10;
    strcpy(mapa[1].nome, "Argentina");  strcpy(mapa[1].cor, "Azul");     mapa[1].tropas = 5;
    strcpy(mapa[2].nome, "Madagascar"); strcpy(mapa[2].cor, "Vermelha"); mapa[2].tropas = 3;
    strcpy(mapa[3].nome, "Egito");      strcpy(mapa[3].cor, "Amarela");  mapa[3].tropas = 4;

    // Sorteia a missão (passamos o endereço do ponteiro para ele ser preenchido)
    atribuirMissao(&missaoJogador, bancoDeMissoes, 5);

    printf("========================================\n");
    printf("           WAR: MISSOES C               \n");
    printf("========================================\n");
    
    exibirMissao(missaoJogador);

    int jogoAtivo = 1;
    while (jogoAtivo) {
        exibirMapa(mapa, numTerritorios);

        int idxAtacante, idxDefensor;
        
        printf("\n--- SEU TURNO (Sua Cor: %s) ---\n", corDoJogador);
        
        // Mostra apenas os territórios que pertencem ao jogador
        printf("Seus territorios disponiveis: ");
        for(int i=0; i < numTerritorios; i++) {
            if(strcmp(mapa[i].cor, corDoJogador) == 0) printf("[%d:%s] ", i, mapa[i].nome);
        }

        printf("\nEscolha o ID do SEU territorio para iniciar o ataque (-1 para sair): ");
        if (scanf("%d", &idxAtacante) != 1) break;

        if (idxAtacante == -1) break;

        // Validação: O território escolhido precisa ser do jogador
        if (idxAtacante < 0 || idxAtacante >= numTerritorios || strcmp(mapa[idxAtacante].cor, corDoJogador) != 0) {
            printf("\n!!! ERRO: Escolha um ID da lista de SEUS territorios acima !!!\n");
            continue;
        }
        
        // Regra: Não pode atacar se tiver apenas 1 tropa
        if (mapa[idxAtacante].tropas <= 1) {
            printf("\n!!! ERRO: O territorio %s so tem 1 tropa. Precisa de 2 ou mais para atacar !!!\n", mapa[idxAtacante].nome);
            continue;
        }

        // Mostra alvos inimigos
        printf("\nAlvos INIMIGOS disponiveis: ");
        for(int i=0; i < numTerritorios; i++) {
            if(strcmp(mapa[i].cor, corDoJogador) != 0) printf("[%d:%s] ", i, mapa[i].nome);
        }
        printf("\nEscolha o ID do territorio INIMIGO que deseja invadir: ");
        scanf("%d", &idxDefensor);

        // Validação: Não pode atacar a si mesmo
        if (idxDefensor < 0 || idxDefensor >= numTerritorios || strcmp(mapa[idxDefensor].cor, corDoJogador) == 0) {
            printf("\n!!! ERRO: Escolha um ID de um territorio INIMIGO !!!\n");
            continue;
        }

        // Realiza o ataque (passagem por referência usando ponteiros)
        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

        // Verifica se a missão sorteada no início foi cumprida
        if (verificarMissao(missaoJogador, mapa, numTerritorios, corDoJogador)) {
            printf("\n========================================\n");
            printf("!!! MISSAO CUMPRIDA: VITORIA !!!\n");
            printf("Objetivo alcancado: [%s]\n", missaoJogador);
            printf("========================================\n");
            exibirMapa(mapa, numTerritorios);
            jogoAtivo = 0;
        }
    }

    // LIBERAÇÃO DE MEMÓRIA: Importante para não travar o computador (memory leak)
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// Função que sorteia e aloca memória para o texto da missão
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    // malloc reserva o tamanho exato do texto da missão sorteada
    *destino = (char*)malloc((strlen(missoes[sorteio]) + 1) * sizeof(char));
    if (*destino != NULL) {
        strcpy(*destino, missoes[sorteio]); // Copia o texto para a memória alocada
    }
}

void exibirMissao(char* missao) {
    printf("SUA MISSAO SECRETA: %s\n", missao);
}

// Lógica de batalha usando dados (1 a 6)
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n>>> BATALHA: %s (%d) vs %s (%d) <<<\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("RESULTADO: Vitoria! %s agora e seu!\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor); // O território muda de cor
        
        // Transfere metade das tropas do atacante para o novo território
        int movidas = atacante->tropas / 2;
        defensor->tropas = movidas;
        atacante->tropas -= movidas;
    } else {
        printf("RESULTADO: Derrota! Perdeu 1 tropa em %s.\n", atacante->nome);
        atacante->tropas--; // O atacante perde soldados se falhar
    }
}

// Função que checa as condições de vitória
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    int contagem = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) contagem++;
    }

    // Lógica para missão de "3 territórios"
    if (strstr(missao, "3 territorios") && contagem >= 3) return 1;
    
    // Lógica para missão de conquistar Madagascar
    if (strstr(missao, "Madagascar")) {
        for(int i=0; i<tamanho; i++) {
            if(strcmp(mapa[i].nome, "Madagascar") == 0 && strcmp(mapa[i].cor, corJogador) == 0) return 1;
        }
    }
    return 0; // Ainda não venceu
}

void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n========== MAPA ATUAL ==========\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID %d: [%-10s] | Cor: %-10s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("================================\n");
}

void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);   // Devolve a memória do mapa ao sistema
    free(missao); // Devolve a memória da string de missão
}
