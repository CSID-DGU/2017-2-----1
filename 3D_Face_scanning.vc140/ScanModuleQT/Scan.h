#pragma once
#undef foreach
#undef Q_FOREACH


#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);


#include<vtkPoints.h>
#include<vtkOBJExporter.h>
#include<librealsense2\rs.hpp>
#include <iomanip>

#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkInteractorStyle.h>
#include<vtkCellArray.h>
#include<vtkPolyData.h>
#include<vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include<vtkimageData.h>
#include<vtkImageViewer2.h>

#include"vtkSTLWriter.h"
#include<string>

#include <utility>
#include <cstdint>

#include"../DepthModule/device.h"
#include"MeshPreview.h"
#include"DepthMapPreviewer.h"


#include"vtkImageMapper3D.h"
#include"vtkImageHybridMedian2D.h"
#include"vtkImageMedian3D.h"
#include"vtkImageActor.h"
#include"vtkImageGaussianSmooth.h"
#include"vtkSmoothPolyDataFilter.h"
#include"vtkDelaunay2D.h"
#include"vtkPolyDataNormals.h"
#include<vector>
#include<omp.h>
enum { color, depth };
enum { omp, ompNsimd, serial };


class Scan
{
private:
	const int width = 1280;
	const int height = 720;
	const int INF = 999999999;
	std::vector<rs2::frame> frames;

public:

	void InsertFrame(rs2::frame &frame)
	{
		frames.push_back(frame);
	}

	~Scan()
	{
		if(points !=NULL && points!=nullptr)	
			points->Delete();
		points = NULL;
		frames.clear();
	}
	Scan()
	{
		points = nullptr;
	}

	void frames2Points();
	void frames2PointsCutOutlier();
	std::string saveImage(rs2::frame &frame, std::string filepath, int filetype){}

	void frame2Points(const rs2::frame& frame);

	/*mode0 : omp  mode1 : omp with simd   else serial*/
	void MeshConstruction(MeshPreview *viewer,int mode, int saveType, int ThreadSize = 4);

	
	void printDepthMap(DepthMapPreviewer *viewer, realsense::Device* device, realsense::RS_400_STREAM_TYPE type);
	void ReleaseModel()
	{
		
		if (points != NULL && points!=nullptr)
		{
			points->Delete();
			points=NULL;
		}

		points = vtkPoints::New();
		frames.clear();

	}
	void Delete()
	{
		points->Delete();
		points = NULL;
		frames.clear();
	}

	/*
	사실 Filter class를 따로 구성하는것도 좋을 것 같음.
	*/
	void gaussianRad(DepthMapPreviewer *viewer, double rad)
	{
		vtkImageGaussianSmooth *gaus = vtkImageGaussianSmooth::New();
		gaus->SetInputData(viewer->GetImageData());
		gaus->SetRadiusFactor(rad);
		gaus->Update();

		//viewer->m_ImageData->DeepCopy(gaus->GetOutput());
		viewer->m_ImageData->ShallowCopy(gaus->GetOutput());
		viewer->m_ImageData->Modified();


		viewer->GetActor()->GetMapper()->SetInputConnection(gaus->GetOutputPort());
		//viewer->GetActor()->Modified();

		viewer->GetRenderWindow()->Modified();
		viewer->GetRenderWindow()->Render();
	}

	void gaussianStd(DepthMapPreviewer *viewer, double std)
	{
		vtkImageGaussianSmooth *gaus = vtkImageGaussianSmooth::New();
		gaus->SetInputData(viewer->GetImageData());
		gaus->SetStandardDeviation(std);
		gaus->Update();

		//viewer->m_ImageData->DeepCopy(gaus->GetOutput());

		viewer->m_ImageData->ShallowCopy(gaus->GetOutput());
		viewer->m_ImageData->Modified();


		viewer->GetActor()->GetMapper()->SetInputConnection(gaus->GetOutputPort());
		//viewer->GetActor()->Modified();

		viewer->GetRenderWindow()->Modified();
		viewer->GetRenderWindow()->Render();
	}


	void imageMedian3D(DepthMapPreviewer *viewer, int value)
	{
		auto temp = viewer->GetImageData()->GetScalarPointer();
		unsigned short * _value = static_cast<unsigned short*>(temp);
		vtkImageMedian3D *Median2D = vtkImageMedian3D::New();


		Median2D->SetInputData(viewer->GetOriginImage());//얘는 원본 이미지로 할 것.
		Median2D->SetKernelSize(value, value, value);
		Median2D->Update();

		//viewer->m_ImageData->DeepCopy(Median2D->GetOutput());
		viewer->m_ImageData->ShallowCopy(Median2D->GetOutput());
		viewer->m_ImageData->Modified();


		//vtkImageActor* hybridMedianActor =vtkImageActor::New();
		viewer->GetActor()->GetMapper()->SetInputConnection(Median2D->GetOutputPort());
		viewer->GetActor()->Modified();

		viewer->GetRenderWindow()->Modified();
		viewer->GetRenderWindow()->Render();
		Median2D->Delete();
	}


	void meshSmooth(MeshPreview *viewer, double Relaxation)
	{
		vtkSmoothPolyDataFilter* smoothFilter =vtkSmoothPolyDataFilter::New();
		smoothFilter->SetInputData(viewer->GetPolyDataAt(0));
		smoothFilter->SetNumberOfIterations(100);
		smoothFilter->SetRelaxationFactor(Relaxation);
		smoothFilter->FeatureEdgeSmoothingOff();
		smoothFilter->BoundarySmoothingOn();
		smoothFilter->Update();

		//viewer->GetRenderer()->RemoveActor(viewer->GetActorAt(0));
		//viewer->GetActorAt(0)->Delete();
		//viewer->GetActorAt(0) = vtkActor::New();

		//viewer->GetMapperAt(0)->Delete();
		//viewer->GetMapperAt(0) = vtkPolyDataMapper::New();

		//viewer->GetPolyDataAt(0)->ReleaseData();
		//viewer->GetPolyDataAt(0)->Delete();
		//viewer->GetPolyDataAt(0) = vtkPolyData::New();

		viewer->GetMapperAt(0)->SetInputData(smoothFilter->GetOutput());
		viewer->GetActorAt(0)->SetMapper(viewer->GetMapperAt(0));
		viewer->GetActorAt(0)->Modified();
		viewer->GetRenderWindow()->Modified();
		viewer->Rendering();
	}
	void upDataPoint(DepthMapPreviewer *viewer)
	{
		//viewer->GetActor()->GetInput()->GetScalarPointer();
		//auto temp= viewer->GetActor()->GetInput()->GetScalarPointer();
		for (int i = 0; i < 1280 * 720; i++)
		{
			double *temp;
			temp = viewer->GetImageData()->GetPoint(i);
			double orig[3];

			points->GetPoint(i, orig);

			double val = temp[2] / 1024.0;

			if (val<1 && val>-1 && val != 0)
				points->SetPoint(i, orig[0], orig[1], val);

			else
				points->SetPoint(i, 0, 0, 0);
		}
	}

private:
	vtkPoints *points;
	vtkRenderer* MeshConstruct(MeshPreview *viewer, vtkPoints *point, int saveType);
	double getDistane(double *src, double *tar);
	void cellInsert(vtkCellArray *cell, int number, long long index1, long long index2, long long index3, long long disp=0);
	void MeshConstructWithOMP(MeshPreview *viewer,vtkPoints *point, int saveType, int ThreadSize);
	vtkRenderer* MeshConstructWithOMPnSIMD(MeshPreview *viewer,vtkPoints *point, int saveType, int ThreadSize);


};