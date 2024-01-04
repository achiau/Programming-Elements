#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define numInputs 9
#define numHiddenNodes 64
#define numOutputs 1
#define numTrainingSets 40
#define MAX_CONNECTIONS 100

#define numEpochs 10000
#define learningRate 0.1
//----------------------------------------- 
// Print Board
//-----------------------------------------
void printBoard(int board[]) {
    printf("\n %c | %c | %c        0|1|2\n", board[0] == 0 ? ' ' : board[0] == 1 ? 'X' : 'O',
                                           board[1] == 0 ? ' ' : board[1] == 1 ? 'X' : 'O',
                                           board[2] == 0 ? ' ' : board[2] == 1 ? 'X' : 'O');
    printf("---|---|---\n");
    printf(" %c | %c | %c        3|4|5\n", board[3] == 0 ? ' ' : board[3] == 1 ? 'X' : 'O',
                                           board[4] == 0 ? ' ' : board[4] == 1 ? 'X' : 'O',
                                           board[5] == 0 ? ' ' : board[5] == 1 ? 'X' : 'O');
    printf("---|---|---\n");
    printf(" %c | %c | %c        6|7|8\n\n", board[6] == 0 ? ' ' : board[6] == 1 ? 'X' : 'O',
                                             board[7] == 0 ? ' ' : board[7] == 1 ? 'X' : 'O',
                                             board[8] == 0 ? ' ' : board[8] == 1 ? 'X' : 'O');
}

//----------------------------------------- 
// check wins and draws
//-----------------------------------------
//Draws
int drawCheck(int l_board[]){
    for(int i = 0; i < 9; i++){
        if (l_board[i] == 0){
            return 0;
        }
    }
    return 1;
}
int winCheck(int l_board[]){
    // Rows
    for (int i = 0; i < 3; i++){
        if (l_board[3*i] == l_board[3*i + 1] && l_board[3*i + 1] == l_board[3*i + 2]){
            return l_board[3*i];
        }
    }

    // Columns
    for (int j = 0; j < 3; j++){
        if (l_board[j] == l_board[3 + j] && l_board[3 + j] == l_board[6 + j]){
            return l_board[j];
        }
    }

    // Diagonal Top Left to Bottom Right
    if (l_board[0] == l_board[4] && l_board[0] == l_board[8]){
        return l_board[0];
    }

    // Diagonal Top Right to bottom Left
    if (l_board[2] == l_board[4] && l_board[2] == l_board[6]){
        return l_board[2];
    }

    return 0;
}
//----------------------------------------- 
// initialize weights
//-----------------------------------------
double init_weights() { return ((double)rand()) / ((double)RAND_MAX); }  
//----------------------------------------- 
// activation function sigmoid and derivative
//-----------------------------------------
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}
double dsigmoid(double x) {
    double sigmoid_x = sigmoid(x);
    return sigmoid_x * (1 - sigmoid_x);
}
//----------------------------------------- 
// shuffle
//-----------------------------------------
void shuffle(int *array, size_t n){
 if (n > 1)
    { size_t i;
        for (i = 0; i < n - 1; i++)
        { size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
//----------------------------------------- 
// struct
//-----------------------------------------
struct NeuralNetwork {
    double inputWeights[numInputs][numHiddenNodes];
    double hiddenWeights[numHiddenNodes][numOutputs];
    double hiddenBias[numHiddenNodes]; 
    double outputBias[numOutputs];
};
//----------------------------------------- 
// initialize weights
//-----------------------------------------
void initializeWeights(struct NeuralNetwork *network) {
    // Initialize inputWeights and hiddenWeights with random values
    for (int i = 0; i < numInputs; i++) {
        for (int j = 0; j < numHiddenNodes; j++) {
            network->inputWeights[i][j] = init_weights();
        }
    }

    for (int i = 0; i < numHiddenNodes; i++) {
        for (int j = 0; j < numOutputs; j++) {
            network->hiddenWeights[i][j] = init_weights();
        }
        network->hiddenBias[i] = init_weights();
    }
    for (int i = 0; i < numOutputs; i++) {
        network->outputBias[i] = init_weights(); // Inicialização do bias da camada de saída
    }
}
//----------------------------------------- 
// aply sigmoid
//-----------------------------------------
void applySigmoid(double *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = sigmoid(array[i]);
    }
}
//----------------------------------------- 
// forward propagation
//-----------------------------------------
void forwardPropagation(struct NeuralNetwork *network, double inputs[numInputs], double outputs[numOutputs]) {
    // Compute outputs for the hidden layer
    double hiddenLayer[numHiddenNodes];
    for (int i = 0; i < numHiddenNodes; i++) {
        double sum = network->hiddenBias[i]; //add bias
        for (int j = 0; j < numInputs; j++) {
            sum += inputs[j] * network->inputWeights[j][i];
        }
        hiddenLayer[i] = sigmoid(sum);
    }

    // Compute final outputs
    for (int i = 0; i < numOutputs; i++) {
        double sum = network->outputBias[i];
        for (int j = 0; j < numHiddenNodes; j++) {
            sum += hiddenLayer[j] * network->hiddenWeights[j][i];
        }
        outputs[i] = sum;
      //  printf("Sum:%f",sum);
    }
     applySigmoid(outputs, numOutputs);
     
}
//-----------------------------------------
// Training the neural network,back propagation updating errors
//-----------------------------------------
void train(struct NeuralNetwork *network, double trainingInputs[numTrainingSets][numInputs], double trainingOutputs[numTrainingSets][numOutputs]) {
    for (int epoch = 0; epoch < numEpochs; epoch++) {
        // Loop through training sets
        for (int t = 0; t < numTrainingSets; t++) {
            double inputs[numInputs];
            double hiddenLayer[numHiddenNodes];
            double outputs[numOutputs];

            // Copy training data for this set
            for (int i = 0; i < numInputs; i++) {
                inputs[i] = trainingInputs[t][i];
            }
            for (int i = 0; i < numOutputs; i++) {
                outputs[i] = trainingOutputs[t][i];
            }

            // Forward propagation to get network's prediction
            forwardPropagation(network, inputs, outputs);

            // Backpropagation - Update weights and biases based on errors

            // Calculate output layer errors
            double outputErrors[numOutputs];
            for (int i = 0; i < numOutputs; i++) {
                outputErrors[i] = outputs[i] * (1 - outputs[i]) * (outputs[i] - trainingOutputs[t][i]);
            }

            // Update output layer weights and bias
            for (int i = 0; i < numOutputs; i++) {
                // Update bias for output layer
                network->outputBias[i] -= learningRate * outputErrors[i];

                // Update weights for output layer
                for (int j = 0; j < numHiddenNodes; j++) {
                    double gradient = outputErrors[i] * hiddenLayer[j];
                    network->hiddenWeights[j][i] -= learningRate * gradient;
                }
            }

            // Calculate hidden layer errors
            double hiddenErrors[numHiddenNodes];
            for (int i = 0; i < numHiddenNodes; i++) {
                double error = 0.0;
                for (int j = 0; j < numOutputs; j++) {
                    error += outputErrors[j] * network->hiddenWeights[i][j];
                }
                hiddenErrors[i] = hiddenLayer[i] * (1 - hiddenLayer[i]) * error;
            }

            // Update hidden layer weights and bias
            for (int i = 0; i < numHiddenNodes; i++) {
                // Update bias for hidden layer
                network->hiddenBias[i] -= learningRate * hiddenErrors[i];

                // Update weights for hidden layer
                for (int j = 0; j < numInputs; j++) {
                    double gradient = hiddenErrors[i] * inputs[j] * dsigmoid(hiddenLayer[i]);
                    network->inputWeights[j][i] -= learningRate * gradient;
                }
            }
            
        }
    }
}

//-----------------------------------------
// Predicting the AI's move
//-----------------------------------------
int predictMove(struct NeuralNetwork *network, int board[]) {
        double inputs[numInputs];
    double outputs[numOutputs];

    int availableMoves[numInputs];
    int numAvailableMoves = 0;

    for (int i = 0; i < numInputs; i++) {
        if (board[i] == 0) {
            availableMoves[numAvailableMoves++] = i;
        }
    }

    // Inicializar as entradas para a rede neural com os valores do tabuleiro diretamente
    for (int i = 0; i < numInputs; i++) {
        inputs[i] = board[i];  // Cada valor do tabuleiro será diretamente uma entrada
    }

    forwardPropagation(network, inputs, outputs);

    int bestMove = -1;
    double highestOutput = -1.0;
    for (int i = 0; i < numOutputs; i++) {
        if (outputs[i] > highestOutput) {
            highestOutput = outputs[i];
            bestMove = i;
        }
    }
     
    return bestMove;

}
//-----------------------------------------
// AI move
//-----------------------------------------
int getAIMove(struct NeuralNetwork *network, int board[]) {
      int availableMoves[numInputs];
    int numAvailableMoves = 0;

    // Available moves
    for (int i = 0; i < numInputs; i++) {
        if (board[i] == 0) {
            availableMoves[numAvailableMoves++] = i;
        }
    }

    double bestScore = -INFINITY; // Initialize score
    int bestMove = -1;

    // Check for potential winning moves or opponent winning moves
    for (int i = 0; i < numAvailableMoves; i++) {
        int move = availableMoves[i];

        // Create a temporary board version to simulate the move
        int tempBoard[numInputs];
        for (int j = 0; j < numInputs; j++) {
            tempBoard[j] = board[j];
        }

        // Simulate the move on the temporary board (considering the AI's move, represented by -1)
        tempBoard[move] = -1;

        // Check if the move leads to a win for the AI
        int winner = winCheck(tempBoard);
        if (winner == -1) {
            return move; // If the move leads to a win, take it
        }

        // Use neural network through predictMove to get the score
        double score = predictMove(network, tempBoard);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}
//-----------------------------------------
//getUserMove
//-----------------------------------------
void getUserMove(int board[]) {
    int move;
    printf("Enter your move (0-8): ");
    scanf("%d", &move);

    // Ensure the chosen move is valid
    while (move < 0 || move > 8 || board[move] != 0) {
        printf("Invalid move. Enter your move (0-8): ");
        scanf("%d", &move);
    }

    // Update the board with the user's move
    if (board[move] == 0){
    board[move] = 1;} // Assuming the user is represented by '1' on the board
}
//-----------------------------------------
//main function
//-----------------------------------------
int main() {
    struct NeuralNetwork network;
    initializeWeights(&network);
    int board[9] = {0}; // Initialize the board; 0 represents empty spaces

    printf("Tic Tac Toe Game!\n");

    int gameNotFinished = 1;
    int currentPlayer = 1; // 1 for human, -1 for AI
  int choice;
do{
    printf("Choose your opponent:\n");
    printf("1- Bot (random moves)\n");
    printf("2- AI\n Ans:");
    scanf("%d",&choice); } while( choice != 1 && choice!=2);
       
    while (gameNotFinished) {
     printBoard(board);

        if (currentPlayer == 1) {
            getUserMove(board); // Get human player's move
        } else if (choice == 1) {
                // Bot (random move) logic
                int availableMoves[9];
                int numAvailableMoves = 0;
                for (int i = 0; i < 9; i++) {
                    if (board[i] == 0) {
                        availableMoves[numAvailableMoves++] = i;
                    }
                }
                int randomMove = availableMoves[rand() % numAvailableMoves];
                board[randomMove] = -1; 
             }
        else {
               int aiMove = getAIMove(&network, board); // Get AI's move
            board[aiMove] = -1; // Update the board with AI's move

        }

        // Check for win/loss/draw conditions
        int winner = winCheck(board);
        if (winner == 1) {
             printBoard(board);
            printf("Congratulations! \n You won!\n");
            break;
        } else if (winner == -1) {
            printBoard(board);
            printf("You lost\n Try Again!\n");
            break;
        } else if (drawCheck(board)) {
            printBoard(board);
            printf("It's a draw!\n");
            break;
        }

        // Switch players for the next turn
        currentPlayer *= -1;
    }

    return 0;
}


    

   


