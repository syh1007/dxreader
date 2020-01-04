#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "DRObject.h"
#include "DxfReader.h"
#include <vector>
#include <string>

using namespace std;
using namespace dxfreader;


int main(int argc, char ** argv) 
{

    // Check given arguments:
    if (argc < 2) 
	{		
        std::cout << "\nUsage: test <DXF file>\n\n";
	}

	vector<DxfObject* > objList;
	int count;
	DxfReader * dr = new DxfReader(argv[1]);
    
	dr->ParseDxfFile();
	dr->GetObjectList(objList);
	
	count = objList.size();
	for (int i = 0; i < count ; i++)
	{
		switch (objList[i]->GetType())
		{
			case DxfObject::LINE:
				{
					DxfLine * line = dynamic_cast<DxfLine*> (objList[i]);

					if (!line)
						cout << "dynamic_cast is wrong!" <<endl;
					else
						cout << line->ToString() << endl;
				}
				break;
			case DxfObject::CIRCLE:
				{
					DxfCircle * circle = dynamic_cast<DxfCircle*> (objList[i]);
				
					if (!circle)
						cout << "dynamic_cast is wrong!" << endl;
					else
						cout << circle->ToString() << endl;
				}
				break;
			case DxfObject::ARC:
				{
					DxfArc * arc = dynamic_cast<DxfArc*> (objList[i]);
					
					if (!arc)
						cout << "dynamic_cast is wrong!" << endl;
					else
						 cout << arc->ToString() << endl;
				}
				break;
			case DxfObject::POLYLINE:
				{
					DxfPolyLine * polyLine = dynamic_cast<DxfPolyLine*> (objList[i]);

					if (!polyLine)
						cout << "dynamic_cast is wrong!" << endl;
					else
						cout << polyLine->ToString() << endl;
				}
				break;
			case DxfObject::POINT:
				{
					DxfPoint * point = dynamic_cast<DxfPoint*> (objList[i]);
					
					if (!point)
						cout << "dynamic_cast is wrong!" << endl;
					else
						cout << point->ToString() << endl;
				}
				break;
			default:
				break;
		}
	}
	return 0;
}


