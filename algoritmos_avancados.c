#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10
#define TAM_NOME 50
#define TAM_PISTA 100

// ==================== ESTRUTURAS ====================

// Estrutura para representar um cômodo da mansão (árvore binária)
typedef struct Sala {
    char nome[TAM_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para armazenar pistas coletadas (BST)
typedef struct NoPista {
    char pista[TAM_PISTA];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Estrutura para a tabela hash (pista -> suspeito)
typedef struct NoHash {
    char pista[TAM_PISTA];
    char suspeito[TAM_NOME];
    struct NoHash *proximo;
} NoHash;

typedef struct {
    NoHash *tabela[TAM_HASH];
} TabelaHash;

// ==================== FUNÇÕES DE SALA ====================

/**
 * criarSala() - Cria dinamicamente um cômodo da mansão
 * @nome: nome identificador do cômodo
 * Retorna: ponteiro para a nova sala criada
 * 
 * Aloca memória dinamicamente para um novo nó da árvore binária
 * que representa um cômodo da mansão, inicializando seus campos.
 */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ==================== FUNÇÕES DE PISTAS (BST) ====================

/**
 * inserirPista() - Insere a pista coletada na árvore BST
 * @raiz: ponteiro para a raiz da árvore de pistas
 * @pista: texto da pista a ser inserida
 * Retorna: ponteiro atualizado da raiz
 * 
 * Insere uma nova pista na árvore binária de busca de forma ordenada,
 * mantendo a propriedade BST onde elementos menores ficam à esquerda
 * e maiores à direita.
 */
NoPista* inserirPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        NoPista *novaPista = (NoPista*)malloc(sizeof(NoPista));
        if (novaPista == NULL) {
            printf("Erro ao alocar memoria!\n");
            exit(1);
        }
        strcpy(novaPista->pista, pista);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se a pista já existe, não insere novamente
    
    return raiz;
}

// Função auxiliar para exibir todas as pistas coletadas (em ordem)
void exibirPistas(NoPista *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ==================== FUNÇÕES DE TABELA HASH ====================

// Função hash simples baseada na soma dos caracteres
int funcaoHash(const char *chave) {
    int hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash += chave[i];
    }
    return hash % TAM_HASH;
}

// Inicializa a tabela hash com todas as posições NULL
void inicializarHash(TabelaHash *th) {
    for (int i = 0; i < TAM_HASH; i++) {
        th->tabela[i] = NULL;
    }
}

/**
 * inserirNaHash() - Insere associação pista/suspeito na tabela hash
 * @th: ponteiro para a tabela hash
 * @pista: chave (pista encontrada)
 * @suspeito: valor (nome do suspeito relacionado)
 * 
 * Adiciona um novo par chave-valor na tabela hash, tratando colisões
 * através de encadeamento (lista ligada).
 */
void inserirNaHash(TabelaHash *th, const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    NoHash *novo = (NoHash*)malloc(sizeof(NoHash));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = th->tabela[indice];
    th->tabela[indice] = novo;
}

/**
 * encontrarSuspeito() - Consulta o suspeito correspondente a uma pista
 * @th: ponteiro para a tabela hash
 * @pista: chave a ser buscada
 * Retorna: nome do suspeito ou NULL se não encontrado
 * 
 * Busca na tabela hash o suspeito associado a uma pista específica,
 * percorrendo a lista encadeada em caso de colisão.
 */
char* encontrarSuspeito(TabelaHash *th, const char *pista) {
    int indice = funcaoHash(pista);
    NoHash *atual = th->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// ==================== SISTEMA DE PISTAS POR SALA ====================

// Define qual pista está associada a cada sala da mansão
const char* obterPistaDaSala(const char *nomeSala) {
    if (strcmp(nomeSala, "Hall de Entrada") == 0) return "Pegadas lamacentas";
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Livro de venenos aberto";
    if (strcmp(nomeSala, "Sala de Jantar") == 0) return "Copo com residuo suspeito";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca ensanguentada";
    if (strcmp(nomeSala, "Escritorio") == 0) return "Carta de ameaca";
    if (strcmp(nomeSala, "Quarto Principal") == 0) return "Cofre arrombado";
    if (strcmp(nomeSala, "Jardim") == 0) return "Luvas sujas de terra";
    if (strcmp(nomeSala, "Adega") == 0) return "Garrafa de vinho rara";
    if (strcmp(nomeSala, "Salao de Festas") == 0) return "Lenco com iniciais";
    if (strcmp(nomeSala, "Porao") == 0) return "Ferramenta de arrombamento";
    return NULL;
}

// ==================== EXPLORAÇÃO ====================

/**
 * explorarSalas() - Navega pela árvore e ativa o sistema de pistas
 * @salaAtual: ponteiro para a sala onde o jogador está
 * @pistasColetadas: ponteiro para a raiz da BST de pistas
 * @th: ponteiro para a tabela hash
 * Retorna: árvore BST atualizada com novas pistas
 * 
 * Função recursiva que permite ao jogador explorar a mansão interativamente,
 * coletando pistas e armazenando-as na BST. O jogador pode navegar para
 * esquerda, direita ou sair para fazer a acusação final.
 */
NoPista* explorarSalas(Sala *salaAtual, NoPista *pistasColetadas, TabelaHash *th) {
    if (salaAtual == NULL) {
        printf("\nVoce chegou a um beco sem saida. Voltando...\n");
        return pistasColetadas;
    }
    
    printf("\n========================================\n");
    printf("Voce esta em: %s\n", salaAtual->nome);
    
    // Verifica se há pista nesta sala
    const char *pistaEncontrada = obterPistaDaSala(salaAtual->nome);
    if (pistaEncontrada != NULL) {
        printf("PISTA ENCONTRADA: %s\n", pistaEncontrada);
        
        // Adiciona à BST de pistas
        pistasColetadas = inserirPista(pistasColetadas, pistaEncontrada);
        printf("Pista adicionada ao seu inventario!\n");
    } else {
        printf("Nenhuma pista encontrada aqui.\n");
    }
    
    // Menu de navegação
    printf("\nPara onde deseja ir?\n");
    if (salaAtual->esquerda != NULL) {
        printf("  [E] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("  [D] Direita -> %s\n", salaAtual->direita->nome);
    }
    printf("  [S] Sair e fazer acusacao\n");
    printf("Escolha: ");
    
    char escolha;
    scanf(" %c", &escolha);
    
    if (escolha == 'e' || escolha == 'E') {
        if (salaAtual->esquerda != NULL) {
            pistasColetadas = explorarSalas(salaAtual->esquerda, pistasColetadas, th);
        } else {
            printf("\nNao ha caminho a esquerda!\n");
        }
        // Retorna à sala atual para permitir explorar outro caminho
        pistasColetadas = explorarSalas(salaAtual, pistasColetadas, th);
    } else if (escolha == 'd' || escolha == 'D') {
        if (salaAtual->direita != NULL) {
            pistasColetadas = explorarSalas(salaAtual->direita, pistasColetadas, th);
        } else {
            printf("\nNao ha caminho a direita!\n");
        }
        // Retorna à sala atual para permitir explorar outro caminho
        pistasColetadas = explorarSalas(salaAtual, pistasColetadas, th);
    }
    // Se escolheu 'S', apenas retorna para encerrar a exploração
    
    return pistasColetadas;
}

// ==================== VERIFICAÇÃO FINAL ====================

// Conta recursivamente quantas pistas apontam para um suspeito específico
int contarPistasPorSuspeito(NoPista *raiz, TabelaHash *th, const char *suspeitoAlvo) {
    if (raiz == NULL) return 0;
    
    int count = 0;
    char *suspeito = encontrarSuspeito(th, raiz->pista);
    
    if (suspeito != NULL && strcmp(suspeito, suspeitoAlvo) == 0) {
        count = 1;
    }
    
    count += contarPistasPorSuspeito(raiz->esquerda, th, suspeitoAlvo);
    count += contarPistasPorSuspeito(raiz->direita, th, suspeitoAlvo);
    
    return count;
}

/**
 * verificarSuspeitoFinal() - Conduz à fase de julgamento final
 * @pistasColetadas: árvore BST com todas as pistas coletadas
 * @th: tabela hash com associações pista-suspeito
 * 
 * Permite ao jogador fazer uma acusação final e verifica se há evidências
 * suficientes (pelo menos 2 pistas) para sustentar a acusação. Exibe o
 * resultado da investigação.
 */
void verificarSuspeitoFinal(NoPista *pistasColetadas, TabelaHash *th) {
    printf("\n========================================\n");
    printf("       FASE DE JULGAMENTO\n");
    printf("========================================\n");
    printf("\nPistas coletadas durante a investigacao:\n");
    exibirPistas(pistasColetadas);
    
    printf("\n\nSuspeitos disponiveis:\n");
    printf("  1. Lady Scarlet\n");
    printf("  2. Professor Plum\n");
    printf("  3. Colonel Mustard\n");
    printf("  4. Mrs. White\n");
    
    printf("\nQuem voce acusa? (digite o nome completo): ");
    char acusado[TAM_NOME];
    scanf(" %[^\n]", acusado);
    
    // Verifica quantas pistas apontam para o acusado
    int numPistas = contarPistasPorSuspeito(pistasColetadas, th, acusado);
    
    printf("\n========================================\n");
    printf("     RESULTADO DA INVESTIGACAO\n");
    printf("========================================\n");
    printf("Acusado: %s\n", acusado);
    printf("Pistas que apontam para o suspeito: %d\n\n", numPistas);
    
    if (numPistas >= 2) {
        printf("*** CASO RESOLVIDO! ***\n");
        printf("Evidencias suficientes! %s foi considerado culpado!\n", acusado);
        printf("Parabens, detetive! Voce solucionou o caso!\n");
    } else {
        printf("*** CASO NAO RESOLVIDO! ***\n");
        printf("Evidencias insuficientes para acusar %s.\n", acusado);
        printf("O culpado escapou... Tente novamente!\n");
    }
}

// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    printf("========================================\n");
    printf("  DETECTIVE QUEST\n");
    printf("  O Misterio da Mansao\n");
    printf("========================================\n");
    printf("\nBem-vindo, detetive!\n");
    printf("Um crime foi cometido nesta mansao.\n");
    printf("Explore os comodos, colete pistas e descubra o culpado!\n");
    
    // ===== CRIAR ESTRUTURA DA MANSÃO (Árvore Binária) =====
    Sala *hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Jantar");
    hall->esquerda->esquerda = criarSala("Escritorio");
    hall->esquerda->direita = criarSala("Quarto Principal");
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Jardim");
    hall->esquerda->esquerda->esquerda = criarSala("Adega");
    hall->direita->direita->direita = criarSala("Porao");
    hall->esquerda->direita->direita = criarSala("Salao de Festas");
    
    // ===== INICIALIZAR ESTRUTURAS DE DADOS =====
    NoPista *pistasColetadas = NULL;
    TabelaHash tabelaHash;
    inicializarHash(&tabelaHash);
    
    // ===== POPULAR TABELA HASH (pista -> suspeito) =====
    inserirNaHash(&tabelaHash, "Pegadas lamacentas", "Colonel Mustard");
    inserirNaHash(&tabelaHash, "Livro de venenos aberto", "Professor Plum");
    inserirNaHash(&tabelaHash, "Copo com residuo suspeito", "Professor Plum");
    inserirNaHash(&tabelaHash, "Faca ensanguentada", "Lady Scarlet");
    inserirNaHash(&tabelaHash, "Carta de ameaca", "Mrs. White");
    inserirNaHash(&tabelaHash, "Cofre arrombado", "Colonel Mustard");
    inserirNaHash(&tabelaHash, "Luvas sujas de terra", "Colonel Mustard");
    inserirNaHash(&tabelaHash, "Garrafa de vinho rara", "Lady Scarlet");
    inserirNaHash(&tabelaHash, "Lenco com iniciais", "Mrs. White");
    inserirNaHash(&tabelaHash, "Ferramenta de arrombamento", "Lady Scarlet");
    
    // ===== INICIAR EXPLORAÇÃO INTERATIVA =====
    printf("\n\nPressione ENTER para comecar a investigacao...");
    getchar();
    
    pistasColetadas = explorarSalas(hall, pistasColetadas, &tabelaHash);
    
    // ===== FASE DE ACUSAÇÃO FINAL =====
    verificarSuspeitoFinal(pistasColetadas, &tabelaHash);
    
    printf("\n========================================\n");
    printf("Obrigado por jogar Detective Quest!\n");
    printf("========================================\n");
    
    return 0;
} 