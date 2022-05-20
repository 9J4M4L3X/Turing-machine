#include "Tm.h"

void Tm::ucitajProgram(const string& program)
{
	ifstream fajl;
	fajl.open(program);
	if (fajl) {
		string linija;
		while (getline(fajl, linija)) {
			string stanje0, stanje1;
			char vr0, vr1;
			Pomeraj pom;
			int i = 0;

			while (linija[i] != '(') i++;
			i++;
			while (linija[i] == ' ') i++;

			while (linija[i] != ',') {
				stanje0.push_back(linija[i]);
				i++;
			}
			stanje0.push_back('\0');
			i++;

			while (linija[i] == ' ') i++;
			vr0 = linija[i];

			while (linija[i] != '(') i++;
			i++;

			while (linija[i] == ' ') i++;
			while (linija[i] != ',') {
				stanje1.push_back(linija[i]);
				i++;
			}
			stanje1.push_back('\0');
			i++;

			while (linija[i] == ' ') {
				i++;
			}
			vr1 = linija[i];
			i += 2;
			while (linija[i] == ' ') {
				i++;
			}
			int pomeraj = 1;
			if(linija[i] == '+') pomeraj = linija[i + 1] - '0';
			if(linija[i] == '-') pomeraj = -(linija[i + 1] - '0');
			if(linija[i] != '+' && linija[i] != '-') pomeraj = linija[i] - '0';

			if (pomeraj > 0) pom = Pomeraj::desno;
			else pom = Pomeraj::levo;

			Naredba* nova_naredba = new Naredba();
			nova_naredba->postaviTV(vr0);
			nova_naredba->postaviSV(vr1);
			nova_naredba->postaviTS(stanje0);
			nova_naredba->postaviSS(stanje1);
			nova_naredba->postaviPomeraj(pom);

			naredbe.push_back(nova_naredba);
		}
		fajl.close();
	}
	else {
		cout << "Fajl sa programom ne moze da se otvori!";
		exit(1);
	}
}
void Tm::ucitajTraku(const string& tape)
{
	ifstream f1;
	f1.open(tape);
	if (f1) {
		int flag = 0, i = 0;
		char b;
		while (f1 >> b) {
			if ((i % 2) == 0) {
				traka_.push_back(b); if (!flag && (b == 'b')) indeks_++;
				if (!flag && (b != 'b')) { this->postaviVrednost(b); flag = 1; }
			}
			i++;
		}
		f1.close();
	}
	else {
		cout << "Fajl sa trakom ne moze da se otvori!";
		exit(1);
	}
}
void Tm::ispisiTraku()
{
	cout << "\nUcitana traka:\n";
	for (vector<Karakter>::iterator it = traka_.begin(); it != traka_.end(); it++) cout << "| "<< *it << " |";
	cout << '\n';
}
std::list<Karakter>::iterator Tm::apdejtujIndeks()
{
	std::list<Karakter>::iterator it = nova_traka_.begin();
	int indeks = indeks_;
	while (indeks > 0 && it != nova_traka_.end()) {
		it++; indeks--;
	}
	return it;
}
void Tm::ispisiNovuTraku() {
	cout << "\nNova traka:\n";
	for (std::list<Karakter>::iterator iter = nova_traka_.begin(); iter != nova_traka_.end(); iter++) {
		cout << "| " << *iter << " |";
	}
	cout << '\n';
	
	ofstream izlaz;
	izlaz.open("Izlaz.txt");
	for (std::list<Karakter>::iterator iter = nova_traka_.begin(); iter != nova_traka_.end(); iter++) {
		izlaz << "| " << *iter << " |";
	}
	izlaz.close();
}
bool Tm::izvrsiNaredbu()
{
	int flag = 0;

	if (!broj_inic) {//ako nije inicijalizovao nova_traka_ onda udje u if
		for (std::vector<Karakter>::iterator it = traka_.begin(); it != traka_.end(); it++)
			nova_traka_.push_back(*it);
		broj_inic++;
	}

	std::list<Karakter>::iterator iter = this->apdejtujIndeks();
	if (iter == nova_traka_.end()) {
		flag = 0;
		return flag;
	}
	vrednost_stanja_ = *iter; //trenutna vrednost na koju pokazuje TM

	for (std::vector<Naredba*>::iterator it = naredbe.begin(); it != naredbe.end(); it++) {
		string tr_st = (*it)->vratiTrStanje();//trenutno stanje naredbe
		char tr_vr = (*it)->vratiTrVred();//vrednost stanja naredbe

		if ((strcmp(trenutno_stanje_.c_str(), tr_st.c_str()) == 0) && (tr_vr == vrednost_stanja_)) {
			trenutno_stanje_ = (*it)->vratiSlStanje();

			*iter = (*it)->vratiSlVred();

			if ((*it)->vratiPomeraj() == Pomeraj::desno) { 
				indeks_++;
			}
			else {
				indeks_--;
			}

			flag = 1; 
			if (strcmp((*it)->vratiSlStanje().c_str(), "q+") == 0) {
				cout << "\nZavrsio sa pozitivnim ishodom!\n";
				flag = 0;
			}
			if (strcmp((*it)->vratiSlStanje().c_str(), "q-") == 0) {
				cout << "\nZavrsio sa negativnim ishodom!\n";
				flag = 0;
			}

			break; //ako smo pronasli naredbu izlazi se iz for posle obrade
		}	
	}
	return flag;
}
void Tm::pokreniSimulaciju()
{
	cout << "Unesite ime trake koju zelite da unesete(ne treba .txt da se dodaje):\n";
	string ime_trake;
	cin >> ime_trake;
	this->ucitajTraku(ime_trake + ".txt");

	this->ispisiTraku();

	cout << "Unesite ime programa koji se izvrsava(ne treba .txt da se dodaje):\n";
	string ime_programa;
	cin >> ime_programa;
	this->ucitajProgram(ime_programa + ".txt");

	while (this->izvrsiNaredbu());

	this->ispisiNovuTraku();
}





