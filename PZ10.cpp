#include <iostream>
#include <vector>
#include <limits>

using namespace std;

template <typename tip>
class Grana;

template <typename tip>
class Cvor;

template <typename tip>
class GranaIterator;

template <typename tip>
class UsmjereniGraf {
public:
    UsmjereniGraf() {}
    UsmjereniGraf(int brojCvorova);
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;
    virtual void dodajGranu(int prviCvor, int drugiCvor, float tezina = 0) = 0;
    virtual void obrisiGranu(int prviCvor, int drugiCvor) = 0;
    virtual void postaviTezinuGrane(int prviCvor, int drugiCvor, float tezina = 0) = 0;
    virtual float dajTezinuGrane(int prviCvor, int drugiCvor) const = 0;
    virtual bool postojiGrana(int prviCvor, int drugiCvor) = 0;
    virtual void postaviOznakuCvora(int brojCvora, tip oznaka) = 0;
    virtual tip dajOznakuCvora(int brojCvora) const = 0;
    virtual void postaviOznakuGrane(int prviCvor, int drugiCvor, tip oznaka) = 0;
    virtual tip dajOznakuGrane(int prviCvor, int drugiCvor) const = 0;

    virtual Grana<tip> dajGranu(int prviCvor, int drugiCvor) = 0;
    virtual Cvor<tip> dajCvor(int brojCvora) = 0;

    GranaIterator<tip> dajGranePocetak() {
        //std::cout << "NESTO" <<std::endl;
        return ++GranaIterator<tip>(this, 0, -1); //vraca prvu granu
    }

    GranaIterator<tip> dajGraneKraj() {
        //std::cout << "NESTO" <<std::endl;
        return GranaIterator<tip>(this, dajBrojCvorova(), 0); 
    }
};

template <typename tip>
class Cvor {
    UsmjereniGraf<tip>* graf;
    int cvor;
public:
    Cvor(UsmjereniGraf<tip>* usmjereniGraf, int brojCvora): cvor(brojCvora), graf(usmjereniGraf) {}

    tip dajOznaku() const {
        return graf->dajOznakuCvora(cvor);
    }

    void postaviOznaku(tip oznaka) {
        graf->postaviOznakuCvora(cvor, oznaka);
    }

    int dajRedniBroj() const {
        return cvor;
    }
};

template <typename tip>
class Grana {
    UsmjereniGraf<tip>* graf;
    int prviCvor;
    int drugiCvor;
public:
    Grana(UsmjereniGraf<tip>* usmjereniGraf, int prvi, int drugi): graf(usmjereniGraf), prviCvor(prvi), drugiCvor(drugi) {}
    
    float dajTezinu() const {
        return graf->dajTezinuGrane(prviCvor, drugiCvor);
    }

    void postaviTezinu(float tezina) {
        graf->postaviTezinuGrane(prviCvor, drugiCvor, tezina);
    }

    tip dajOznaku() const {
        return graf->dajOznakuGrane(prviCvor, drugiCvor);
    }

    void postaviOznaku(tip oznaka) {
        graf->postaviOznakuGrane(prviCvor, drugiCvor, oznaka);
    }

    Cvor<tip> dajPolazniCvor() const {
        return graf->dajCvor(prviCvor);
    }

    Cvor<tip> dajDolazniCvor() const {
        return graf->dajCvor(drugiCvor);
    }
};

template <typename tip>
class GranaIterator {
    UsmjereniGraf<tip>* graf;
    int prviCvor;
    int drugiCvor;

public:
    GranaIterator(UsmjereniGraf<tip>* usmjereniGraf, int prvi, int drugi): graf(usmjereniGraf), prviCvor(prvi), drugiCvor(drugi) {}

    Grana<tip> operator*() {
        return graf->dajGranu(prviCvor, drugiCvor);
    }

    bool operator ==(const GranaIterator &iter) const {
        if(prviCvor == iter.prviCvor && drugiCvor == iter.drugiCvor && graf == iter.graf) return true;
        return false;
    }
    
    bool operator !=(const GranaIterator &iter) const {
    return !(*this == iter);
    }

    GranaIterator& operator ++() {
        if(drugiCvor + 1 >= graf->dajBrojCvorova()) {
            drugiCvor = 0;
            prviCvor++;
        }
        else{
            drugiCvor++;
        }

        while(prviCvor < graf->dajBrojCvorova() && !graf->postojiGrana(prviCvor, drugiCvor)) {
            if(drugiCvor + 1 >= graf->dajBrojCvorova()) {
                drugiCvor = 0; prviCvor++;
            }
            else{
                drugiCvor++;
            }
        }
        
        return *this;
    }

    GranaIterator operator ++(int) {
        GranaIterator<tip> staraGrana = *this;
        ++(*this);
        return staraGrana;
    }

};

template <typename tip>
class MatricaGraf: public UsmjereniGraf<tip> {
    std::vector<std::vector<float>> matricaSusjedstva;
    float pocetnaTezina = std::numeric_limits<float>::infinity();

    std::vector<tip> oznakeCvorova;
    std::vector<std::vector<tip>> oznakeGrana;

    bool provjeraCvora(int indeks) const {
        if(indeks < 0 || indeks >= dajBrojCvorova()) return false;
        return true;
    }

public:
    MatricaGraf() {}
    ~MatricaGraf() {}
    MatricaGraf(int n);
    
    int dajBrojCvorova() const;
    void postaviBrojCvorova(int brojCvorova);
    void dodajGranu(int prviCvor, int drugiCvor, float tezina = 0);
    void obrisiGranu(int prviCvor, int drugiCvor);
    void postaviTezinuGrane(int prviCvor, int drugiCvor, float tezina = 0);
    float dajTezinuGrane(int prviCvor, int drugiCvor) const;
    bool postojiGrana(int prviCvor, int drugiCvor);
    void postaviOznakuCvora(int brojCvora, tip oznaka);
    tip dajOznakuCvora(int brojCvora) const;
    void postaviOznakuGrane(int prviCvor, int drugiCvor, tip oznaka);
    tip dajOznakuGrane(int prviCvor, int drugiCvor) const;

    Grana<tip> dajGranu(int prviCvor, int drugiCvor);
    Cvor<tip> dajCvor(int brojCvora);
    
};

template <typename tip>
MatricaGraf<tip>::MatricaGraf(int n) {
    if(n < 0) throw std::domain_error("Veličina je negativna!");

    matricaSusjedstva.resize(n);
    for(int i = 0; i < n; i++) matricaSusjedstva.at(i).resize(n, pocetnaTezina);

    oznakeCvorova.resize(n);

    tip nova = tip();
    oznakeGrana.resize(n);
    for(int i = 0; i < n; i++) oznakeGrana.at(i).resize(n, nova);
}

template <typename tip>
int MatricaGraf<tip>::dajBrojCvorova() const {
    return matricaSusjedstva.size();
}

template <typename tip>
void MatricaGraf<tip>::postaviBrojCvorova(int noviBrojCvorova) {
    if(noviBrojCvorova < matricaSusjedstva.size()) throw std::domain_error("Ne možete smanjiti veličinu matrice!");

    matricaSusjedstva.resize(noviBrojCvorova);
    for(int i = 0; i < noviBrojCvorova; i++) matricaSusjedstva.at(i).resize(noviBrojCvorova, pocetnaTezina);

    tip nova = tip();
    oznakeCvorova.resize(noviBrojCvorova, nova);
    oznakeGrana.resize(noviBrojCvorova);
    for(int i = 0; i < noviBrojCvorova; i++) oznakeGrana.at(i).resize(noviBrojCvorova, nova);
}

template <typename tip>
void MatricaGraf<tip>::dodajGranu(int prviCvor, int drugiCvor, float tezina) {
    if(provjeraCvora(prviCvor) && provjeraCvora(drugiCvor)) matricaSusjedstva.at(prviCvor).at(drugiCvor) = tezina;
}

template <typename tip>
void MatricaGraf<tip>::obrisiGranu(int prviCvor, int drugiCvor) {
    if(provjeraCvora(prviCvor) && provjeraCvora(drugiCvor)) matricaSusjedstva.at(prviCvor).at(drugiCvor) = pocetnaTezina;
}

template <typename tip>
void MatricaGraf<tip>::postaviTezinuGrane(int prviCvor, int drugiCvor, float tezina) {
    if(provjeraCvora(prviCvor) && provjeraCvora(drugiCvor)) matricaSusjedstva.at(prviCvor).at(drugiCvor) = tezina;
}

template <typename tip>
float MatricaGraf<tip>::dajTezinuGrane(int prviCvor, int drugiCvor) const {
    //if(matricaSusjedstva.at(prviCvor).at(drugiCvor) == pocetnaTezina) 
    return matricaSusjedstva.at(prviCvor).at(drugiCvor);
}

template <typename tip>
bool MatricaGraf<tip>::postojiGrana(int prviCvor, int drugiCvor) {
    if(matricaSusjedstva.at(prviCvor).at(drugiCvor) == pocetnaTezina) return false;
    return true;
}

template <typename tip>
void MatricaGraf<tip>::postaviOznakuCvora(int brojCvora, tip oznaka) {
    if(provjeraCvora(brojCvora)) oznakeCvorova.at(brojCvora) = oznaka;
}

template <typename tip>
tip MatricaGraf<tip>::dajOznakuCvora(int brojCvora) const {
    if(provjeraCvora(brojCvora)) return oznakeCvorova.at(brojCvora);
    tip nova = tip();
    return nova;
}

template <typename tip>
void MatricaGraf<tip>::postaviOznakuGrane(int prviCvor, int drugiCvor, tip oznaka) {
    if(provjeraCvora(prviCvor) && provjeraCvora(drugiCvor)) oznakeGrana.at(prviCvor).at(drugiCvor) = oznaka;
}

template <typename tip>
tip MatricaGraf<tip>::dajOznakuGrane(int prviCvor, int drugiCvor) const {
    if(provjeraCvora(prviCvor) && provjeraCvora(drugiCvor)) return oznakeGrana.at(prviCvor).at(drugiCvor);
    tip nova = tip();
    return nova;
}

template <typename tip>
Grana<tip> MatricaGraf<tip>::dajGranu(int prviCvor, int drugiCvor) {
    Grana<tip> grana(this, prviCvor, drugiCvor);
    return grana;
}

template <typename tip>
Cvor<tip> MatricaGraf<tip>::dajCvor(int brojCvora) {
    Cvor<tip> cvor(this, brojCvora);
    return cvor;
}



int main() {
    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 0, 1.2);
    g->dodajGranu(1, 2, 0.1);
    g->dodajGranu(0, 0, 3.14);
    auto iter = g->dajGranePocetak();
    auto iter2 = g->dajGraneKraj();
    if(iter == iter2)std::cout << "nijeok" <<std::endl;
    for (GranaIterator<bool> iter = g->dajGranePocetak(); iter != g->dajGraneKraj(); ++iter)
      std::cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << "," << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
    delete g;
    
    return 0;
}
