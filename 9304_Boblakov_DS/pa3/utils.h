#ifndef PA3_UTILS_H
#define PA3_UTILS_H

#include <thread>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>

typedef std::vector<std::vector<int>> Matrix;
typedef std::pair<Matrix, Matrix> pMatrix;

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::vector<std::vector<int>> generateMatrix(int m_width, int m_height) {
    std::vector<int> init_vec(m_height, 0);
    Matrix gen_matrix(m_width, init_vec);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-500, 500);
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            gen_matrix[i][j] = distr(gen);
        }
    }
    return gen_matrix;
}


struct Node {

    const Matrix data;
    std::shared_ptr<Node> next = nullptr;

    explicit Node(Matrix value) : data(std::move(value)) {}

};

class CustomQueue {

    std::shared_ptr<Node> head{};

public:
    Matrix pop() {
        std::shared_ptr<Node> first{};

        first = head;

        while (!first || !std::atomic_compare_exchange_weak(&head, &first, first->next)){
            first = head;
        }

        return first->data;
    }

    void push(const Matrix &value) {
        auto newNode = std::make_shared<Node>(value);

        std::shared_ptr<Node> first{};

        first = head;
        newNode->next = first;

        while (!std::atomic_compare_exchange_weak(&head, &first, newNode)){
            first = head;
            newNode->next = first;
        }
    }
};

#endif //PA3_UTILS_H