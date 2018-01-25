#pragma once
#include<LandMarkInteractorStyle.h>

vtkStandardNewMacro(LandMarkInteractorStyle);

void LandMarkInteractorStyle::OnLeftButtonUp()
{
	
	int* clickPos = this->GetInteractor()->GetEventPosition();
	picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
	

	if(LastPickedActor !=NULL && mainActor!=NULL && LastPickedActor!=mainActor)
	{
		vtkMapper *mapperMove = LastPickedActor->GetMapper();
		vtkSphereSource *sp = vtkSphereSource::New();

		double* pos = picker->GetPickPosition();
		this->GetDefaultRenderer()->RemoveActor(LastPickedActor);
		sp->SetCenter(pos[0], pos[1], pos[2]);
		sp->SetRadius(radius);
		mapperMove->SetInputConnection(sp->GetOutputPort());

		LastPickedActor->SetMapper(mapperMove);
		this->GetDefaultRenderer()->AddActor(LastPickedActor);
		this->Modified();

	}

	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
	this->SetMotionFactor(1);
}

void LandMarkInteractorStyle::Delete()
{
	vtkActorCollection * actorCol = NULL;

	if (this->GetDefaultRenderer())
		actorCol = this->GetDefaultRenderer()->GetActors();


	if (actorCol)
	{
		actorCol->InitTraversal();
		const int loopSIZE = actorCol->GetNumberOfItems();
		vtkActor *nextActor;

		for (vtkIdType i = 0; i<loopSIZE; i++)
		{
			nextActor = actorCol->GetNextActor();
			std::cout << i;
			if (nextActor != mainActor)
			{
				this->GetDefaultRenderer()->RemoveActor(nextActor);
				nextActor->GetMapper()->Delete();
				nextActor->Delete();
				nextActor = NULL;
			}
		}
	}


	if (LastPickedProperty) 
	{ 
		LastPickedProperty->Delete(); 
		LastPickedProperty = NULL;
	}
	if (LastPickedActor) 
	{
		LastPickedActor->Delete(); 
		LastPickedActor = NULL;
	}
	if (picker) 
	{
		picker->Delete();
		picker = NULL;
	}

	if (this->GetDefaultRenderer() != NULL)
		this->GetDefaultRenderer()->Render();
}

/**�ٽ� load�ϱ� ���Ͽ� ������� Reset �Ѵ�. */
void LandMarkInteractorStyle::Reset()
{
	vtkActorCollection * actorCol = NULL;

	if (this->GetDefaultRenderer())
		actorCol = this->GetDefaultRenderer()->GetActors();

	
	if (actorCol)
	{
		actorCol->InitTraversal();
		const int loopSIZE = actorCol->GetNumberOfItems();
		vtkActor *nextActor;
		
		for (vtkIdType i = 0;i<loopSIZE; i++)
		{
			nextActor = actorCol->GetNextActor();
			std::cout << i;
			if (nextActor != mainActor)
			{
				this->GetDefaultRenderer()->RemoveActor(nextActor);
				nextActor->GetMapper()->Delete();
				nextActor->Delete();
				nextActor = NULL;
			}
		}
	}

	if (LastPickedActor) 
	{ 
		LastPickedActor->Delete();
		LastPickedActor = NULL; 
	}

	if (this->GetDefaultRenderer() != NULL)
	{
		this->GetDefaultRenderer()->Render();
	}
	
}

std::vector<double3> LandMarkInteractorStyle::getData()
{
	std::vector<double3> vec;
	if (this->GetDefaultRenderer() == NULL)return vec;

	vtkActorCollection * actorCol=this->GetDefaultRenderer()->GetActors();
	actorCol->InitTraversal();

	for (vtkIdType i = 0; i < actorCol->GetNumberOfItems(); i++)
	{
		vtkActor *nextActor = actorCol->GetNextActor();
		double *pos = nextActor->GetCenter();
		std::cout << pos[0]<<" "<<pos[1]<<" "<<pos[2] << "\n";

		vec.push_back(double3(pos[0], pos[1], pos[2]));
	}

	return vec;
}
void LandMarkInteractorStyle::initialize()
{
	LastPickedActor = NULL;
	LastPickedProperty = vtkProperty::New();
}

void LandMarkInteractorStyle::OnLeftButtonDown()
{
	int* clickPos = this->GetInteractor()->GetEventPosition();
	
	picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
	
	vtkActor* temp=picker->GetActor();
	double* pos = picker->GetPickPosition();
	
	int mode = 2;

	if (temp == mainActor)mode = 2;
	else mode = 1;
	
	if (mode==1) //��ĥ�ϱ� 
	{
		if (LastPickedActor)this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
	
		this->LastPickedActor = picker->GetActor();
		if (this->LastPickedActor)
		{
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
			this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
			this->LastPickedActor->GetProperty()->SetSpecular(0.0);
		}
	}

	else if (mode==2) //�׸���
	{
		
		vtkSphereSource *sp = vtkSphereSource::New();
		sp->SetCenter(pos[0], pos[1], pos[2]);
		sp->SetRadius(radius);
		
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();

		mapper->SetInputConnection(sp->GetOutputPort());

		vtkActor* actor = vtkActor::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(0, 0, 1);

		if (pos[0] == 0 || pos[1] == 0 || pos[2] == 0)
		{
			sp->Delete();
			mapper->Delete();
			actor->Delete();
		}
		else this->GetDefaultRenderer()->AddActor(actor);//Rendering
	}
	
	this->Modified();
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	this->SetMotionFactor(0);
}

void LandMarkInteractorStyle::OnKeyPress()
{
	std::string PressedKey = this->Interactor->GetKeySym();

	if (PressedKey == "Delete" && LastPickedActor!=NULL)//LastPickedActor�� �����ȴ�. 
	{
		//vtkPolyData* data=LastPickedActor->GetMapper()->GetInput()->deepCopy;
		LastPickedActor->GetMapper()->Delete();
		this->GetDefaultRenderer()->RemoveActor(LastPickedActor);
		LastPickedActor->Delete();
		this->GetDefaultRenderer()->Render();
		LastPickedActor = NULL;
	}

	this->GetDefaultRenderer()->GetRenderWindow()->Render();
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}