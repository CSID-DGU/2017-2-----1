#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "stdafx.h"

using namespace std;
using namespace rs;
using namespace pcl;
using namespace io;
/*
TODO
cloud �� arr ��ü�ϱ� 
*/
class vtkWriter {
public:
	std::string filename;
	PointCloud<PointXYZ> *Cloud;
	int subWidth;
	int subHeight;
	int subPOINTS;
	vtkWriter(std::string file, PointCloud<PointXYZ>* cloud) {
		filename = file;
		Cloud = cloud;
		subWidth = Cloud->width;
		subHeight = Cloud->height;
		subPOINTS = subWidth*subHeight;
		saveVTK();
	}
	void saveVTK() {
		int ct = 0;
		std::ofstream fout(filename);
		fout << "# vtk DataFile Version 3.0\nvtk output\nASCII\nDATASET POLYDATA\nPOINTS " << subPOINTS << " float" << std::endl;
		for (int i = 0; i < subPOINTS; i++) {
			fout << Cloud->points[i].x << " " << Cloud->points[i].y << " " << Cloud->points[i].z << std::endl;
			if ((i + subWidth < subPOINTS) && ((i + 1) % subWidth != 0)) {
				if ((Cloud->points[i].x != 0) && (Cloud->points[i + subWidth].x != 0) && (Cloud->points[i + 1].x != 0))
					ct += 1;
			}
			if (((i + subWidth + 1) < subPOINTS) && ((i + 1) % subWidth != 0)) {
				if ((Cloud->points[i + 1].x != 0) && (Cloud->points[i + subWidth].x != 0) && (Cloud->points[i + subWidth + 1].x != 0))
					ct += 1;
			}
		}
		fout << "\nPOLYGONS " << ct << " " << ct * 4 << std::endl;
		for (int i = 0; i < subPOINTS; i++) {
			if ((i + subWidth < subPOINTS) && ((i + 1) % subWidth != 0)) {
				if ((Cloud->points[i].x != 0) && (Cloud->points[i + subWidth].x != 0) && (Cloud->points[i + 1].x != 0))
					fout << "3 " << i << " " << i + subWidth << " " << i + 1 << std::endl;
			}
			if (((i + subWidth + 1) < subPOINTS) && ((i + 1) % subWidth != 0)) {
				if ((Cloud->points[i + 1].x != 0) && (Cloud->points[i + subWidth].x != 0) && (Cloud->points[i + subWidth + 1].x != 0))
					fout << "3 " << i + subWidth << " " << i + 1 << " " << i + subWidth + 1 << std::endl;
			}
		}
		fout.close();
	}
};