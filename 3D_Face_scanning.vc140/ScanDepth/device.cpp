#include "device.h"

void Realsense::init() {
	rs2::context ctx;
	this->deviceList = ctx.query_devices();
	if (this->deviceList.size() == 0) {
		//throw runtime_error("������ ��ġ�� �����ϴ�. ��ġ������ Ȯ�����ּ���.");
		cout << "������ ��ġ�� �����ϴ�. ��ġ������ Ȯ�����ּ���." << endl;
		exit(0);
	}
	this->deviceNum = this->deviceList.size();
	cout << "��ġ������ Ȯ���Ͽ����ϴ�. ���� ��ġ�� ������ " << this->deviceNum << "�� �Դϴ�." << endl;
}

void Realsense::info(int devNum) {
	this->isInit();
	if ((devNum < MIN_CAM_NUM-1) || (devNum>this->deviceNum-1)) {
		string message = "����� ��ġ�� Index���� �Ѿ���ϴ�. ���� ��ġ�� ������ ";
		message += to_string(this->deviceNum); message += "�� �Դϴ�. �Էµ� index �� : ";
		message += to_string(devNum);
		//throw runtime_error(message);
		cout << message << endl;
		exit(0);
	}

	auto dev = this->deviceList[devNum];
	cout << " Device info: \n";
	for (auto j = 0; j < RS2_CAMERA_INFO_COUNT; ++j)
	{
		auto param = static_cast<rs2_camera_info>(j);
		if (dev.supports(param))
			cout << "    " << left << setw(30) << rs2_camera_info_to_string(rs2_camera_info(param))
			<< ": \t" << dev.get_info(param) << endl;
	}
	//��Ʈ���� Ȯ�� 

	

}

void Realsense::startStreaming(int devNum, int streamType) {
	this->isInit();
	auto dev = this->deviceList[devNum];
	string serial_number = dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);

	//config�� ���� �ش� ī�޶� ������.
	//https://github.com/IntelRealSense/librealsense/blob/master/include/librealsense2/hpp/rs_pipeline.hpp#L118 ����
	rs2::config c;
	c.enable_device(serial_number);

	//stream �� ���� : RS2_STREAM_ANY, RS2_STREAM_DEPTH, RS2_STREAM_COLOR , RS2_STREAM_GPIO, RS2_STREAM_COUNT ��
	//https://github.com/IntelRealSense/librealsense/blob/c8ee8fa1912b9297df13bfe097d527667fe0afba/include/librealsense2/h/rs_sensor.h#L37
	if (streamType == color)
		c.enable_stream(RS2_STREAM_COLOR);
	else if (streamType == depth)
		c.enable_stream(RS2_STREAM_DEPTH);
	
	
	rs2::pipeline pipe;
	rs2::pipeline_profile profile = pipe.start(c);
	this->pipe_map.emplace(devNum, unit{ {}, pipe, profile });
	
	//throw notImplemented_error();
}

void Realsense::capture(int devNum, int streamType) {
	unit cam_unit = this->pipe_map[0];
	rs2::frameset data = cam_unit.pipe.wait_for_frames();
	throw notImplemented_error();
	printf("a");
}

void Realsense::stopStreaming(int devNum, int streamType) {
}

bool Realsense::isInit() {
	if (this->deviceNum == NULL) {
		//throw runtime_error("��ġ �ʱ�ȭ ���μ��� : init()�� �̷������ �ʾҽ��ϴ�. �̸� ���� ������ �ּ���.");
		cout << "��ġ �ʱ�ȭ ���μ��� : init()�� �̷������ �ʾҽ��ϴ�. �̸� ���� ������ �ּ���." << endl;
		return false;
	}
	return true;
}

