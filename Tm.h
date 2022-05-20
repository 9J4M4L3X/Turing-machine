#ifndef TM_H_
#define TM_H_

#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<list>

using namespace std;

enum Pomeraj{levo, desno};
typedef char Karakter;

class Naredba {
public:
    Naredba(): pomeraj_(Pomeraj::desno), tr_vred_('b'), sled_vred_('b'){}
    string vratiTrStanje() { return tr_stanje_; }
    char vratiTrVred() { return tr_vred_; }
    string vratiSlStanje() { return sled_stanje_; }
    char vratiSlVred() { return sled_vred_; }
    Pomeraj vratiPomeraj() { return pomeraj_; }

    void postaviTS(string ts) { tr_stanje_ = ts; }
    void postaviSS(string ss) { sled_stanje_ = ss; }
    void postaviTV(char tv) { tr_vred_ = tv; }
    void postaviSV(char sv) { sled_vred_ = sv; }
    void postaviPomeraj(Pomeraj pomeraj) { pomeraj_ = pomeraj; }

private:
    string tr_stanje_, sled_stanje_;
    char tr_vred_, sled_vred_;
    Pomeraj pomeraj_;
};

class Tm {
public:
    Tm(): trenutno_stanje_("q0"), indeks_(0), vrednost_stanja_('b') {}
    
    
    void ucitajProgram(const string&);
    void ucitajTraku(const string&);
    void ispisiTraku();
    bool izvrsiNaredbu();
    void ispisiNovuTraku();

    void pokreniSimulaciju();

	std::list<Karakter>::iterator apdejtujIndeks();
    void postaviVrednost(char vr) { vrednost_stanja_ = vr; }
    string trenStanje() { return trenutno_stanje_; }
    char vredStanja() { return vrednost_stanja_; }
    vector<Karakter> vratiTraku() { return traka_; }
private:
    string trenutno_stanje_;//trenutno stanje TM
    char vrednost_stanja_;//trenutna vrednost koju ocitava TM
    vector<Karakter> traka_;//ucitana traka
    list<Karakter> nova_traka_;//traka nakon izvrsavanja programa
    vector<Naredba*> naredbe;//niz naredbi koje se ucitavaju iz fajla
    int indeks_, broj_inic = 0;
};

#endif