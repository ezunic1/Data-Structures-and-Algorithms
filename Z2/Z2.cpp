#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
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

void pretraga(Stek<vector<int> >& s, int trazeni){
    if(s.brojElemenata() == 0){
        std::cout << "Nema elementa";
        return;
    }
    std::vector<int> pom;
    pom = s.skini();
    int dno = 0;
    int vrh = pom.size() - 1;
    int srednji = 0;
    bool x = false;
    if(pom.size() != 0){
        if(pom.at(vrh) < trazeni  || pom.at(dno) > trazeni){
            pretraga(s,trazeni); 
        }else{
        while(vrh >= dno){
            srednji = (dno + vrh) / 2;
            if(pom.at(srednji) == trazeni){
                std::cout << srednji << " " << s.brojElemenata();
                s.stavi(pom);
                x = true;
                return;
            }else if( pom.at(srednji) > trazeni){
                vrh = srednji - 1;
            }else
                dno = srednji + 1;
        }
        if(!x) std::cout << "Nema elementa";
        }
    }
    else
        pretraga(s, trazeni);
    s.stavi(pom);
}

void t1() {
    Stek<vector<int>> s;
    // element na poziciji 2 1
    s.stavi({1, 2, 3, 4, 5});
    s.stavi({6, 7, 8, 9, 10});
    pretraga(s, 8);
    std::cout << std::endl;
}

void t2() {
    Stek<vector<int>> s;
    // nema elementa
    s.stavi({1, 2, 3, 4, 5});
    s.stavi({6, 7, 8, 9, 10});
    pretraga(s, 11);
    std::cout << std::endl;
}

void t3() {
    // prazan stek
    Stek<vector<int>> s;
    pretraga(s, 5);
    std::cout << std::endl;
}




int main(){
    t1();
    t2();
    t3();
    return 0; 
}
