#ifndef SCANMODULEQT_H
#define SCANMODULEQT_H

#include <QtWidgets/QMainWindow>
#include "ui_scanmoduleqt.h"
#include"MeshPreview.h"
#include"ImagePreview.h"
#include"DepthMapPreviewer.h"

#include"vtkPointData.h"
#include"vtkDoubleArray.h"
#include"vtkFloatArray.h"
#include"Scan.h"
#include"vtkSTLWriter.h"
#include"vtkPNGWriter.h"

#include"vtkPNGReader.h"
#include"vtkSTLReader.h"
#include"vtkPLYWriter.h"
#include"vtkPLYReader.h"
#include"vtkTexture.h"
class ScanModuleQT : public QMainWindow
{
	Q_OBJECT

public:
	ScanModuleQT(QWidget *parent = 0);
	~ScanModuleQT();

	
	int										m_IsMeshPreviewer;
	MeshPreview*							m_MeshPreviewer;

	int										m_IsDepthPreviewer;
	DepthMapPreviewer						*m_DepthPreviewer;

	void InitializeUi();
public slots:
	void slotCapBtn();
	void slotNextBtn();



	void InitializeVariables();
	void InitializeScene();
	int DestroyVariables();
	void imageMedian3D(int val);
	void gausianFilterStd(int std);
	void gausianFilterRad(int rad);
	void MeshSmoothing();

	void GausStdBtn();
	void GausRadBtn();
	void MedianBtn();
	void slotMultiCapBtn();
	void slotApplyFilter();
	void slotNextBtn2();
	void slotResetBtn();
	void slotTextureMapping();
private:
	Ui::ScanModuleQTClass ui;

	realsense::Device *RealSenseD415;
	Scan *Scanner;
};

#endif // SCANMODULEQT_H
