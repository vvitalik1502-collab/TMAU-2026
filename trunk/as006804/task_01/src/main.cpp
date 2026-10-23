#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <algorithm> // Добавлено для std::replace

// Подключаем заголовочные файлы моделей
#include "LinearModel.h"
#include "NonLinearModel.h"
#include "DiffEqModel.h"

// Функция для генерации входного сигнала u(t)
double getInputSignal(int type, double t) {
    // type 1: Step (Ступенчатое) = 1.0
    if (type == 1) return 1.0;
    // type 2: Impulse (Импульсное) = 1.0 только на первом шаге, потом 0
    if (type == 2) return (t < 0.01) ? 1.0 : 0.0; // Чуть уменьшил шаг для точности импульса
    // type 3: Harmonic (Гармоническое) = sin(t)
    if (type == 3) return sin(t);
    return 0.0;
}

void runSimulation(Model* model, const std::string& modelName, int signalType, int steps, double dt) {
    std::cout << "\n--- Simulating " << modelName << " with Signal Type " << signalType << " ---\n";
    std::cout << std::setw(10) << "Step" << std::setw(15) << "Time" << std::setw(15) << "Input U" << std::setw(15) << "Output Y" << std::endl;

    // Формируем безопасное имя файла (заменяем пробелы и точки на подчеркивания)
    std::string safeName = modelName;
    std::replace(safeName.begin(), safeName.end(), ' ', '_');
    std::replace(safeName.begin(), safeName.end(), '.', '_');
    std::string filename = safeName + "_signal" + std::to_string(signalType) + ".csv";
    
    // Открываем файл для сохранения данных (для построения графиков)
    std::ofstream outFile(filename);
    outFile << "Time,Input,Output\n";

    for (int i = 0; i < steps; ++i) {
        double time = i * dt;
        double u = getInputSignal(signalType, time);
        
        // Вызов полиморфного метода step
        model->step(u);
        
        double y = model->getY();

        // Вывод в консоль (табличный вид)
        std::cout << std::setw(10) << i 
                  << std::setw(15) << std::fixed << std::setprecision(3) << time
                  << std::setw(15) << u
                  << std::setw(15) << y << std::endl;

        // Запись в CSV
        outFile << time << "," << u << "," << y << "\n";
    }
    outFile.close();
    std::cout << "Data saved to " << filename << std::endl;
}

int main() {
    int steps = 50; // Количество шагов симуляции (n)
    double dt = 0.1; // Шаг времени

    // Параметры для моделей (можно изменить)
    // Linear 1.4: a, b
    double lin_a = 0.9; 
    double lin_b = 0.5;
    
    // NonLinear 2.6: a1, a2, b
    double nl_a1 = 0.1;
    double nl_a2 = 0.2;
    double nl_b = 0.5;

    // DiffEq 3.8: a1, a2, b
    double de_a1 = 0.5;
    double de_a2 = 0.1;
    double de_b = 1.0;

    std::cout << "Enter number of steps (n): ";
    std::cin >> steps;

    // ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ: Проверка устойчивости для LinearModel (Model 1.4)
    // Корень характеристического уравнения z = a. Устойчиво, если |a| < 1.
    if (std::abs(lin_a) >= 1.0) {
        std::cout << "\n[ВНИМАНИЕ] Коэффициент 'a' >= 1. Система НЕУСТОЙЧИВА! Процесс будет расходящимся.\n" << std::endl;
    }

    // Создаем объекты моделей (полиморфизм)
    Model* linearModel = new LinearModel(lin_a, lin_b, 0.0, dt);
    Model* nonLinearModel = new NonLinearModel(nl_a1, nl_a2, nl_b, 0.0, dt);
    Model* diffEqModel = new DiffEqModel(de_a1, de_a2, de_b, 0.0, dt);

    // Запуск для разных сигналов (1-Step, 2-Impulse, 3-Harmonic)
    // Тестируем каждую модель на всех трех типах сигналов
    
    // Linear Model Tests
    runSimulation(linearModel, "Linear 1.4", 1, steps, dt);
    linearModel->setY(0); // Сброс состояния
    runSimulation(linearModel, "Linear 1.4", 2, steps, dt);
    linearModel->setY(0);
    runSimulation(linearModel, "Linear 1.4", 3, steps, dt);

    // NonLinear Model Tests
    runSimulation(nonLinearModel, "NonLinear 2.6", 1, steps, dt);
    nonLinearModel->setY(0);
    runSimulation(nonLinearModel, "NonLinear 2.6", 2, steps, dt);
    nonLinearModel->setY(0);
    runSimulation(nonLinearModel, "NonLinear 2.6", 3, steps, dt);

    // DiffEq Model Tests
    runSimulation(diffEqModel, "DiffEq 3.8", 1, steps, dt);
    diffEqModel->setY(0);
    runSimulation(diffEqModel, "DiffEq 3.8", 2, steps, dt);
    diffEqModel->setY(0);
    runSimulation(diffEqModel, "DiffEq 3.8", 3, steps, dt);

    delete linearModel;
    delete nonLinearModel;
    delete diffEqModel;

    return 0;
}