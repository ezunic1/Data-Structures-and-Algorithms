#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace std;
template <typename tip>
class DvostraniRed{
    struct Cvor{
        tip el;
        Cvor * sljedeci;
        Cvor * prethodni;
        Cvor (const tip &e, Cvor *s, Cvor *c): el(e), sljedeci(s), prethodni(c) {}
    };
    int duzina;
    Cvor * prvi, *zadnji;
    public:
    DvostraniRed();
    DvostraniRed(const DvostraniRed &s);
    DvostraniRed &operator=(const DvostraniRed &s);
    ~DvostraniRed(){brisi();}
    void brisi();
    void staviNaVrh(const tip &e);
    void staviNaCelo(const tip &e);
    tip skiniSaVrha();
    tip skiniSaCela();
    //tip skini();
    tip& celo();
    tip& vrh();
    int brojElemenata(){return duzina;}
    void ispisi();
};

template<typename tip>
DvostraniRed<tip>::DvostraniRed(){
    duzina = 0;
    prvi = nullptr;
    zadnji = nullptr;
}

template <typename tip>
DvostraniRed<tip>::DvostraniRed(const DvostraniRed &s){
    duzina = 0;
    Cvor * p = s.prvi;
    while(p != nullptr){
        staviNaCelo(p->el);
        p = p->sljedeci;
    }
}

template <typename tip>
DvostraniRed<tip> &DvostraniRed<tip>::operator=(const DvostraniRed &s){
    if(&s == this)return *this;
    brisi();
    Cvor * p = s.prvi;
    while(p != nullptr){
        staviNaCelo(p->el);
        p = p->sljedeci;
    }
    return *this;
}

template <typename tip>
void DvostraniRed<tip>::brisi(){
    while(duzina != 0)skiniSaCela();
}

template<typename tip>
void DvostraniRed<tip>::staviNaVrh(const tip &e){
    Cvor * novi = new Cvor{e, nullptr, nullptr};
    if(duzina == 0){
        prvi = novi;
        zadnji = novi;
    }else{
        zadnji->sljedeci = novi;
        novi->prethodni = zadnji;
        zadnji = novi;    
    }
    duzina++;
}

template<typename tip>
tip DvostraniRed<tip>::skiniSaVrha(){
    if(duzina == 0)throw std::range_error("DvostraniRed je prazan.");
    tip pom = zadnji->el;
    auto x = zadnji;
    if(prvi == zadnji){
        prvi =  nullptr;
        delete zadnji;
    }else{
        zadnji = zadnji->prethodni;
        x->sljedeci = nullptr;
        delete x; 
    }
   
    duzina --;
    return pom;
}

template<typename tip>
void DvostraniRed<tip>::staviNaCelo(const tip &e){
    Cvor * novi = new Cvor{e, nullptr, nullptr};
    if(duzina == 0){
        prvi = novi;
        zadnji = novi;
    }else{
        novi->sljedeci = prvi;  
        prvi->prethodni = novi;
        prvi = novi;
    }
    duzina++;
}

template<typename tip>
tip DvostraniRed<tip>::skiniSaCela(){
    if(duzina == 0)throw std::range_error("DvostraniRed je prazan.");
    tip pom = prvi->el;
    auto x = prvi;
    if(prvi == zadnji){
        prvi = zadnji = nullptr;
    }else{
        prvi = prvi->sljedeci;
        prvi->prethodni = nullptr; 
    }
    delete x;
    duzina --;
    return pom;
}

template <typename tip>
tip &DvostraniRed<tip>::celo(){
    if(duzina == 0)throw std::range_error("DvostraniRed je prazan.");
    return prvi->el;
}
template <typename tip>
tip &DvostraniRed<tip>::vrh(){
    if(duzina == 0)throw std::range_error("DvostraniRed je prazan.");
    return zadnji->el;
}

template <typename tip>
void DvostraniRed<tip>::ispisi(){
    if(duzina != 0){
        auto x = prvi;
    while(x != zadnji){
        std::cout << x->el << std::endl;
        x = x->sljedeci;
    }
    std::cout << x->el << std::endl;  
    }
}

void t1(){
    DvostraniRed<double> d;
    if(d.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t2(){
    DvostraniRed<int> d;
    d.staviNaCelo(10);
    DvostraniRed<int> x;
    x = d;
    if(x.celo() == 10)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t3(){
    DvostraniRed<int> s;
    s.staviNaCelo(1);
    s.staviNaCelo(2);
    s.staviNaCelo(3);
    DvostraniRed<int> x = s;
    if(x.brojElemenata() == 3)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t4(){
    DvostraniRed<int> s;
    s.staviNaVrh(1);
    s.staviNaVrh(2);
    s.staviNaVrh(3);
    if(s.celo() == 1) std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t5(){
    DvostraniRed<double> k;
    k.staviNaCelo(1.1);
    k.staviNaCelo(2.2);
    k.staviNaCelo(3.3);
    k.staviNaCelo(4.4);
    k.brisi();
    //std::cout << k.brojElemenata() << " ----------" <<std::endl;
    if(k.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t6(){
    DvostraniRed <int> s;
    s.staviNaCelo(10);
    s.staviNaCelo(20);
    s.staviNaCelo(30);
    s.staviNaCelo(40);
    s.staviNaCelo(50);
    s.skiniSaCela();
    if(s.celo() == 40)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t7(){
    DvostraniRed<int> s;
    try{
        std::cout << s.celo() << std::endl;
    }catch(std::range_error &e){
        std::cout << e.what() <<std::endl;
    }catch(...){
        std::cout<< "Pogresan izuzetak" <<std::endl;
    }
}

void t8(){
    DvostraniRed<int> s;
    try{
        s.skiniSaCela();
    }catch(std::range_error &e){
        std::cout << e.what() <<std::endl;
    }catch(...){
        std::cout<< "Pogresan izuzetak" <<std::endl;
    }
}

void t9(){
    DvostraniRed<int> s;
    s.staviNaCelo(1);
    s.staviNaCelo(2);
    s.staviNaCelo(3);
    s.staviNaCelo(4);
    int x = s.brojElemenata();
    for(int i = 0; i < x;  i++){
        s.skiniSaCela();
    }
    if(s.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

int main(){
    t1();
    t2();
    t3();
    t4();
    t5();
    t6();
    t7();
    t8();
    t9();
    return 0; 
}

