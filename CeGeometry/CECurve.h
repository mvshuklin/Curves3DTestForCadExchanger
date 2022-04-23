//************************************************
//************************************************
// ������� �������� ������ �� CAD Exchanger
//            ���������� ������
//                CECurve.h
//************************************************
//************************************************
//             ����������:
// CePoint - �����
// CeVector - ������
// CeCurve - ������
// CeCircle - ����������
// CeEllipse - ������
// CeHelix - �������
//************************************************
//************************************************
#ifndef CECURVE_H
#define CECURVE_H

#undef  DLLIMPEXP
#ifdef CEGEOMETRY_EXPORTS
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif


namespace CadExchangerTestSpace {

//************************************************
// CePoint - �����
//************************************************
struct DLLIMPEXP CePoint {
  double x, y, z;
  
  CePoint() : x(0.), y(0.), z(0.){ }
  CePoint(double xx, double yy) : x(xx), y(yy), z(0.){ }
  CePoint(double xx, double yy, double zz) : x(xx), y(yy), z(zz){ }
  CePoint(const CePoint& pt1, const CePoint& pt2) : x((pt1.x+pt2.x)/2.), y((pt1.y+pt2.y)/2.), z((pt1.z+pt2.z)/2.){ } // Mid
  
  // ���������� ����� �������
  double distTo(const CePoint& other) const;
};

//************************************************
// CeVector - ������
//************************************************
struct DLLIMPEXP CeVector {
  double x, y, z;

  CeVector() : x(1.0), y(0.), z(0.){ }
  CeVector(double xx, double yy) : x(xx), y(yy), z(0.){ }
  CeVector(double xx, double yy, double zz) : x(xx), y(yy), z(zz){ }
  CeVector(const CePoint& pt1, const CePoint& pt2) : x(pt2.x-pt1.x), y(pt2.y-pt1.y), z(pt2.z-pt1.z){ } // Dif
  
  double length() const; // ����� �������
  
  void setPolar2D(double XYAngle, double length); // ����. �������� �������
  
  CeVector operator * (double scale) const { return CeVector (x * scale, y * scale, z * scale); }
  CeVector& operator *= (double scale) { x*=scale;  y*=scale;  z*=scale;  return *this; }
};

//************************************************
// CeCurve - ������
//************************************************
class DLLIMPEXP CeCurve {
  public:
    virtual ~CeCurve() { }; // ��� ������������ �����������, ������ ����������� �������� �����-�� �� �����! ���������� �� �����������.

    virtual CePoint pointAt(double param) const=0;   // ����� �� ������
    virtual CeVector firstDeriv(double param) const; // 1-� ����������� (�����������). ����� ������� ����� ������� ��������
    
    virtual double startParam() const { return 0.; }
    virtual double endParam() const;
    
    virtual void print() const;
    virtual bool valid() const=0; // �������� ������������
};

//************************************************
// CeCircle - ����������
//************************************************
class DLLIMPEXP CeCircle : public CeCurve {
    CePoint cen;
    double rad;
  public:
    CeCircle(const CePoint& Cen, double Rad) : cen(Cen), rad(Rad) { }
    CeCircle(double xx, double yy, double Rad) : cen(xx,yy), rad(Rad) { }
    
    CePoint getCenter() const { return cen; }
    double getRadius() const { return rad; }

    CePoint pointAt(double param) const;
    CeVector firstDeriv(double param) const;
    
    void print() const;
    bool valid() const;
};


//************************************************
// CeEllipse - ������
//************************************************
class DLLIMPEXP CeEllipse : public CeCurve {
    CePoint cen;
    double radX, radY;
  public:
    CeEllipse(double xx, double yy, double RadX, double RadY) : cen(xx,yy), radX(RadX), radY(RadY) { }

    CePoint pointAt(double param) const;

    void print() const;
    bool valid() const;
};


//************************************************
// CeHelix - �������
//************************************************
class DLLIMPEXP CeHelix : public CeCurve {
    CePoint cen;
    double rad, step;
    // static const int coil=2; // ����� ������

  public:
    CeHelix(double xx, double yy, double Rad, double Step) : cen(xx,yy), rad(Rad), step(Step) { }
    
    double endParam() const;
    
    CePoint pointAt(double param) const;

    void print() const;
    bool valid() const;
};


} // namespace CadExchangerTestSpace

#endif //CECURVE_H
