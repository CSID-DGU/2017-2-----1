#include <fstream>
#include <iostream>
int main() {

	std::ifstream fin; // Ŭ���� ��ü
	fin.open("./resulttmp.txt"); // ���ϰ�θ� ������ ���� �ִ�
	double tmp;
	fin >> tmp;
	std::cout << tmp;
}