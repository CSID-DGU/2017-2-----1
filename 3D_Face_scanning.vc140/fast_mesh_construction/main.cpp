#include "stdafx.h"
#include "vtkWriter.h"
#include "Device.h"

using namespace std;
using namespace rs;
using namespace pcl;
using namespace io;

int main() try {

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PCLPointCloud2 cloud_blob;
	cout << "PCD ���ϰ�θ� �Է����ּ���.." << endl;
	string filePath; getline(std::cin, filePath);
	cout << "������ �ε��մϴ�.." << endl;
	pcl::io::loadPCDFile(filePath, cloud_blob);
	cout << "���Ϸε� ��!" << endl;
	pcl::fromPCLPointCloud2(cloud_blob, *cloud);

	PointCloud<PointXYZ> cloud2 = *cloud;
	cloud2.width = cloud->width;
	cloud2.height = cloud->height;
	cloud2.is_dense = false;
	cloud2.points.resize(cloud2.width*cloud2.height);
	cout << cloud2.width << endl;
	cout << cloud2.height << endl;
	
	pcl::visualization::CloudViewer viewer("PCL Viewer");
	viewer.showCloud(cloud2.makeShared());
	while (!viewer.wasStopped());
	
	std::string result_name;
	cout << "vtk �����̸�(Ȯ���� ����) >>" << endl;
	std::cin >> result_name;

	vtkWriter fout(result_name+".vtk", &cloud2);
	cout << "File is saved." << endl;
	return EXIT_SUCCESS;
}
catch (const rs::error & e)
{
	// Method calls against librealsense objects may throw exceptions of type rs::error
	printf("rs::error was thrown when calling %s(%s):\n", e.get_failed_function().c_str(), e.get_failed_args().c_str());
	printf("    %s\n", e.what());
	return EXIT_FAILURE;
}