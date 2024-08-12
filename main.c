#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

// Função para ler o conteúdo de um arquivo para uma string
char* read_file(const char* filename) {
    FILE *file = fopen(filename, "r");  // Abre o arquivo em modo leitura
    if (!file) {  // Verifica se o arquivo foi aberto corretamente
        perror("Failed to open file");
        return NULL;
    }
    fseek(file, 0, SEEK_END);  // Move o ponteiro para o final do arquivo
    long length = ftell(file);  // Obtém a posição atual do ponteiro, que é o tamanho do arquivo
    fseek(file, 0, SEEK_SET);  // Move o ponteiro de volta para o início do arquivo

    char *content = malloc(length + 1);  // Aloca memória para o conteúdo do arquivo
    fread(content, 1, length, file);  // Lê o conteúdo do arquivo para a string
    content[length] = '\0';  // Adiciona o caractere nulo ao final da string

    fclose(file);  // Fecha o arquivo
    return content;  // Retorna o conteúdo lido
}

int main() {
    const char* filename = "data.json";  // Nome do arquivo JSON
    char *json_string = read_file(filename);  // Lê o conteúdo do arquivo JSON

    if (json_string == NULL) {  // Verifica se a leitura foi bem-sucedida
        return 1;
    }

    // Analisa o conteúdo JSON
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {  // Verifica se a análise foi bem-sucedida
        printf("Error parsing JSON.\n");
        free(json_string);  // Libera a memória alocada para o conteúdo do arquivo
        return 1;
    }

    // Imprime o cabeçalho da tabela
    printf("%-20s | %s\n", "Key", "Value");
    printf("---------------------|-------------------------\n");

    // Itera sobre todos os pares chave-valor no objeto JSON
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {
        if (cJSON_IsString(item) || cJSON_IsNumber(item)) {  // Verifica se o item é uma string ou número
            printf("%-20s | %s\n", item->string, cJSON_PrintUnformatted(item));
        } else if (cJSON_IsObject(item)) {  // Se o item é um objeto, iterar sobre os pares chave-valor do objeto
            cJSON *sub_item = NULL;
            cJSON_ArrayForEach(sub_item, item) {
                if (cJSON_IsString(sub_item) || cJSON_IsNumber(sub_item)) {
                    printf("%-20s | %s\n", sub_item->string, cJSON_PrintUnformatted(sub_item));
                }
            }
        }
    }

    // Liberar memória
    cJSON_Delete(json);  // Libera a memória alocada para o objeto JSON
    free(json_string);  // Libera a memória alocada para o conteúdo do arquivo

    return 0;
}
