#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename tipK, typename tipV>
class HashMapaLan {
private:
    struct Par {
        tipK kljuc;
        tipV vrijednost;
        Par(tipK k, tipV v) : kljuc(k), vrijednost(v) {}
    };

    std::vector<std::list<Par>> tabela;
    unsigned int (*hashFunkcija)(tipK, unsigned int);
    int velicina;
    int brElemenata;

public:
    HashMapaLan(int velicina = 100) : tabela(velicina), velicina(velicina), brElemenata(0), hashFunkcija(nullptr) {}

    ~HashMapaLan() {
        obrisi();
    }

    void definisiHashFunkciju(unsigned int (*funkcija)(tipK, unsigned int)) {
        hashFunkcija = funkcija;
    }

    void obrisi() {
        for (auto &lista : tabela) {
            lista.clear();
        }
        brElemenata = 0;
    }

    void obrisi(const tipK &kljuc) {
        if (!hashFunkcija) throw std::logic_error("Hash funkcija nije definisana.");
        unsigned int indeks = hashFunkcija(kljuc, velicina);
        auto &lista = tabela[indeks];

        auto it = std::find_if(lista.begin(), lista.end(), [&kljuc](const Par &par) {
            return par.kljuc == kljuc;
        });

        if (it != lista.end()) {
            lista.erase(it);
            brElemenata--;
        }
    }

    tipV &operator[](const tipK &kljuc) {
        if (!hashFunkcija) throw std::logic_error("Hash funkcija nije definisana.");
        unsigned int indeks = hashFunkcija(kljuc, velicina);
        auto &lista = tabela[indeks];

        auto it = std::find_if(lista.begin(), lista.end(), [&kljuc](const Par &par) {
            return par.kljuc == kljuc;
        });

        if (it != lista.end()) {
            return it->vrijednost;
        } else {
            lista.emplace_back(kljuc, tipV());
            brElemenata++;
            return lista.back().vrijednost;
        }
    }

    tipV operator[](const tipK &kljuc) const {
        if (!hashFunkcija) throw std::logic_error("Hash funkcija nije definisana.");
        unsigned int indeks = hashFunkcija(kljuc, velicina);
        const auto &lista = tabela[indeks];

        auto it = std::find_if(lista.begin(), lista.end(), [&kljuc](const Par &par) {
            return par.kljuc == kljuc;
        });

        if (it != lista.end()) {
            return it->vrijednost;
        } else {
            return tipV();
        }
    }

    int brojElemenata() const {
        return brElemenata;
    }
};

template <typename spp> class GranaIterator;
template <typename spp> class Grana;
template <typename spp> class Cvor;
template <typename spp> class ListaGraf;

template <typename spp> class UsmjereniGraf {
public:
  UsmjereniGraf() {}
  UsmjereniGraf(int a) {}
  virtual ~UsmjereniGraf() {}
  
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int a)  = 0;
  virtual void dodajGranu(int p, int d, float t)  = 0;
  virtual void obrisiGranu(int p, int d)  = 0;
  virtual bool  postojiGrana(int p, int d)  = 0;
  virtual void postaviTezinuGrane(int p, int d, float t) = 0;
  virtual spp dajOznakuGrane(int p, int d)  = 0;
  virtual spp dajOznakuCvora(int b)  = 0;
  virtual float dajTezinuGrane(int p, int d) = 0;
  virtual void postaviOznakuCvora(int b, spp a)  = 0;
  virtual void postaviOznakuGrane(int p, int d, spp a)  = 0;
   Grana<spp> dajGranu(int p, int d) {return Grana<spp>(this,p,d);} 
   Cvor<spp> dajCvor(int b) { return Cvor<spp>(this,b); }

   GranaIterator<spp> dajGranePocetak() {
    GranaIterator<spp> it(this,0,-1);
    return ++it;
  }

   GranaIterator<spp> dajGraneKraj() {
    GranaIterator<spp> it(this,-1,-1);
    return it;
  }
  virtual GranaIterator<spp> dajSljedecuGranu(int p, int d) = 0;
};

template <typename spp> class Cvor  {

  UsmjereniGraf<spp> *u;
  spp oznaka;
  int broj;
  std::vector<Grana<spp>> grane;
  std::vector<Cvor<spp>> susjedi;

public:

  Cvor(UsmjereniGraf<spp> *a, int b) : u(a), broj(b) {}

  int dajRedniBroj() const { return broj; }

  void postaviOznaku(spp a) { u->postaviOznakuCvora(broj, a); }

  spp dajOznaku() const { return u->dajOznakuCvora(broj); }

  void dodajVezu(Grana<spp> g, Cvor<spp> s) {
    grane.push_back(g);
    susjedi.push_back(s);
  }
  void obrisiVezu(Grana<spp> g, Cvor<spp> s) {

    for (int i = 0; i < susjedi.size(); i++)
      if (s == susjedi[i]) {

        for (int j = i; j < susjedi.size() - 1; j++) {
          susjedi[j] = susjedi[j + 1];
          grane[j] = grane[j + 1];
        }
        break;
      }
    susjedi.pop_back();
    grane.pop_back();
  }

  std::vector<Grana<spp>> dajGrane() const { return grane; }
  std::vector<Cvor<spp>> dajSusjede() const { return susjedi; }
};

template <typename spp> class Grana  {

  UsmjereniGraf<spp> *u;
  spp oznaka;
  float tezina;
  int p, d;

public:
  Grana(UsmjereniGraf<spp> *ug, int polazni, int dolazni)
      : u(ug), p(polazni), d(dolazni){ }
  float dajTezinu() const { return u->dajTezinuGrane(p,d); }
  void postaviTezinu(float a) {u->postaviTezinuGrane(
      p, d, a);}
  void postaviOznaku(spp a) {
   u->postaviOznakuGrane(p, d, a); }
  spp dajOznaku() const { return u->dajOznakuGrane(p,d); }
  Cvor<spp> dajPolazniCvor() const { return u->dajCvor(p); };
  Cvor<spp> dajDolazniCvor() const { return u->dajCvor(d); };
};

template <typename spp> class GranaIterator {

  int p, d;
  UsmjereniGraf<spp> *u;

public:
  GranaIterator( UsmjereniGraf<spp> *us, int a, int b):p(a),d(b),u(us){};

 Grana<spp> operator*(){return u->dajGranu(p,d);}
 bool operator==(const GranaIterator &it)const{
     return it.u == u && it.p == p && it.d == d;
 }
 bool operator!=(const GranaIterator &it)const{
     return it.u != u || it.p != p || it.d != d;
 }
 GranaIterator &operator++(){
     GranaIterator<spp> sljedeci = u->dajSljedecuGranu(p,d);
     p  = sljedeci.p;
     d = sljedeci.d;
     return *this;
 }
 GranaIterator operator++(int){
     GranaIterator<spp> ovaj = *this;
     ++(*this);
     return ovaj;
 }

};


template <typename spp> class ListaGraf : public UsmjereniGraf<spp> {

  struct grana {
      spp oznaka;
      float tezina;
      bool postoji;
  };
    std::vector<std::vector<grana>> lista;
    std::vector<spp> oznake;

public:
  ListaGraf(int b) : UsmjereniGraf<spp>(b) {
    postaviBrojCvorova(b);
  }
     int dajBrojCvorova() const override{
        return lista.size();}


   void postaviBrojCvorova(int a) override{
       grana g;
       g.postoji = false;
       for(int i = 0 ; i < lista.size() ; i++)
       lista[i].resize(a,g);
       std::vector<grana> ubaci(a,g);
       lista.resize(a,ubaci);
       oznake.resize(a);
   }
   bool postojiGrana(int p, int d) override{
       return lista[p][d].postoji;
   }

   void dodajGranu(int p, int d, float t) override {
       grana g; g.tezina = t; g.postoji = true; lista[p][d] = g;
   } 
   void obrisiGranu(int p, int d) override {
        lista[p][d].postoji = false;
   }
   void postaviTezinuGrane(int p, int d, float t) override {
       lista[p][d].tezina = t;
   } 
   spp dajOznakuCvora(int b) override {
       return oznake[b];
  }
  
   spp dajOznakuGrane(int p, int d) override {
      return lista[p][d].oznaka;
  }
   float dajTezinuGrane(int p, int d) override {
       return lista[p][d].tezina;
   }
   void postaviOznakuCvora(int b, spp a) override{
       oznake[b] = a;
   }
   void postaviOznakuGrane(int p, int d, spp a)override {
      lista[p][d].oznaka = a; 
  }
  GranaIterator<spp> dajGranePocetak() { GranaIterator<spp> it(this, 0, -1); ++it; return it; } 
  GranaIterator<spp> dajGraneKraj() { GranaIterator<spp> it(this, -1, -1); return it; } 
  
  GranaIterator<spp> dajSljedecuGranu(int p, int d)override{
      for(int i=p;i<lista.size();i++){
          for(int j=0;j<lista.size();j++){
              if(i == p && j <= d)continue;

              if(lista[i][j].postoji)
              return GranaIterator<spp>(this,i,j);
          }
      }
        return GranaIterator<spp>(this,-1,-1);
  }
  
};

template <typename spp>
void bfs(UsmjereniGraf<spp> *g, std::vector<Cvor<spp>> &b, Cvor<spp> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (b[i].dajRedniBroj() == c.dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int br = g->dajBrojCvorova();
  for (int i = 0; i < br; i++)
    if (!dodati)
      bfs(g, b, g->dajCvor(i));
}

template <typename spp>
void dfs(UsmjereniGraf<spp> *g, std::vector<Cvor<spp>> &b, Cvor<spp> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (c.dajRedniBroj() == b[i].dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int brojCvorova = g->dajBrojCvorova();
  for (int i = 0; i < brojCvorova; i++)
    if (g->postojiGrana(c.dajRedniBroj(), i) && !dodati)
      dfs(g, b, g->dajCvor(i));
}

int main() {

  return 0;
}
