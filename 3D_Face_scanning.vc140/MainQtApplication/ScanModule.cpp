#include "ScanModule.hpp"
ScanModule::ScanModule(QWidget *parent) : QWidget(parent){
	ui.setupUi(this);
	
	connect(ui.NextButton, SIGNAL(clicked()), this, SLOT(on_NextButton_clicked()));
	connect(ui.ScanButton, SIGNAL(clicked()), this, SLOT(on_ScanButton_clicked()));
}
void ScanModule::on_NextButton_clicked() {
	//data������ ALign ó�� 


	//���� ������
	reg->setData(data);
	this->close();
	reg->show();
}
void ScanModule::on_ScanButton_clicked() {
	//data������ ALign ó�� 
	ui.ScanButton->setEnabled(false);
	ui.NextButton->setEnabled(false);

	//���μ��� ó�� 
	QEventLoop loop;
	QTimer::singleShot(1500, &loop, SLOT(quit()));
	loop.exec();
	qApp->processEvents();

	ui.ScanButton->setEnabled(true);
	ui.NextButton->setEnabled(true);

}