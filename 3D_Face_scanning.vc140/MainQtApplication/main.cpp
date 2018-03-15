#pragma warning (disable : 4996)

#include "iomodule.h"
#include "InitializeModule.hpp" //초기화 부분 모듈 및 ui include
#include "MainModule.hpp"
#include <QtWidgets/QApplication>

VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//초기화 진행..
	InitializeModule i;
	i.show();
	MainModule m;
	//m.setSetting(i.init());
	i.Init();
	i.close();
	//초기화 끝!
	m.show();


	//메인윈도우 진행
	//IOModule w;
	//w.show();
	//메인윈도우 끝
	return a.exec();
}