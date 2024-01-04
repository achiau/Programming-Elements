// nn_base.h
#ifndef NN_BASE_H
#define NN_BASE_H

typedef struct {
    double *weights;
    int num_weights;
} NeuralUnit;

typedef struct {
    int num_inputs;
    int num_hidden;
    int num_outputs;
    NeuralUnit *hidden_layer;
    double *output_weights;
} NeuralNetwork;

NeuralNetwork initializeNeuralNetwork(int num_inputs, int num_hidden, int num_outputs);

void propagate(NeuralNetwork nn, double *input, double *output);

void loadWeightsFromFile(NeuralNetwork *nn, const char *filename);

void printWeights(NeuralNetwork nn);

#endif
