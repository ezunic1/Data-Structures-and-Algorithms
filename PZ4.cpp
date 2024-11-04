#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

template <typename tip>
class Red{
    struct Cvor{
        tip el;
        Cvor * sljedeci;
        Cvor (const tip &e, Cvor *s): el(e), sljedeci(s) {}
    };
    int duzina;
    Cvor * prvi, *zadnji;
    public:
    Red();
    Red(const Red &s);
    Red &operator=(const Red &s);
    ~Red(){brisi();}
    void brisi();
    void stavi(const tip &e);
    tip skini();
    tip& celo();
    int brojElemenata(){return duzina;}
    void ispisi();
};

template<typename tip>
Red<tip>::Red(){
    duzina = 0;
    prvi = nullptr;
    zadnji = nullptr;
}

template <typename tip>
Red<tip>::Red(const Red &s){
    duzina = 0;
    Cvor * p = s.prvi;
    while(p != nullptr){
        stavi(p->el);
        p = p->sljedeci;
    }
}

template <typename tip>
Red<tip> &Red<tip>::operator=(const Red &s){
    if(&s == this)return *this;
    brisi();
    Cvor * p = s.prvi;
    while(p != nullptr){
        stavi(p->el);
        p = p->sljedeci;
    }
    return *this;
}

template <typename tip>
void Red<tip>::brisi(){
    while(duzina != 0)skini();
}

template<typename tip>
void Red<tip>::stavi(const tip &e){
    Cvor * novi = new Cvor{e, nullptr};
    if(duzina == 0){
        prvi = novi;
        zadnji = novi;
    }else{
        zadnji->sljedeci = novi;
        zadnji = novi;    
    }
    duzina++;
}

template <typename tip>
tip Red<tip>::skini(){
    if(duzina == 0)throw std::range_error("Red je prazan.");
    tip pom = prvi->el;
    auto x = prvi;
    if(prvi == zadnji){
        prvi = nullptr;
        zadnji = nullptr;
    }
    else
        prvi = prvi->sljedeci;
    delete x;
    duzina--;
    return pom;
}

template <typename tip>
tip &Red<tip>::celo(){
    if(duzina == 0)throw std::range_error("Red je prazan.");
    return prvi->el;
}

template <typename tip>
void Red<tip>::ispisi(){
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
    Red<double> d;
    if(d.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t2(){
    Red<int> d;
    d.stavi(10);
    Red<int> x;
    x = d;
    if(x.celo() == 10)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t3(){
    Red<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    Red<int> x = s;
    if(x.brojElemenata() == 3)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t4(){
    Red<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    if(s.celo() == 1) std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t5(){
    Red<double> k;
    k.stavi(1.1);
    k.stavi(2.2);
    k.stavi(3.3);
    k.stavi(4.4);
    k.brisi();
    if(k.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t6(){
    Red <int> s;
    s.stavi(10);
    s.stavi(20);
    s.stavi(30);
    s.stavi(40);
    s.stavi(50);
    s.skini();
    if(s.celo() == 20)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t7(){
    Red<int> s;
    try{
        std::cout << s.celo() << std::endl;
    }catch(std::range_error &e){
        std::cout << e.what() <<std::endl;
    }catch(...){
        std::cout<< "Pogresan izuzetak" <<std::endl;
    }
}

void t8(){
    Red<int> s;
    try{
        s.skini();
    }catch(std::range_error &e){
        std::cout << e.what() <<std::endl;
    }catch(...){
        std::cout<< "Pogresan izuzetak" <<std::endl;
    }
}

void t9(){
    Red<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    s.stavi(4);
    int x = s.brojElemenata();
    for(int i = 0; i < x;  i++){
        s.skini();
    }
    if(s.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

template <typename tip>
int presjek(Red<tip> s1, Red<tip> s2 ){
    int brojac = 0;
    int pom = 0;
    if(s1.brojElemenata() > s2.brojElemenata()) 
        pom = s1.brojElemenata();
    else pom = s2.brojElemenata();
    for(int i = 0; i < 100; i++){
        tip t1 = s1.celo(), t2 = s2.celo();
        //std::cout << "t1: " << t1 << "  t2: " << t2 << std::endl;
        //std::cout << "OK" << std::endl;
        if(t1 == t2){
            brojac++;
            s1.skini();
            s2.skini();
        }
        else if(t1 > t2) s1.skini();
        else if(t2 > t1) s2.skini();
        if(s1.brojElemenata() == 0 || s2.brojElemenata() == 0)break;
    }
    return brojac;
}



/*Napisati funkciju:
int spoji(Red<int> s1, Red<int> s2, Red<int>& s3)
Funkcija prima Red s1 koji je sortiran rastući, Red s2 koji je sortiran opadajući i s3 koji je prazan.
Potrebno je prebaciti elemente iz Reda s1 i s2 u Red s3 tako da Red bude sortiran rastući pri čemu
je dozvoljeno koristiti samo tri data Reda, tj. nisu dozvoljene dodatne kolekcije.
Primjer: Neka je Red s1: {1, 3, 5, 8} a s2: {9, 6, 4, 2} (prvi element je na dnu Reda). Nakon
pozvane funkcije spoji Red s3 treba da bude {1, 2, 3, 4, 5, 6, 8, 9}.*/

int spoji(Red<int> s1, Red<int> s2, Red<int>& s3){
    int x = s2.brojElemenata();
    for(int i = 0; i < x; i++){
        s3.stavi(s2.skini());
    }
    s3.ispisi();
    std::cout << std::endl;

    for(int i = 0; i < 100; i++){
        int t1 = s1.celo(), t2 = s3.celo();
        if(t1 > t2) s2.stavi(s1.skini());
        else if(t2 > t1) s2.stavi(s3.skini());
        else{
            s2.stavi(s1.skini());
            s2.stavi(s3.skini());
        } 
        if(s1.brojElemenata() == 0 || s3.brojElemenata() == 0)break;
    }
    s2.ispisi();
    std::cout << std::endl;


    x = s2.brojElemenata();
    for(int i = 0; i < x; i++){
        s3.stavi(s2.skini());
    }
    return 0;   
}

void test(){
    int brel;
    Red<int> r;
    for (int i(1); i<=5; i++)
        r.stavi(i);
    {
    // Konstruktor kopije
    Red<int> r2(r);
    Red<int> r3;
    (r3=r).brojElemenata();
    // Destruktivna samododjela
    (r=r).brojElemenata();
    r.skini();
    brel = r2.brojElemenata();
    for (int i(0); i<brel; i++)
        std::cout << r2.skini() << " ";
    brel = r3.brojElemenata();
    for (int i(0); i<brel; i++)
        std::cout << r3.skini() << " ";
    }
    brel = r.brojElemenata();
    for (int i(0); i<brel; i++)
        std::cout << r.skini() << " ";
        
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
    test();
   /*Red<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    Red<int> k;
    k.stavi(10);
    k.stavi(9);
    k.stavi(8);
    k.stavi(7);
    k.stavi(6);
    Red<int> pom;
    spoji(s, k, pom);
    pom.ispisi();*/
    /*Red<int> r;
    r.stavi(10);
    r.stavi(20);
    r.stavi(30);
    r.ispisi();
    Red<int> k = r;
    k.ispisi();*/
    return 0; 
}