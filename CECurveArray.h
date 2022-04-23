//************************************************
//************************************************
// ������� �������� ������ �� CAD Exchanger
//            ����� ������
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
// ���������������� ��������� �� ������
//************************************************
class CeSmartCurvePointer {
    std::shared_ptr<CeCurve> ptr;

  public:
    double cirRad; // ������ ���������� ��� 0, ���� �� ����������. ������������ ������� ��������� �������� �������������� ��� ������������ ��������!

    CeSmartCurvePointer(CeCurve* Ptr, double CirRad=0.) : ptr(Ptr), cirRad(CirRad) { }
    
    bool operator < (const CeSmartCurvePointer& other) const { return cirRad <  other.cirRad; }
    
    void print() const { ptr->print(); }
};

//************************************************
// ����� ������
//************************************************
class CeCurveArray {
    std::vector<CeSmartCurvePointer> curveVec;
  public:
    int size() const { return int(curveVec.size()); }
    bool add(CeCurve* pCurve, double cRad=0.);   // �������� ������
    bool add(const CeSmartCurvePointer& scp);    // �������� ������
    bool randomPolulate(int count=100);          // ��������� ���������� �������
    void printArray(const char* header) const;   // ����� ������ ������
    void sortRad();                              // ���������� �� ����������� �������
    double sumRad() const;                       // ������������ ��������
    double sumRadSingleThread() const;           // ������������ �������� ��� ��������� ���������������
    double sumRadMultiThread() const;            // ������������ �������� c ���������� ���������������
    
    const CeSmartCurvePointer& operator[](int n) const { return curveVec[n]; } // ��������� �� ������� (������)
    // CeSmartCurvePointer& operator[](int n)          { return curveVec[n]; } // ��������� �� ������� (������)
};


} // namespace CadExchangerTestSpace

#endif //CECURVEARRAY_H
