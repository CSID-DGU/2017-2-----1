#include "InitializeModule.hpp"
#include <qprogressbar.h>

InitializeModule::InitializeModule(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

}
void InitializeModule::Init() {

	ui.progressBar->setRange(0, 100);
	ui.progressBar->setValue(0);
	ui.progressBar->show();
	for (int i = 1; i <= 100; i++) {
		_sleep(20); //�ʱ�ȭ�� 3�ʸ� �������.
		ui.progressBar->setValue(i);
	}
}