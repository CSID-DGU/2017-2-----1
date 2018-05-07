
#include"AlignModule.h"

/**
Member ������ �ִ� left, front, right�� Align�� �����ϰ�
����� vtkRenderer�� ����.
*/

AlignModule::AlignModule()
{
	InitializeVariables();
	InitializeUi();
}
void AlignModule::InitializeVariables()
{
	left=NULL;
	front=NULL;
	right = NULL;

	leftStyle = frontStyle = rightStyle = NULL;


	left  = new MeshPreview(5);
	front = new MeshPreview(5);
	right = new MeshPreview(5);

	left->Create3DScene();
	front->Create3DScene();
	right->Create3DScene();


	leftStyle  = LandMarkInteractorStyle::New();
	frontStyle = LandMarkInteractorStyle::New();
	rightStyle = LandMarkInteractorStyle::New();

	resultMesh = new MeshPreview(3);
	resultMesh->CreateModel("",0);
	resultMesh->Create3DScene();
}

int AlignModule::DestroyVariables()
{
	return 1;
}

void AlignModule::slotLanMarkLeft()
{
	leftStyle->initialize();
	left->setStyle(leftStyle);
	leftStyle->setRadis(left->GetActorAt(0));

	leftStyle->setMainActors(left->m_Actor);
	leftStyle->SetDefaultRenderer(left->GetRenderer());
	
}
void AlignModule::slotLanMarkFront()
{
	frontStyle->initialize();
	front->setStyle(frontStyle);
	frontStyle->setRadis(front->GetActorAt(0));

	frontStyle->setMainActors(front->m_Actor);
	frontStyle->SetDefaultRenderer(front->GetRenderer());
}
void AlignModule::slotLanMarkRight()
{
	rightStyle->initialize();
	right->setStyle(rightStyle);
	rightStyle->setRadis(right->GetActorAt(0));

	rightStyle->setMainActors(right->m_Actor);
	rightStyle->SetDefaultRenderer(right->GetRenderer());
}


void AlignModule::slotLeftOrigin()
{
	left->GetInteractor()->SetInteractorStyle(left->m_3DStyle);
}
void AlignModule::slotFrontOrigin()
{
	front->GetInteractor()->SetInteractorStyle(front->m_3DStyle);
}
void AlignModule::slotRightOrigin()
{
	right->GetInteractor()->SetInteractorStyle(right->m_3DStyle);
}



void AlignModule::slotFrontDefault()
{
	frontStyle->Reset();
//	front->GetRenderWindow()->AddRenderer(front->m_Renderer);
	
}
void AlignModule::slotRightDefault()
{
	rightStyle->Reset();
//	right->GetRenderWindow()->AddRenderer(right->m_Renderer);
	
}
void AlignModule::slotLeftDefault()
{
	leftStyle->Reset();
//	left->GetRenderWindow()->AddRenderer(left->m_Renderer);
	
}




void AlignModule::InitializeUi()
{
	ui.setupUi(this);

	//ADD-HHS
	connect(ui.AlignBtn, SIGNAL(clicked()), this, SLOT(slotAlign()));
	connect(ui.LandMarkBtn_1, SIGNAL(clicked()), this, SLOT(slotLanMarkLeft()));
	connect(ui.LandMarkBtn_2, SIGNAL(clicked()), this, SLOT(slotLanMarkFront()));
	connect(ui.LandMarkBtn_3, SIGNAL(clicked()), this, SLOT(slotLanMarkRight()));

	connect(ui.ToOrignBtn_1, SIGNAL(clicked()), this, SLOT(slotLeftDefault()));
	connect(ui.ToOrignBtn_2, SIGNAL(clicked()), this, SLOT(slotFrontDefault()));
	connect(ui.ToOrignBtn_3, SIGNAL(clicked()), this, SLOT(slotRightDefault()));

	connect(ui.DefaultBtn_1, SIGNAL(clicked()), this, SLOT(slotLeftOrigin()));
	connect(ui.DefaultBtn_2, SIGNAL(clicked()), this, SLOT(slotFrontOrigin()));
	connect(ui.DefaultBtn_3, SIGNAL(clicked()), this, SLOT(slotRightOrigin()));
	int sizeX = 0;	int sizeY = 0;
	
	sizeX = this->ui.cadLeft->width();	sizeY = this->ui.cadLeft->height();
	left->ConnectSceneToCtrl(reinterpret_cast<void*>(this->ui.cadLeft->winId()), sizeX, sizeY);
	
	sizeX = this->ui.cadRight->width();	sizeY = this->ui.cadRight->height();
	right->ConnectSceneToCtrl(reinterpret_cast<void*>(this->ui.cadRight->winId()), sizeX, sizeY);

	sizeX = this->ui.cadFront->width();	sizeY = this->ui.cadFront->height();
	front->ConnectSceneToCtrl(reinterpret_cast<void*>(this->ui.cadFront->winId()), sizeX, sizeY);
}

vtkSmartPointer<vtkPolyData> AlignModule::point2mesh(vtkPoints *pts, vtkMatrix4x4 *Mat)
{
	vtkPolyData* poly = vtkPolyData::New();
	vtkCellArray *cell = vtkCellArray::New();

	
	cell->InsertNextCell(pts->GetNumberOfPoints());
	for (int i = 0; i < pts->GetNumberOfPoints(); i++)
	{
		cell->InsertCellPoint(i);
	}

	poly->SetPolys(cell);
	poly->SetPoints(pts);

	cell->Delete();

	vtkSmartPointer<vtkTransform>mat2trans= vtkSmartPointer<vtkTransform>::New();
	
	vtkSmartPointer<vtkTransformPolyDataFilter>trans = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	
	mat2trans->SetMatrix(Mat);

	trans->SetTransform(mat2trans);

	trans->SetInputData(poly);
	trans->Modified();

	poly->DeepCopy(trans->GetOutput());
	return poly;
}
void AlignModule::registration(vtkPolyData *left, vtkPolyData *leftFront, vtkPolyData* rightFront, vtkPolyData* right)
{
	//const int alpha = 10;//If alpha big -> too slow
	//vtkSmartPointer<vtkIterativeClosestPointTransform> IcpleftFront =vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	//IcpleftFront->SetSource(left);
	//IcpleftFront->SetTarget(leftFront);
	//IcpleftFront->GetLandmarkTransform()->SetModeToRigidBody();
	//IcpleftFront->SetMaximumNumberOfIterations(alpha);
	////icp->StartByMatchingCentroidsOn();
	//IcpleftFront->Modified();
	//IcpleftFront->Update();

	//vtkSmartPointer<vtkIterativeClosestPointTransform> IcprightFront = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	//IcprightFront->SetSource(right);
	//IcprightFront->SetTarget(rightFront);
	//IcprightFront->GetLandmarkTransform()->SetModeToRigidBody();
	//IcprightFront->SetMaximumNumberOfIterations(alpha);
	////icp->StartByMatchingCentroidsOn();
	//IcprightFront->Modified();
	//IcprightFront->Update();


	//// Transform the source points by the ICP solution
	//vtkSmartPointer<vtkTransformPolyDataFilter> leftFrontFilter =vtkSmartPointer<vtkTransformPolyDataFilter>::New();

	//leftFrontFilter->SetInputData(resultMesh->GetPolyDataAt(0));
	//leftFrontFilter->SetTransform(IcpleftFront);
	//leftFrontFilter->Update();

	//vtkSmartPointer<vtkTransformPolyDataFilter> rightFrontFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();

	//rightFrontFilter->SetInputData(resultMesh->GetPolyDataAt(2));
	//rightFrontFilter->SetTransform(IcprightFront);
	//rightFrontFilter->Update();



	//resultMesh->GetPolyDataAt(2)->ReleaseData();
	//resultMesh->GetPolyDataAt(2)->Delete();
	//resultMesh->m_PolyData[2] = vtkPolyData::New();
	//resultMesh->GetPolyDataAt(2)->DeepCopy(leftFrontFilter->GetOutput());
	//resultMesh->GetMapperAt(2)->SetInputData(resultMesh->m_PolyData[2]);
	//resultMesh->GetActorAt(2)->Modified();


	//resultMesh->GetPolyDataAt(0)->ReleaseData();
	//resultMesh->GetPolyDataAt(0)->Delete();
	//resultMesh->m_PolyData[0] = vtkPolyData::New();
	//resultMesh->GetPolyDataAt(0)->DeepCopy(rightFrontFilter->GetOutput());
	//resultMesh->GetMapperAt(0)->SetInputData(resultMesh->m_PolyData[0]);
	//resultMesh->GetActorAt(0)->Modified();
	//


	resultMesh->GetRenderWindow()->Render();
	resultMesh->GetRenderWindow()->Start();

}

void AlignModule::slotAlign()
{
	if (left == nullptr || front == nullptr || right == nullptr)
	{
		std::cout << "���� 3�� ����� ����\n";
		return;
	}

	else
	{
		if (!resultMesh)
		{
			resultMesh->ReleaseModel();
			resultMesh->CreateModel("",0);
		}
		std::vector<double3>leftMark;
		std::vector<double3>frontMark;
		std::vector<double3>rightMark;

		leftMark = extractLandMark(left->GetRenderer());
		frontMark = extractLandMark(front->GetRenderer());
		rightMark = extractLandMark(right->GetRenderer());

	
		vtkSmartPointer<vtkPoints>leftPts = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPoints>rightPts = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPoints>frontNleft = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPoints>frontNright = vtkSmartPointer<vtkPoints>::New();

		for (int i = 0; i < leftMark.size(); i++)
			leftPts->InsertNextPoint(leftMark[i].X,leftMark[i].Y,leftMark[i].Z);
		for (int i = 0; i < frontMark.size() / 2; i++)
			frontNleft->InsertNextPoint(frontMark[i].X, frontMark[i].Y, frontMark[i].Z);
		for (int i = frontMark.size() / 2; i < frontMark.size(); i++)
			frontNright->InsertNextPoint(frontMark[i].X, frontMark[i].Y, frontMark[i].Z);
		for (int i = 0; i < rightMark.size(); i++)
			rightPts->InsertNextPoint(rightMark[i].X, rightMark[i].Y, rightMark[i].Z);
		

		vtkSmartPointer<vtkLandmarkTransform>left2front = vtkSmartPointer<vtkLandmarkTransform>::New();
		left2front->SetSourceLandmarks(leftPts);
		left2front->SetTargetLandmarks(frontNleft);
		left2front->SetModeToRigidBody();
		left2front->Update(); //should this be here?


		vtkSmartPointer<vtkLandmarkTransform>right2front = vtkSmartPointer<vtkLandmarkTransform>::New();
		right2front->SetSourceLandmarks(rightPts);
		right2front->SetTargetLandmarks(frontNright);
		right2front->SetModeToRigidBody();
		right2front->Update(); //should this be here?
		

		vtkSmartPointer<vtkAppendPolyData>leftpoly = vtkSmartPointer<vtkAppendPolyData>::New();
		vtkSmartPointer<vtkAppendPolyData>frontpoly = vtkSmartPointer<vtkAppendPolyData>::New();
		vtkSmartPointer<vtkAppendPolyData>rightpoly = vtkSmartPointer<vtkAppendPolyData>::New();
		

		vtkSmartPointer<vtkImageAppend>leftImage= vtkSmartPointer<vtkImageAppend>::New();
		vtkSmartPointer<vtkImageAppend>frontImage = vtkSmartPointer<vtkImageAppend>::New();
		vtkSmartPointer<vtkImageAppend>rightImage = vtkSmartPointer<vtkImageAppend>::New();


		leftImage->SetAppendAxis(1);
		frontImage->SetAppendAxis(1);
		rightImage->SetAppendAxis(1);
		for (int i = 0; i < 5; i++)leftImage->AddInputData(left->GetImageData(i));
		for (int i = 0; i < 5; i++)frontImage->AddInputData(front->GetImageData(i));
		for (int i = 0; i < 5; i++)rightImage->AddInputData(right->GetImageData(i));
		leftImage->Update();
		frontImage->Update();
		rightImage->Update();

		for (int i = 0; i < 5; i++)setTransformedCord(left->GetPolyDataAt(i), left2front);
		for (int i = 0; i < 5; i++)setTransformedCord(front->GetPolyDataAt(i), nullptr);
		for (int i = 0; i < 5; i++)setTransformedCord(right->GetPolyDataAt(i), right2front);



		for (int i = 0; i < 5; i++)leftpoly->AddInputData(left->GetPolyDataAt(i));
		for (int i = 0; i < 5; i++)frontpoly->AddInputData(front->GetPolyDataAt(i));
		for (int i = 0; i < 5; i++)rightpoly->AddInputData(right->GetPolyDataAt(i));
		

		frontpoly->Update();
		rightpoly->Update();
		leftpoly->Update();
		
		vtkSmartPointer<vtkTransformPolyDataFilter> leftFilt = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		leftFilt->SetInputData(leftpoly->GetOutput());
		leftFilt->SetTransform(left2front);
		leftFilt->Update();
		

		vtkSmartPointer<vtkTransformPolyDataFilter>rightFilt = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		rightFilt->SetInputData(rightpoly->GetOutput());
		rightFilt->SetTransform(right2front);
		rightFilt->Update();


		resultMesh->GetPolyDataAt(0)->DeepCopy(leftFilt->GetOutput());
		resultMesh->GetPolyDataAt(1)->DeepCopy(frontpoly->GetOutput());
		resultMesh->GetPolyDataAt(2)->DeepCopy(rightFilt->GetOutput());

		
		std::cout << *left2front->GetMatrix();
		/////////////////////////////////////////////////////////
		/*resultMesh->m_ImageData[0]->DeepCopy(leftImage->GetOutput());
		resultMesh->m_ImageData[0]->Modified();*/

	/*	vtkRenderer *rend = vtkRenderer::New();
		vtkImageActor *act = vtkImageActor::New();
		act->SetInputData(resultMesh->m_ImageData[0]);
		act->Update();

		vtkRenderWindow *win = vtkRenderWindow::New();
		vtkRenderWindowInteractor *it = vtkRenderWindowInteractor::New();
		rend->ResetCamera();
		rend->AddActor(act);

		win->AddRenderer(rend);
		win->Start();
		it->SetRenderWindow(win);
		it->Start();*/
		////////////////////////////////////////////////

		//for (int i = 0; i < 5; i++)setTransformedCord(left->GetPolyDataAt(i), left2front);
		//for (int i = 0; i < 5; i++)setTransformedCord(right->GetPolyDataAt(i), right2front);



		resultMesh->m_ImageData[0]->DeepCopy(leftImage->GetOutput());
		resultMesh->m_ImageData[0]->Modified();
		resultMesh->m_ImageData[1]->DeepCopy(frontImage->GetOutput());
		resultMesh->m_ImageData[1]->Modified();
		//resultMesh->m_ImageData[2]->DeepCopy(rightImage->GetOutput());
		//resultMesh->m_ImageData[2]->Modified();

		
		qweqwe(resultMesh, 0);
		qweqwe(resultMesh, 1);

		for (int i = 0; i < 2; i++)
		{
			resultMesh->GetTextureAt(i)->SetInputData(resultMesh->m_ImageData[i]);
			resultMesh->GetTextureAt(i)->Modified();
			resultMesh->GetActorAt(i)->SetTexture(resultMesh->GetTextureAt(i));
			resultMesh->GetActorAt(i)->Modified();
		}

		resultMesh->GetRenderWindow()->Render();
		resultMesh->GetRenderWindow()->Start();
	}
}

void AlignModule::qweqwe(MeshPreview *test, int index)
{
	if (index != 5)
	{
		vtkSmartPointer<vtkFloatArray>coord = vtkSmartPointer<vtkFloatArray>::New();
		test->GetPolyDataAt(index)->GetPointData()->GetTCoords();
		coord->SetNumberOfComponents(2);

		const int SIZE = test->GetPolyDataAt(index)->GetPointData()->GetTCoords()->GetNumberOfTuples();
		for (int i = 0; i < SIZE; i++)
		{
			double tuple[3];
			tuple[0] = test->GetPolyDataAt(0)->GetPointData()->GetTCoords()->GetComponent(i, 0);
			tuple[1] = test->GetPolyDataAt(0)->GetPointData()->GetTCoords()->GetComponent(i, 1);
			tuple[2] = 0.0;
			tuple[1] = tuple[1] * 0.25;

			if (i < 2 * SIZE / 4)
				tuple[1] += 0.25;
			else if (i < 3 * SIZE / 4)
				tuple[1] += 0.50;
			else //if (i < 4 * SIZE / 4)
				tuple[1] += 0.75;

			coord->InsertNextTuple(tuple);
		}

		coord->Modified();
		test->GetPolyDataAt(index)->GetPointData()->SetTCoords(coord);;
	}


	else
	{
		

	}
}


void AlignModule::setTransformedCord(vtkPolyData *poly, vtkLandmarkTransform *land)
{
	if (land != nullptr) {
		vtkSmartPointer<vtkTransform>trans = vtkSmartPointer<vtkTransform>::New();
		trans->SetMatrix(land->GetMatrix());

	
		vtkSmartPointer<vtkTransformTextureCoords> transCoord = vtkSmartPointer<vtkTransformTextureCoords>::New();
		transCoord->SetInputData(poly);

		transCoord->SetPosition(trans->GetOrientation());
		transCoord->Update();

		poly->DeepCopy(transCoord->GetOutput());
		//<--���⼭ coord * 4�ؾߵǳ�
		//poly->GetPointData()->SetTCoords(transCoord->GetPolyDataOutput()->GetPointData()->GetTCoords());
	}
}


std::vector<double3> AlignModule::extractLandMark(vtkRenderer *rend)
{
	std::vector<double3>vec;
	if (rend == NULL)return vec;

	vtkActorCollection * actorCol = rend->GetActors();
	actorCol->InitTraversal();

	for (vtkIdType i = 0; i < actorCol->GetNumberOfItems(); i++)
	{
		vtkActor *nextActor = actorCol->GetNextActor();
		
		if (i >= 5)//������ 0~4�� �׸��� �׸�
		{
			double *pos = nextActor->GetCenter();
			vec.push_back(double3(pos[0], pos[1], pos[2]));
		}
	}

	return vec;

}


void AlignModule::Copy(MeshPreview* src, MeshPreview* des)
{
	if (des != NULL)
	{
		des->ReleaseModel();
	}

	//des->CreateTexture("", 0);
	des->CreateModel("", 0);

	
	for (int i = 0; i < 5; i++)
	{
		des->m_PolyData[i]->DeepCopy(src->GetPolyDataAt(i));
		des->m_ImageData[i]->DeepCopy(src->m_ImageData[i]);
		des->m_ImageData[i]->Modified();
		des->m_Texture[i]->SetInputData(des->m_ImageData[i]);
		des->GetActorAt(i)->SetTexture(des->m_Texture[i]);
		des->GetTextureAt(i)->Update(); 
		des->GetActorAt(i)->Modified();
	}

	des->GetRenderer()->Modified();
	des->GetRenderWindow()->Render();
}


void AlignModule::setRight(MeshPreview *rend)
{
	Copy(rend, right);
	rightStyle->setRadis(right->GetActorAt(0));
}

void AlignModule::setLeft(MeshPreview *rend)
{
	//front = rend;
	//left = rend;
	Copy(rend, left);
	leftStyle->setRadis(left->GetActorAt(0));
}

void AlignModule::setFront(MeshPreview *rend)
{
	//front = rend;
	//left = rend;
	//frontStyle->setRadis(rend->GetActorAt(0));
	Copy(rend, front);
	frontStyle->setRadis(front->GetActorAt(0));
}