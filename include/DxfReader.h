#ifndef __DXFREADER_H
#define __DXFREADER_H

#include <string>
#include "DRDefine.h"
#include "DRObject.h"
#include "dl_creationadapter.h"

NAMESPACE_DR_BEGIN

class DxfReader : public DL_CreationAdapter
{
public:
	DxfReader(const std::string & dxfile);
    
	virtual ~DxfReader();

    int ParseDxfFile();

    int GetObjectList(std::vector<DxfObject *> & objList);

public:	
    static void LineToArc(const DxfLine & line, DxfArc & arc);

    static double GetArcAngle(const DxfPoint & center,
                                const DxfPoint & p);

    static double GetEllipseRoAngle(double mx, double my);

    static void OCStoWCS(const DxfExtrusion & extru,
                            DxfPoint & p );



private:
    virtual void addPoint(const DL_PointData & );

    virtual void addLine(const DL_LineData & );

    virtual void addArc(const DL_ArcData & );

    virtual void addCircle(const DL_CircleData & );

    virtual void addPolyline(const DL_PolylineData & );

    virtual void addVertex(const DL_VertexData & );

    virtual void addEllipse(const DL_EllipseData & );

    virtual void addDimAlign(const DL_DimensionData & ,
                             const DL_DimAlignedData & );
    virtual void addDimLinear(const DL_DimensionData & ,
                              const DL_DimLinearData & );
    virtual void addDimRadial(const DL_DimensionData & ,
                              const DL_DimRadialData & );
    virtual void addDimDiametric(const DL_DimensionData & ,
                                 const DL_DimDiametricData & );
    virtual void addDimAngular(const DL_DimensionData & ,
                               const DL_DimAngularData & );
    virtual void addDimAngular3P(const DL_DimensionData & ,
                                 const DL_DimAngular3PData & );
    virtual void addDimOrdinate(const DL_DimensionData & ,
                                const DL_DimOrdinateData & );

    virtual void addText(const DL_TextData & );

    virtual void addMText(const DL_MTextData & );

    virtual void addBlock(const DL_BlockData & data);

    virtual void endBlock();

private:
    std::string m_dxfile;

    bool m_flagBlock;//filter the entities in Block

    int m_pointCnt;//the points' size for every PolyLine

    int m_flagPolyLine;// PolyLine flag 1:closed  0:default

    std::vector<DxfPoint> m_pointList; //save the points for every PolyLine

    std::vector<DxfObject *> m_objList;

};

NAMESPACE_DR_END

#endif

