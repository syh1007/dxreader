#include "DRObject.h"
#include <iostream>
#include <sstream>

NAMESPACE_DR_BEGIN

const std::string DxfObject::SEPARATOR_MEM = ",";
const std::string DxfObject::SEPARATOR_OBJ = ";";

std::string DxfExtrusion::ToString()
{
    std::stringstream ss;

    ss << m_edx << DxfObject::SEPARATOR_MEM << m_edy
       << DxfObject::SEPARATOR_MEM <<  m_edz ;

    return ss.str();
}

std::string DxfPoint::ToSimpleString()
{
    std::stringstream ss;

    ss << m_x << SEPARATOR_MEM << m_y
        << SEPARATOR_MEM << m_bulge ;

    return ss.str();
}

std::string DxfPoint::ToString()
{
    std::stringstream ss;

    ss << "Point:" << m_x << SEPARATOR_MEM
       << m_y << SEPARATOR_MEM << m_bulge << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfPoint::Clone()
{
    DxfPoint * point = new DxfPoint(m_x, m_y, m_bulge);

    return point;
}

std::string DxfLine::ToString()
{
    std::stringstream ss;

    ss << "Line:" << m_beginP.ToSimpleString()
       << SEPARATOR_MEM
       << m_endP.ToSimpleString() << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfLine:: Clone()
{
    DxfLine * line = new DxfLine(m_beginP, m_endP);

    return line;
}

std::string DxfCircle::ToString()
{
    std::stringstream ss;

    ss << "Circle:" << m_centerP.ToSimpleString()
       << SEPARATOR_MEM << m_radius << SEPARATOR_MEM
       <<  m_extrusion.ToString() << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfCircle::Clone()
{
    DxfCircle * circle = new DxfCircle(m_centerP, m_radius,
                                                m_extrusion);

    return circle;
}

std::string DxfArc::ToString()
{
    std::stringstream ss;

    ss << "Arc:" << m_centerP.ToSimpleString() << SEPARATOR_MEM
       << m_radius << SEPARATOR_MEM << m_startAngle
       << SEPARATOR_MEM << m_endAngle << SEPARATOR_MEM
       << m_extrusion.ToString() << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfArc::Clone()
{
    DxfArc * arc = new DxfArc(m_centerP, m_radius,
                              m_startAngle, m_endAngle,
                              m_extrusion);

    return arc;
}

std::string DxfPolyLine::ToString()
{
    std::stringstream ss;

    ss << "PolyLine Size:" << m_pointList.size() << SEPARATOR_MEM
       << "Flag:"<< m_isClosed << SEPARATOR_MEM
       << "Extru:" << m_extrusion.ToString() << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfPolyLine::Clone()
{
    DxfPolyLine * polyline = new DxfPolyLine(m_pointList, m_extrusion,
                                                   m_isClosed);

    return polyline;
}

std::string DxfEllipse::ToString()
{
    std::stringstream ss;

    ss << "Ellipse:" << m_centerP.ToSimpleString() << SEPARATOR_MEM
       << m_majorP.ToSimpleString() << SEPARATOR_MEM << m_majorLen
       << SEPARATOR_MEM << m_minorLen << SEPARATOR_MEM
       << m_startAngle << SEPARATOR_MEM << m_endAngle << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfEllipse::Clone()
{
    DxfEllipse * ellipse = new DxfEllipse(m_centerP, m_majorP,
                                          m_majorLen, m_minorLen,
                                          m_startAngle, m_endAngle);

    return ellipse;
}

std::string DxfDimension::ToString()
{
    std::stringstream ss;

    ss << "Dimension:" << m_dimType << SEPARATOR_MEM
       << m_defineP.ToSimpleString() << SEPARATOR_MEM
       << m_textMidP.ToSimpleString() << SEPARATOR_MEM
       << m_textString << SEPARATOR_MEM << m_realValue << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfDimension::Clone()
{
    DxfDimension * dimension = new DxfDimension(m_dimType, m_defineP,
                                                m_textMidP, m_textString,
                                                m_realValue);

    return dimension;
}

std::string DxfDimLiner::ToString()
{
    std::stringstream ss;

    ss << "DimLiner:" << m_dimension.ToString() << SEPARATOR_MEM
       << m_defineP1.ToSimpleString() << SEPARATOR_MEM
       << m_defineP2.ToSimpleString() << SEPARATOR_MEM
       << m_angle << SEPARATOR_MEM << m_oblique << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfDimLiner::Clone()
{
    DxfDimLiner * dimension = new DxfDimLiner(m_defineP1, m_defineP2, m_angle,
                                              m_oblique, m_dimension);

    return dimension;
}

std::string DxfDimRadOrDia::ToString()
{
    std::stringstream ss;

    ss << "DimRadOrDia:" << m_dimension.ToString() << SEPARATOR_MEM
       << m_defineP.ToSimpleString() << SEPARATOR_MEM
       << m_leaderLen << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfDimRadOrDia::Clone()
{
    DxfDimRadOrDia * dimension = new DxfDimRadOrDia(m_defineP, m_leaderLen,
                                                    m_dimension);

    return dimension;
}


std::string DxfDimAngular::ToString()
{
    std::stringstream ss;

    ss << "DimLiner:" << m_dimension.ToString() << SEPARATOR_MEM
       << m_defineP1.ToSimpleString() << SEPARATOR_MEM
       << m_defineP2.ToSimpleString() << SEPARATOR_MEM
       << m_defineP3.ToSimpleString() << SEPARATOR_MEM
       << m_defineP4.ToSimpleString() << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfDimAngular::Clone()
{
    DxfDimAngular * dimension = new DxfDimAngular(m_defineP1, m_defineP2,
                                                  m_defineP3, m_defineP4,
                                                  m_dimension);

    return dimension;
}

std::string DxfDimOrdinate::ToString()
{
    std::stringstream ss;

    ss << "DimOrdinate:" << m_dimension.ToString() << SEPARATOR_MEM
       << m_defineP1.ToSimpleString() << SEPARATOR_MEM
       << m_defineP2.ToSimpleString() << SEPARATOR_MEM
       << m_dimType << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfDimOrdinate::Clone()
{
    DxfDimOrdinate * dimension = new DxfDimOrdinate(m_defineP1, m_defineP2,
                                                    m_dimType, m_dimension);


    return dimension;
}

std::string DxfText::ToString()
{
    std::stringstream ss;

    ss << "Text:" << m_insertP.ToSimpleString() << SEPARATOR_MEM
       << m_alignP.ToSimpleString() << SEPARATOR_MEM
       << m_height << SEPARATOR_MEM << m_xScale << SEPARATOR_MEM
       << m_hJustify << SEPARATOR_MEM << m_vJustify << SEPARATOR_MEM
       << m_text << SEPARATOR_MEM << m_rotatedAngle << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfText::Clone()
{
    DxfText * dimension = new DxfText(m_insertP, m_alignP,
                                      m_height, m_xScale,
                                      m_generaFlag, m_hJustify,
                                      m_vJustify, m_text, m_style,
                                      m_rotatedAngle);

    return dimension;
}

std::string DxfMText::ToString()
{
    std::stringstream ss;

    ss << "MText:" << m_insertP.ToSimpleString() << SEPARATOR_MEM
       << m_xDirectionP.ToSimpleString() << SEPARATOR_MEM
       << m_height << SEPARATOR_MEM << m_width << SEPARATOR_MEM
       << m_attachment << SEPARATOR_MEM << m_drawDirection << SEPARATOR_MEM
       << m_lineSpaceStyle << SEPARATOR_MEM << m_lineSpaceFactor << SEPARATOR_MEM
       << m_text << SEPARATOR_MEM << m_style << SEPARATOR_MEM
       << m_rotatedAngle << SEPARATOR_OBJ;

    return ss.str();
}

DxfObject * DxfMText::Clone()
{
    DxfMText * text = new DxfMText(m_insertP, m_xDirectionP,
                                   m_height, m_width,
                                   m_attachment, m_drawDirection,
                                   m_lineSpaceStyle, m_lineSpaceFactor,
                                   m_text, m_style,
                                   m_rotatedAngle);

    return text;
}

NAMESPACE_DR_END

