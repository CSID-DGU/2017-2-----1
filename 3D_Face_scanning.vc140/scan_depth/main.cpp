#include "stdafx.h"
#include "EndPoint.h"
#include"DrawWindow.h"
//#include"Neural.h"
#include"Device.h"

using namespace std;
using namespace rs;
using namespace pcl;
using namespace io;
int main() try {
	Device realSense;
	PointCloud<PointXYZ> cloud;
	DrawWindows win(1280, 960, "Your Face", realSense.ctx.get_device_count() * 2);
	win.init(realSense.dev, realSense.devices);

	cout << "���󼾽� Ȱ��ȭ ��..." << endl;

	/*
	TODO
	1. ��/��/����� ���� cloud 3�� �����ϱ� 
	2. �� depth�̹����� �Կ��ϴ� ��� �߰��ϱ�.
	*/

	FrontEndPoint FrontEndPoints;
	RightEndPoint RightEndPoints;
	LeftEndPoint LeftEndPoints;
	FrontEndPoints.setOriginPoints(realSense.dev);
	RightEndPoints.setOriginPoints(realSense.dev);
	LeftEndPoints.setOriginPoints(realSense.dev);


	do {
		///depth �̹��� �Կ�
		FrontEndPoints.initPoint();
		rs::intrinsics depth_intrin = FrontEndPoints.getDepthImage();
		cloud.width = depth_intrin.width;
		cloud.height = depth_intrin.height;
		cloud.is_dense = false;
		cloud.points.resize(cloud.width * cloud.height);
		FrontEndPoints.setCloud(&cloud);
		//cout << "PCL �� ��ȯ��..." << endl;
		std::cout << "\n\n";

	} while (FrontEndPoints.isRightEndPointPosition() == true);


	for (int row = 0; row < FrontEndPoints.S.Idx - FrontEndPoints.N.Idx; row += cloud.width)
	{
		for (int i = FrontEndPoints.N.Idx - (FrontEndPoints.E.Idx - FrontEndPoints.W.Idx) / 2 + row; i < FrontEndPoints.N.Idx + (FrontEndPoints.E.Idx - FrontEndPoints.W.Idx) / 2 + row; i++) {
			int idx = i;
			cloud.points[idx].x = - FrontEndPoints.points[idx].x;
			cloud.points[idx].y = - FrontEndPoints.points[idx].y;
			cloud.points[idx].z = - FrontEndPoints.points[idx].z;
		}
	}
	
	cout << "Resizing..." << endl;
	PointCloud<PointXYZ> r_cloud;
	r_cloud.width = abs(FrontEndPoints.W.Col - FrontEndPoints.E.Col);
	r_cloud.height = abs(FrontEndPoints.S.Row - FrontEndPoints.N.Row);
	r_cloud.is_dense = false;
	r_cloud.points.resize(r_cloud.width * r_cloud.height);

	int newindex = 0;
	for (int i = FrontEndPoints.N.Row; i < FrontEndPoints.S.Row; i ++)
	{
		int idx = FrontEndPoints.W.Col + i*cloud.width;
		for (int j = 0; j < r_cloud.width; j++) {
			r_cloud.points[newindex].x = -FrontEndPoints.points[idx+j].x;
			r_cloud.points[newindex].y = -FrontEndPoints.points[idx+j].y;
			r_cloud.points[newindex].z = -FrontEndPoints.points[idx+j].z;
			++newindex;
		}
	}
	cout << "Done." << endl;
	
	
	//RightEndPoint,LeftEndPoint�� Front�� ���� �ڵ�� �����ϸ� �� 
	string result_name;
	cout << "��� ���� �̸�(pcd) : "; cin >> result_name;
	savePCDFileASCII(result_name + ".pcd", r_cloud);
	cout << "Done." << endl;

	pcl::visualization::CloudViewer viewer("PCL Viewer");
	viewer.showCloud(r_cloud.makeShared());
	while (!viewer.wasStopped());

	return EXIT_SUCCESS;
}
catch (const rs::error & e)
{
	// Method calls against librealsense objects may throw exceptions of type rs::error
	printf("rs::error was thrown when calling %s(%s):\n", e.get_failed_function().c_str(), e.get_failed_args().c_str());
	printf("    %s\n", e.what());
	return EXIT_FAILURE;
}
