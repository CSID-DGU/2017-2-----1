#pragma once
#include "ui_registrationmodule.h"
#include "Data.cpp"
/*
���ϳ��� ��ü�� ���鵵�� �ϱ�

*/
class RegistrationModule : public QWidget {
	Q_OBJECT

public:
	RegistrationModule(QWidget *parent = Q_NULLPTR);
	~RegistrationModule() {
		printf("reg_exited");
	}
	void setData(Data *data) {
		this->data = data;
	}
private slots:
	void SaveButtonClicked();
	void QuitButtonClicked();
private:
	Ui::RegistrationModule ui;
public:
	Data *data;
};