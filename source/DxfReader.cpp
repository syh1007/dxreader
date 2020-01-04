#include "DxfReader.h"
#include "dl_dxf.h"


using namespace std;

NAMESPACE_DR_BEGIN

#define PI 3.141592657

DxfReader::DxfReader(const string & dxfile)
{

    m_dxfile = dxfile;
}

int DxfReader::ParseDxfFile()
{
    DL_Dxf * dld = new DL_Dxf();

    bool rst = dld->in(m_dxfile.c_str(), this);

    delete dld;
    return rst ? 0 : -1;
}

DxfReader::~DxfReader()
{
    std::vector<DxfObject *>::iterator iter;
    for (iter = m_objList.begin(); iter != m_objList.end(); ++iter)
    {
        DxfObject * obj = *iter;

        delete obj;
    }

    m_objList.clear();
}



int DxfReader::GetObjectList(std::vector<DxfObject *> & objList)
{
    if (m_objList.empty())
    {
        return -1;
    }

    std::vector<DxfObject *>::iterator iter;
    for (iter = m_objList.begin(); iter != m_objList.end(); ++iter)
    {
        DxfObject * obj = *iter;

        objList.push_back(obj->Clone());
    }

    return 0;
}

void DxfReader::addPoint(const DL_PointData & data)
{
    if (!m_flagBlock)
    {
        DxfPoint * point = new DxfPoint(data.x, data.y);

        m_objList.push_back(point);
    }
}


void DxfReader::addLine(const DL_LineData & data)
{
    if (!m_flagBlock)
    {
        DxfPoint begin = DxfPoint(data.x1, data.y1);

        DxfPoint end = DxfPoint(data.x2, data.y2);

        DxfLine * line = new  DxfLine(begin, end);

        m_objList.push_back(line);
    }
}


void DxfReader::addArc(const DL_ArcData & data)
{
    if (!m_flagBlock)
    {
        DxfPoint center = DxfPoint(data.cx, data.cy);

        DxfExtrusion extrusion = DxfExtrusion(data.edx, data.edy, data.edz);

        DxfArc * arc = new DxfArc(center, data.radius,
                                  data.angle1, data.angle2, extrusion);

        m_objList.push_back(arc);
    }
}


void DxfReader::addCircle(const DL_CircleData & data)
{
    if (!m_flagBlock )
    {
        DxfPoint center = DxfPoint(data.cx, data.cy);

        DxfExtrusion extrusion = DxfExtrusion(data.edx, data.edy, data.edz);

        DxfCircle * circle = new  DxfCircle(center, data.radius, extrusion);

        m_objList.push_back(circle);
    }

}


void DxfReader::addPolyline(const DL_PolylineData & data)
{
    m_pointCnt = data.number;

    m_flagPolyLine = data.flags;
}


void DxfReader::addVertex(const DL_VertexData & data)
{
    if (!m_flagBlock)
    {
        DxfPoint point = DxfPoint(data.x, data.y, data.bulge);

        DxfExtrusion extrusion = DxfExtrusion(data.edx, data.edy, data.edz);

        m_pointList.push_back(point);

        m_pointCnt--;

        if (m_pointCnt == 0)
        {
            DxfPolyLine * polyLine = new DxfPolyLine(m_pointList, extrusion, m_flagPolyLine);

            m_objList.push_back(polyLine);

            m_pointList.clear();
        }
    }

}

void DxfReader::addEllipse(const DL_EllipseData & data)
{
    if (!m_flagBlock )
    {
        DxfPoint center = DxfPoint(data.cx, data.cy);

        DxfPoint mapoint = DxfPoint(data.mx, data.my);

        double malen = sqrt(pow(data.mx, 2) + pow(data.my, 2));

        double minlen = malen * data.ratio;

        DxfEllipse * ellipse = new DxfEllipse(center, mapoint, malen, minlen,
                                              data.angle1, data.angle2);
        m_objList.push_back(ellipse);
    }
}

//AcDbAlignedDimension
void DxfReader:: addDimAlign(const DL_DimensionData & data1,
                             const DL_DimAlignedData & data2)
{
    if (!m_flagBlock)
    {
        DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                                data1.text, data1.realValue);
        DxfPoint p1 = DxfPoint(data2.epx1, data2.epy1);

        DxfPoint p2 = DxfPoint(data2.epx2, data2.epy2);

        //no angle and oblique
        DxfDimLiner * dim = new DxfDimLiner(p1, p2, 0.0, 0.0, dimension);

        m_objList.push_back(dim);
    }
}

//AcDbAlignedDimension、AcDbRotatedDimension
void DxfReader:: addDimLinear(const DL_DimensionData & data1,
                              const DL_DimLinearData & data2)
{
    if (!m_flagBlock)
    {
        DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                               data1.text, data1.realValue);
        DxfPoint p1 = DxfPoint(data2.dpx1, data2.dpy1);

        DxfPoint p2 = DxfPoint(data2.dpx2, data2.dpy2);

        DxfDimLiner * dim = new DxfDimLiner(p1, p2, data2.angle,
                                                    data2.oblique, dimension);

        m_objList.push_back(dim);
    }
}

// AcDbDiametricDimension
void DxfReader::addDimRadial(const DL_DimensionData & data1 ,
                             const DL_DimRadialData & data2)
{
    if (!m_flagBlock)
    {
        DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                               data1.text, data1.realValue);

        DxfDimRadOrDia * dim = new DxfDimRadOrDia(DxfPoint(data2.dpx, data2.dpy),
                                                data2.leader, dimension);
        m_objList.push_back(dim);
    }
}

//AcDbRadialDimension
void DxfReader::addDimDiametric(const DL_DimensionData & data1,
                                 const DL_DimDiametricData & data2)
{
    if (!m_flagBlock)
    {
       DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                               data1.text, data1.realValue);

        DxfDimRadOrDia * dim = new DxfDimRadOrDia(DxfPoint(data2.dpx, data2.dpy),
                                                data2.leader, dimension);
        m_objList.push_back(dim);
    }
}

//AcDb2LineAngularDimension
void DxfReader:: addDimAngular(const DL_DimensionData & data1,
                               const DL_DimAngularData & data2)
{
    if (!m_flagBlock)
    {
       DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                               data1.text, data1.realValue);

       DxfDimAngular * dim = new DxfDimAngular(DxfPoint(data2.dpx1, data2.dpy1),
                                                DxfPoint(data2.dpx2, data2.dpy2),
                                                DxfPoint(data2.dpx3, data2.dpy3),
                                                DxfPoint(data2.dpx4, data2.dpy4), dimension);
        m_objList.push_back(dim);
    }
}

//AcDb3PointAngularDimension
void DxfReader:: addDimAngular3P(const DL_DimensionData & data1,
                                 const DL_DimAngular3PData & data2)
{
    if (!m_flagBlock)
    {
        DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                               data1.text, data1.realValue);

        DxfDimAngular * dim = new DxfDimAngular(DxfPoint(data2.dpx1, data2.dpy1),
                                                DxfPoint(data2.dpx2, data2.dpy2),
                                                DxfPoint(data2.dpx3, data2.dpy3),
                                                DxfPoint(0.0, 0.0), dimension);
        m_objList.push_back(dim);
    }
}

//AcDbOrdinateDimension
void DxfReader:: addDimOrdinate(const DL_DimensionData & data1,
                                const DL_DimOrdinateData & data2)
{
    if (!m_flagBlock)
    {
        DxfDimension  dimension = DxfDimension(data1.type, DxfPoint(data1.dpx, data1.dpy),
                                               DxfPoint(data1.mpx,data1.mpy),
                                               data1.text, data1.realValue);

        DxfDimOrdinate * dim = new DxfDimOrdinate(DxfPoint(data2.dpx1, data2.dpy1),
                                                  DxfPoint(data2.dpx2, data2.dpy2),
                                                  data2.xtype, dimension);
        m_objList.push_back(dim);
    }
}

void DxfReader::addText(const DL_TextData & data)
{
    if (!m_flagBlock)
    {
        DxfText * text = new DxfText(DxfPoint(data.ipx, data.ipy),
                                      DxfPoint(data.apx, data.apy), data.height,
                                      data.xScaleFactor, data.textGenerationFlags,
                                      data.hJustification, data.vJustification,
                                      data.text, data.style, data.angle);
        m_objList.push_back(text);
    }
}

void DxfReader::addMText(const DL_MTextData & data)
{
    if (!m_flagBlock)
    {
        DxfMText * mtext = new DxfMText(DxfPoint(data.ipx, data.ipy),
                                        DxfPoint(data.dirx, data.diry),
                                        data.height, data.width, data.attachmentPoint,
                                        data.drawingDirection, data.lineSpacingStyle,
                                        data.lineSpacingFactor,
                                        data.text, data.style, data.angle);

        m_objList.push_back(mtext);
    }
}

void DxfReader::addBlock(const DL_BlockData & data)
{
    m_flagBlock = true;
}

void DxfReader::endBlock()
{
    m_flagBlock = false;
}


void DxfReader::OCStoWCS(const DxfExtrusion & extru, DxfPoint & point)
{
    double Nx = extru.m_edx, Ny = extru.m_edy, Nz = extru.m_edz;

    double XAx, YAx, ZAx, Ax;
    double XAy, YAy, ZAy, Ay;

    //逆任意轴算法
    if (fabs(Nx) < 1.0 / 64 && fabs(Ny) < 1.0 / 64)
    {
        Ax = sqrt(pow(Nx, 2) + pow(Nz, 2));

        XAx = Nz / Ax;
        YAx = 0;
        ZAx = -Nx / Ax;

        Ay = sqrt(pow(Nx, 2) + pow(Ny, 2)
            + pow(pow(Nx, 2) + pow(Nz, 2), 2)
            + pow(Ny, 2) * pow(Nz, 2));

        XAy = -Nz * Ny / Ay;
        YAy = (pow(Nx, 2) + pow(Nz, 2)) / Ay;
        ZAy = -Ny * Nz / Ay;
    }
    else
    {
        Ax = sqrt(pow(Nx, 2) + pow(Ny, 2));
        XAx = -Ny / Ax;
        YAx = Nx / Ax;;
        ZAx = 0;

        Ay = sqrt(pow(Nx, 2) * pow(Nz, 2)
            + pow(pow(Nx, 2) + pow(Ny, 2), 2)
            + pow(Ny, 2) * pow(Nz, 2));

        XAy = -Nx * Nz / Ay;
        YAy = -Ny * Nz / Ay;
        ZAy = (pow(Ny, 2) + pow(Nz, 2)) / Ay;

    }

    double wx = XAx * point.m_x + XAy * point.m_y;
    double wy = YAx * point.m_x + YAy * point.m_y;

    point.m_x = wx;
    point.m_y = wy;
}

//由圆心和圆上一点，求出该点的角度
double  DxfReader::GetArcAngle(const DxfPoint & center,
                               const DxfPoint & p)
{
    double cx = center.m_x, cy = center.m_y;
    double x = p.m_x, y = p.m_y;
    double r = sqrt(pow(x - cx, 2) + pow(y - cy, 2));

    double ox, oy;
    ox = cx + r;
    oy = cy;

    double kox, koy;//基准向量k
    kox = ox - cx;
    koy = oy - cy;

    double kx, ky;//要计算的向量
    kx = x - cx;
    ky = y - cy;

    double pcos, psin;
    pcos = (kx * kox + ky * koy) / pow(r, 2);//点乘
    psin = (kox * ky - kx * koy) / pow(r, 2);//叉乘

    if (psin >= 0)
    {
        if (pcos > 1)//防止浮点数溢出
            pcos = 1;
        if (pcos < -1)
            pcos = -1;
        return acos(pcos) * 180 / PI;
    }
    else
    {
        if (pcos > 1)
            pcos = 1;
        if (pcos < -1)
            pcos = -1;
        return  360 - acos(pcos) * 180 / PI;
    }
}

void  DxfReader::LineToArc(const DxfLine & line, DxfArc & arc)//已知起始点和终止点两点坐标，以及起始点凸度，若为弧则求圆心和半径
{
    //已知条件
    double sx = line.m_beginP.m_x;
    double sy = line.m_beginP.m_y;
    double ex = line.m_endP.m_x;
    double ey = line.m_endP.m_y;
    double bulge = line.m_beginP.m_bulge;

    //待求值
    double r = 0, cx = 0, cy = 0;

    //中间变量值
    double theta_degree;//圆心角,包角，弧度表示
    double dCenterX1 = 0, dCenterY1 = 0;//中间值圆心坐标1
    double dCenterX2 = 0, dCenterY2 = 0;//中间值圆心坐标2
    double dLength; //弦长
    double angleChordX = 0;//弦向量X正方向的角度，度数表示
    bool direction = true;//判断顺圆还是逆圆
    bool isMinorArc = true;//圆弧半径是否为较小的

    //圆心角
    theta_degree = 4 * atan(fabs(bulge));//弧度表示

    //弦长
    dLength = sqrt(pow(sx - ex, 2) + pow(sy - ey, 2));

    //圆弧半径
    r = fabs(0.5 * dLength / sin(0.5 * theta_degree));


    double k = 0.0;//弦的斜率
    double k_verticle = 0.0;//弦的中垂线的斜率
    double mid_x = 0.0, mid_y = 0.0;//弦的中点坐标

    double a = 1.0, b = 1.0, c = 1.0;

    if (fabs(ey - sy) <= 1.0e-6)//两点Y值相同
    {
        dCenterX1 = (sx + ex) / 2.0;
        dCenterX2 = (sx + ex) / 2.0;
        dCenterY1 = sy + sqrt(r * r - (sx - ex) * (sx - ex) / 4.0);//下半圆上的点
        dCenterY2 = ey - sqrt(r * r - (sx - ex) * (sx - ex) / 4.0);//上半圆上的点
    }
    else if (fabs(ex - sx) <= 1.0e-6)//两点X值相同
    {
        dCenterY1 = (sy + ey) / 2.0;
        dCenterY2 = (sy + ey) / 2.0;
        dCenterX1 = sx + sqrt(r * r - (sy - ey) * (sy - ey) / 4.0);//左半圆上的点
        dCenterX2 = ex - sqrt(r * r - (sy - ey) * (sy - ey) / 4.0);//右半圆上的点
    }
    else
    {
        k = (ey - sy) / (ex - sx);
        k_verticle = -1.0 / k;
        mid_x = (sx + ex) / 2.0;
        mid_y = (sy + ey) / 2.0;
        //直线相交求解
        a = 1.0 + k_verticle * k_verticle;
        b = -2 * mid_x - k_verticle * k_verticle * (sx + ex);
        c = mid_x * mid_x + k_verticle * k_verticle * (sx + ex) * (sx + ex) / 4.0
            - (r * r - ((mid_x - sx) * (mid_x - sx) + (mid_y - sy) * (mid_y - sy)));

        dCenterX1 = (-1.0 * b + sqrt(b * b - 4 * a * c)) / (2 * a);//二元一次方程解
        dCenterX2 = (-1.0 * b - sqrt(b * b - 4 * a * c)) / (2 * a);
        dCenterY1 = k_verticle * dCenterX1 - k_verticle * mid_x + mid_y;//垂直于弦的那条直线，带入求解
        dCenterY2 = k_verticle * dCenterX2 - k_verticle * mid_x + mid_y;
    }


    /*凸度绝对值小于1表示圆弧包角小于180°，凸度绝对值大于1表示圆弧包角大于180°*/
    if (fabs(bulge) <= 1)
        isMinorArc = true;//包角小于180
    else
        isMinorArc = false;


    //确定圆弧的顺逆
    if (bulge < 0)
        direction = true;//顺圆
    else
        direction = false;

    //Get Center
    angleChordX = acos((1 * (ex - sx) + 0 * (ey - sy)) / dLength) * 180 / PI;//度数表示

    if ((ey - sy) < 0)
    {
        angleChordX *= -1;
    }

    if ((angleChordX > 0 && angleChordX < 180) || angleChordX == 180)
    {
        if (direction)//顺圆
        {
            if (isMinorArc)
            {
                cx = dCenterX1;
                cy = dCenterY1;
            }
            else
            {
                cx = dCenterX2;
                cy = dCenterY2;
            }
        }
        else//逆圆
        {
            if (isMinorArc)
            {
                cx = dCenterX2;
                cy = dCenterY2;
            }
            else
            {
                cx = dCenterX1;
                cy = dCenterY1;
            }
        }
    }
    else
    {
        if (direction)//顺圆
        {
            if (isMinorArc)
            {
                cx = dCenterX2;
                cy = dCenterY2;
            }
            else
            {
                cx = dCenterX1;
                cy = dCenterY1;
            }
        }
        else//逆圆
        {
            if (isMinorArc)
            {
                cx = dCenterX1;
                cy = dCenterY1;
            }
            else
            {
                cx = dCenterX2;
                cy = dCenterY2;
            }
        }
    }

    //Construct ARC
    arc.m_radius = r;
    arc.m_centerP.m_x = cx;
    arc.m_centerP.m_y = cy;

    if (direction)
    {
        arc.m_startAngle = GetArcAngle(arc.m_centerP, line.m_endP);//终止点计算起始角

        arc.m_endAngle = GetArcAngle(arc.m_centerP, line.m_beginP);//起始点计算终止角
    }
    else
    {
        arc.m_startAngle = GetArcAngle(arc.m_centerP, line.m_beginP);

        arc.m_endAngle = GetArcAngle(arc.m_centerP, line.m_endP);
    }
}


double DxfReader::GetEllipseRoAngle(double mx, double my)
{
    if (mx > 0)
    {
        if (my > 0)//第一象限
            return atan(my / mx) * 180 / PI;
        if (my < 0)//第四象限
            return atan(my / mx) * 180 / PI + 360;
    }

    else if (mx < 0)
    {
        return atan(my / mx) * 180 / PI + 180;
    }
    else
    {
        if (my > 0)
            return 90;
        else
            return 270;
    }

}

NAMESPACE_DR_END

