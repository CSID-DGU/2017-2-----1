#pragma warning (disable : 4996)

#include "iomodule.h"
#include "InitializeModule.hpp" //�ʱ�ȭ �κ� ��� �� ui include
#include "MainModule.hpp"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	_CrtDumpMemoryLeaks();
	
	//Program Init Process
	InitializeModule i;
	i.show();
	/*if (i.Init() == false) {
		exit(-1);
	}*/
	i.close();

	//Run main module after initialize
	MainModule m;
	m.show();

	return a.exec();
}
