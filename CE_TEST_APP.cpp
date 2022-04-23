//************************************************
//************************************************
// Решение тестовой задачи от CAD Exchanger
//          Главная программа
//           CE_TEST_APP.cpp
//************************************************
//************************************************
#include <stdio.h>
#include "CECurveArray.h"

int main() {
  // Инициализация набора кривых
  CadExchangerTestSpace::CeCurveArray curveArray;
  if(!curveArray.randomPolulate()) return 0;
  curveArray.printArray("Random curves array:");

  // Выборка окружностей
  CadExchangerTestSpace::CeCurveArray circleArray;
  for (int i = 0; i < curveArray.size(); i++) {
    if (curveArray[i].cirRad) circleArray.add(curveArray[i]);
  }

  // Обработка набора окружностей
  circleArray.sortRad();              // Сортировка по возрастанию радиуса
  auto sumRad = circleArray.sumRad(); // Суммирование радиусов
  circleArray.printArray("Sorting circles array:");
  printf("\n\n sumRad=%.2f\n\n", sumRad);

  return 0;
}
