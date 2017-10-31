#include "stdafx.h"


using namespace std;
using namespace rs;
using namespace pcl;
using namespace io;

int main(){
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PCLPointCloud2 cloud_blob; 
	cout << "PCD ���ϰ�θ� �Է����ּ���.." << endl;
	string filePath; getline(std::cin, filePath);
	cout << "������ �ε��մϴ�.." << endl;
	pcl::io::loadPCDFile(filePath, cloud_blob);
	cout << "���Ϸε� ��!" << endl;
	pcl::fromPCLPointCloud2(cloud_blob, *cloud);

	pcl::visualization::CloudViewer viewer("PCL Viewer");
	viewer.showCloud(cloud->makeShared());
	while (!viewer.wasStopped());
	return (0);
}
