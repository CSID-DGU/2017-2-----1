#pragma warning (disable : 4996)

#include "iomodule.h"
#include "InitializeModule.hpp" //�ʱ�ȭ �κ� ��� �� ui include
#include "MainModule.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	_CrtDumpMemoryLeaks();
	//�ʱ�ȭ ����..
	InitializeModule i;
	i.show();
	MainModule m;
	//m.setSetting(i.init());
	i.Init();
	i.close();
	//�ʱ�ȭ ��!
	m.show();


	//���������� ����
	//IOModule w;
	//w.show();
	//���������� ��
	return a.exec();
}
