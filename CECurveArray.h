//************************************************
//************************************************
// Решение тестовой задачи от CAD Exchanger
//            Набор кривых
//           CECurveArray.h
//************************************************
//************************************************
#ifndef CECURVEARRAY_H
#define CECURVEARRAY_H

#include <vector>
#include <memory>
#include "CeGeometry/CECurve.h"

namespace CadExchangerTestSpace {
  
//************************************************
// Интеллектуальный указатель на кривую
//************************************************
class CeSmartCurvePointer {
    std::shared_ptr<CeCurve> ptr;

  public:
    double cirRad; // Радиус окружности или 0, если не окружность. Дублирование радиуса позволяет повысить быстродействие при суммировании радиусов!

    CeSmartCurvePointer(CeCurve* Ptr, double CirRad=0.) : ptr(Ptr), cirRad(CirRad) { }
    
    bool operator < (const CeSmartCurvePointer& other) const { return cirRad <  other.cirRad; }
    
    void print() const { ptr->print(); }
};

//************************************************
// Набор кривых
//************************************************
class CeCurveArray {
    std::vector<CeSmartCurvePointer> curveVec;
  public:
    int size() const { return int(curveVec.size()); }
    bool add(CeCurve* pCurve, double cRad=0.);   // Добавить кривую
    bool add(const CeSmartCurvePointer& scp);    // Добавить кривую
    bool randomPolulate(int count=100);          // Заполнить случайными кривыми
    void printArray(const char* header) const;   // Вывод списка кривых
    void sortRad();                              // Сортировка по возрастанию радиуса
    double sumRad() const;                       // Суммирование радиусов
    double sumRadSingleThread() const;           // Суммирование радиусов без поддержки многопоточности
    double sumRadMultiThread() const;            // Суммирование радиусов c поддержкой многопоточности
    
    const CeSmartCurvePointer& operator[](int n) const { return curveVec[n]; } // Обращение по индексу (чтение)
    // CeSmartCurvePointer& operator[](int n)          { return curveVec[n]; } // Обращение по индексу (запись)
};


} // namespace CadExchangerTestSpace

#endif //CECURVEARRAY_H
