#include <iostream>;
#include <fstream>;

using namespace std;

unsigned short age = 1, population = 100, wheat = 2800, acres = 1000;

void baseReport() {
	cout << "Год: " << age << endl;
	cout << "Кол-во населения: " << population << endl;
	cout << "Кол-во пшеницы: " << wheat << endl;
	cout << "Кол-во акров для посевов: " << acres << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	baseReport();
	while(true){}
	return 0;
}