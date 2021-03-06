﻿#include"Scan.h"
#include"vtkImageMapper.h"
#include"vtkActor2D.h"


double Scan::getDistane(double *src, double *tar)
{
	double retv = 0.0;
	//if (tar[0] == 0)return INF;
	//retv += (src[0] - tar[0])*(src[0] - tar[0]);
	//retv += (src[1] - tar[1])*(src[1] - tar[1]);
	retv += abs((src[2] - tar[2]));
	return retv;
}

void  Scan::cellInsert(vtkCellArray *cell, int number, long long index1, long long index2, long long index3, long long disp)
{
	cell->InsertNextCell(number);
	cell->InsertCellPoint(index1 + disp); cell->InsertCellPoint(index2 + disp); cell->InsertCellPoint(index3 + disp);

}


void Scan::frames2PointsCutOutlier()
{
	if (points == nullptr)
	{
		points = vtkPoints::New();
	}

	rs2::pointcloud pc;
	rs2::points rsPoints;
	std::vector<const rs2::vertex*> ver;

	for (int i = 0; i < frames.size(); i++)
	{
		rsPoints = pc.calculate(frames[i]);
		ver.push_back(rsPoints.get_vertices());
	}

	for (int j = 0; j < 1280 * 720; j++)
	{
		std::vector<double>Z;
		for (int i = 0; i < frames.size(); i++)
		{
			if (ver[i][j].z != 0 && ver[i][j].z < 1 && ver[i][j].z > -1)
			{
				Z.push_back(ver[i][j].z);
			}
		}

		double _Z;
		_Z = 0;

		std::sort(Z.begin(), Z.end());

		int cnt = 0;
		for (int i = Z.size()*0.3; i < Z.size()*0.7; i++)
		{
			_Z += Z[i];
			cnt++;
		}
		if (cnt <= 1)
			points->InsertNextPoint(0, 0, 0);
		else
		{
			_Z /= cnt;
			points->InsertNextPoint(ver[0][j].x, ver[0][j].y, _Z);
		}

		Z.clear();
	}

	ver.clear();
	frames.clear();

}
void Scan::frames2Points()
{
	if (points == nullptr)
	{
		points = vtkPoints::New();
	}

	rs2::pointcloud pc;
	rs2::points rsPoints;
	std::vector<const rs2::vertex*> ver;

	for (int i = 0; i < frames.size(); i++)
	{
		rsPoints = pc.calculate(frames[i]);
		ver.push_back(rsPoints.get_vertices());
	}

	for (int j = 0; j < 1280 * 720; j++)
	{
		int cnt = 0;
		double X, Y, Z;
		X = Y = Z = 0;

		for (int i = 0; i < frames.size(); i++)
		{
			if (ver[i][j].z != 0 && ver[i][j].z < 1 && ver[i][j].z > -1)
			{
				X += ver[i][j].x;
				Y += ver[i][j].y;
				Z += ver[i][j].z;
				cnt++;
			}
		}

		if (cnt == 0)
			points->InsertNextPoint(0, 0, 0);
		else
		{
			X /= cnt; Y /= cnt; Z /= cnt;
			points->InsertNextPoint(X, Y, Z);
		}
	}

	ver.clear();
	frames.clear();

}
void  Scan::MeshConstructWithOMP(MeshPreview *viewer, vtkPoints *point, int saveType, int ThreadSize)
{
	std::cout << " " << point->GetNumberOfPoints() << "\n";
	std::cout << viewer->m_Actor.size() << " ";


#pragma omp parallel num_threads(ThreadSize)		 
	{
		vtkPoints *threadPoint = vtkPoints::New();
		vtkCellArray *threadCell = vtkCellArray::New();
		const int alpha = 0.005;

		threadPoint->SetNumberOfPoints(width*height / ThreadSize);

#pragma omp for
		for (int i = 0; i < width*height; i++)
		{
			double temp[3];
			point->GetPoint(i, temp);
			threadPoint->SetPoint(i - (omp_get_thread_num()) * width*height / ThreadSize, temp);
		}

		for (vtkIdType i = 0; i < threadPoint->GetNumberOfPoints(); i++)
		{
			if (i + 1 + width > threadPoint->GetNumberOfPoints())continue;
			if ((i + 1) % width == 0)continue;

			double orign[3], right[3], down[3], diga[3];

			threadPoint->GetPoint(i, orign);
			if (orign[0] == 0)continue;

			threadPoint->GetPoint(i + 1, right);
			threadPoint->GetPoint(i + width, down);
			threadPoint->GetPoint(i + width + 1, diga);

			double _dia = getDistane(orign, diga);
			double _down = getDistane(orign, down);
			double _right = getDistane(orign, right);

		//	if (_right > alpha)continue;

			if (_down < _dia)
			{
				if (right[0] != 0 && down[0] != 0)
				{
					cellInsert(threadCell, 3, i, i + 1, i + width);
					if (diga[0] != 0)
						cellInsert(threadCell, 3, i + 1, i + width + 1, i + width);
				}
			}

			else
			{
				if (diga[0] != 0)
				{
					if (right[0] != 0)
						cellInsert(threadCell, 3, i, i + 1, i + width + 1);
					if (down[0] != 0)
						cellInsert(threadCell, 3, i, i + width + 1, i + width);
				}
			}

		}

		#pragma omp critical
		{
			//if (omp_get_thread_num() == 1)/*여기서만 제대로 set이 안되는 이유가 있을까?*/
		/*	if(0)
			{
				viewer->GetPolyDataAt(omp_get_thread_num())->SetPoints(threadPoint);
				viewer->GetPolyDataAt(omp_get_thread_num())->SetPolys(threadCell);
				viewer->GetPolyDataAt(omp_get_thread_num())->Modified();
				viewer->GetMapperAt(1)->SetInputData(viewer->GetPolyDataAt(1));
				viewer->GetActorAt(1)->SetMapper(viewer->GetMapperAt(1));
				viewer->GetRenderer()->AddActor(viewer->GetActorAt(1));

				std::cout << viewer->GetPolyDataAt(omp_get_thread_num())->GetPolys()->GetNumberOfCells() << "개개개\n";
			}
*/
			//else
			{
				viewer->GetPolyDataAt(omp_get_thread_num())->SetPoints(threadPoint);
				viewer->GetPolyDataAt(omp_get_thread_num())->SetPolys(threadCell);
				viewer->GetPolyDataAt(omp_get_thread_num())->Modified();
				/*viewer->GetPolyDataAt(1)->SetPoints(threadPoint);
				viewer->GetPolyDataAt(1)->SetPolys(threadCell);
				viewer->GetMapperAt(1)->SetInputData(viewer->GetPolyDataAt(1));
				viewer->GetActorAt(1);
				viewer->GetActorAt(1);
				viewer->GetRenderer()->AddActor(viewer->GetActorAt(1));*/
			}
		}

		//threadPoint->Delete();
		//threadCell->Delete();
	}

	vtkPoints *boundary = vtkPoints::New();
	vtkCellArray *cellBoundary = vtkCellArray::New();
	vtkPolyData *polyBoundary = vtkPolyData::New();

	for (int j = 1; j <= ThreadSize - 1; j++)
	{
		for (int i = j*width * height / ThreadSize - width; i <j*width * height / ThreadSize + width * 2 - width; i++)
		{
			double temp[3];
			point->GetPoint(i, temp);
			boundary->InsertNextPoint(temp);
		}
	}


	for (int j = 0; j < ThreadSize - 1; j++)
	{
		for (vtkIdType i = 2 * j*width; i < 2 * width*j + width; i++)
		{
			double orign[3], right[3], down[3], diga[3];
			boundary->GetPoint(i, orign);
			boundary->GetPoint(i + 1, right);
			boundary->GetPoint(i + width, down);
			boundary->GetPoint(i + width + 1, diga);

			if (orign[0] == 0)continue;

			double _dia = getDistane(orign, diga);
			double _down = getDistane(orign, down);

			if (_down < _dia)
			{
				if (right[0] != 0 && down[0] != 0)
				{
					cellInsert(cellBoundary, 3, i, i + 1, i + width);
					if (diga[0] != 0)
						cellInsert(cellBoundary, 3, i + 1, i + width + 1, i + width);
				}
			}

			else
			{
				if (diga[0] != 0)
				{
					if (right[0] != 0)
						cellInsert(cellBoundary, 3, i, i + 1, i + width + 1);
					if (down[0] != 0)
						cellInsert(cellBoundary, 3, i, i + width + 1, i + width);
				}
			}

		}
	}

	std::cout << omp_get_num_threads() << "\n";
	viewer->GetPolyDataAt(ThreadSize)->SetPoints(boundary);
	viewer->GetPolyDataAt(ThreadSize)->SetPolys(cellBoundary);//이거 주석 풀고 나갔다 옴
	viewer->GetPolyDataAt(ThreadSize)->Modified();
	//boundary->Delete();
	//cellBoundary->Delete();

	viewer->GetRenderWindow()->Modified();
	viewer->GetRenderWindow()->Render();

	std::cout << "Done";
}

vtkRenderer* Scan::MeshConstruct(MeshPreview *viewer, vtkPoints *point, int saveType)
{
	vtkCellArray *cell = vtkCellArray::New();

	double alpha = 0.005;
	for (vtkIdType i = 0; i < point->GetNumberOfPoints() - width; i++)
	{
		double orign[3], right[3], down[3], diga[3];
		point->GetPoint(i, orign);
		if (orign[0] == 0)continue;

		point->GetPoint(i + 1, right);
		point->GetPoint(i + width, down);
		point->GetPoint(i + width + 1, diga);

		if ((i + 1) % width == 0)continue;
		if (orign[0] == 0)continue;

		double _dia = getDistane(orign, diga);
		double _down = getDistane(orign, down);
		double _right = getDistane(orign, right);

		if (_right > alpha)continue;

		if (_down < _dia)
		{
			if (right[0] != 0 && down[0] != 0)
			{
				if (_down < alpha)
					cellInsert(cell, 3, i, i + 1, i + width);//down�� ����
				if (diga[0] != 0 && _dia <alpha)
					cellInsert(cell, 3, i + 1, i + width + 1, i + width);
			}
		}

		else
		{
			if (diga[0] != 0 && _dia<alpha)
			{
				if (right[0] != 0)
					cellInsert(cell, 3, i, i + 1, i + width + 1);
				if (down[0] != 0 && _down<alpha)
					cellInsert(cell, 3, i, i + width + 1, i + width);
			}
		}
	}


	viewer->GetPolyDataAt(0)->SetPoints(point);
	viewer->GetPolyDataAt(0)->SetPolys(cell);
	/*viewer->GetMapperAt(0)->Modified();
	viewer->GetActorAt(0)->Modified();
	viewer->GetRenderer()->Modified();*/
	/*if (saveType == 1)
	{
	vtkSTLWriter* stlWriter = vtkSTLWriter::New();
	stlWriter->SetFileName("my.stl");
	stlWriter->SetInputData(viewer->GetPolyDataAt(0));
	stlWriter->Write();
	stlWriter->Delete();
	}

	else if (saveType == 2) {
	vtkOBJExporter *obj = vtkOBJExporter::New();
	obj->SetInput(viewer->GetRenderWindow());
	obj->SetFilePrefix("mine");
	obj->Write();
	obj->Delete();
	}*/
	/*vtkOBJExporter *obj = vtkOBJExporter::New();
	obj->SetInput(viewer->GetRenderWindow());
	obj->SetFilePrefix("mine");
	obj->Write();
	obj->Delete();*/

	viewer->GetRenderWindow()->Modified();
	viewer->GetRenderWindow()->Render();

	cell->Delete();
	return viewer->GetRenderer();
}

void Scan::MeshConstruction(MeshPreview *viewer, int mode, int saveType, int ThreadSize)
{
	if (this->points == nullptr)
	{
		std::cout << "Points are not setted";
		return;
	}
	switch (mode)
	{
	case 0:
		MeshConstructWithOMP(viewer, this->points, saveType, ThreadSize);
		break;
	case 1:
		MeshConstructWithOMPnSIMD(viewer, this->points, saveType, ThreadSize);
		break;
	default:
		MeshConstruct(viewer, this->points, saveType);
		break;
	}
}


/*
�� �κ� omp�����ϴ°� �����غ� ��.
*/
void Scan::frame2Points(const rs2::frame& frame)
{
	rs2::pointcloud pc;
	rs2::points rsPoints;
	rsPoints = pc.calculate(frame);


	if (points == nullptr)
		points = vtkPoints::New();

	auto v = rsPoints.get_vertices();

	////std::ofstream str("test.txt");
	for (auto i = 0; i < width*height; i++)
	{
	/*	if (v[i].z >= 1 || v[i].z <= -1 || v[i].z==0)
			points->InsertNextPoint(v[i].x, v[i].y, 0);
		else*/
		points->InsertNextPoint(v[i].x, v[i].y, v[i].z);

	}

}

vtkRenderer*  Scan::MeshConstructWithOMPnSIMD(MeshPreview *viewer, vtkPoints *point, int saveType, int ThreadSize)
{
	vtkRenderer *threadRenderer = vtkRenderer::New();
	vtkRenderWindow *win = vtkRenderWindow::New();
	vtkRenderWindowInteractor * interactor = vtkRenderWindowInteractor::New();
	interactor->SetRotation(0.1);

	//std::cout << point->GetNumberOfPoints() << "\n";

	const int ThreadNum = ThreadSize;

#pragma omp parallel num_threads(ThreadNum)	shared(threadRenderer) 
	{
		vtkPoints *threadPoint = vtkPoints::New();
		vtkCellArray *threadCell = vtkCellArray::New();
		vtkPolyData *threadPoly = vtkPolyData::New();
		vtkPolyDataMapper *threadMapper = vtkPolyDataMapper::New();
		vtkActor *threadActor = vtkActor::New();

		__declspec(align(64)) double _orign[4] = { 0, };
		__declspec(align(64)) double _right[4] = { 0, };
		__declspec(align(64)) double _down[4] = { 0, };
		__declspec(align(64)) double _diga[4] = { 0, };
		__declspec(align(64)) double result[4] = { 0, };

#pragma omp for
		for (int i = 0; i < width*height; i++)
		{
			double temp[3];
			point->GetPoint(i, temp);
			threadPoint->InsertNextPoint(temp);
		}

		for (int j = 0; j<threadPoint->GetNumberOfPoints() - width; j += 4)
		{
			/*seting*/
			int index = 0;
			double orign[3], right[3], down[3], diga[3];
			for (int k = j; k < j + 4; k++)
			{
				threadPoint->GetPoint(k, orign);
				threadPoint->GetPoint(k + 1, right);
				threadPoint->GetPoint(k + width, down);
				threadPoint->GetPoint(k + width + 1, diga);

				_orign[index] = orign[2];
				_right[index] = right[2];
				_down[index] = down[2];
				_diga[index] = diga[2];
				index++;
			}

			/*chunk calculate*/
			__m256d __m256d_orign = _mm256_loadu_pd(_orign);
			__m256d __m256d_down = _mm256_loadu_pd(_down);
			__m256d __m256d_diga = _mm256_loadu_pd(_diga);
			__m256d __m256d_result = _mm256_sub_pd(_mm256_sub_pd(__m256d_orign, __m256d_down), \
				_mm256_sub_pd(__m256d_orign, __m256d_diga));

			__m256d_result = _mm256_mul_pd(__m256d_result, __m256d_result);//�Ÿ��� �������� ����.
			_mm256_store_pd(result, __m256d_result);

			/*make poly*/
			for (int i = 0; i < 4; i++)
			{
				if (_orign[i] == 0)continue;

				if (result[i] > 0)//�Ʒ��� ����
				{
					if (_right[i] != 0 && _down[i] != 0)
					{
						cellInsert(threadCell, 3, i, i + 1, i + width, j);
						if (_diga[i] != 0)
							cellInsert(threadCell, 3, i + 1, i + width + 1, i + width, j);
					}
				}

				else //if (result[i] < 0)//�밢������
				{
					if (_diga[i] != 0)
					{
						if (_right[i] != 0)
							cellInsert(threadCell, 3, i, i + 1, i + width + 1, j);
						if (_down[i] != 0)
							cellInsert(threadCell, 3, i, i + width + 1, i + width, j);
					}
				}
			}
		}

		//_start = omp_get_wtime();

		threadPoly->SetPoints(threadPoint);
		threadPoly->SetPolys(threadCell);

		threadMapper->SetInputData(threadPoly);
		threadActor->SetMapper(threadMapper);

#pragma omp critical
		{
			threadRenderer->AddActor(threadActor);
		}
		threadPoint->Delete();
		threadCell->Delete();
		threadPoly->Delete();
		threadMapper->Delete();
		threadActor->Delete();

		//printf("%lf\n\n", omp_get_wtime() - _start);
	}


	vtkPoints *boundary = vtkPoints::New();
	vtkCellArray *cellBoundary = vtkCellArray::New();
	vtkPolyData *polyBoundary = vtkPolyData::New();
	vtkActor *actorBoundary = vtkActor::New();
	vtkPolyDataMapper *mapperBoundary = vtkPolyDataMapper::New();

	for (int j = 1; j <= ThreadNum - 1; j++)
	{
		for (int i = j*width * height / ThreadNum - width; i <j*width * height / ThreadNum + width * 2 - width; i++)
		{
			double temp[3];
			point->GetPoint(i, temp);
			boundary->InsertNextPoint(temp);
		}
	}

	/*���⵵ simd�����غ� ��.*/
	for (int j = 0; j < ThreadNum - 1; j++)
	{
		double orign[3], right[3], down[3], diga[3];
		for (vtkIdType i = 2 * j*width; i < 2 * width*j + width; i++)
		{
			boundary->GetPoint(i, orign);
			boundary->GetPoint(i + 1, right);
			boundary->GetPoint(i + width, down);
			boundary->GetPoint(i + width + 1, diga);


			if (orign[0] == 0)continue;

			double _dia = getDistane(orign, diga);
			double _down = getDistane(orign, down);

			if (_down < _dia)
			{
				if (right[0] != 0 && down[0] != 0)
				{
					cellInsert(cellBoundary, 3, i, i + 1, i + width);
					if (diga[0] != 0)
						cellInsert(cellBoundary, 3, i + 1, i + width + 1, i + width);
				}
			}

			else
			{
				if (diga[0] != 0)
				{
					if (right[0] != 0)
						cellInsert(cellBoundary, 3, i, i + 1, i + width + 1);
					if (down[0] != 0)
						cellInsert(cellBoundary, 3, i, i + width + 1, i + width);
				}
			}

		}
	}
	polyBoundary->SetPoints(boundary);
	polyBoundary->SetPolys(cellBoundary);
	mapperBoundary->SetInputData(polyBoundary);
	actorBoundary->SetMapper(mapperBoundary);

	threadRenderer->AddActor(actorBoundary);

	threadRenderer->GetActiveCamera()->ParallelProjectionOff();
	win->AddRenderer(threadRenderer);
	//win->Render();


	interactor->SetRenderWindow(win);
	//interactor->Start();

	boundary->Delete();
	cellBoundary->Delete();
	polyBoundary->Delete();
	actorBoundary->Delete();
	mapperBoundary->Delete();

	interactor->Delete();
	threadRenderer->Delete();
	win->Delete();

	return threadRenderer;
	//printf("%lf\n\n", omp_get_wtime() - _start);
}