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
	��� Filter class�� ���� �����ϴ°͵� ���� �Ű����� ���߿� �ð����� �غ����� ����.
	*/
	void gaussianRad(DepthMapPreviewer *viewer, double rad)
	{
		vtkImageGaussianSmooth *gaus = vtkImageGaussianSmooth::New();
		gaus->SetInputData(viewer->GetImageData());
		gaus->SetRadiusFactor(rad);
		gaus->Update();

		//viewer->m_ImageData->DeepCopy(gaus->GetOutput());

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

		viewer->GetActor()->GetMapper()->SetInputConnection(gaus->GetOutputPort());
		//viewer->GetActor()->Modified();

		viewer->GetRenderWindow()->Modified();
		viewer->GetRenderWindow()->Render();
	}

	void hybridMedian2D(DepthMapPreviewer *viewer,int value)
	{
		vtkImageHybridMedian2D *Median2D = vtkImageHybridMedian2D::New();

		Median2D->SetInputData(viewer->GetImageData());
		//vtkImageActor* hybridMedianActor =vtkImageActor::New();
		Median2D->Update();
		viewer->GetActor()->GetMapper()->SetInputConnection(Median2D->GetOutputPort());
		viewer->GetActor()->Modified();

		viewer->GetRenderWindow()->Modified();
		viewer->GetRenderWindow()->Render();
		Median2D->Delete();

	}
	void imageMedian3D(DepthMapPreviewer *viewer, int value)
	{
		auto temp = viewer->GetImageData()->GetScalarPointer();

		unsigned short * _value = static_cast<unsigned short*>(temp);

		vtkImageMedian3D *Median2D = vtkImageMedian3D::New();

		Median2D->SetInputData(viewer->GetImageData());
		Median2D->SetKernelSize(value, value, value);
		Median2D->Update();
		//vtkImageActor* hybridMedianActor =vtkImageActor::New();
		viewer->GetActor()->GetMapper()->SetInputConnection(Median2D->GetOutputPort());
		viewer->GetActor()->Modified();

		viewer->GetRenderWindow()->Modified();
		viewer->GetRenderWindow()->Render();
		Median2D->Delete();
	}


	void meshSmooth(MeshPreview *viewer, double Relaxation)
	{
		std::cout << "asdasd";

		//vtkSmartPointer<vtkDelaunay2D> delaunay =
		//	vtkSmartPointer<vtkDelaunay2D>::New();

		//delaunay->SetInputData(viewer->GetPolyDataAt(0));
		//delaunay->Update();

		vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter =
			vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
		smoothFilter->SetInputData(viewer->GetPolyDataAt(0));
		smoothFilter->SetNumberOfIterations(10);
		smoothFilter->SetRelaxationFactor(1);
		smoothFilter->FeatureEdgeSmoothingOff();
		smoothFilter->BoundarySmoothingOn();
		smoothFilter->Update();


		//vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
		//normalGenerator->SetInputConnection(smoothFilter->GetOutputPort());
		//normalGenerator->ComputePointNormalsOn();
		//normalGenerator->ComputeCellNormalsOn();
		//normalGenerator->Update();

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
		viewer->GetActor()->GetInput()->GetScalarPointer();

		auto temp= viewer->GetActor()->GetInput()->GetScalarPointer();

		unsigned short * _value = static_cast<unsigned short*>(temp);
		
		for (int i = 0; i < 1280*720; i++)
		{
			//std::cout << unsigned short(_value[i])<<" ";
			double temp[3];
			points->GetPoint(i, temp);
			double val = _value[i] / 1024.0;

			if(val<1 && val>-1 && val!=0)
				points->SetPoint(i, temp[0], temp[1], val);
			else 
				points->SetPoint(i, 0,0,0);
		}
		//65536

	}

	
	//void printPointCloud(MeshPreview *viewer)
	//{
	//	if (points == nullptr)
	//	{
	//		std::cout << "Point is not setted";
	//		return;
	//	}
	//	viewer->GetPolyDataAt(0)->SetPoints(points);

	//	vtkVertexGlyphFilter* vertexFilter =vtkVertexGlyphFilter::New();

	//	vertexFilter->SetInputData(viewer->GetPolyDataAt(0));
	//	vertexFilter->Update();

	//	//vtkPolyData* polydata =vtkPolyData::New();
	//	viewer->GetPolyDataAt(0)->ShallowCopy(vertexFilter->GetOutput());
	//	
	//	viewer->GetActorAt(0)->GetProperty()->SetPointSize(0.5f);
	//	// Setup colors
	//

	//	/*vtkUnsignedCharArray* colors =vtkUnsignedCharArray::New();
	//	colors->SetNumberOfComponents(3);
	//	colors->SetName("Colors");
	//	colors->InsertNextTuple(red);
	//	colors->InsertNextTupleValue(red);
	//	colors->InsertNextTupleValue(green);
	//	colors->InsertNextTupleValue(blue);*/
	//	//vtkUnsignedCharArray* colors = vtkUnsignedCharArray::New();

	//	
	//	
	//	//colors->InsertNextTuple(1.0, 0.0, 1.0);
	//	//viewer->GetPolyDataAt(0)->GetPointData()->SetScalars(colors);
	//	viewer->GetMapperAt(0)->SetInputData(viewer->GetPolyDataAt(0));


	//	viewer->GetActorAt(0)->SetMapper(viewer->GetMapperAt(0));
	//	viewer->GetActorAt(0)->GetProperty()->SetPointSize(5);

	//	

	//	viewer->GetRenderWindow()->AddRenderer(viewer->GetRenderer());
	//	viewer->GetInteractor()->SetRenderWindow(viewer->GetRenderWindow());

	//	viewer->GetRenderer()->AddActor(viewer->GetActorAt(0));

	//	viewer->GetRenderWindow()->Render();
	//}
private:
	vtkPoints *points;
	vtkRenderer* MeshConstruct(MeshPreview *viewer, vtkPoints *point, int saveType);
	double getDistane(double *src, double *tar);
	void cellInsert(vtkCellArray *cell, int number, long long index1, long long index2, long long index3, long long disp=0);
	void MeshConstructWithOMP(MeshPreview *viewer,vtkPoints *point, int saveType, int ThreadSize);
	vtkRenderer* MeshConstructWithOMPnSIMD(MeshPreview *viewer,vtkPoints *point, int saveType, int ThreadSize);


};