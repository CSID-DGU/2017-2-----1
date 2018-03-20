#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Align.h"

#include"vtkRenderer.h"
#include"vtkActor.h"
#include<vector>
#include"vtkLandmarkTransform.h"
#include"vtkPoints.h"
#include"vtkPolyData.h"
#include"vtkVertexGlyphFilter.h"


#include"vtkRenderWindow.h"
#include"vtkPolyDataMapper.h"
#include"vtkTransformPolyDataFilter.h"
#include"vtkProperty.h"
#include"vtkPolyDataMapper.h"
#include"vtkRenderWindow.h"
#include"vtkRenderWindowInteractor.h"
#include"double3.h"
//#include"vtkMatrix4x4.h"

#include"MeshPreview.h"
#include"vtkAssembly.h"

#include"LandMarkInteractorStyle.h"
#include"vtkAppendPolyData.h"
#include"vtkTransformPolyDataFilter.h"
enum { LEFT, FRONT, RIGHT };
class AlignModule : public QMainWindow
{
	Q_OBJECT
private:
	MeshPreview *left;
	MeshPreview *front;
	MeshPreview *right;


	LandMarkInteractorStyle *leftStyle;
	LandMarkInteractorStyle *frontStyle;
	LandMarkInteractorStyle *rightStyle;
public:
	MeshPreview *resultMesh;
	void Rendering()
	{
		left->Rendering();
		front->Rendering();
		right->Rendering();
	}
	AlignModule();
	void mergeActors(MeshPreview *mesh, int place);
	//void registeration();
	std::vector<double3> extractLandMark(vtkRenderer *rend, int flag);
	void setRight(MeshPreview *rend);
	void setFront(MeshPreview *rend);
	void setLeft(MeshPreview *rend);

	void InitializeVariables();
	int DestroyVariables();
	void InitializeUi();
	
public slots:
	void slotAlign();
	void slotLanMarkLeft();
	void slotLanMarkFront();
	void slotLanMarkRight();

	void slotLeftDefault();
	void slotFrontDefault();
	void slotRightDefault();


	void slotLeftOrigin();
	void slotFrontOrigin();
	void slotRightOrigin();
private:
	Ui::AlignModuleClass ui;

};
