#pragma once
#include <iostream>
class notImplemented_error : public std::logic_error
{
public:
	notImplemented_error() : std::logic_error("����� ���� �������� �ʾҽ��ϴ�.") { };
};