#pragma once
#include "ui_initializemodule.h"

class InitializeModule : public QWidget { //QWidget ���..
	Q_OBJECT

public :
	InitializeModule(QWidget* parent = Q_NULLPTR);
	void Init();
private : 

	Ui::InitializeModule ui;
};