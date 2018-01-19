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
	if ((devNum <0) || (devNum>this->deviceNum-1)) {
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
	throw notImplemented_error();
}

bool Realsense::isInit() {
	if (this->deviceNum == NULL) {
		//throw runtime_error("��ġ �ʱ�ȭ ���μ��� : init()�� �̷������ �ʾҽ��ϴ�. �̸� ���� ������ �ּ���.");
		cout << "��ġ �ʱ�ȭ ���μ��� : init()�� �̷������ �ʾҽ��ϴ�. �̸� ���� ������ �ּ���." << endl;
		return false;
	}
	return true;
}

