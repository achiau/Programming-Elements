#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Estrutura de dados para uma unidade da rede neural
typedef struct {
    double *weights;  // Array de pesos para as conexões
    int num_weights;  // Número de pesos (incluindo o peso do bias)
} NeuralUnit;

typedef struct unit {
	double	inp; 
	double	out;  
    double delta;
} unit_t;

// Estrutura de dados para a rede neural
typedef struct {
    int num_inputs;    // Número de entradas
    int num_hidden;    // Número de unidades na camada oculta
    int num_outputs;   // Número de saídas
    NeuralUnit *hidden_layer;  // Array de unidades na camada oculta
    double *output_weights;   // Pesos para as conexões da camada oculta para a saída
    double *inputs;   // Array para guardar os valores dos inputs
    unit_t *hidden;   
    unit_t *output;   
    double *expected_outputs;   // Array para guardar os valores dos outputs esperados
} NeuralNetwork;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Função para inicializar uma rede neural
NeuralNetwork* initializeNeuralNetwork(int num_inputs, int num_hidden, int num_outputs) {
    NeuralNetwork *nn = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));

    nn->num_inputs = num_inputs;
    nn->num_hidden = num_hidden;
    nn->num_outputs = num_outputs;

    nn->hidden_layer = (NeuralUnit *)malloc(num_hidden * sizeof(NeuralUnit));
    nn->output_weights = (double *)malloc(num_hidden * sizeof(double));
    nn->inputs = (double *)malloc(num_inputs * sizeof(double));
    nn->hidden = (unit_t *)malloc(num_hidden * sizeof(unit_t));
    nn->output = (unit_t *)malloc(num_outputs * sizeof(unit_t));
    nn->expected_outputs = (double *)malloc(num_outputs * sizeof(double));

    for (int i = 0; i < num_hidden; i++) {
        nn->hidden_layer[i].num_weights = num_inputs + 1;  // +1 para o peso do bias
        nn->hidden_layer[i].weights = (double *)malloc((num_inputs + 1) * sizeof(double));

        for (int j = 0; j < num_inputs + 1; j++) {
            nn->hidden_layer[i].weights[j] = (rand() % 1000) / 1000.0 - 0.5;
        }
    }

    for (int i = 0; i < num_hidden; i++) {
        nn->output_weights[i] = (rand() % 1000) / 1000.0 - 0.5;
    }

    return nn;
}

// Função para propagar os dados pela rede neural
void propagate(NeuralNetwork* nn, double *input, double *output) {
    // Propagação pela camada oculta
    for (int i = 0; i < nn->num_hidden; i++) {
        double sum = 0.0;

        for (int j = 0; j < nn->hidden_layer[i].num_weights - 1; j++) {
            sum += input[j] * nn->hidden_layer[i].weights[j];
        }

        sum += nn->hidden_layer[i].weights[nn->hidden_layer[i].num_weights - 1];
        
        nn->hidden[i].inp = sum;
        nn->hidden[i].out = sigmoid(sum);
    }

    // Propagação para a camada de saída
    for (int i = 0; i < nn->num_outputs; i++) {
        double sum = 0.0;

        for (int j = 0; j < nn->num_hidden; j++) {
            sum += nn->hidden[j].out * nn->output_weights[j];
        }

        sum += nn->output_weights[nn->num_hidden - 1];

        nn->output[i].inp = sum;
        nn->output[i].out = sigmoid(sum);
        output[i] = nn->output[i].out;
    }
}

// Função para retropropagação
void backpropagate(NeuralNetwork* nn, double *input, double *expected_output, double learning_rate) {
    // Calcular os deltas para a camada de saída
    for (int i = 0; i < nn->num_outputs; i++) {
        double error = expected_output[i] - nn->output[i].out;
        nn->output[i].delta = error * sigmoid_derivative(nn->output[i].out);
    }

    // Calcular os deltas para a camada oculta
    for (int i = 0; i < nn->num_hidden; i++) {
        double error = 0.0;
        for (int j = 0; j < nn->num_outputs; j++) {
            error += nn->output[j].delta * nn->output_weights[i];
        }
        nn->hidden[i].delta = error * sigmoid_derivative(nn->hidden[i].out);
    }

    // Atualizar os pesos da camada de saída
    for (int i = 0; i < nn->num_outputs; i++) {
        for (int j = 0; j < nn->num_hidden; j++) {
            nn->output_weights[j] += learning_rate * nn->output[i].delta * nn->hidden[j].out;
        }
    }

    // Atualizar os pesos da camada oculta
    for (int i = 0; i < nn->num_hidden; i++) {
        for (int j = 0; j < nn->hidden_layer[i].num_weights - 1; j++) {
            nn->hidden_layer[i].weights[j] += learning_rate * nn->hidden[i].delta * input[j];
        }
        nn->hidden_layer[i].weights[nn->hidden_layer[i].num_weights - 1] += learning_rate * nn->hidden[i].delta;
    }
}

// Função para treinar a rede neural
void train(NeuralNetwork* nn, double inputs[][2], double outputs[][1], int num_samples, double learning_rate, int epochs) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        double total_error = 0.0;

        for (int i = 0; i < num_samples; i++) {
            double predicted_output[nn->num_outputs];
            propagate(nn, inputs[i], predicted_output);

            double instance_error = 0.0;
            for (int j = 0; j < nn->num_outputs; j++) {
                instance_error += 0.5 * pow(outputs[i][j] - predicted_output[j], 2);
            }
            total_error += instance_error;

            backpropagate(nn, inputs[i], outputs[i], learning_rate);
        }

        // Exibir erro a cada 1000 épocas
        if (epoch % 1000 == 0) {
            printf("Epoch %d - Error: %lf\n", epoch, total_error);
        }
    }
}

// Função para imprimir os pesos da rede neural
void printWeights(NeuralNetwork *nn) {
    printf("Pesos da Camada Oculta:\n");
    for (int i = 0; i < nn->num_hidden; i++) {
        for (int j = 0; j < nn->hidden_layer[i].num_weights; j++) {
            printf("%lf ", nn->hidden_layer[i].weights[j]);
        }
        printf("\n");
    }

    printf("Pesos da Camada Oculta para a Saída:\n");
    for (int i = 0; i < nn->num_hidden; i++) {
        printf("%lf ", nn->output_weights[i]);
    }
    printf("\n");
}

// Função para liberar a memória alocada para a rede neural
void freeNeuralNetwork(NeuralNetwork *nn) {
    for (int i = 0; i < nn->num_hidden; i++) {
        free(nn->hidden_layer[i].weights);
    }

    free(nn->hidden_layer);
    free(nn->output_weights);
    free(nn->inputs);
    free(nn->hidden);
    free(nn->output);
    free(nn->expected_outputs);
    free(nn);
}

int main() {
    // Informações da rede neural
    int num_inputs = 2;
    int num_hidden = 3;
    int num_outputs = 1;

    // Inicialize a rede neural
    NeuralNetwork *nn = initializeNeuralNetwork(num_inputs, num_hidden, num_outputs);

    // Informações dos dados de treinamento
    int num_samples = 4;
    double inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double outputs[4][1] = {{0}, {1}, {1}, {0}};

    // Hiperparâmetros
    double learning_rate = 0.5;
    int epochs = 10000;

    // Treinamento da rede neural
    train(nn, inputs, outputs, num_samples, learning_rate, epochs);

    // Teste da rede neural treinada
    printf("Resultado da Propagação:\n");
    for (int i = 0; i < num_samples; i++) {
        double predicted_output[nn->num_outputs];
        propagate(nn, inputs[i], predicted_output);
        printf("Saída %d: %lf\n", i, predicted_output[0]);
    }

    // Imprimir os pesos
    printWeights(nn);

    // Liberar a memória alocada
    freeNeuralNetwork(nn);

    return 0;
}
