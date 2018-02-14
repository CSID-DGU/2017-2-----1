#include "ui_mainmodule.h"
#include "ui_calibrationmodule.h"

class MainModule : public QWidget { //QWidget ���..
	Q_OBJECT

public:
	MainModule(QWidget* parent = Q_NULLPTR);
	void runCalibrationModule();
	void runScanModule();
	void runViewerModule();
	void runExpModule();

private:
	Ui::MainModule ui;
};