#include "MainModule.hpp"


MainModule::MainModule(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	//��ư �ʱ�ȭ
	connect(ui.runCalibrationModule, &QPushButton::clicked, [this] {runCalibrationModule(); });
	connect(ui.runScanModule, &QPushButton::clicked, [this] {runScanModule(); });
	connect(ui.runViewerModule, &QPushButton::clicked, [this] {runViewerModule(); });
	connect(ui.runExpModule, &QPushButton::clicked, [this] {runExpModule(); }); 
}

void MainModule::runCalibrationModule() {

}

void MainModule::runScanModule() {

}

void MainModule::runViewerModule() {

}

void MainModule::runExpModule() {

}