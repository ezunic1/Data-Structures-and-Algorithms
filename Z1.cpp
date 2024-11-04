#include <iostream>
#include <stdexcept>

using namespace std;

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

template<typename tip>
class Iterator;

template <typename tip>
class DvostrukaLista : public Lista<tip>{
    int broj;
    struct Cvor{
        tip el;
        Cvor * sljedeci;
        Cvor * prethodni;
        Cvor(const tip &el, Cvor * sljedeci, Cvor * prethodni): el(el), sljedeci(sljedeci), prethodni(prethodni){};
    };
    Cvor * prvi, * zadnji, * sad;
    public: 
    DvostrukaLista();
    DvostrukaLista(const DvostrukaLista <tip> &n);
    DvostrukaLista &operator=(const DvostrukaLista<tip> &n);
	~DvostrukaLista();
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
    friend class Iterator<tip>;
};
template <typename tip>
DvostrukaLista<tip>::DvostrukaLista(){
    prvi = nullptr;
    zadnji = nullptr;
    sad = nullptr;
    broj = 0;
}
template <typename tip>
DvostrukaLista<tip>::DvostrukaLista(const DvostrukaLista<tip> &d){
    prvi = nullptr;
    zadnji = nullptr;
    sad = nullptr;
    broj = 0;
    for(int i = 0; i < d.broj; i++){
        dodajIza(d[i]);
        sljedeci();
    } 
}

template <typename tip>
DvostrukaLista<tip> &DvostrukaLista<tip>::operator=(const DvostrukaLista<tip> &n){
    if(&n !=this){
        while(prvi != zadnji){
            Cvor * novi = prvi->sljedeci;
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
            sljedeci();
        } 
    }
    return *this;
}

template <typename tip>
DvostrukaLista<tip>:: ~DvostrukaLista(){
    while(prvi != zadnji){
        Cvor * novi = prvi->sljedeci;
        delete prvi;
        prvi = novi;
    }
    delete prvi;
    prvi = nullptr;
    zadnji = nullptr;
    sad = nullptr;
    broj = 0;
}

template <typename tip>
tip &DvostrukaLista<tip>::trenutni()const{
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    return sad->el;
}

template <typename tip>
bool DvostrukaLista<tip>::sljedeci(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    if(sad == zadnji) return false;
    sad = sad->sljedeci;
    return true;
}

template <typename tip>
bool DvostrukaLista<tip>::prethodni(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    if(sad == prvi) return false;
    sad = sad->prethodni;
    return true;
}

template<typename tip>
void DvostrukaLista<tip>::pocetak(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    sad = prvi;
}
template<typename tip>
void DvostrukaLista<tip>::kraj(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    sad = zadnji;
}

template<typename tip>
void DvostrukaLista<tip>::obrisi(){
    if(broj == 0)throw std::logic_error("Lista je prazna.");
    Cvor *pom = sad;
    if (sad == prvi && sad == zadnji) {
        prvi = zadnji = sad = nullptr;
    } else if (sad == prvi) {
        prvi = sad->sljedeci;
        sad = prvi;
        prvi->prethodni = nullptr;
    } else if (sad == zadnji) {
        zadnji = sad->prethodni;
        zadnji->sljedeci = nullptr;
        sad = zadnji;
    } else {
        sad->prethodni->sljedeci = sad->sljedeci;
        sad->sljedeci->prethodni = sad->prethodni;
        sad = sad->sljedeci;
    }
    delete pom;
    /*Cvor * pom = nullptr;
    if(sad == zadnji){
        if(sad == prvi){
            delete sad;
            sad = nullptr;
            zadnji = nullptr;
            prvi = nullptr;
        }else{
            //sad->prethodni->sljedeci = sad->sljedeci;
            //delete sad;
            sad->prethodni->sljedeci = nullptr;
            pom = sad;
            sad = sad->prethodni;
            delete pom;
        }
    }else if(sad == prvi){
        pom = sad;
        sad = sad->sljedeci;
        prvi = sad;
        sad->prethodni = nullptr;
        delete pom;
    }else{
        pom = sad;
        sad = sad->sljedeci;
        pom->prethodni->sljedeci = sad;
        sad->prethodni = pom->prethodni;
        delete pom;
    }*/
    broj--;
}

template<typename tip>
void DvostrukaLista<tip>::dodajIspred(const tip &el){
    Cvor * pom = new Cvor{el, nullptr, nullptr};
    if(broj == 0){
        sad = pom;
        prvi = pom;
        zadnji = pom;
    }else if(prvi == sad){
        prvi = pom;
        pom->sljedeci = sad;
        sad->prethodni = pom;
    }else{
        pom->prethodni = sad->prethodni;
        pom->sljedeci = sad;
        sad->prethodni->sljedeci = pom;
        sad->prethodni = pom;
    }
    broj++;
}

template<typename tip>
void DvostrukaLista<tip>::dodajIza(const tip &el){
    Cvor * pom = new Cvor{el, nullptr, nullptr};
    if(prvi == nullptr){
        prvi = pom;
        sad = pom;
        zadnji = pom;
    }else if(sad != zadnji){
        pom->prethodni = sad;
        pom->sljedeci = sad->sljedeci;
        sad->sljedeci->prethodni = pom;
        sad->sljedeci = pom;
    }else{
        zadnji = pom;
        pom->prethodni = sad;
        sad->sljedeci = pom;
    }
    broj++;
}
template<typename tip>
tip & DvostrukaLista<tip>::operator[](int i)const{
    if(i < 0 || i >= broj) throw std::logic_error("Ne postoji element sa tim indeksom.");
    int pom = 0;
    for(Cvor * novi = prvi; novi != nullptr; novi = novi->sljedeci){
        if(pom == i)return novi->el;
        pom++;
    }
    return prvi->el;
}


template<typename tip>
void DvostrukaLista<tip>::ispisi()const{
    std::cout << std::endl;
    for(Cvor * novi = prvi; novi !=nullptr; novi = novi->sljedeci){
        std::cout << novi->el << "  ";
    }
    std::cout << std::endl;
}

template<typename tip>
class Iterator{
    const DvostrukaLista<tip> * l;
    typename DvostrukaLista<tip>::Cvor *trenutniLista;
    public:
    Iterator(const DvostrukaLista<tip> &n) : l(&n), trenutniLista(n.sad){}
    tip &trenutni()const;
    bool prethodni();
    bool sljedeci();
    void pocetak();
    void kraj();
};

template<typename tip>
bool Iterator<tip>::prethodni(){
    if(l->brojElemenata() == 0)throw std::logic_error("Lista je prazna.");
    if(trenutniLista == l->prvi)return false;
    trenutniLista = trenutniLista->prethodni;
    return true;
}

template<typename tip>
bool Iterator<tip>::sljedeci(){
    if(l->brojElemenata() == 0)throw std::logic_error("Lista je prazna.");
    if(trenutniLista == l->zadnji)return false;
    trenutniLista = trenutniLista->sljedeci;
    return true;
}

template<typename tip>
tip &Iterator<tip>::trenutni()const{
    if(l->brojElemenata() == 0)throw std::logic_error("Lista je prazna.");
    return trenutniLista->el;
}

template<typename tip>
void Iterator<tip>::kraj(){
    if(l->brojElemenata() == 0)throw std::logic_error("Lista je prazna.");
    trenutniLista = l->kraj;
}

template<typename tip>
void Iterator<tip>::pocetak(){
    if(l->brojElemenata() == 0)throw std::logic_error("Lista je prazna.");
    trenutniLista = l->prvi;
}

template<typename tip>
tip dajMaksimum(const Lista<tip>& n){
    if(n.brojElemenata() == 0) throw std::logic_error("Lista je prazna.");
    Iterator<tip> it((const DvostrukaLista<tip>&)n);
    it.pocetak();
    tip max = it.trenutni();
    while(it.sljedeci()){
        if(it.trenutni() > max)
        max = it.trenutni();
    }
    return max;
}


void test_1288() {
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 0; i < 40000; i++) l->dodajIspred(5);
    l->pocetak();
    l->dodajIspred(4);
    std::cout << l->trenutni() << " ";
    l->prethodni();
    std::cout << l->trenutni();
    delete l;
    // Očekivani ispis: 5 4
}

void test_1289() {
    Lista<std::string>* l = new DvostrukaLista<std::string>;
    l->dodajIza("abc");
    std::string s = l->trenutni();
    std::cout << s;
    delete l;
    // Očekivani ispis: abc
}

void test_1290() {
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 1; i <= 5; i++) l->dodajIza(i);
    do {
        std::cout << l->trenutni() << " ";
    } while (l->sljedeci());
    l->dodajIza(6);
    l->sljedeci();
    std::cout << l->trenutni();
    delete l;
    // Očekivani ispis: 1 5 4 3 2 6
}

void test_1291() {
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 1; i <= 5; i++) l->dodajIspred(i);
    for (int i = 0; i < 5; i++) std::cout << (*l)[i] << " ";
    delete l;
    // Očekivani ispis: 2 3 4 5 1
}

void test_1292() {
    DvostrukaLista<int> lista;
    for (int i = 1; i <= 10000; i++) lista.dodajIspred(i);
    std::cout << lista.brojElemenata() << " ";
    for (int i = 1; i <= 10000; i++) lista.obrisi();
    std::cout << lista.brojElemenata();
    // Očekivani ispis: 10000 0
}

void test_1293() {
    DvostrukaLista<int> lista;
    for (int i = 1; i <= 5; i++) lista.dodajIza(i);
    std::cout << lista.brojElemenata() << " ";
    for (int i = 1; i <= 5; i++) lista.obrisi();
    std::cout << lista.brojElemenata();
    // Očekivani ispis: 5 0
}

void test_1294() {
    DvostrukaLista<int> lista;
    for (int i = 1; i <= 5; i++) lista.dodajIspred(i);
    {
        DvostrukaLista<int> lista2(lista);
        DvostrukaLista<int> lista3;
        (lista3 = lista).brojElemenata();
        (lista = lista).brojElemenata();
        lista.obrisi();
        std::cout << lista2.brojElemenata() << " " << lista2[2];
        std::cout << " " << lista3.brojElemenata() << " " << lista3[2] << " ";
    }
    std::cout << lista.brojElemenata() << " " << lista[2];
    // Očekivani ispis: 5 4 5 4 4 4
}

void test_1295() {
    DvostrukaLista<int> n;
    for (int i = 1; i <= 5; i++) {
        n.dodajIza(i);
        n.sljedeci();
    }
    n.pocetak();
    n.sljedeci();
    n.sljedeci();
    n.obrisi();
    for (int i = 0; i < n.brojElemenata(); i++) std::cout << n[i] << " ";
    std::cout << n.trenutni();
    // Očekivani ispis: 1 2 4 5 4
}

void test_1296() {
    DvostrukaLista<int> lista;
    for (int i = 1; i <= 5; i++) lista.dodajIspred(i);
    const DvostrukaLista<int>& konst(lista);
    std::cout << konst.brojElemenata() << " " << konst.trenutni();
    std::cout << " " << konst[0] << std::endl;
    lista.trenutni() = 15;
    lista[0] = 20;
    std::cout << konst.trenutni() << " " << konst[0] << std::endl;
    // Očekivani ispis: 5 1 2\n15 20
}

void test_1297() {
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 0; i < 40000; i++) l->dodajIspred(5);
    for (int i = 0; i < 100000; i++) {
        l->pocetak();
        l->kraj();
    }
    delete l;
    std::cout << "OK";
    // Očekivani ispis: OK
}

void test1298() {
    // Test performansi metoda pocetak() i kraj()
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 0; i < 40000; i++)
        l->dodajIspred(5);
    for (int i = 0; i < 100000; i++) {
        l->pocetak();
        l->kraj();
    }
    delete l;
    std::cout << "OK\n"; // Očekivani ispis: OK
}

void test1299() {
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 0; i < 100000; i++)
        l->dodajIspred(i);
    std::cout << dajMaksimum(*l) << "\n"; // Očekivani ispis: 99999
    delete l;
}

void test1300() {
    Lista<int>* l = new DvostrukaLista<int>;
    for (int i = 0; i < 100; i++)
        l->dodajIspred(i);
    std::cout << dajMaksimum(*l) << "\n"; // Očekivani ispis: 99
    delete l;
}

void test1301() {
    DvostrukaLista<int> l;
    for (int i = 0; i <= 100000; i++)
        l.dodajIspred(i);
    std::cout << dajMaksimum(l) << "\n"; // Očekivani ispis: 100000
}

void test1302() {
    DvostrukaLista<int> l;
    for (int i = 0; i <= 100000; i++) {
        l.dodajIza(i);
        l.sljedeci();
    }
    l.pocetak();
    for (int i = 0; i < 500; i++)
        l.sljedeci();
    std::cout << l.trenutni() << " " << dajMaksimum(l) << " " << l.trenutni() << "\n";
    // Očekivani ispis: 500 100000 500
}

void test1303() {
    DvostrukaLista<int> l;
    l.dodajIspred(1);
    std::cout << l.trenutni() << " ";
    l.obrisi();
    l.dodajIza(1);
    l.dodajIza(2);
    l.dodajIza(8);
    std::cout << l.trenutni() << " ";
    l.dodajIza(3);
    l.dodajIza(4);
    l.dodajIza(5);
    l.pocetak();
    l.kraj();
    std::cout << l.trenutni() << " ";
    l.pocetak();
    std::cout << l.trenutni() << " ";
    l.sljedeci();
    std::cout << l.trenutni() << " ";
    l.prethodni();
    std::cout << l.trenutni() << " ";
    std::cout << l[1] << " ";
    std::cout << l.brojElemenata() << " ";
    l.pocetak();
    for (int i = 0; i < l.brojElemenata(); i++) {
        std::cout << l.trenutni() << " ";
        l.sljedeci();
    }
    std::cout << "\n";
    // Očekivani ispis: 1 1 2 1 5 1 5 6 1 5 4 3 8 2
}
template <typename tip>
void TestDajMaksimum() {
    DvostrukaLista<tip> l;
    l.dodajIza(100);
    if(dajMaksimum(l) != 100) std::cout << " Nije OK ";
    else std::cout << " OK ";
}



int main() {
    test_1288();
    std::cout <<std::endl;
    test_1289();
    std::cout <<std::endl;
    test_1290();
    std::cout <<std::endl;
    test_1291();
    std::cout <<std::endl;
    test_1292();
    std::cout <<std::endl;
    test_1293();
    std::cout <<std::endl;
    test_1294();
    std::cout <<std::endl;
    test_1295();
    std::cout <<std::endl;
    test_1296();
    std::cout <<std::endl;
    test_1297();
    std::cout <<std::endl;
    test1298();
    std::cout <<std::endl;
    test1299();
    std::cout <<std::endl;
    test1300();
    std::cout <<std::endl;
    test1301();
    std::cout <<std::endl;
    test1302();
    std::cout <<std::endl;
    test1303();
    std::cout <<std::endl;
    TestDajMaksimum<int>();
    return 0;
}