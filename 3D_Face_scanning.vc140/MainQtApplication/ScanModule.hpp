#pragma once
#include "ui_scanmodule.h"
/*
���ϳ��� ��ü�� ���鵵�� �ϱ�

*/
class ScanModule : public QWidget{
	Q_OBJECT

public:
	ScanModule(QWidget *parent = Q_NULLPTR);

private slots:
	void on_NextButton_clicked();
private:
	Ui::ScanModule ui;
};