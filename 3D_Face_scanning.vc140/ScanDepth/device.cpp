#include "device.h"

void Realsense::init() {
	rs2::context ctx;
	this->deviceList = ctx.query_devices();
	if (this->deviceList.size() == 0) {
		//throw runtime_error("감지된 장치가 없습니다. 장치연결을 확인해주세요.");
		cout << "감지된 장치가 없습니다. 장치연결을 확인해주세요." << endl;
		exit(0);
	}
	this->deviceNum = this->deviceList.size();
	cout << "장치연결을 확인하였습니다. 현재 장치의 갯수는 " << this->deviceNum << "개 입니다." << endl;
}

void Realsense::info(int devNum) {
	this->isInit();
	if ((devNum <0) || (devNum>this->deviceNum-1)) {
		string message = "연결된 장치의 Index값을 넘어섰습니다. 현재 장치의 갯수는 ";
		message += to_string(this->deviceNum); message += "개 입니다. 입력된 index 값 : ";
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
	//스트리밍 확인 

	

}

void Realsense::startStreaming(int devNum, int streamType) {
	this->isInit();
	throw notImplemented_error();
}

bool Realsense::isInit() {
	if (this->deviceNum == NULL) {
		//throw runtime_error("장치 초기화 프로세스 : init()이 이루어지지 않았습니다. 이를 먼저 수행해 주세요.");
		cout << "장치 초기화 프로세스 : init()이 이루어지지 않았습니다. 이를 먼저 수행해 주세요." << endl;
		return false;
	}
	return true;
}

