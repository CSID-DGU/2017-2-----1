#include "ui_mainmodule.h"
#include "ui_calibrationmodule.h"
#include "ui_scanmodule.h"
/*TODO
- runViewer��� ��Ʈ���� ����
*/
class MainModule : public QWidget { //QWidget ���..
	Q_OBJECT

public:
	MainModule(QWidget* parent = Q_NULLPTR);
	void runCalibrationModule();
	void runScanModule();
	void runViewerModule();
	void runExpModule();
private slots:
	void cellSelected(int nRow, int nCol);
private:
	Ui::MainModule ui;
};