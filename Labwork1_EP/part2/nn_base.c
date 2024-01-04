#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Estrutura de dados para uma unidade da rede neural
typedef struct {
    double *weights;  // Array de pesos para as conexões
    int num_weights;  // Número de pesos (incluindo o peso do viés)
} NeuralUnit;

// Estrutura de dados para a rede neural
typedef struct {
    int num_inputs;    // Número de entradas
    int num_hidden;    // Número de unidades na camada oculta
    int num_outputs;   // Número de saídas
    NeuralUnit *hidden_layer;  // Array de unidades na camada oculta
    double *output_weights;   // Pesos para as conexões da camada oculta para a saída
} NeuralNetwork;

// Função sigmoide
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Função para inicializar uma rede neural
NeuralNetwork initializeNeuralNetwork(int num_inputs, int num_hidden, int num_outputs) {
    NeuralNetwork nn;
    nn.num_inputs = num_inputs;
    nn.num_hidden = num_hidden;
    nn.num_outputs = num_outputs;

    // Inicializar a camada oculta
    nn.hidden_layer = (NeuralUnit *)malloc(num_hidden * sizeof(NeuralUnit));
    int i;
    for (i = 0; i < num_hidden; i++) {
        nn.hidden_layer[i].num_weights = num_inputs + 1;  // +1 para o peso do viés
        nn.hidden_layer[i].weights = (double *)malloc((num_inputs + 1) * sizeof(double));

        // Inicializar pesos para a camada oculta com zero
        for (int j = 0; j < num_inputs + 1; j++) {
            nn.hidden_layer[i].weights[j] = 0.0;
        }
    }

    // Inicializar os pesos para a camada oculta para a saída
    nn.output_weights = (double *)malloc(num_hidden * sizeof(double));
    for (int i = 0; i < num_hidden; i++) {
        nn.output_weights[i] = 0.0;
    }

    return nn;
}

// Função para propagar uma matriz de valores de entrada pela rede neural
void propagate(NeuralNetwork nn, double *input, double *output) {
    // Propagar através da camada oculta
    for (int i = 0; i < nn.num_hidden; i++) {
        double sum = 0.0;

        // Calcular a soma ponderada dos pesos e entradas
        for (int j = 0; j < nn.hidden_layer[i].num_weights - 1; j++) {
            sum += input[j] * nn.hidden_layer[i].weights[j];
        }

        // Adicionar o viés
        sum += nn.hidden_layer[i].weights[nn.hidden_layer[i].num_weights - 1];

        // Aplicar a função sigmoide
        output[i] = sigmoid(sum);
    }

    // Propagar para a camada de saída
    double temp_output[nn.num_outputs];
    for (int i = 0; i < nn.num_outputs; i++) {
        double sum = 0.0;
        
        // Calcular a soma ponderada dos pesos e saídas da camada oculta
        for (int j = 0; j < nn.num_hidden; j++) {
            sum += output[j] * nn.output_weights[j];
        }

        // Adicionar o viés
        sum += nn.output_weights[nn.num_hidden - 1];

        // Aplicar a função sigmoide
        temp_output[i] = sigmoid(sum);
    }
}

// Função para carregar pesos de um arquivo
void loadWeightsFromFile(NeuralNetwork *nn, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para leitura.\n");
        exit(1);
    }

    // Ler o número de entradas, unidades ocultas e saídas
    fscanf(file, "%d %d %d", &nn->num_inputs, &nn->num_hidden, &nn->num_outputs);

    // Inicializar a rede neural com os valores lidos
    *nn = initializeNeuralNetwork(nn->num_inputs, nn->num_hidden, nn->num_outputs);

    // Ler pesos para a camada oculta
    for (int i = 0; i < nn->num_hidden; i++) {
        for (int j = 0; j < nn->hidden_layer[i].num_weights; j++) {
            fscanf(file, "%lf", &nn->hidden_layer[i].weights[j]);
        }
    }

    // Ler pesos da camada oculta para a saída
    for (int i = 0; i < nn->num_hidden; i++) {
        fscanf(file, "%lf", &nn->output_weights[i]);
    }

    fclose(file);
}

// Função para imprimir os pesos da rede neural
void printWeights(NeuralNetwork nn) {
    printf("Pesos da Camada Oculta:\n");
    for (int i = 0; i < nn.num_hidden; i++) {
        for (int j = 0; j < nn.hidden_layer[i].num_weights; j++) {
            printf("%lf ", nn.hidden_layer[i].weights[j]);
        }
        printf("\n");
    }

    printf("Pesos da Camada Oculta para a Saída:\n");
    for (int i = 0; i < nn.num_hidden; i++) {
        printf("%lf ", nn.output_weights[i]);
    }
    printf("\n");
}

int main() {
    NeuralNetwork nn;
    double input[2] = {0.5, 0.7};
    double output[1];

    // Informe o caminho correto do arquivo de pesos
    const char *weightsFile = "/home/achiau/Documents/Eprog/TP1/part2/testmodel.txt";

    // Carregue os pesos do arquivo
    loadWeightsFromFile(&nn, weightsFile);

    // Propague os valores de entrada pela rede neural
    propagate(nn, input, output);

    // Imprima os resultados
    printf("Resultado da Propagação:\n");
    for (int i = 0; i < nn.num_outputs; i++) {
        printf("Saída %d: %lf\n", i + 1, output[i]);
    }

    return 0;
}
