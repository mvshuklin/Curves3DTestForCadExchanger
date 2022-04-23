//************************************************
//************************************************
// Решение тестовой задачи от CAD Exchanger
//            Библиотека кривых
//               CECurve.cpp
//************************************************
//************************************************
//             СОДЕРЖАНИЕ:
// Утилиты
// CePoint - Точка
// CeVector - Вектор
// CeCurve - Кривая
// CeCircle - Окружность
// CeEllipse - Эллипс
// CeHelix - Спираль
//************************************************
//************************************************
#include <stdio.h>
#include <math.h>
#include "CECurve.h"

const double PI = 3.14159265358979323846;
const double EPS = 1E-7; // Минимальная дистанция
const double CeEpsAng = 3.E-4;
const double CeEpsValid = EPS*1000.;
const double CeMaxSize = 1E9;

using namespace CadExchangerTestSpace;

//************************************************
// Утилиты
//************************************************
void printCommon(const CeCurve* pCurve, const char* Name, const CePoint& cen) {
  if(pCurve==nullptr) return;
  double param=PI/4;
  CePoint pt=pCurve->pointAt(param);
  CeVector deriv=pCurve->firstDeriv(param);
  printf("%s Pt=%5.1f,%5.1f,%4.1f Der=%5.1f,%5.1f,%4.1f Cen=%2.0f,%2.0f", Name, pt.x, pt.y, pt.z, deriv.x, deriv.y, deriv.z, cen.x, cen.y); 
}


//************************************************
// CePoint - Точка
//************************************************
// Расстояние между точками
double CePoint::distTo(const CePoint& other) const {
  double dx=x-other.x, dy=y-other.y, dz=z-other.z;
  return sqrt(dx*dx+dy*dy+dz*dz);
}


//************************************************
// CeVector - Вектор
//************************************************
// Длина вектора
double CeVector::length() const {
  return sqrt(x*x+y*y+z*z);
}

// Иниц. плоского вектора
void CeVector::setPolar2D(double XYAngle, double length) {
  x=length*cos(XYAngle);
  y=length*sin(XYAngle);
  z=0.;
}


//************************************************
// CeCurve - Кривая
//************************************************
  // 1-я производная (касательная). Длина вектора равна радиусу кривизны
CeVector CeCurve::firstDeriv(double param) const {
  double eps=EPS, epsA=CeEpsAng;
  CePoint prev=pointAt(param-epsA),  pt=pointAt(param), next=pointAt(param+epsA), mid(prev, next);
  CeVector deriv(prev, next);
  double h=pt.distTo(mid);   // Прогиб
  double L=deriv.length();   // Хорда
  double curRad= h>eps/10000. ? L*L/(8.*h) : CeMaxSize;  // Радиус кривизны
  double factor= L>eps ? curRad / L : CeMaxSize;
  deriv *= factor;  return deriv;
}

double CeCurve::endParam() const {
  return 2*PI;
}

void CeCurve::print() const {
  printf("Curve:"); 
}

//************************************************
// CeCircle - Окружность
//************************************************
CePoint CeCircle::pointAt(double param) const {
  return CePoint(cen.x + rad*cos(param), cen.y + rad*sin(param));
}

CeVector CeCircle::firstDeriv(double param) const {
  CeVector vec;  vec.setPolar2D(param+PI/2., rad);  return vec;
}

void CeCircle:: print() const {  printCommon(this, "Circle: ", cen);  printf(" R=%3.0f", rad); };
bool CeCircle:: valid() const { return rad>CeEpsValid; }


//************************************************
// CeEllipse - Эллипс
//************************************************
CePoint CeEllipse::pointAt(double param) const {
  return CePoint(cen.x + radX*cos(param), cen.y + radY*sin(param));
}
void CeEllipse::print() const { printCommon(this, "Ellipse:", cen);  printf(" RX=%2.0f RY=%2.0f", radX, radY); };
bool CeEllipse::valid() const { return radX>CeEpsValid && radY>CeEpsValid; }


//************************************************
// CeHelix - Спираль
//************************************************
const int CeHelixCoil=2; // Число витков

CePoint CeHelix::pointAt(double param) const {
  return CePoint(cen.x + rad*cos(param), cen.y + rad*sin(param), param * step/(2.*PI));
}
double CeHelix::endParam() const { return CeHelixCoil * 2*PI; }

void CeHelix::print() const { printCommon(this, "Helix:  ", cen);  printf(" R=%3.0f Step=%2.0f", rad, step); };
bool CeHelix::valid() const { return rad>CeEpsValid && step>CeEpsValid; }
