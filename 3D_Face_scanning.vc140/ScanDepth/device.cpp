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

void Realsense::stopStreaming(int devNum, int streamType) {

	this->isInit();
	auto dev = this->deviceList[devNum];
	string serial_number = dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);

	rs2::config c;
	c.enable_device(serial_number);

	if (streamType == color)
		c.disable_stream(RS2_STREAM_COLOR);
	else if (streamType == depth)
		c.disable_stream(RS2_STREAM_DEPTH);
}

rs2::frame Realsense::capture(int devNum, int streamType, int restNum) {
	unit cam_unit = this->pipe_map[devNum];
	this->restFrame(cam_unit,restNum);
	rs2::frameset data = cam_unit.pipe.wait_for_frames();
	rs2::frame frameData;
	if (streamType == color) {
		frameData = data.get_color_frame();
	}
	else if (streamType == depth) {
		//frameData = data.get_depth_frame();
		frameData = data.first(RS2_STREAM_DEPTH);
	}
	else return NULL;
	return frameData;
}

std::vector<rs2::frame> Realsense::capture(int devNum, int streamType, int frameNum, int restNum) {
	unit cam_unit = this->pipe_map[devNum];
	this->restFrame(cam_unit, restNum);
	rs2::frameset data = cam_unit.pipe.wait_for_frames();
	std::vector<rs2::frame> frameData;
	for (int i = 0; i < frameNum; i++) {
		if (streamType == color) {
			frameData.push_back(data.get_color_frame());
		}
		else if (streamType == depth){
			frameData.push_back(data.get_depth_frame());
		}
		else return frameData;
		data = cam_unit.pipe.wait_for_frames();
	}
	return frameData;
}

std::string Realsense::saveImage(rs2::frame &frame, string filepath, int filetype) {
	throw notImplemented_error();
	rs2::colorizer color_map;

	if (auto vf = frame.as<rs2::video_frame>())
	{
		auto stream = frame.get_profile().stream_type();
		// Use the colorizer to get an rgb image for the depth stream
		if (vf.is<rs2::depth_frame>()) vf = color_map(frame);

		// Write images to disk
		std::stringstream png_file;
		png_file << "rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
		//stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(), vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
		std::cout << "Saved " << png_file.str() << std::endl;
	}
	return "���� �̱��� �Դϴ�~";
}

vtkPoints* Realsense::frameToVtkPoints(rs2::frame &frame) {
	rs2::pointcloud pc;
	rs2::points rsPoints;
	rsPoints = pc.calculate(frame);
	vtkPoints * vtkPoints = vtkPoints::New();
	auto v = rsPoints.get_vertices();
	for (auto i = 0; i < rsPoints.size(); i++) {
		if (v[i].z != 0) {
			vtkPoints->InsertNextPoint(v[i].x, v[i].y, v[i].z);
			//cout << v[i].x << " " << v[i].y << " " << v[i].z << endl;
		}
	}
	return vtkPoints;
}

// private function
void Realsense::restFrame(unit &cam_unit, int num) {
	for (int i = 0; i < num; i++) {
		cam_unit.pipe.wait_for_frames();
	}
}

bool Realsense::isInit() {
	if (this->deviceNum == NULL) {
		//throw runtime_error("��ġ �ʱ�ȭ ���μ��� : init()�� �̷������ �ʾҽ��ϴ�. �̸� ���� ������ �ּ���.");
		cout << "��ġ �ʱ�ȭ ���μ��� : init()�� �̷������ �ʾҽ��ϴ�. �̸� ���� ������ �ּ���." << endl;
		return false;
	}
	return true;
}

