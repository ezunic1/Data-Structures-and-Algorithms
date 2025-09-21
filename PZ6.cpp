#include <iostream>
#include <stdexcept>

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

};

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

void t1() {
    NizMapa<string, string> mapa;
    mapa["Sarajevo"] = "BiH";
    mapa["Zagreb"] = "Hrvatska";
    if(mapa.brojElemenata() == 2)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(mapa["Sarajevo"] == "BiH")std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(mapa["Zagreb"] == "Hrvatska")std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    mapa.obrisi();
    if(mapa.brojElemenata() == 0)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t2(){
    NizMapa<int, string> mapa;
    mapa[1] = "Jedan";
    mapa[2] = "Dva";
    if(mapa.brojElemenata() == 2)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(mapa[1] == "Jedan")std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(mapa[2] == "Dva")std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    mapa[3] = "Tri";
    if(mapa[3] == "Tri")std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t3(){
    NizMapa<string, int> mapa;
    mapa["Jedan"] = 1;
    mapa["Dva"] = 2;

    NizMapa<string, int> kopija = mapa;
    if(kopija.brojElemenata() == 2)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(kopija["Jedan"] == 1)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(kopija["Dva"] == 2)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;

    
}

void t4(){
    NizMapa<string, int> mapa;
    mapa["Jedan"] = 1;
    mapa["Dva"] = 2;
    NizMapa<string, int> dodjela;
    dodjela = mapa;
    if(dodjela.brojElemenata() == 2)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(dodjela["Jedan"] == 1)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    if(dodjela["Dva"] == 2)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}

void t5(){
    NizMapa<string, int> mapa;
    mapa["Jedan"] = 1;
    mapa["Dva"] = 2;
    mapa["Jedan"] = 10;
    if(mapa["Jedan"] == 10)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
    mapa.obrisi();
    if(mapa["Jedan"] != 10)std::cout <<"OK" <<std::endl;
    else std::cout << "NIJE OK" << std::endl;
}


int main(){
    t1();
    t2();
    t3();
    t4();
    t5();
    return 0;
}
