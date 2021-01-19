#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

int generateRandomNumber (int from, int to) {
    int randomNumber = rand() % (to - from) + from;
    return randomNumber;
}

double generateRandomDoubleNumber (double from, double to) {
    double randomNumber = (double)rand() / RAND_MAX;
    return -(from + randomNumber * (from - to));
}

int calculateC(std::vector<std::vector<int>>& matrix, std::vector<int> &perm, int numberOfTasks, int numberOfMachines) {
    std::vector <int> previousTaskEndTime(numberOfMachines + 1, 0);
    std::vector <int> currentTaskEndTime(numberOfMachines + 1, 0);

    for (int j = 1; j <= numberOfTasks; j++) {
        for (int k = 1; k <= numberOfMachines; k++) {
            currentTaskEndTime[k] = std::max(previousTaskEndTime[k], currentTaskEndTime[k - 1]) + matrix[perm[j-1]][k];
        }
        previousTaskEndTime = currentTaskEndTime;
        currentTaskEndTime.assign(numberOfMachines+1, 0);
    }
    return previousTaskEndTime[numberOfMachines];
}

void checkIfEqual(int firstRandomNumber, int secondRandomNumber, int numberOfTasks) {
    if (firstRandomNumber == secondRandomNumber) {
        while (firstRandomNumber == secondRandomNumber) {
            secondRandomNumber = generateRandomNumber(1, numberOfTasks);
        }
    }
}

void switchPlaces(int firstPosition, int secondPosition, std::vector<int>& perm) {
    int tempPosition = perm[secondPosition];
    perm[secondPosition] = perm[firstPosition];
    perm[firstPosition] = tempPosition;
}

void loadData(int &numberOfTasks, int &numberOfMachines, std::vector <std::vector <int>> &matrix, std::string numberOfFile) {
    std::ifstream file ("/home/jerry/QtProjects/MaxFlowProblem/data" + numberOfFile +".txt");
    std::string stringNumberOfTasks;
    std::string stringNumberOfMachines;
    file >> stringNumberOfTasks;
    file >> stringNumberOfMachines;
    std::istringstream (stringNumberOfTasks) >> numberOfTasks;
    std::istringstream (stringNumberOfMachines) >> numberOfMachines;
    matrix.resize(numberOfTasks+1);
    for (int i = 1; i < numberOfTasks+1; i++) {
        matrix[i].resize(numberOfMachines);
    }
    if (!file.good()) {
        std::cout << std::endl << "Can't open the file" << std::endl;
    }
    else {
        int lineCounter = 0;
        std::string line;
        getline(file, line);
        while (getline(file, line)) {
            std::istringstream lline (line);
            for (int i = 0; i < numberOfMachines; i++) {
                lline >> matrix[lineCounter + 1][i];
            }
            lineCounter++;
        }
    }
    for (int i = 1; i < numberOfTasks + 1; i++) {
        matrix[i].insert(matrix[i].begin(), 0);
    }
    for (int i = 0; i < numberOfMachines + 1; i++) {
        matrix[0].push_back(0);
    }
}

void printMatrix(int numberOfTasks, int numberOfMachines, std::vector <std::vector <int>> matrix) {
    for (int i = 0; i < numberOfTasks+1; i++) {
        for (int j = 0; j < numberOfMachines+1; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}

int saResult(std::vector <std::vector <int>> matrix, int numberOfTasks, int numberOfMachines) {
    std::vector <int> perm;
    for (int i = 1; i < numberOfTasks + 1; i++)
        perm.push_back(i);
    std::vector <int> bestPerm = perm;
    int initialResult; // x0
    initialResult = calculateC(matrix, perm, numberOfTasks, numberOfMachines);
    int currentResult = initialResult; // x
    int bestResult = currentResult;
    int newResult = currentResult;
    double initialTemperature = 1000;
    double currentTemperature = initialTemperature;
    double finalTemperature = 0.01;
    double delta;
    double p;
    double z;
    double coolingRate = 0.9995;
    while (currentTemperature > finalTemperature) {
        int firstRandomNumber;
        firstRandomNumber = generateRandomNumber(1, numberOfTasks);
        int secondRandomNumber;
        secondRandomNumber = generateRandomNumber(1, numberOfTasks);
        checkIfEqual(firstRandomNumber, secondRandomNumber, numberOfTasks);
        switchPlaces(firstRandomNumber, secondRandomNumber, perm);
        newResult = calculateC(matrix, perm, numberOfTasks, numberOfMachines);
        if (bestResult > newResult) {
            bestResult = newResult;
            bestPerm = perm;
        }
        if (newResult <= currentResult)
            currentResult = newResult;
        else {
            delta = newResult - currentResult;
            p = exp(-delta/currentTemperature);
            z = generateRandomDoubleNumber(0, 1);
            if (z < p)
                currentResult = newResult;
            currentTemperature *= coolingRate;
        }
    }
    return bestResult;
}

void printResults(int result, int numberOfFile) {
    int intermediateResults[9] = {48, 799, 1340, 1991, 1540, 1856, 2787, 7108, 8042};
    int bestResults[9] = {47, 637, 1163, 1805, 1137, 1471, 2397, 5967, 6369};

    std::cout << "Intermediate result: " + std::to_string(intermediateResults[numberOfFile-1]) << std::endl;
    std::cout << "Result obtained: " + std::to_string(result) << std::endl;
    std::cout << "Best result: " + std::to_string(bestResults[numberOfFile-1]) << std::endl;
}

int chooseFile() {
    int number;
    std::cout << "Choose a data file (from 1 to 9): ";
    std::cin >> number;
    return number;
}

int main() {
    int numberOfTasks = 0;
    int numberOfMachines = 0;
    std::vector <std::vector <int>> matrix;
    int result = 0;
    while (1) {
    int numberOfFile = chooseFile();
    loadData(numberOfTasks, numberOfMachines, matrix, std::to_string(numberOfFile));
    printMatrix(numberOfTasks, numberOfMachines, matrix);
    result = saResult(matrix, numberOfTasks, numberOfMachines);
    printResults(result, numberOfFile);
    }
    return 0;
}
