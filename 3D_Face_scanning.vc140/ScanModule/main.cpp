#include<iostream>
#include<string>
#include"Scan.h"


#include"../DepthModule/device.h"
int main()
{
	std::string serial= realsense::getFirstSerial();
	realsense::Device *dev=new realsense::Device(serial);

	dev->selectSensorAndStreamProps();
	rs2::frame fra= dev->capture(realsense::RS400_STREAM_DEPTH);

	
	Scan *sc = new Scan();
	sc->frame2Points(fra);//frame�� ���������� updata�ǰ� �ϴ°͵� ������ ������
	sc->MeshConstruction(0, 0, 4);



	return 0;
}