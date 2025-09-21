#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

template<typename tipK, typename tipV>
class Mapa{
    public:
    Mapa(){}
    virtual ~Mapa(){}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const tipK& kljuc) = 0;
    virtual tipV& operator [](const tipK &kljuc) = 0;
    virtual tipV operator [](const tipK &kljuc) const = 0;
};

template<typename tipK, typename tipV>
class NizMapa : public Mapa<tipK, tipV>{
    int kapacitet;
    int duzina;
    std::pair<tipK, tipV>* niz;
    public:
    NizMapa<tipK, tipV>();
    NizMapa<tipK, tipV>(const NizMapa<tipK, tipV> &n);
    NizMapa<tipK, tipV>& operator =(const NizMapa<tipK, tipV> &n);
    ~NizMapa();
    tipV& operator [](const tipK &kljuc);
    tipV operator [](const tipK &kljuc) const;
    int brojElemenata() const { return duzina; };
    void obrisi();
    void obrisi(const tipK &kljuc);
    void ispisi()const{
        for(int i = 0; i < brojElemenata(); i++){
            std::cout << niz[i].first << " " << niz[i].second <<std::endl;
        }
    }
};

template<typename tipK, typename tipV>
class BinStabloMapa : public Mapa<tipK, tipV>{
    struct Cvor{
        tipK kljuc;
        tipV vrijednost;
        Cvor *lijevi, *desni, *roditelj;
        Cvor(tipK k = tipK(), tipV v = tipV()) : kljuc(k), vrijednost(v), lijevi(nullptr),
        desni(nullptr), roditelj(nullptr){}
    };
    Cvor *novi(tipK i = tipK(), tipV u = tipV()) {
      Cvor *temp = new Cvor();
      temp->kljuc = i;
      temp->vrijednost = u;
      temp->desni = temp->lijevi = temp->roditelj = nullptr;
      return temp;
    }

    Cvor *pretraga(Cvor *p, tipK i) const {
      if (p == nullptr || p->kljuc == i)
        return p;
      if (p->kljuc < i)
        return pretraga(p->desni, i);
      else if (p->kljuc > i)
        return pretraga(p->lijevi, i);
      return p;
    }

    Cvor *dodaj(Cvor *p, tipK i, tipV u = tipV(), Cvor *rod = nullptr) {
      if (p == nullptr) {
        p = novi(i, u);
        p->roditelj = rod;
        return p;
      }
      if (i > p->kljuc)
        p->desni = dodaj(p->desni, i, u, p);
      else
        p->lijevi = dodaj(p->lijevi, i, u, p);
      return p;
    }

    Cvor *nasljednik(Cvor *t) {
      auto ovaj = t;
      while (ovaj && ovaj->lijevi != nullptr)
        ovaj = ovaj->lijevi;
      return ovaj;
    }

    void konstruktor(Cvor *a, Cvor *b) {
      if (b == nullptr)
        return;
      konstruktor(a, b->lijevi);
      konstruktor(a, b->desni);
      dodaj(a, b->kljuc, b->vrijednost);
    }

    Cvor *obrisiC(Cvor *t, tipK i) {
      if (t == nullptr)
        return t;
      if (i < t->kljuc)
        t->lijevi = obrisiC(t->lijevi, i);
      else if (i > t->kljuc)
        t->desni = obrisiC(t->desni, i);
      else {
        if (t->lijevi == nullptr) {
          Cvor *temp = t->desni;
          delete t;
          return temp;
        } else if (t->desni == nullptr) {
          Cvor *temp = t->lijevi;
          delete t;
          return temp;
        }
        auto q = nasljednik(t->desni);
        t->kljuc = q->kljuc;
        t->vrijednost=q->vrijednost;
        t->desni = obrisiC(t->desni, q->kljuc);
      }

      return t;
    }

    void obrisi_stablo(Cvor *t) {
      if (t == nullptr)
        return;
      obrisi_stablo(t->lijevi);
      obrisi_stablo(t->desni);
      delete t;
    }
    int duzina;
    Cvor *p;
    public:
    BinStabloMapa<tipK, tipV>();
    BinStabloMapa<tipK, tipV>(const BinStabloMapa<tipK, tipV> &n);
    BinStabloMapa<tipK, tipV>& operator =(const BinStabloMapa<tipK, tipV> &n);
    ~BinStabloMapa(){obrisi_stablo(p);};
    //tipV& operator [](const tipK &kljuc);
    //tipV operator [](const tipK &kljuc) const;
    int brojElemenata() const { return duzina; };
    void obrisi();
    void obrisi(const tipK &kljuc);

    tipV& operator [](const tipK &kljuc) {
        if (p == nullptr) {
            p = novi(kljuc);
            duzina++;
            return p->vrijednost;
        }
        auto c = pretraga(p, kljuc);
        if (c == nullptr) {
            dodaj(p, kljuc);
            duzina++;
            c = pretraga(p, kljuc);
        }
        return c->vrijednost;
    }

    tipV operator [](const tipK &kljuc) const {
        auto c = pretraga(p, kljuc);
        if (c != nullptr)
        return c->vrijednost;
        return tipV();
    }
};

template<typename tipK, typename tipV>
BinStabloMapa<tipK,tipV>::BinStabloMapa() : duzina(0), p(nullptr){}
template<typename tipK, typename tipV>
BinStabloMapa<tipK,tipV>::BinStabloMapa(const BinStabloMapa<tipK, tipV> &n){
    duzina = n.duzina;
    if (n.duzina == 0)
      p = nullptr;

    else {
      p = novi(n.p->kljuc, n.p->vrijednost);
      konstruktor(p, n.p);
    }

}
template<typename tipK, typename tipV>
BinStabloMapa<tipK, tipV>&BinStabloMapa<tipK,tipV>::operator = (const BinStabloMapa<tipK, tipV> &n) {
    if (this == &n)
      return *this;

    obrisi();
    duzina = n.duzina;
    if (n.duzina == 0)
      p = nullptr;

    else {
      p->kljuc=n.p->kljuc; p->vrijednost=n.p->vrijednost;
      konstruktor(p, n.p);
    }
    return *this;
  }
template<typename tipK, typename tipV>
void BinStabloMapa<tipK, tipV>::obrisi() {
    obrisi_stablo(p);
    p = novi();
    duzina = 0;
}

template<typename tipK, typename tipV>
void BinStabloMapa<tipK, tipV>::obrisi(const tipK &kljuc) {
    if (p->kljuc == kljuc)
      p = obrisiC(p, kljuc);
    else
      obrisiC(p, kljuc);
      duzina--;
}




template<typename tipK, typename tipV>
NizMapa<tipK, tipV>::NizMapa(){
    duzina = 0;
    kapacitet = 1;
    niz = new std::pair<tipK, tipV>[kapacitet]{};
}

template<typename tipK, typename tipV>
NizMapa<tipK, tipV>::~NizMapa(){
    delete []niz;
    duzina = 0;
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &n): kapacitet(n.kapacitet), duzina(n.duzina) {
    try {
        niz = new std::pair<TipKljuca, TipVrijednosti>[kapacitet]{};
        for(int i = 0; i < n.duzina; i++) {
            niz[i] = n.niz[i];
        }
    }
    catch(...) {
        delete [] niz;
        throw;
    }
}

template<typename tipK, typename tipV>
NizMapa<tipK, tipV>&NizMapa<tipK,tipV>::operator = (const NizMapa<tipK, tipV> &n){
    if(&n != this){
        delete [] niz;
        duzina = n.brojElemenata();
        kapacitet = n.kapacitet;
        niz = new std::pair<tipK, tipV>[kapacitet]{};
        for(int i = 0; i < duzina; i++){
            niz[i] = n.niz[i];
        }
    }
    return *this;
}

template <typename tipK, typename tipV>
tipV& NizMapa<tipK, tipV>::operator [](const tipK &kljuc) {
    bool kljucPostoji = false;
    int indeks = 0;
    tipV vrati = tipV();
    for(int i = 0; i < duzina; i++) {
        if(niz[i].first == kljuc) {
            kljucPostoji = true;
            indeks = i;
            break;
        }
    }
    
    if(kljucPostoji) return niz[indeks].second;
   // std::cout << "uglaste" <<std::endl;
    if(!kljucPostoji) {
        indeks = duzina;
        if(duzina < kapacitet) {
            niz[duzina].first = kljuc;
            niz[duzina++].second = vrati;
        }
        else {
            std::pair<tipK, tipV>* novi = new std::pair<tipK, tipV>[++kapacitet]{};
            for(int i = 0; i < duzina; i++) novi[i] = niz[i];
            delete[] niz;
            niz = novi;
            niz[duzina].first = kljuc;
            niz[duzina++].second = vrati;
        }
      
    }
    return niz[indeks].second;
}

template <typename tipK, typename tipV>
tipV NizMapa<tipK, tipV>::operator [](const tipK &kljuc) const {
    bool kljucPostoji = false;
    int indeks = 0;
    tipV vrati = tipV();

    for(int i = 0; i < duzina; i++) {
        if(niz[i].first == kljuc) {
            kljucPostoji = true;
            indeks = i;
            break;
        }
    }
    if(kljucPostoji) return niz[indeks].second;
    return vrati;
}

template<typename tipK, typename tipV>
void NizMapa<tipK,tipV>::obrisi(){
    duzina = 0;
    kapacitet = 1;
    delete [] niz;
    niz = new std::pair<tipK, tipV>[kapacitet];
}

template<typename tipK, typename tipV>
void NizMapa<tipK,tipV>::obrisi(const tipK &kljuc){
    bool pom = false;
    for(int i = 0; i < this->duzina; i++){
        if(kljuc == niz[i].first){
            pom = true;
            break;
        }
    }
    if(pom){
        for(int i = 0; i < duzina; i++) {
            if(niz[i].first == kljuc) {
                for(int j = i; j < duzina - 1; j++) {
                    niz[j] = niz[j+1];
                }
                duzina--;
                break;
            }  
        }
    }else{
        throw std::logic_error("Nema tog kljuca.");
    }

}


using sat=std::chrono::high_resolution_clock;

int main(){
    //Analiza performansi NizMape i BinStabloMape
    //Zakljucujemo da je i pri dodavanju i pri pristupu daleko brza klasa BinStabloMapa.
    //Dodavanje elemenata u NizMapu
    NizMapa<int,int> mapa;
    auto start = std::chrono::high_resolution_clock::now();
    std::srand(std::time(nullptr));
    for(int i = 0; i < 10000; i++){
        mapa[std::rand()] = std::rand();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    std::cout << "Dodavanje - NizMapa: " << duration.count() / 10000 << " mikrosekundi" << std::endl;
    //Dodavanje elemenata u BinStabloMapu
    BinStabloMapa<int, int> binMapa;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10000; i++){
        binMapa[std::rand()] = std::rand();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Dodavanje - BinStabloMapa: " << duration.count() / 10000 << " mikrosekundi" << std::endl;
    //Pristup elemenata u NizMapi
    NizMapa<int, int> mapa2;
    for(int i = 0; i < 10000; i++){
        mapa2[i] = std::rand();
    }
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10000; i++){
        int x = mapa2[i];
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Pristup - NizMapa: " << duration.count() / 10000 << " mikrosekundi" << std::endl;
    //Pristup elemenata u BinStabloMapi
    BinStabloMapa<int, int> mapa3;
    for(int i = 0; i < 10000; i++){
        mapa3[i] = std::rand();
    }
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10000; i++){
        int x = mapa3[i];
    }
    //mapa3[1000] = 10;
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Pristup - BinStabloMapa: " << duration.count() / 10000 << " mikrosekundi" << std::endl;
    return 0;
}
