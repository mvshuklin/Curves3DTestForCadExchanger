//************************************************
//************************************************
// Решение тестовой задачи от CAD Exchanger
//            Набор кривых
//           CECurveArray.cpp
//************************************************
//************************************************
#include <algorithm>
#include <omp.h>
#include <cstdlib> // rand
#include "CECurveArray.h"

namespace CadExchangerTestSpace {

//************************************************
// Утилиты
//************************************************
// Случайное число в диапазоне
int randRange(int start=1, int end=100) {
  if(end-start<1 || start<0) { printf("\nError range!"); return 0; }
  int rnd=rand() % (end-start+1) + start;
  return rnd;
}

//************************************************
// Набор кривых
//************************************************
// Добавить кривую
bool CeCurveArray::add(CeCurve *pCurve, double cRad) {
  if(pCurve==nullptr) { printf("\nMemory error!");  return false; }
  CeSmartCurvePointer curvePtr(pCurve, cRad); // Создаём заранее для автоосвоб. памяти при ошибке
  if(!pCurve->valid()) { printf("\nCurve no valid! ");  pCurve->print(); return false; }
  curveVec.push_back(curvePtr);
  return true;
}
// Добавить кривую
bool CeCurveArray::add(const CeSmartCurvePointer& scp) {
  curveVec.push_back(scp);
  return true;
}

// Заполнить случайными кривыми
bool CeCurveArray::randomPolulate(int count) {
  curveVec.clear();

  try {
    double x = 0., y = 0.;
    for(int i=0;i<count;i++) {
      int type=randRange(0,2);
      double rad=double(randRange());
      if(type==0) {
        if(!add(new CeCircle(x,y,rad), rad)) return false;
      }
      if(type==1) {
        double rad2=double(randRange());
        if(!add(new CeEllipse(x,y,rad,rad2))) return false;
      }
      if(type==2) {
        double step=double(randRange());
        if(!add(new CeHelix(x,y,rad,step))) return false;
      }
      x+=1.;  y+=1.;
    }
  }
  catch (...) { printf("\nOut memory error!");  return false; }
  return true;
}

// Вывод списка кривых
void CeCurveArray::printArray(const char* header) const {
  if (header) printf("\n\n  %s", header);
  for (int i = 0; i < size(); i++) {
    printf("\n%3d ", i);
    curveVec[i].print();
  }
}

// Сортировка по возрастанию радиуса
void CeCurveArray::sortRad() {
  std::sort(curveVec.begin(), curveVec.end());
}

// Суммирование радиусов
double CeCurveArray::sumRad() const {
  const int minMultiThreadSize = 1000; // При малом размере массива, затраты на создание потоков превышают выигрыш от многопоточности.
                                       // Значение minMultiThreadSize выбрано по результатам тестирования быстродействия (8-ядерный процессор).
  return size() < minMultiThreadSize ? sumRadSingleThread() : sumRadMultiThread();
}

// Суммирование радиусов без поддержки многопоточности
double CeCurveArray::sumRadSingleThread() const {
  double sum = 0.;
  for (int i = size() - 1; i >= 0; i--) sum += curveVec[i].cirRad;
  return sum;
}

// Суммирование радиусов c поддержкой многопоточности
double CeCurveArray::sumRadMultiThread() const {
  double sum = 0.;  int sz = size();
#pragma omp parallel for reduction(+:sum)
  for (int i = sz - 1; i >= 0; i--) 
    sum += curveVec[i].cirRad;
  return sum;
}


} // namespace CadExchangerTestSpace
