#include <iostream>;
#include <fstream>;
#include <string>;

using namespace std;

unsigned short age = 1, population = 100, wheat = 2800, acres = 1000, wheatForPeople = 0, wheatForSow = 0;
bool isCorrect = false;
string input = "";

void baseReport() {
	cout << "Год: " << age << endl;
	cout << "Кол-во населения: " << population << endl;
	cout << "Кол-во пшеницы: " << wheat << endl;
	cout << "Кол-во акров для посевов: " << acres << endl;
}

void toDo() {
	short acreCost = rand() % 10 + 17;
	while (!isCorrect)
	{
		try {
			cout << "Акров к покупке/продаже (стоимость акра " << acreCost << "): ";
			cin >> input;
			if (stoi(input) == 0) {
				isCorrect = true;
				break;
			}

			if (input[0] == '-') {
				if (abs(stoi(input)) > acres) {
					cout << "У вас не хватает акров" << endl;
					break;
				}
			}

			short cost = acreCost * stoi(input);
			if (cost > wheat) {
				cout << "У вас не хватает пшеницы" << endl;
				break;
			}
			wheat -= cost;

			acres += stoi(input);
			isCorrect = true;
		}
		catch (exception e) {
			cout << "Ошибка ввода или неожиданный результат" << endl;
		}

	}
	input = "";
	isCorrect = false;

	while (!isCorrect)
	{
		try {
			cout << "Пшеницы горожанам: ";
			cin >> input;
			if (stoi(input) == 0) {
				isCorrect = true;
				break;
			}

			if (input[0] == '-') {
				cout << "Вы не можете отобрать у горожан то, чего у них нет" << endl;
				break;
			} else if (stoi(input) > wheat) {
				cout << "У вас не хватает пшеницы" << endl;
				break;
			}
			wheatForPeople += stoi(input);
			wheat -= stoi(input);
			isCorrect = true;
		}
		catch (exception e) {
			cout << "Ошибка ввода или неожиданный результат" << endl;
		}
	}
	input = "";
	isCorrect = false;

	while (!isCorrect)
	{
		try {
			cout << "Пшеницы на посев(0.5): ";
			cin >> input;
			if (stoi(input) == 0) {
				isCorrect = true;
				break;
			}

			if (input[0] == '-') {
				cout << "в земле нет пшеницы" << endl;
				break;
			}
			else if (stoi(input) > wheat) {
				cout << "У вас не хватает пшеницы" << endl;
				break;
			}
			wheatForSow += stoi(input);
			wheat -= stoi(input);
			isCorrect = true;
		}
		catch (exception e) {
			cout << "Ошибка ввода или неожиданный результат" << endl;
		}
	}
	input = "";
	isCorrect = false;
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	while (age<10) {
		cout << endl;

		baseReport();
		toDo();
		age++;
	}
	return 0;
}