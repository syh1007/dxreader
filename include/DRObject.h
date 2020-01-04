#ifndef __DXFOBJECT_H
#define __DXFOBJECT_H

#include <string>
#include "DRDefine.h"
#include <vector>

NAMESPACE_DR_BEGIN

struct DxfObject
{
    enum __TYPE {

        POINT,
        LINE,
        CIRCLE,
        ARC,
        POLYLINE,
        ELLIPSE,
        DIMENSION,
        DIMLINEAR,
        DIMRADORDIA,
        DIMANGULAR,
        DIMORDINATE,
        TEXT,
        MTEXT
    };

    static const std::string SEPARATOR_MEM ;
    static const std::string SEPARATOR_OBJ ;

    DxfObject(__TYPE type)
        : m_type(type)
    {
    }

    virtual ~DxfObject()
    {
    }

    enum __TYPE GetType()
    {
        return m_type;
    }

    virtual DxfObject * Clone() = 0;

    virtual std::string ToString() = 0;

private:
    enum __TYPE m_type;
};

/***
* @param direction. Direction of axis along which the entity shall be extruded
                    this is also the Z axis of the Entity coordinate system
* @param elevation. Distance of the entities XY plane from the origin of the
                    world coordinate system
***/
struct DxfExtrusion
{
    //direction
    double m_edx;
    double m_edy;
    double m_edz;

    double m_elevation;

    DxfExtrusion(double edx = 0.0, double edy = 0.0, double edz = 1.0,
            double elevation = 0.0)
        : m_edx(edx), m_edy(edy), m_edz(edz), m_elevation(elevation)
    {
    }

    std::string ToString();
};



struct DxfPoint : public DxfObject
{
    //the point in WCS
    double m_x;
    double m_y;


    //the point with the bulge which is not 0 will get a arc!!!
    double m_bulge;

    DxfPoint(double x, double y, double bulge = 0)
        : DxfObject(POINT), m_x(x), m_y(y), m_bulge(bulge)
    {
    }

    DxfPoint(): DxfObject(POINT), m_x(0), m_y(0), m_bulge(0)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;

    //other object use function to simply the point String
    std::string ToSimpleString() ;


};

struct DxfLine : public DxfObject
{
    //the point in WCS
    DxfPoint m_beginP;
    DxfPoint m_endP;

    DxfLine(const DxfPoint & begin, const DxfPoint & end)
        : DxfObject(LINE), m_beginP(begin), m_endP(end)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};

struct DxfCircle : public DxfObject
{
    //the point in OCS
    DxfPoint m_centerP;
    double m_radius;

    DxfExtrusion m_extrusion;

    DxfCircle(const DxfPoint & center, double r,
            const DxfExtrusion & extru)
        : DxfObject(CIRCLE), m_centerP(center), m_radius(r),
          m_extrusion(extru)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};


struct DxfArc : public DxfObject
{
    //the point in OCS
    DxfPoint m_centerP;
    double m_radius;
    double m_startAngle, m_endAngle;

    DxfExtrusion m_extrusion;

    DxfArc(const DxfPoint & p, double r, double sa, double ea,
              const DxfExtrusion & extru)
        : DxfObject(ARC), m_centerP(p), m_radius(r), m_startAngle(sa),
         m_endAngle(ea), m_extrusion(extru)
    {
    }

    DxfArc() :DxfObject(ARC), m_radius(0), m_startAngle(0), m_endAngle(0)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};

/***
The 2D PolyLine become LWPolyLine in R14 and above.
***/

struct DxfPolyLine : public DxfObject
{
    //the point in OCS
    std::vector<DxfPoint> m_pointList;

    //The flag of LWPolyLine and PolyLine, 1 = closed, default = 0
    int m_isClosed;

    DxfExtrusion m_extrusion;

    DxfPolyLine(const std::vector<DxfPoint> & plist,
               const DxfExtrusion & extru, int closed = 0 )
        : DxfObject(POLYLINE), m_pointList(plist),
          m_extrusion(extru),  m_isClosed(closed)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};

struct DxfEllipse : public DxfObject
{

    //the point in WCS
    DxfPoint m_centerP;

    //endpoint of major axis, relative to the center
    DxfPoint m_majorP;

    double m_majorLen, m_minorLen;
    double m_startAngle,  m_endAngle;


    DxfEllipse(const DxfPoint & p,  const DxfPoint & mp, double mal,
               double mil, double sa, double ea)
        : DxfObject(ELLIPSE), m_centerP(p), m_majorP(mp), m_majorLen(mal),
          m_minorLen(mil), m_startAngle(sa), m_endAngle(ea)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};

//Common Dimension
struct DxfDimension : public DxfObject
{
    int m_dimType;
    /**
     * Dimension type.
     * 0   Rotated, horizontal, or vertical
     * 1   Aligned
     * 2   Angular
     * 3   Diametric
     * 4   Radius
     * 5   Angular 3-point
     * 6   Ordinate
     * 64  Ordinate type. This is a bit value (bit 7)
     *     used only with integer value 6. If set,
     *     ordinate is X-type; if not set, ordinate is
     *     Y-type
     * 128 This is a bit value (bit 8) added to the
     *     other group 70 values if the dimension text
     *     has been positioned at a user-defined
     *     location rather than at the default location
     **/

    DxfPoint m_defineP, m_textMidP;
    std::string m_textString;
    double m_realValue;

    DxfDimension(int type, const DxfPoint & dp, const DxfPoint & mp,
                    const std::string & text, double realvalue)
        : DxfObject(DIMENSION), m_dimType(type), m_defineP(dp), m_textMidP(mp),
          m_textString(text), m_realValue(realvalue)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};

//Linear and Rotated Dimension
struct DxfDimLiner : public DxfObject
{
    DxfPoint m_defineP1, m_defineP2;

    double m_angle, m_oblique;

    DxfDimension m_dimension;

    DxfDimLiner(const DxfPoint & p1, const DxfPoint & p2, double ang,
                double obl, const DxfDimension & dim)
        : DxfObject(DIMLINEAR), m_defineP1(p1), m_defineP2(p2), m_angle(ang), m_oblique(obl),
          m_dimension(dim)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString() ;
};

//Radial and Diameter Dimension
struct DxfDimRadOrDia : public DxfObject
{
    DxfPoint m_defineP;
    double m_leaderLen;
    DxfDimension m_dimension;

    DxfDimRadOrDia(const DxfPoint & p, double length,
                   const DxfDimension & dim)
        : DxfObject(DIMRADORDIA), m_defineP(p), m_leaderLen(length),
          m_dimension(dim)
    {

    }

    virtual DxfObject * Clone();

    virtual std::string ToString();
};


//Angular Dimension
struct DxfDimAngular : public DxfObject
{
    DxfPoint m_defineP1, m_defineP2, m_defineP3, m_defineP4;
    DxfDimension m_dimension;

    DxfDimAngular(const DxfPoint & p1, const DxfPoint & p2,
                  const DxfPoint & p3, const DxfPoint & p4,
                  const DxfDimension & dim)
        : DxfObject(DIMANGULAR), m_defineP1(p1), m_defineP2(p2), m_defineP3(p3),
        m_defineP4(p4), m_dimension(dim)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString();
};

//Ordinate Dimension
struct DxfDimOrdinate : public DxfObject
{
    DxfPoint m_defineP1, m_defineP2;
    DxfDimension m_dimension;

    //true: X-type, false: Y-type
    bool m_dimType;

    DxfDimOrdinate(const DxfPoint & p1, const DxfPoint & p2,
                   bool type, const DxfDimension & dim)
        : DxfObject(DIMORDINATE), m_defineP1(p1), m_defineP2(p2), m_dimType(type),
          m_dimension(dim)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString();
};

struct DxfText : public DxfObject
{
    DxfPoint m_insertP, m_alignP;
    double m_height;
    double m_xScale, m_rotatedAngle;

    /**
     @Horizontal justification.
     * 0 = Left (default), 1 = Center, 2 = Right,
     * 3 = Aligned, 4 = Middle, 5 = Fit
     * For 3, 4, 5 the vertical alignment has to be 0.
     @Vertical justification.
     * 0 = Baseline (default), 1 = Bottom, 2 = Middle, 3= Top
    **/
    int m_hJustify, m_vJustify;

    /* 0 = default, 2 = Backwards, 4 = Upside down */
    int m_generaFlag;

    std::string m_text, m_style;

    DxfText(const DxfPoint & p1, const DxfPoint & p2, double height,
            double scale, int flag, int H, int V,
            const std::string & text, const std::string & style, double ang)
        : DxfObject(TEXT), m_insertP(p1), m_alignP(p2), m_height(height),
          m_xScale(scale), m_generaFlag(flag),
          m_hJustify(H), m_vJustify(V), m_text(text), m_style(style), m_rotatedAngle(ang)
    {
    }

    virtual DxfObject * Clone();

    virtual std::string ToString();
};

struct DxfMText : public DxfObject
{
    DxfPoint m_insertP, m_xDirectionP;

    double m_height, m_width, m_rotatedAngle;
    int m_lineSpaceStyle, m_lineSpaceFactor;

        /**
         *Attachment point.
         *1 = Top left, 2 = Top center, 3 = Top right,
         *4 = Middle left, 5 = Middle center, 6 = Middle right,
         *7 = Bottom left, 8 = Bottom center, 9 = Bottom right
        **/
    int m_attachment;

    // draw direction. 1:left to right 3:top to bottom 5:by style
    int m_drawDirection;

    std::string m_text, m_style;

    DxfMText(const DxfPoint & p1, const DxfPoint & p2, double height,
             double width, int attach, int drawDirect,
             int linespaceStyle, int linespaceFactor,
             const std::string & text, const std::string & style, double ang)
        : DxfObject(MTEXT), m_insertP(p1), m_xDirectionP(p2), m_height(height),
        m_width(width), m_attachment(attach), m_drawDirection(drawDirect),
        m_lineSpaceStyle(linespaceStyle), m_lineSpaceFactor(linespaceFactor),
        m_text(text), m_style(style), m_rotatedAngle(ang)
    {

    }

    virtual DxfObject * Clone();

    virtual std::string ToString();
};

NAMESPACE_DR_END

#endif

