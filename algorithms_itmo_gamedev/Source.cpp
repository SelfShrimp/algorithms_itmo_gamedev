#include <iostream>;
#include <fstream>;
#include <string>;

using namespace std;

unsigned short age = 1, population = 100, wheat = 2800, acres = 1000, wheatForPeople = 0, wheatForSow = 0, wheatPick = 0, wheatPerAcre = 1;
short death = 0;
float aveDeath = 0;
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
	wheatForPeople = 0;
	wheatForSow = 0;

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
					continue;
				}
			}

			short cost = acreCost * stoi(input);
			if (cost > wheat) {
				cout << "У вас не хватает пшеницы" << endl;
				continue;
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
				continue;
			}

			if (input[0] == '-') {
				cout << "Вы не можете отобрать у горожан то, чего у них нет" << endl;
				continue;
			} else if (stoi(input) > wheat) {
				cout << "У вас не хватает пшеницы" << endl;
				continue;
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
				continue;
			}
			else if (stoi(input) > wheat) {
				cout << "У вас не хватает пшеницы" << endl;
				continue;
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
	cout << endl;
}

void resultPerYear() {
	death = 0;
	wheatPerAcre = rand() % 6 + 1;
	wheatPick = 0;
	if (wheatForSow * 2 > acres) {
		cout << "вы выделили пшеницы больше, чем можете посадить, часть пшеницы пропала" << endl;
		wheatPick += wheatPerAcre * acres;
	}
	else {
		wheatPick += wheatPerAcre * wheatForSow * 2;
	}
	cout << "Мы собрали " << wheatPick << " пшеницы"<<endl;
	wheat += wheatPick;
	short wheatTaxForRat = (float)(rand() % 8) / 100 * wheat;
	cout << "Крысы съели "<< wheatTaxForRat<< " пшеницы" << endl;
	death = population - wheatForPeople / 20;
	if (death < 0) death = 0;
	aveDeath += (float) death / population;
	population -= death;
	cout << "Погибло: " << death << endl;
}

void newPeople() {
	short people = death / 2 + (5 - wheatPerAcre) * wheat / 600 + 1;
	if (people < 0) { people = 0; }
	else if (people > 50) { people = 50; }
	cout << "Прибыло: " << people << endl;
	population += people;
}

void doom() {
	float plagueChance = (static_cast<float>(rand() % 100)) / (static_cast<float>(100));
	if (plagueChance <= 0.15)
	{
		population *= 0.5;
		cout << "Случилась чума, население сократилось вдвое" << endl;
	}
	else
		cout << "Чума обошла нас стороной в этом году" << endl;
}

void gameFinsihed() {
	cout << "Вы продержались 10 лет!"<<endl;
	aveDeath /= 10;
	cout << "Средняя смертность: " << aveDeath << endl;
	short acresPerPeople = acres / population;
	cout << "Акров на жителя: " << acresPerPeople << endl;
	if (aveDeath > 0.33 && acresPerPeople < 7.0) cout << "Плохо";
	else if (aveDeath > 0.1 && acresPerPeople < 9.0) cout << "Нормально";
	else if (aveDeath > 0.03 && acresPerPeople < 10.0) cout << "Хорошо";
	else cout << "Отлично";
	cout << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	while (age<10) {
		cout << endl;

		baseReport();
		toDo();
		resultPerYear();
		if ((float)death / population >= 0.45f || population <= 0) {
			cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.Теперь вы вынуждены влачить жалкое существование в изгнании" << endl;
			while(true){}
			return 0;
		}
		newPeople();
		doom();
		age++;
	}
	gameFinsihed();
	while(true){}
	return 0;
}