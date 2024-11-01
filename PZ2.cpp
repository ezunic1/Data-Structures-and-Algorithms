#include <iostream>
#include <stdexcept>


template <typename tip>
class Lista{
    public:
    Lista(){}
    virtual ~Lista(){}
    virtual int brojElemenata() const = 0;
    virtual tip &trenutni()const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0; 
    virtual void obrisi() = 0;
    virtual void dodajIspred(const tip& el) = 0;
    virtual void dodajIza(const tip &el) = 0;
    virtual tip& operator[](int i)const = 0; 
    //virtual tip operator[](int i) = 0; 
};

template <typename tip>
class NizLista : public Lista<tip>{
    int max_broj = 10;
    int broj = 0;
    int sad = 0;
    tip ** niz;
    void prosirivanje(){
        tip **novi;
        novi = new tip *[2*max_broj]{};
        for(int i = 0; i < max_broj; i++){
            novi[i] = new tip (*niz[i]);
        }
        max_broj*=2;
        for(int i = 0; i < broj; i++){
            delete niz[i];
        }
        delete []niz;
        niz = novi;
       // max_broj*=2;
        novi = nullptr;
    }
    public:
    NizLista();
    NizLista(const NizLista <tip> &n);
    NizLista &operator=(const NizLista<tip> &n);
	~NizLista();
    int brojElemenata() const override{return broj;}
    tip &trenutni()const override;
    bool prethodni() override;
    bool sljedeci() override;
    void kraj()override;
    void pocetak()override;
    void obrisi() override;
    void dodajIspred(const tip& el) override;
    void dodajIza(const tip& el) override;
    tip & operator[](int i)const override;
    //tip operator[](int i) override;
    void ispisi()const;
};

template<typename tip>
void NizLista<tip>::ispisi()const{
    for(int i = 0; i < broj; i++){
        std::cout << *niz[i] << " ";
    }
    std::cout << std::endl;
}

template<typename tip>
NizLista<tip>::NizLista(){
    max_broj = 10;
    broj = 0;
    sad = 0;
    niz = new tip*[10]{};
}

template<typename tip>
NizLista<tip>::NizLista(const NizLista<tip> &n) : max_broj(n.max_broj), broj(n.broj), sad(n.sad), niz(new tip*[n.max_broj]{}){
    try{
        for(int i = 0; i < n.broj; i++){
            niz[i] = new tip(*n.niz[i]);
        }
    }catch(...){
        for(int i = 0; i < n.broj; i++){
            delete niz[i];
        }
        delete []niz;
        throw;
    }

}

template <typename tip>
NizLista<tip> &NizLista<tip>::operator=(const NizLista<tip> &n){
    if(&n !=this){
        for(int i = 0; i < broj; i++){
            delete niz[i];
        }
        delete [] niz;
        broj = n.broj;
        max_broj = n.max_broj;
        sad = n.sad;
        niz = new tip*[n.max_broj]{};
        for(int i = 0; i < broj; i++){
            niz[i] = new tip(*n.niz[i]);
        }
    }
    return *this;
}

template<typename tip>
NizLista<tip>::~NizLista(){
    for(int i = 0; i < broj; i++){
        delete niz[i];
    }
    delete [] niz;
    niz = nullptr;
}

template<typename tip>
tip& NizLista<tip>::trenutni()const {
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    return *niz[sad];
}

template<typename tip>
bool NizLista<tip>::prethodni(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    if(sad == 0) return false;
    sad--;
    return true;
}

template<typename tip>
bool NizLista<tip>::sljedeci(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    if(sad == broj-1) return false;
    sad++;
    return true;
}

template<typename tip>
void NizLista<tip>::kraj(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    sad = broj-1;
}

template<typename tip>
void NizLista<tip>::pocetak(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    sad = 0;
}

template<typename tip>
void NizLista<tip>::obrisi(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    delete niz[sad];
    if(sad == broj - 1 && sad != 0)sad--;
    else{
        for(int i = sad; i < broj-1; i++)
            niz[i] = niz[i+1];
        niz[broj-1] = nullptr;
    }
    broj--;
}

template <typename tip>
void NizLista<tip>::dodajIza(const tip& el){
    if(broj < max_broj){
        if(broj == 0) niz[broj++] = new tip(el);
        else{
            for(int i = broj; i > sad; i--)
                niz[i] = niz[i-1];
            niz[sad+1] = new tip(el);
            broj++;
        }
    }else{
        prosirivanje();
        for(int i = broj; i > sad; i--)
            niz[i] = niz[i-1];
        niz[sad+1] = new tip(el);
        broj++;
    }
}

template <typename tip>
void NizLista<tip>::dodajIspred(const tip& el){
    if(broj < max_broj){
        if(broj == 0) niz[broj++] = new tip(el);
        else{
            for(int i = broj; i > sad; i--)
                niz[i] = niz[i-1];
            niz[sad] = new tip(el);
            broj++;
            sad++;
        }
    }else{
        prosirivanje();
        for(int i = broj; i > sad; i--)
            niz[i] = niz[i-1];
        niz[sad] = new tip(el);
        broj++;
        sad++;
    }
}

template <typename tip>
tip &NizLista<tip>::operator[](int i)const{
    if(i < 0 || i >= broj) throw std::logic_error("Ne postoji element sa tim indeksom.");
    return *(niz[i]);
}

template<typename tip>
class JednostrukaLista : public Lista<tip>{
    int broj;
    struct Cvor{
        tip el;
        Cvor * sljedeci;
        Cvor(const tip &el, Cvor *sljedeci) : el(el), sljedeci(sljedeci){}
    };
    Cvor *prvi, *zadnji, *sad;
    public:
    JednostrukaLista();
    JednostrukaLista(const JednostrukaLista <tip> &n);
    JednostrukaLista &operator=(const JednostrukaLista<tip> &n);
	~JednostrukaLista();
    int brojElemenata() const override{return broj;}
    tip &trenutni() const override;
    bool prethodni() override;
    bool sljedeci() override;
    void kraj()override;
    void pocetak()override;
    void obrisi() override;
    void dodajIspred(const tip& el) override;
    void dodajIza(const tip& el) override;
    tip & operator[](int i) const override;
    void ispisi()const;
};

template<typename tip>
void JednostrukaLista<tip>::ispisi()const{
    std::cout << std::endl;
    for(Cvor * novi = prvi; novi !=nullptr; novi = novi->sljedeci){
        std::cout << novi->el << "  ";
    }
    std::cout << std::endl;
}

template<typename tip>
JednostrukaLista<tip>::JednostrukaLista(){
    prvi = nullptr;
    zadnji = nullptr;
    sad = nullptr;
    broj = 0;
}

template<typename tip>
JednostrukaLista<tip>::JednostrukaLista(const JednostrukaLista &n){
    prvi = nullptr;
    zadnji = nullptr;
    sad = nullptr;
    broj = 0;
    for(int i = 0; i < n.broj; i++){
        dodajIza(n[i]);
        sljedeci();
    }
}
template <typename tip>
JednostrukaLista<tip> & JednostrukaLista<tip>::operator=(const JednostrukaLista<tip> &n){
    if(&n !=this){
        while(prvi !=zadnji){
            Cvor *novi = prvi ->sljedeci;
            delete prvi;
            prvi = novi;
        }
        delete prvi;
        prvi = nullptr;
        zadnji = nullptr;
        sad = nullptr;
        broj = 0;
        for(int i = 0; i < n.broj; i++){
            dodajIza(n[i]);
        }
    }
    return *this;
}


template<typename tip>
JednostrukaLista<tip>::~JednostrukaLista(){
    while(prvi !=zadnji){
        Cvor *novi = prvi->sljedeci;
        delete prvi;
        prvi = novi;
    }
    delete prvi;
    prvi = nullptr;
    zadnji = nullptr;
    sad = nullptr;
    broj = 0;
}

template<typename tip>
tip & JednostrukaLista<tip>::trenutni()const{
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    return sad->el;
}

template<typename tip>
bool JednostrukaLista<tip>::prethodni(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    if(sad == prvi) return false;
    Cvor *novi = prvi;
    while(novi->sljedeci !=sad) novi = novi->sljedeci;
    sad = novi;
    return true;
}

template<typename tip>
bool JednostrukaLista<tip>::sljedeci(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    if(sad == zadnji) return false;
    sad = sad->sljedeci;
    return true;
}

template<typename tip>
void JednostrukaLista<tip>::pocetak(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    sad = prvi;
}
template<typename tip>
void JednostrukaLista<tip>::kraj(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    sad = zadnji;
}

template <typename tip>
void JednostrukaLista<tip>::obrisi(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    Cvor * novi = nullptr;
    if(sad == zadnji){
        novi = prvi;
        if(sad == prvi){
            delete sad;
            novi = nullptr;
            prvi = nullptr;
            zadnji = nullptr;
            sad = nullptr;
        }else{
            while(novi->sljedeci !=sad)novi = novi->sljedeci;
            novi->sljedeci = nullptr;
            delete sad;
            sad = novi;
            zadnji = novi;
        }
    }else if(sad == prvi){
        novi = sad;
        sad = sad->sljedeci;
        prvi = sad;
        delete novi;
    }else{
        novi = prvi;
        while(novi->sljedeci != sad) novi = novi->sljedeci; ///////////////////////////////////////////////////////////////
        novi->sljedeci = sad->sljedeci;
        delete sad;
        sad = novi->sljedeci;
    }
    broj--;
}
template<typename tip>
void JednostrukaLista<tip>::dodajIspred(const tip &el){
    Cvor * novi = new Cvor{el,nullptr};
    if(broj == 0){
        prvi = novi;
        zadnji = prvi;
        sad = prvi;
    }else if(prvi == sad){
        novi->sljedeci = sad;
        prvi = novi;
    }else{
        Cvor * pom = prvi;
        while(pom->sljedeci !=sad) pom = pom->sljedeci;
        pom->sljedeci = novi;
        novi->sljedeci = sad;  
    }
    broj++;
}

template<typename tip>
void JednostrukaLista<tip>::dodajIza(const tip &el){
    Cvor *novi = new Cvor{el, nullptr};
    if(prvi == nullptr){
        prvi = novi;
        zadnji = prvi;
        sad = prvi;
    }else{
        if(sad == zadnji) zadnji = novi;
        Cvor *pom = sad->sljedeci;
        //while(pom!=sad) pom = pom->sljedeci;
        sad->sljedeci = novi;
        novi->sljedeci = pom;
    }
    broj++;
}

template <typename tip>
tip & JednostrukaLista<tip>::operator[](int i)const{
    if(i < 0 || i >= broj) throw std::logic_error("Ne postoji element sa tim indeksom.");
    int pom = 0;
    for(Cvor * novi = prvi; novi !=nullptr;novi = novi->sljedeci){
        if(pom == i)return novi->el;
        pom++;
    }
    return prvi->el;
   /* int pom = 0;
    Cvor * novi = prvi;
    while(pom != i){
        novi = novi->sljedeci;
        pom++;
    }
    return novi->el;*/
}



//TESTOVI

void TestVelicine(){
    NizLista<int> n;
    if(n.brojElemenata() == 0)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}
void TestVelicine2(){
    JednostrukaLista<int> n;
    if(n.brojElemenata() == 0)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}


void TestTrenutni(){
    NizLista<int> n;
    n.dodajIspred(123);
    if(n.trenutni() == 123) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}
void TestTrenutni2(){
    JednostrukaLista<int> n;
    n.dodajIspred(123);
    if(n.trenutni() == 123) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestSljedeci(){
    NizLista<int> n;
    n.dodajIspred(123);
    if(!n.sljedeci())std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.dodajIza(321);
    n.sljedeci();
    if(n.trenutni() == 321)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}
void TestSljedeci2(){
    JednostrukaLista<int> n;
    n.dodajIspred(123);
    if(!n.sljedeci())std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.dodajIza(321);
    n.sljedeci();
    if(n.trenutni() == 321)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestPrethodni(){
    NizLista<int> n;
    n.dodajIspred(123);
    if(!n.sljedeci())std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.dodajIspred(321);
    n.prethodni();
    if(n.trenutni() == 321)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestPrethodni2(){
    JednostrukaLista<int> n;
    n.dodajIspred(123);
    if(!n.sljedeci())std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.dodajIspred(321);
    n.prethodni();
    if(n.trenutni() == 321)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}


void TestDodajIspred(){
    NizLista<int> n;
    n.dodajIspred(1);
    n.dodajIspred(2);
    if(n.brojElemenata() == 2)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.prethodni();
    if(n.trenutni() == 2) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestDodajIspred2(){
    JednostrukaLista<int> n;
    n.dodajIspred(1);
    n.dodajIspred(2);
    if(n.brojElemenata() == 2)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.prethodni();
    if(n.trenutni() == 2) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}


void TestDodajIza(){
    NizLista<int> n;
    n.dodajIza(1);
    n.dodajIza(2);
    if(n.brojElemenata() == 2)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.sljedeci();
    if(n.trenutni() == 2) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestDodajIza2(){
    JednostrukaLista<int> n;
    n.dodajIza(1);
    n.dodajIza(2);
    if(n.brojElemenata() == 2)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n.sljedeci();
    if(n.trenutni() == 2) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}


void TestPocetak(){
    NizLista<int> n;
    n.dodajIspred(1);
    n.dodajIspred(2);
    n.dodajIspred(3);
    n.dodajIspred(4);
    n.pocetak();
    n.dodajIspred(100);
    n.pocetak();
    if(n.trenutni() == 100) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestPocetak2(){
    JednostrukaLista<int> n;
    n.dodajIspred(1);
    n.dodajIspred(2);
    n.dodajIspred(3);
    n.dodajIspred(4);
    n.pocetak();
    n.dodajIspred(100);
    n.pocetak();
    if(n.trenutni() == 100) std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestKraj(){
    NizLista<int> n;
    try{
        n.kraj();
    }catch(std::logic_error){
        std::cout << "OK" << std::endl;
    }
}
void TestKraj2(){
    JednostrukaLista<int> n;
    try{
        n.kraj();
    }catch(std::logic_error){
        std::cout << "OK" << std::endl;
    }
}

void TestObrisi(){
    NizLista<double> n;
    n.dodajIspred(123.123);
    n.obrisi();
    if(n.brojElemenata() == 0)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    try{
        n.obrisi();
    }catch(std::logic_error){
        std::cout << "OK" << std::endl;
    }
}

void TestObrisi2(){
    JednostrukaLista<double> n;
    n.dodajIspred(123.123);
    n.obrisi();
    if(n.brojElemenata() == 0)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    try{
        n.obrisi();
    }catch(std::logic_error){
        std::cout << "OK" << std::endl;
    }
}

void TestZagrada(){
    NizLista<double> n;
    n.dodajIspred(123.5);
    n.dodajIza(321);
    n.dodajIza(987);
    n.dodajIza(654);
    if(n[n.brojElemenata()-1] == 321)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n[0] = 10;
    n.pocetak();
    if(n.trenutni() == 10)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void TestZagrada2(){
    JednostrukaLista<double> n;
    //n.dodajIspred(123.5);
    n.dodajIza(321);
    n.dodajIza(987);
    n.dodajIza(654);
    if(n[n.brojElemenata()-1] == 987)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
    n[0] = 10;
    n.pocetak();
    if(n.trenutni() == 10)std::cout << "OK" << std::endl;
    else std::cout << "Nije OK" << std::endl;
}

void test1(){//5
    Lista<int>* l;
    l = new JednostrukaLista<int>;
    for (int i(0); i<40000; i++)
        l->dodajIspred(5);
    std::cout << l->trenutni() <<std::endl;
    delete l;
}

void test2(){
    Lista<std::string>* l;
    l = new JednostrukaLista<std::string>;
     l->dodajIza("abc"); 
     std::string s = l->trenutni();
     std::cout << s <<std::endl; 
     delete l;
}

void test3(){
    JednostrukaLista<int> jednostruka;
    NizLista<int> nizLista;

    // Dodavanje elementa ispred
    jednostruka.dodajIspred(1);
    nizLista.dodajIspred(1);
    std::cout << jednostruka.trenutni() << " " << nizLista.trenutni() << std::endl;

    // Brisanje trenutnog elementa
    jednostruka.obrisi();
    nizLista.obrisi();

    // Dodavanje elemenata iza
    jednostruka.dodajIza(1);
    jednostruka.dodajIza(2);
    jednostruka.dodajIza(8);
    nizLista.dodajIza(1);
    nizLista.dodajIza(2);
    nizLista.dodajIza(8);
    std::cout << jednostruka.trenutni() << " " << nizLista.trenutni() << std::endl;

    // Dodavanje još elemenata
    jednostruka.dodajIza(3);
    jednostruka.dodajIza(4);
    jednostruka.dodajIza(5);
    nizLista.dodajIza(3);
    nizLista.dodajIza(4);
    nizLista.dodajIza(5);

    // Pomeranje na kraj
    jednostruka.kraj();
    nizLista.kraj();
    std::cout << jednostruka.trenutni() << " " << nizLista.trenutni() << std::endl;

    // Pomeranje na početak
    jednostruka.pocetak();
    nizLista.pocetak();
    std::cout << jednostruka.trenutni() << " " << nizLista.trenutni() << std::endl;

    // Sledeći element
    jednostruka.sljedeci();
    nizLista.sljedeci();
    std::cout << jednostruka.trenutni() << " " << nizLista.trenutni() << std::endl;

    // Prethodni element
    jednostruka.prethodni();
    nizLista.prethodni();
    std::cout << jednostruka.trenutni() << " " << nizLista.trenutni() << std::endl;

    // Ispis elemenata na određenim indeksima
    std::cout << jednostruka[1] << " " << nizLista[1] << std::endl;

    // Broj elemenata
    std::cout << jednostruka.brojElemenata() << " " << nizLista.brojElemenata() << std::endl;

    // Ispis svih elemenata
    jednostruka.pocetak();
    nizLista.pocetak();
    for (int i = 0; i < jednostruka.brojElemenata(); i++) {
        std::cout << jednostruka.trenutni() << " ";
        jednostruka.sljedeci();
    }
    std::cout << std::endl;

    for (int i = 0; i < nizLista.brojElemenata(); i++) {
        std::cout << nizLista.trenutni() << " ";
        nizLista.sljedeci();
    }
    std::cout << std::endl;
}

/*void test4(){// ne radi
    NizLista<int> niz;

    // Dodavanje elemenata ispred
    for (int i = 1; i <= 5; i++) {
        niz.dodajIspred(i);
    }

    {
        NizLista<int> niz2(niz); // Kopiranje liste
        NizLista<int> niz3;       // Deklaracija prazne liste
        niz3 = niz;               // Dodeljivanje liste

        niz.obrisi();             // Brisanje elemenata iz originalne liste

        // Ispis broja elemenata u kopijama
        std::cout << niz2.brojElemenata(); // Broj elemenata u niz2
        std::cout << " " << niz3.brojElemenata() << " "; // Broj elemenata u niz3
    }

    // Ispis broja elemenata u originalnoj listi nakon brisanja
    std::cout << niz.brojElemenata();
}*/

void test7(){
    Lista<int> *l = new NizLista<int>; // Dinamičko alociranje NizLista

    // Dodavanje 100000 elemenata ispred
    
    for (int i = 0; i < 100000; i++) {
        l->dodajIspred(5);
    }
    // Ispis trenutnog elementa
    std::cout << l->trenutni() << std::endl;
    delete l;

}

void test4(){
    NizLista<int> niz;
    for (int i(1); i<=5; i++)
        niz.dodajIspred(i);
    {
        NizLista<int> niz2(niz);
        NizLista<int> niz3;
        niz3=niz;
        niz.obrisi();
        std::cout << niz2.brojElemenata();
        std::cout << " " << niz3.brojElemenata() << " ";
    }
    std::cout << niz.brojElemenata();
}

void test6(){
    Lista<int>* l;
    l = new NizLista<int>;
    for (int i(0); i<100000; i++)
    l->dodajIspred(5);
    std::cout << l->trenutni();
    delete l;
}

void test12(){
    NizLista<int> niz;
for (int i(1); i<=10000; i++)
	niz.dodajIspred(i);
std::cout << niz.brojElemenata() << " ";
for (int i(1); i<=10000; i++)
	niz.obrisi();
std::cout << niz.brojElemenata();
}

void test16(){
    JednostrukaLista<int> n;
for (int i(1); i<=5; i++) {
	n.dodajIza(i);
	n.sljedeci();
}
n.pocetak();
n.sljedeci();
n.sljedeci();
n.obrisi();
for (int i(0); i<n.brojElemenata(); i++)
	std::cout << n[i] << " ";
std::cout << n.trenutni();
}
int main(){
   /* NizLista<int> niz;
    niz.dodajIspred(10);
    niz.dodajIspred(20);
    niz.trenutni();
    niz.ispisi();
    std::cout << niz.trenutni() << std::endl;
    niz.obrisi();
    niz.ispisi();
    niz.dodajIspred(11);
    niz.dodajIspred(12);
    niz.dodajIspred(13);
    niz.ispisi();
    std::cout << niz[4];*/
    //std::cout << niz.brojElemenata();
    //std::cout << niz.trenutni()++ << std::endl;

    /*
    TestVelicine();
    TestTrenutni();
    TestSljedeci();
    TestPrethodni();
    TestDodajIspred();
    TestDodajIza();
    TestPocetak();
    TestKraj();
    TestObrisi();
    TestZagrada();
    std::cout << std::endl;
    TestVelicine2();
    TestTrenutni2();
    TestSljedeci2();
    TestPrethodni2();
    TestDodajIspred2();
    TestDodajIza2();
    TestPocetak2();
    TestKraj2();
    TestObrisi2();
    TestZagrada2();
    */
    JednostrukaLista<int> k;
    k.dodajIza(10);
    k.dodajIza(20);
    k.dodajIza(30);
    k.ispisi();
    JednostrukaLista<int> s(k);
    s.ispisi();


    return 0;
}