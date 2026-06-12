#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* =========================================
   ESTRUTURA DE DADOS PRINCIPAL
   Representa um estudante com matrícula,
   nome e nota. É a base de todo o sistema.
   ========================================= */
typedef struct {
    int matricula;      /* Número único de identificação do estudante */
    char nome[100];     /* Nome completo (máximo 99 caracteres + '\0') */
    float nota;         /* Nota entre 0.0 e 20.0 */
} Estudante;

/* ---- Declarações antecipadas das funções (protótipos) ---- */
void lerDadosFicheiro(Estudante **v, int *n, char *nomeOriginal);
void exibirEstudantes(Estudante v[], int n);
void adicionarEstudante(Estudante **v, int *n);
void menuOrdenacao(Estudante v[], int n);
void bubbleSortMatricula(Estudante v[], int n);
void selectionSortNome(Estudante v[], int n);
void insertionSortNota(Estudante v[], int n);
void menuPesquisa(Estudante v[], int n);
void pesquisaLinearNome(Estudante v[], int n);
void pesquisaBinariaMatricula(Estudante v[], int n);
void menuGravarFicheiro(Estudante v[], int n, char *nomeOriginal);

/* =========================================
   FUNÇÃO: paraMinusculas
   Converte uma string para letras minúsculas.
   Usada para tornar as pesquisas insensíveis
   a maiúsculas/minúsculas (case-insensitive).
   ========================================= */
void paraMinusculas(char destino[], const char origem[]) {
    int i = 0;
    while (origem[i] != '\0') {
        destino[i] = tolower((unsigned char)origem[i]);
        i++;
    }
    destino[i] = '\0'; /* Termina a string correctamente */
}

/* =========================================
   FUNÇÃO: matriculaExiste
   Verifica se uma matrícula já está em uso
   na lista. Retorna 1 (sim) ou 0 (não).
   Evita duplicatas no sistema.
   ========================================= */
int matriculaExiste(Estudante v[], int n, int mat) {
    for (int i = 0; i < n; i++) {
        if (v[i].matricula == mat) {
            return 1; /* Matrícula encontrada: duplicada! */
        }
    }
    return 0; /* Matrícula disponível */
}

/* =========================================
   FUNÇÃO: gerarMatriculaAleatoria
   Gera um número de matrícula aleatório
   entre 1000 e 9999 que ainda não exista
   na lista. Chamado quando o utilizador
   fornece uma entrada inválida.
   ========================================= */
int gerarMatriculaAleatoria(Estudante v[], int n) {
    int novaMat;
    do {
        novaMat = (rand() % 9000) + 1000; /* Gera número de 4 dígitos */
    } while (matriculaExiste(v, n, novaMat)); /* Repete até encontrar uma livre */
    return novaMat;
}

/* =========================================
   FUNÇÃO: nomeValido
   Valida se o nome fornecido é aceitável:
   - Não pode estar vazio
   - Só pode conter letras e espaços
   - Deve ter pelo menos uma letra
   Retorna 1 (válido) ou 0 (inválido).
   ========================================= */
int nomeValido(const char nome[]) {
    int i = 0;
    int temLetra = 0;
    if (strlen(nome) == 0) return 0; /* Nome vazio: inválido */
    while (nome[i] != '\0') {
        if (isalpha((unsigned char)nome[i])) {
            temLetra = 1; /* Confirmado: tem pelo menos uma letra */
        } else if (nome[i] != ' ') {
            return 0; /* Carácter inválido: nem letra, nem espaço */
        }
        i++;
    }
    return temLetra;
}

/* =========================================
   FUNÇÃO PRINCIPAL: main
   Ponto de entrada do programa.
   Inicializa o gerador de números aleatórios,
   a lista de estudantes e exibe o menu
   principal em loop até o utilizador sair.
   ========================================= */
int main() {
    srand((unsigned int)time(NULL)); /* Inicializa semente aleatória com hora actual */

    Estudante *lista = NULL;         /* Ponteiro para a lista dinâmica de estudantes */
    int totalEstudantes = 0;         /* Contador de estudantes na memória */
    char nomeFicheiroAberto[100] = "dados.txt"; /* Nome do ficheiro activo */
    int opcao;

    /* Loop principal: repete até o utilizador escolher "Sair" (opção 6) */
    do {
        system("cls"); /* Limpa o ecrã antes de mostrar o menu */
        
        printf("=========================================\n");
        printf(" SISTEMA DE GESTAO DE DADOS DE ESTUDANTES \n");
        printf("=========================================\n");
        printf("1. Ler Dados dos Estudantes (Escolher por Numero)\n");
        printf("2. Adicionar Novo Estudante em Memoria\n");
        printf("3. Ordenar e Visualizar Lista de Estudantes\n");
        printf("4. Pesquisar Estudante\n");
        printf("5. Gravar Dados em Ficheiro (Nome_Saida.txt)\n");
        printf("6. Sair do Programa\n");
        printf("=========================================\n");
        printf("Total de estudantes em memoria: %d\n", totalEstudantes);
        printf("=========================================\n");
        printf("Escolha uma opcao: ");

        /* Lê a opção e verifica se foi lida correctamente */
        if (scanf("%d", &opcao) != 1) {
            fflush(stdin);
            opcao = 0; /* Força opção inválida para mostrar erro */
        }

        switch(opcao) {
            case 1:
                system("cls");
                /* Carrega estudantes de um ficheiro .txt escolhido pelo utilizador */
                lerDadosFicheiro(&lista, &totalEstudantes, nomeFicheiroAberto);
                printf("\nPressione qualquer tecla para voltar ao menu...");
                fflush(stdin); getchar();
                break;
            case 2:
                system("cls");
                /* Adiciona manualmente um novo estudante à lista em memória */
                adicionarEstudante(&lista, &totalEstudantes);
                printf("\nPressione qualquer tecla para voltar ao menu...");
                fflush(stdin); getchar();
                break;
            case 3:
                /* Só permite ordenar se houver dados carregados */
                if (totalEstudantes == 0) {
                    system("cls");
                    printf("\n[AVISO] Carregue ou adicione dados primeiro (Opcao 1 ou 2)!\n");
                    printf("\nPressione qualquer tecla para voltar...");
                    fflush(stdin); getchar();
                } else {
                    menuOrdenacao(lista, totalEstudantes);
                }
                break;
            case 4:
                /* Só permite pesquisar se houver dados carregados */
                if (totalEstudantes == 0) {
                    system("cls");
                    printf("\n[AVISO] Carregue ou adicione dados primeiro!\n");
                    printf("\nPressione qualquer tecla para voltar...");
                    fflush(stdin); getchar();
                } else {
                    menuPesquisa(lista, totalEstudantes);
                }
                break;
            case 5:
                /* Só permite gravar se existirem dados na memória */
                if (totalEstudantes == 0) {
                    system("cls");
                    printf("\n[AVISO] Nao ha dados na memoria para gravar!\n");
                    printf("\nPressione qualquer tecla para voltar...");
                    fflush(stdin); getchar();
                } else {
                    menuGravarFicheiro(lista, totalEstudantes, nomeFicheiroAberto);
                }
                break;
            case 6:
                system("cls");
                printf("\nPrograma encerrado com sucesso. Obrigado!\n");
                /* Liberta a memória alocada dinamicamente antes de sair */
                if (lista != NULL) free(lista);
                break;
            default:
                printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
                printf("\nPressione qualquer tecla para tentar de novo...");
                fflush(stdin); getchar();
        }
    } while(opcao != 6); /* Continua até o utilizador escolher sair */

    return 0;
}

/* =========================================
   FUNÇÃO: lerDadosFicheiro
   Lista os ficheiros .txt na pasta actual,
   permite ao utilizador escolher um e carrega
   os dados dos estudantes para a memória.
   Formato esperado por linha: matricula;nome;nota
   ========================================= */
void lerDadosFicheiro(Estudante **v, int *n, char *nomeOriginal) {
    /* Usa o comando do sistema para listar ficheiros .txt e guarda numa lista temporária */
    system("dir /B *.txt > lista_ficheiros.tmp");
    FILE *listaTmp = fopen("lista_ficheiros.tmp", "r");

    if (listaTmp == NULL) {
        printf("\n[ERRO] Falha ao escanear a pasta por ficheiros .txt!\n");
        return;
    }

    char nomes[50][100]; /* Armazena até 50 nomes de ficheiros */
    int totalFicheiros = 0;

    printf("=========================================\n");
    printf("     SELECCAO DE FICHEIRO DE ESTUDANTES  \n");
    printf("=========================================\n\n");
    printf("Escolha o ficheiro digitando o numero correspondente:\n\n");

    /* Lê cada nome de ficheiro e lista para o utilizador (excepto o ficheiro temporário) */
    while (fgets(nomes[totalFicheiros], 100, listaTmp) != NULL) {
        nomes[totalFicheiros][strcspn(nomes[totalFicheiros], "\r\n")] = '\0'; /* Remove newline */
        
        if (strcmp(nomes[totalFicheiros], "lista_ficheiros.tmp") != 0 && strlen(nomes[totalFicheiros]) > 0) {
            printf("%d. %s\n", totalFicheiros + 1, nomes[totalFicheiros]);
            totalFicheiros++;
        }
    }
    fclose(listaTmp);
    system("del lista_ficheiros.tmp"); /* Apaga ficheiro temporário após uso */

    if (totalFicheiros == 0) {
        printf("[AVISO] Nenhum ficheiro .txt foi localizado nesta pasta!\n");
        return;
    }

    int escolha;
    printf("\nDigite o numero do ficheiro desejado: ");
    if (scanf("%d", &escolha) != 1) {
        fflush(stdin);
        printf("\n[ERRO] Entrada invalida!\n");
        return;
    }

    /* Valida se o número escolhido está dentro do intervalo disponível */
    if (escolha < 1 || escolha > totalFicheiros) {
        printf("\n[ERRO] Numero invalido!\n");
        return;
    }

    strcpy(nomeOriginal, nomes[escolha - 1]); /* Guarda o nome do ficheiro escolhido */

    FILE *ficheiro = fopen(nomeOriginal, "r");
    if (ficheiro == NULL) {
        printf("\n[ERRO] Nao foi possivel abrir o ficheiro '%s'.\n", nomeOriginal);
        return;
    }

    /* Limpa a lista anterior antes de carregar novos dados */
    if (*v != NULL) {
        free(*v);
        *v = NULL;
    }
    *n = 0;

    char linha[250];
    /* Lê o ficheiro linha a linha e faz o parse de cada campo separado por ';' */
    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0'; /* Remove caracteres de nova linha */
        if (strlen(linha) == 0) continue;     /* Ignora linhas em branco */

        /* Divide a linha nos três campos usando ';' como separador */
        char *tokenMatricula = strtok(linha, ";");
        char *tokenNome = strtok(NULL, ";");
        char *tokenNota = strtok(NULL, ";");

        if (tokenMatricula && tokenNome && tokenNota) {
            /* Expande dinamicamente o array para acomodar mais um estudante */
            Estudante *temp = (Estudante *) realloc(*v, (*n + 1) * sizeof(Estudante));
            if (temp == NULL) {
                printf("\n[ERRO] Falha critica de alocacao de memoria!\n");
                fclose(ficheiro);
                return;
            }
            *v = temp;
            
            /* Garante que a matrícula tem no máximo 4 dígitos */
            char matCortada[5];
            strncpy(matCortada, tokenMatricula, 4);
            matCortada[4] = '\0';
            
            (*v)[*n].matricula = atoi(matCortada);
            strcpy((*v)[*n].nome, tokenNome);
            (*v)[*n].nota = (float)atof(tokenNota);
            (*n)++; /* Incrementa o total de estudantes carregados */
        }
    }

    fclose(ficheiro);
    printf("\n[SUCESSO] %d estudantes foram carregados do ficheiro '%s'!\n", *n, nomeOriginal);
}

/* =========================================
   FUNÇÃO: exibirEstudantes
   Mostra todos os estudantes da lista
   formatados numa tabela no ecrã.
   ========================================= */
void exibirEstudantes(Estudante v[], int n) {
    printf("\n-------------------------------------------------------------\n");
    printf("%-12s | %-30s | %-5s\n", "Matricula", "Nome do Estudante", "Nota");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-12d | %-30s | %-.2f\n", v[i].matricula, v[i].nome, v[i].nota);
    }
    printf("-------------------------------------------------------------\n");
}

/* =========================================
   FUNÇÃO: adicionarEstudante
   Permite ao utilizador inserir manualmente
   um novo estudante. Valida:
   - Matrícula (gera automaticamente se inválida)
   - Nome (apenas letras e espaços)
   - Nota (obrigatoriamente entre 0 e 20)
   ========================================= */
void adicionarEstudante(Estudante **v, int *n) {
    char entradaMat[100];
    char entradaNome[100];
    float entradaNota;
    int matFinal = 0;
    int i, apenasDigitos = 1;

    printf("=========================================\n");
    printf("        ADICIONAR NOVO ESTUDANTE         \n");
    printf("=========================================\n");
    
    printf("Digite o Numero de Matricula: ");
    fflush(stdin);
    fgets(entradaMat, sizeof(entradaMat), stdin);
    entradaMat[strcspn(entradaMat, "\r\n")] = '\0';

    /* Verifica se todos os caracteres digitados são dígitos numéricos */
    if (strlen(entradaMat) == 0) {
        apenasDigitos = 0;
    } else {
        for (i = 0; entradaMat[i] != '\0'; i++) {
            if (!isdigit((unsigned char)entradaMat[i])) {
                apenasDigitos = 0; /* Encontrou carácter não-numérico */
                break;
            }
        }
    }

    /* Se a entrada for inválida, gera uma matrícula aleatória automaticamente */
    if (!apenasDigitos) {
        matFinal = gerarMatriculaAleatoria(*v, *n);
        printf("[AVISO] Entrada invalida detetada! Foi gerada uma matricula aleatoria: %d\n", matFinal);
    } else {
        /* Usa apenas os primeiros 4 dígitos da matrícula */
        char matCortada[5];
        strncpy(matCortada, entradaMat, 4);
        matCortada[4] = '\0';
        matFinal = atoi(matCortada);
        
        /* Se a matrícula já existir, gera uma nova automaticamente */
        if (matriculaExiste(*v, *n, matFinal)) {
            printf("[AVISO] Esta matricula ja existe! A gerar uma nova matricula aleatoria...\n");
            matFinal = gerarMatriculaAleatoria(*v, *n);
            printf("Nova matricula atribuida: %d\n", matFinal);
        }
    }

    /* Loop de validação do nome: repete até receber um nome válido */
    do {
        printf("Digite o Nome Completo (Apenas Letras): ");
        fflush(stdin);
        fgets(entradaNome, sizeof(entradaNome), stdin);
        entradaNome[strcspn(entradaNome, "\r\n")] = '\0';
        
        if (!nomeValido(entradaNome)) {
            printf("[ERRO] Nome invalido! Nao use numeros ou caracteres especiais.\n");
        }
    } while (!nomeValido(entradaNome));

    /* Loop de validação da nota: repete até receber um valor entre 0 e 20 */
    do {
        printf("Digite a Nota (Intervalo de 0 a 20): ");
        if (scanf("%f", &entradaNota) != 1) {
            fflush(stdin);
            entradaNota = -1.0f; /* Valor inválido para forçar repetição */
        }
        if (entradaNota < 0.0f || entradaNota > 20.0f) {
            printf("[ERRO] Nota invalida! A nota deve estar rigidamente entre 0 e 20.\n");
        }
    } while (entradaNota < 0.0f || entradaNota > 20.0f);

    /* Aloca espaço para mais um estudante na lista dinâmica */
    Estudante *temp = (Estudante *) realloc(*v, (*n + 1) * sizeof(Estudante));
    if (temp == NULL) {
        printf("\n[ERRO] Falha ao alocar memoria!\n");
        return;
    }
    *v = temp;

    /* Preenche os dados do novo estudante na última posição */
    (*v)[*n].matricula = matFinal;
    strcpy((*v)[*n].nome, entradaNome);
    (*v)[*n].nota = entradaNota;

    (*n)++; /* Incrementa o contador de estudantes */
    printf("\n[SUCESSO] Novo estudante inserido com sucesso na memoria!\n");
}

/* =========================================
   FUNÇÃO: menuOrdenacao
   Submenu que permite ao utilizador escolher
   o critério de ordenação da lista:
   - Por matrícula (Bubble Sort)
   - Por nome (Selection Sort)
   - Por nota (Insertion Sort)
   ========================================= */
void menuOrdenacao(Estudante v[], int n) {
    int opcao;
    do {
        system("cls");
        printf("--- SUBMENU: ORDENACAO DE DADOS ---\n");
        printf("1. Ordenar por Matricula (Crescente) -> Bubble Sort\n");
        printf("2. Ordenar por Nome (Alfabetica)   -> Selection Sort\n");
        printf("3. Ordenar por Nota (Decrescente)  -> Insertion Sort\n");
        printf("4. Voltar ao Menu Anterior\n");
        printf("-----------------------------------\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            fflush(stdin);
            opcao = 0;
        }

        switch(opcao) {
            case 1:
                bubbleSortMatricula(v, n); /* Ordena por matrícula crescente */
                exibirEstudantes(v, n);
                printf("\nPressione qualquer tecla para continuar...");
                fflush(stdin); getchar();
                break;
            case 2:
                selectionSortNome(v, n);   /* Ordena por nome alfabeticamente */
                exibirEstudantes(v, n);
                printf("\nPressione qualquer tecla para continuar...");
                fflush(stdin); getchar();
                break;
            case 3:
                insertionSortNota(v, n);   /* Ordena por nota decrescente */
                exibirEstudantes(v, n);
                printf("\nPressione qualquer tecla para continuar...");
                fflush(stdin); getchar();
                break;
            case 4:
                printf("\nA retornar ao Menu Principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
                printf("\nPressione qualquer tecla para tentar de novo...");
                fflush(stdin); getchar();
        }
    } while(opcao != 4);
}

/* =========================================
   FUNÇÃO: bubbleSortMatricula
   Algoritmo Bubble Sort que ordena os
   estudantes por matrícula em ordem
   CRESCENTE. Compara pares adjacentes e
   troca se o da esquerda for maior.
   Complexidade: O(n²)
   ========================================= */
void bubbleSortMatricula(Estudante v[], int n) {
    Estudante temporario; /* Variável auxiliar para a troca */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            /* Se o elemento actual for maior que o seguinte, troca */
            if (v[j].matricula > v[j + 1].matricula) {
                temporario  = v[j];
                v[j]        = v[j + 1];
                v[j + 1]    = temporario;
            }
        }
    }
    printf("\n[SUCESSO] Dados ordenados por Matricula!\n");
}

/* =========================================
   FUNÇÃO: selectionSortNome
   Algoritmo Selection Sort que ordena os
   estudantes por nome em ordem ALFABÉTICA.
   Encontra o menor nome no subarray restante
   e coloca-o na posição correcta.
   Complexidade: O(n²)
   ========================================= */
void selectionSortNome(Estudante v[], int n) {
    int indiceMinimo;
    Estudante temporario;
    for (int i = 0; i < n - 1; i++) {
        indiceMinimo = i; /* Assume que o menor está na posição actual */
        for (int j = i + 1; j < n; j++) {
            /* strcmp retorna negativo se v[j].nome vem antes alfabeticamente */
            if (strcmp(v[j].nome, v[indiceMinimo].nome) < 0) {
                indiceMinimo = j; /* Actualiza o índice do menor encontrado */
            }
        }
        /* Troca o menor encontrado com o elemento da posição i */
        temporario       = v[i];
        v[i]             = v[indiceMinimo];
        v[indiceMinimo]  = temporario;
    }
    printf("\n[SUCESSO] Dados ordenados por Nome!\n");
}

/* =========================================
   FUNÇÃO: insertionSortNota
   Algoritmo Insertion Sort que ordena os
   estudantes por nota em ordem DECRESCENTE
   (do maior para o menor).
   Insere cada elemento na posição correcta
   do subarray já ordenado à sua esquerda.
   Complexidade: O(n²)
   ========================================= */
void insertionSortNota(Estudante v[], int n) {
    Estudante chave; /* Elemento a ser inserido na posição correcta */
    int j;
    for (int i = 1; i < n; i++) {
        chave = v[i]; /* Guarda o elemento actual */
        j = i - 1;
        /* Move os elementos menores que a chave uma posição à frente */
        while (j >= 0 && v[j].nota < chave.nota) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = chave; /* Insere a chave na posição correcta */
    }
    printf("\n[SUCESSO] Dados ordenados por Nota decrescente!\n");
}

/* =========================================
   FUNÇÃO: menuPesquisa
   Submenu que oferece duas formas de
   pesquisar estudantes:
   1. Por nome (pesquisa linear com substring)
   2. Por matrícula (pesquisa binária eficiente)
   ========================================= */
void menuPesquisa(Estudante v[], int n) {
    int opcao;
    do {
        system("cls");
        printf("--- SUBMENU: PESQUISA DE ESTUDANTES ---\n");
        printf("1. Pesquisar por Nome (Contem texto / Flexivel)\n");
        printf("2. Pesquisar por Matricula (Pesquisa Binaria)\n");
        printf("3. Voltar ao Menu Anterior\n");
        printf("---------------------------------------\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            fflush(stdin);
            opcao = 0;
        }

        switch(opcao) {
            case 1:
                pesquisaLinearNome(v, n);  /* Pesquisa percorrendo toda a lista */
                printf("\nPressione qualquer tecla para continuar...");
                fflush(stdin); getchar();
                break;
            case 2:
                pesquisaBinariaMatricula(v, n); /* Pesquisa eficiente em lista ordenada */
                printf("\nPressione qualquer tecla para continuar...");
                fflush(stdin); getchar();
                break;
            case 3:
                printf("\nA retornar ao Menu Principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
                printf("\nPressione qualquer tecla para tentar de novo...");
                fflush(stdin); getchar();
        }
    } while(opcao != 3);
}

/* =========================================
   FUNÇÃO: pesquisaLinearNome
   Percorre TODA a lista comparando o termo
   pesquisado com cada nome (usando strstr).
   A comparação é insensível a maiúsculas.
   Mostra todos os estudantes cujo nome
   contenha o texto digitado.
   Complexidade: O(n)
   ========================================= */
void pesquisaLinearNome(Estudante v[], int n) {
    char termoPesquisa[100];
    char termoMinusculo[100];
    char nomeMinusculoEstudante[100];
    int encontrados = 0;

    printf("\nDigite o termo ou nome do estudante para pesquisar: ");
    fflush(stdin);
    fgets(termoPesquisa, sizeof(termoPesquisa), stdin);
    termoPesquisa[strcspn(termoPesquisa, "\r\n")] = '\0';

    /* Converte o termo de pesquisa para minúsculas (comparação case-insensitive) */
    paraMinusculas(termoMinusculo, termoPesquisa);

    printf("\n[RESULTADOS DA PESQUISA POR NOME: '%s']\n", termoPesquisa);
    printf("-------------------------------------------------------------\n");
    printf("%-12s | %-30s | %-5s\n", "Matricula", "Nome do Estudante", "Nota");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        paraMinusculas(nomeMinusculoEstudante, v[i].nome); /* Converte nome do estudante */
        
        /* strstr verifica se o termo existe como substring no nome */
        if (strstr(nomeMinusculoEstudante, termoMinusculo) != NULL) {
            printf("%-12d | %-30s | %-.2f\n", v[i].matricula, v[i].nome, v[i].nota);
            encontrados++;
        }
    }
    printf("-------------------------------------------------------------\n");
    printf("Total de registros localizados: %d\n", encontrados);
}

/* =========================================
   FUNÇÃO: pesquisaBinariaMatricula
   Ordena a lista por matrícula e depois
   aplica o algoritmo de Pesquisa Binária.
   Divide repetidamente o intervalo de busca
   ao meio até encontrar (ou não) a matrícula.
   Muito mais eficiente que a pesquisa linear!
   Complexidade: O(log n)
   ========================================= */
void pesquisaBinariaMatricula(Estudante v[], int n) {
    int matriculaPesquisa;
    int inicio = 0, fim = n - 1, meio;
    int encontrado = 0;

    /* Ordenação necessária para garantir que a pesquisa binária funcione correctamente */
    bubbleSortMatricula(v, n);

    printf("\nDigite o numero de matricula para pesquisar: ");
    if (scanf("%d", &matriculaPesquisa) != 1) {
        fflush(stdin);
        printf("\n[ERRO] Matricula invalida!\n");
        return;
    }

    /* Divide o intervalo ao meio em cada iteração até encontrar ou esgotar */
    while (inicio <= fim) {
        meio = (inicio + fim) / 2; /* Calcula o índice do elemento do meio */
        if (v[meio].matricula == matriculaPesquisa) {
            /* Matrícula encontrada: mostra os dados do estudante */
            printf("\n-------------------------------------------------------------\n");
            printf("[ESTUDANTE LOCALIZADO]:\nMatricula: %d | Nome: %s | Nota: %.2f\n",
                   v[meio].matricula, v[meio].nome, v[meio].nota);
            printf("-------------------------------------------------------------\n");
            encontrado = 1;
            break;
        } else if (v[meio].matricula < matriculaPesquisa) {
            inicio = meio + 1; /* A matrícula é maior: busca na metade direita */
        } else {
            fim = meio - 1;    /* A matrícula é menor: busca na metade esquerda */
        }
    }
    if (!encontrado) {
        printf("\n[AVISO] Matricula %d nao encontrada.\n", matriculaPesquisa);
    }
}

/* =========================================
   FUNÇÃO: menuGravarFicheiro
   Submenu que permite exportar os dados
   actuais da memória para ficheiro:
   1. Ficheiro de texto (.txt) com formato
      legível: matricula;nome;nota
   2. Ficheiro binário (.dat) com os dados
      brutos da estrutura em memória
   ========================================= */
void menuGravarFicheiro(Estudante v[], int n, char *nomeOriginal) {
    int opcao;
    do {
        system("cls");
        printf("--- SUBMENU: GRAVACAO DE DADOS ---\n");
        printf("1. Exportar para Ficheiro de Texto Padrao (Matricula;Nome;Nota)\n");
        printf("2. Exportar para Ficheiro Binario Puro (.dat)\n");
        printf("3. Voltar ao Menu Anterior\n");
        printf("-----------------------------------\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            fflush(stdin);
            opcao = 0;
        }

        char nomeSaidaTexto[150];
        char nomeSaidaBinario[150];
        char nomeBase[100];
        
        /* Remove a extensão .txt do nome original para criar o nome base */
        strcpy(nomeBase, nomeOriginal);
        char *pExtensao = strstr(nomeBase, ".txt");
        if (pExtensao != NULL) {
            *pExtensao = '\0'; /* Termina a string antes da extensão */
        }

        /* Constrói os nomes dos ficheiros de saída automaticamente */
        sprintf(nomeSaidaTexto,   "%s_Saida.txt", nomeBase);
        sprintf(nomeSaidaBinario, "%s_Saida.dat", nomeBase);

        if (opcao == 1) {
            /* Abre para escrita e grava cada estudante numa linha separada por ';' */
            FILE *fTexto = fopen(nomeSaidaTexto, "w");
            if (fTexto == NULL) { printf("\n[ERRO] Falha ao criar ficheiro!\n"); return; }
            
            for (int i = 0; i < n; i++) {
                fprintf(fTexto, "%d;%s;%.2f\n", v[i].matricula, v[i].nome, v[i].nota);
            }
            fclose(fTexto);
            printf("\n[SUCESSO] Dados salvos e estruturados em '%s'!\n", nomeSaidaTexto);
            printf("\nPressione qualquer tecla para continuar...");
            fflush(stdin); getchar();
        }
        else if (opcao == 2) {
            /* Grava toda a estrutura em binário de uma só vez com fwrite */
            FILE *fBinario = fopen(nomeSaidaBinario, "wb");
            if (fBinario == NULL) { printf("\n[ERRO] Falha ao criar ficheiro!\n"); return; }
            fwrite(v, sizeof(Estudante), n, fBinario); /* Escreve n registos binários */
            fclose(fBinario);
            printf("\n[SUCESSO] Dados salvos com sucesso em '%s'!\n", nomeSaidaBinario);
            printf("\nPressione qualquer tecla para continuar...");
            fflush(stdin); getchar();
        }
        else if (opcao == 3) {
            printf("\nA retornar ao Menu Anterior...\n");
        }
        else {
            printf("\n[ERRO] Opcao invalida!\n");
            printf("\nPressione qualquer tecla para tentar de novo...");
            fflush(stdin); getchar();
        }
    } while(opcao != 3);
}
