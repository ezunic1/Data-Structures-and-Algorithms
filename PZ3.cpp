#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace std;

template <typename tip>
class Stek{
    struct Cvor{
        tip el;
        Cvor * sljedeci;
        Cvor (const tip &e, Cvor *s): el(e), sljedeci(s) {}
    };
    int duzina;
    Cvor * prvi, *zadnji;
    public:
    Stek();
    Stek(const Stek &s);
    Stek &operator=(const Stek &s);
    ~Stek(){brisi();}
    void brisi();
    void stavi(const tip &e);
    tip skini();
    tip& vrh();
    int brojElemenata(){return duzina;}
    void ispisi();
};

template<typename tip>
Stek<tip>::Stek(){
    duzina = 0;
    prvi = nullptr;
    zadnji = nullptr;
}

template <typename tip>
Stek<tip>::Stek(const Stek &s){
    duzina = 0;
    prvi = zadnji = nullptr;
    Cvor * p = s.zadnji;
    Cvor * pom = nullptr;
    while(p != nullptr){
        Cvor * novi = new Cvor{p->el, nullptr};
        if(p == s.zadnji){
            zadnji = novi;
            pom = novi;
        }else{
            pom->sljedeci = novi;
            pom = novi;
        }
        if(p->sljedeci == nullptr) prvi = pom;
        p = p->sljedeci;
        duzina++;
    }

}

template <typename tip>
Stek<tip> &Stek<tip>::operator=(const Stek &s){
    if(&s != this){
        brisi();
        Cvor * p = s.zadnji;
        Cvor *pom = nullptr;
        while(p != nullptr){
            Cvor * novi = new Cvor{p->el, nullptr};
            if(p == s.zadnji){
                zadnji = novi;
                pom = novi;
            }else{
                pom->sljedeci = novi;
                pom = novi;
            }
            if(p->sljedeci == nullptr) prvi = pom;
            p = p->sljedeci;
            duzina++;
        }
    }
    return *this;
}

template <typename tip>
void Stek<tip>::brisi(){
    while(duzina != 0)skini();
}

template<typename tip>
void Stek<tip>::stavi(const tip &e){
    Cvor * novi = new Cvor{e, nullptr};
    if(duzina == 0)
        prvi = novi;
    else {
        novi->sljedeci = zadnji;
    }
    zadnji = novi;
    duzina++;
}

template <typename tip>
tip Stek<tip>::skini(){
    if(duzina == 0)throw std::range_error("Stek je prazan.");
    tip pom = zadnji->el;
    auto x = zadnji;
    zadnji = zadnji->sljedeci;
    duzina--;
    delete x;
    return pom;
}

template <typename tip>
tip &Stek<tip>::vrh(){
    if(duzina == 0)throw std::range_error("Stek je prazan.");
    return zadnji->el;
}

template <typename tip>
void Stek<tip>::ispisi(){
    if(duzina != 0){
        auto x = zadnji;
    while(x != prvi){
        std::cout << x->el << std::endl;
        x = x->sljedeci;
    }
    std::cout << x->el << std::endl;  
    }
}

void t1(){
    Stek<double> d;
    if(d.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t2(){
    Stek<int> d;
    d.stavi(10);
    Stek<int> x;
    x = d;
    if(x.vrh() == 10)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t3(){
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    Stek<int> x = s;
    if(x.brojElemenata() == 3)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t4(){
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    if(s.vrh() == 3) std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t5(){
    Stek<double> k;
    k.stavi(1.1);
    k.stavi(2.2);
    k.stavi(3.3);
    k.stavi(4.4);
    k.brisi();
    if(k.brojElemenata() == 0)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t6(){
    Stek <int> s;
    s.stavi(10);
    s.stavi(20);
    s.stavi(30);
    s.stavi(40);
    s.stavi(50);
    s.skini();
    if(s.vrh() == 40)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t7(){
    Stek<int> s;
    try{
        std::cout << s.vrh() << std::endl;
    }catch(std::range_error &e){
        std::cout << e.what() <<std::endl;
    }catch(...){
        std::cout<< "Pogresan izuzetak" <<std::endl;
    }
}

void t8(){
    Stek<int> s;
    try{
        s.skini();
    }catch(std::range_error &e){
        std::cout << e.what() <<std::endl;
    }catch(...){
        std::cout<< "Pogresan izuzetak" <<std::endl;
    }
}

void t9(){
    Stek<int> s;
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
int presjek(Stek<tip> s1, Stek<tip> s2 ){
    int brojac = 0;
    int pom = 0;
    if(s1.brojElemenata() > s2.brojElemenata()) 
        pom = s1.brojElemenata();
    else pom = s2.brojElemenata();
    for(int i = 0; i < 100; i++){
        tip t1 = s1.vrh(), t2 = s2.vrh();
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

void t10(){
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    Stek<int> k;
    k.stavi(1);
    k.stavi(2);
    k.stavi(10);
    k.stavi(12);
    k.stavi(122);
    if(presjek(s,k) == 2)std::cout << "OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}


/*Napisati funkciju:
int spoji(Stek<int> s1, Stek<int> s2, Stek<int>& s3)
Funkcija prima stek s1 koji je sortiran rastući, stek s2 koji je sortiran opadajući i s3 koji je prazan.
Potrebno je prebaciti elemente iz steka s1 i s2 u stek s3 tako da stek bude sortiran rastući pri čemu
je dozvoljeno koristiti samo tri data steka, tj. nisu dozvoljene dodatne kolekcije.
Primjer: Neka je stek s1: {1, 3, 5, 8} a s2: {9, 6, 4, 2} (prvi element je na dnu steka). Nakon
pozvane funkcije spoji stek s3 treba da bude {1, 2, 3, 4, 5, 6, 8, 9}.*/

int spoji(Stek<int> s1, Stek<int> s2, Stek<int>& s3){
    int x = s2.brojElemenata();
    for(int i = 0; i < x; i++){
        s3.stavi(s2.skini());
    }
    s3.ispisi();
    std::cout << std::endl;

    for(int i = 0; i < 100; i++){
        int t1 = s1.vrh(), t2 = s3.vrh();
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

int main(){
    /*t1();
    t2();
    t3();
    t4();
    t5();
    t6();
    t7();
    t8();
    t9();
    t10();*/
   /* Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);
    Stek<int> k;
    k.stavi(10);
    k.stavi(9);
    k.stavi(8);
    k.stavi(7);
    k.stavi(6);
    Stek<int> pom;
    spoji(s, k, pom);
    pom.ispisi();*/
    return 0; 
}