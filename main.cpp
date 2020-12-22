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
    int sum = 0;
    for (int j = 1; j < numberOfTasks + 1; j++) {
        for (int k = 1; k < numberOfMachines + 1; k++) {
            sum += std::max(matrix[perm[j]][k - 1], matrix[perm[j - 1]][k]) + matrix[perm[j]][k];
        }
    }
    return sum;
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

void loadData(int &numberOfTasks, int &numberOfMachines, std::vector <std::vector <int>> &matrix) {
    std::ifstream file ("/home/jerry/QtProjects/MaxFlowProblem/data1.txt");
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
    double coolingRate = 0.995;
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

int main() {
    int numberOfTasks = 0;
    int numberOfMachines = 0;
    std::vector <std::vector <int>> matrix;
    int result = 0;
    loadData(numberOfTasks, numberOfMachines, matrix);
//    printMatrix(numberOfTasks, numberOfMachines, matrix);
    result = saResult(matrix, numberOfTasks, numberOfMachines);
    std::cout << result << std::endl;
    return 0;
}
