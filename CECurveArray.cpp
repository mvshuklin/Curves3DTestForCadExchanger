//************************************************
//************************************************
// ������� �������� ������ �� CAD Exchanger
//            ����� ������
//           CECurveArray.cpp
//************************************************
//************************************************
#include <algorithm>
#include <omp.h>
#include <cstdlib> // rand
#include "CECurveArray.h"

namespace CadExchangerTestSpace {

//************************************************
// �������
//************************************************
// ��������� ����� � ���������
int randRange(int start=1, int end=100) {
  if(end-start<1 || start<0) { printf("\nError range!"); return 0; }
  int rnd=rand() % (end-start+1) + start;
  return rnd;
}

//************************************************
// ����� ������
//************************************************
// �������� ������
bool CeCurveArray::add(CeCurve *pCurve, double cRad) {
  if(pCurve==nullptr) { printf("\nMemory error!");  return false; }
  CeSmartCurvePointer curvePtr(pCurve, cRad); // ������ ������� ��� ���������. ������ ��� ������
  if(!pCurve->valid()) { printf("\nCurve no valid! ");  pCurve->print(); return false; }
  curveVec.push_back(curvePtr);
  return true;
}
// �������� ������
bool CeCurveArray::add(const CeSmartCurvePointer& scp) {
  curveVec.push_back(scp);
  return true;
}

// ��������� ���������� �������
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

// ����� ������ ������
void CeCurveArray::printArray(const char* header) const {
  if (header) printf("\n\n  %s", header);
  for (int i = 0; i < size(); i++) {
    printf("\n%3d ", i);
    curveVec[i].print();
  }
}

// ���������� �� ����������� �������
void CeCurveArray::sortRad() {
  std::sort(curveVec.begin(), curveVec.end());
}

// ������������ ��������
double CeCurveArray::sumRad() const {
  const int minMultiThreadSize = 1000; // ��� ����� ������� �������, ������� �� �������� ������� ��������� ������� �� ���������������.
                                       // �������� minMultiThreadSize ������� �� ����������� ������������ �������������� (8-������� ���������).
  return size() < minMultiThreadSize ? sumRadSingleThread() : sumRadMultiThread();
}

// ������������ �������� ��� ��������� ���������������
double CeCurveArray::sumRadSingleThread() const {
  double sum = 0.;
  for (int i = size() - 1; i >= 0; i--) sum += curveVec[i].cirRad;
  return sum;
}

// ������������ �������� c ���������� ���������������
double CeCurveArray::sumRadMultiThread() const {
  double sum = 0.;  int sz = size();
#pragma omp parallel for reduction(+:sum)
  for (int i = sz - 1; i >= 0; i--) 
    sum += curveVec[i].cirRad;
  return sum;
}


} // namespace CadExchangerTestSpace
