#pragma once

#include "stdafx.h"
using namespace std;
using namespace rs;
using namespace pcl;
using namespace io;
//TODO:
// index 0�� �Ѿ� ����� ó�� 
class Pos {
public:
	int Row;
	int Col;
	int Idx;

	void toString() {
		std::cout<<"(" << Row << ", " << Col << ")";
	};
}typedef Pos;
class EndPoint {
public:
	PointCloud<PointXYZ> *cloud;
	int width, height;
	Pos Img_CenterPoint, WE_MidPoint;
	Pos  W, E, S, N;
	const rs::float3 *points;
	device * dev;

	EndPoint() {
		initPoint();
	}
	void initPoint() {
		W.Row = -1; 	W.Col = -1; 	W.Idx = -1;
		E.Row = -1;		E.Col = -1;		E.Idx = -1;
		N.Row = -1;		N.Col = -1;		N.Idx = -1;
		S.Row = -1;		S.Col = -1;		S.Idx = -1;
		Img_CenterPoint.Row = -1;	Img_CenterPoint.Col = -1;	Img_CenterPoint.Idx = -1;
		WE_MidPoint.Row = -1;		WE_MidPoint.Col = -1;		WE_MidPoint.Idx = -1;
	}
	void setOriginPoints(device * a_dev) {
		dev = a_dev;
	}
	void setCloud(PointCloud<PointXYZ> *a_cloud) {
		cloud = a_cloud;
		width = cloud->width;
		height = cloud->height;
		setPosition('C', cloud->width*cloud->height / 2 + cloud->width / 2);

	}
	rs::intrinsics getDepthImage() {
		dev->wait_for_frames();
		rs::intrinsics depth_intrin = dev->get_stream_intrinsics(rs::stream::depth);
		auto Points = reinterpret_cast<const rs::float3 *>(dev->get_frame_data(rs::stream::points));
		points = Points;
		return depth_intrin;
	}
	void setResolution(int a_width, int a_height) {
		width = a_width;
		height = a_height;
	}
	void setPosition(char direction, int a_pos) {
		switch (direction) {
		case 'W':
		case 'w':
			W.Idx = a_pos;
			W.Row = (W.Idx +1) / width;
			W.Col = W.Idx % width;
			std::cout << " End Point W is set" << std::endl;
			break;

		case 'E':
		case 'e':
			E.Idx = a_pos;
			E.Row = (E.Idx + 1) / width;
			E.Col = E.Idx % width;
			std::cout << " End Point E is set" << std::endl;
			break;

		case 'S':
		case 's':
			S.Idx = a_pos;
			S.Row = (S.Idx + 1) / width;
			S.Col = S.Idx % width;
			std::cout << " End Point S is set" << std::endl;
			break;

		case 'N':
		case 'n':
			N.Idx = a_pos;
			N.Row = (N.Idx + 1) / width;
			N.Col = N.Idx % width;
			std::cout << " End Point N is set" << std::endl;
			break;
		case 'M':
		case 'm':
			WE_MidPoint.Row = (W.Row + E.Row) / 2;
			WE_MidPoint.Col = (W.Col + E.Col) / 2;
			WE_MidPoint.Idx = (W.Idx + E.Idx) / 2;
			std::cout << " Mid Point is set" << std::endl;
			break;
		case 'C':
		case 'c':
			Img_CenterPoint.Col = cloud->width / 2;
			Img_CenterPoint.Row = cloud->height / 2;
			Img_CenterPoint.Idx = cloud->width*cloud->height / 2 + cloud->width / 2;

			std::cout << " Center Point is set" << std::endl;
			break;
		default:
			ErrorPrint(1);
			std::cout << "[ Error ] : Undefined character " << std::endl;
			break;
		}
	}
	bool isRightEndPointPosition();
	void ErrorPrint(int number) {
		std::cout << "[ Error ] : ";
		switch (number) {
		case 1:
			std::cout << "Undefined character Check 'EndPoint::setPosition()' " << std::endl;
			break;

		}
	}
};

bool EndPoint::isRightEndPointPosition() {
	//printf("%d %d\n", cloud->points.size(), Img_CenterPoint);
	int idx;
	int flag;
	for (int i = 0; i < cloud->width / 2; i++) {	//West
		idx = Img_CenterPoint.Idx - i;
		flag = 0;
		if (points[idx].x == 0.0) {
			flag = 1;
			for (int c = 0; c < 6; c++) {
				if (points[idx - c].x != 0.0) {
					flag = 0;
				}
			}
			if (flag == 1) {
				setPosition('W', idx);
				break;
			}
		}
	}
	for (int i = 0; i < cloud->width / 2; i++) { //east
		idx = Img_CenterPoint.Idx + i;
		flag = 0;
		if (points[idx].x == 0.0) {
			flag = 1;
			for (int c = 0; c < 6; c++) {
				if (points[idx + c].x != 0.0) {
					flag = 0;
				}
			}
			if (flag == 1) {
				setPosition('E', idx);
				break;
			}
		}
	}
	setPosition('M', NULL);
	for (int i = 0; i < cloud->height / 2; i++) { //north
		idx = WE_MidPoint.Idx - (i * cloud->width);
		flag = 0;
		if (points[idx].x == 0.0) {
			flag = 1;
			for (int c = 0; c < 4; c++) {
				if (points[idx - (c * cloud->width)].x != 0.0) {
					flag = 0;
				}
			}
			if (flag == 1) {
				setPosition('N', idx);
				break;
			}
		}
	}
	for (int i = 0; i < cloud->height / 2; i++) { //south
		idx = WE_MidPoint.Idx + (i * cloud->width);
		flag = 0; 
		if (points[idx].x == 0.0) {
			flag = 1;
			for (int c = 0; c < 4; c++) {
				if (points[idx + (c * cloud->width)].x != 0.0) {
					flag = 0;
				}
			}
			if (flag == 1) {
				setPosition('S', idx);
				break;
			}
		}
	}

	std::cout << " W : "; W.toString();
	std::cout << " E : "; E.toString();
	std::cout << " N : "; N.toString();
	std::cout << " S : "; S.toString();
	std::cout << "\n";

	std::cout << " E-W : " << E.Col - W.Col << " S-N : " << (S.Row - N.Row) << std::endl;

	if (W.Idx!= -1 && E.Idx != -1 && S.Idx != -1 && N.Idx != -1 && S.Idx != N.Idx) { // �ʱⰪ�� ���ϰ� ������� �ϴ����� ������ ��� 
		double ratio = ((double)S.Row - N.Row) / ((double)E.Col - W.Col);
		std::cout <<"Width / Height ratio : "<<ratio<<std::endl;
		if (2.0 > ratio && ratio > 1.2) {
			return false;
		}
	}
	return true;
};