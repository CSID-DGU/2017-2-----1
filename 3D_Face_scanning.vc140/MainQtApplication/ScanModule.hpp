#pragma once
#include "ui_scanmodule.h"
#include "Data.cpp"
#include "RegistrationModule.hpp"
#include <qtimer>
/*
- ���ϳ��� ��ü�� ���鵵�� �ϱ�(���� Ŭ�� ����)
- Scan ��� ��� ������ Scan�ɶ� �����ڿ� ���� ���� �Ѿ�� onNext�� ����

*/
class ScanModule : public QWidget{
	Q_OBJECT

public:
	ScanModule(QWidget *parent = Q_NULLPTR);
	void setData(Data *data) {
		this->data = data;
	}
	void setRegistrationModule(RegistrationModule *reg) {
		this->reg = reg;
	}
private slots:
	void on_NextButton_clicked();
	void on_ScanButton_clicked();
private:
	Ui::ScanModule ui;
public:
	Data *data;
	RegistrationModule *reg;
};