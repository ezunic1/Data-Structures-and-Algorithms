#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <functional>
using namespace std;

using sat = std::chrono::high_resolution_clock;

template <typename tipK, typename tipV> class Mapa {

public:
  Mapa<tipK, tipV>() {}
  virtual ~Mapa<tipK, tipV>() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const tipK &kljuc) = 0;
  virtual tipV &operator[](tipK i) = 0;
  virtual const tipV operator[](tipK i) const = 0;
};

template <typename tipK, typename tipV>
class AVLStabloMapa : public Mapa<tipK, tipV> {
    struct Cvor {
        tipK kljuc;
        tipV vrijednost;
        Cvor *lijevi, *desni, *roditelj;
        int balans;
        Cvor(tipK i = tipK(), tipV u = tipV())
            : kljuc(i), vrijednost(u), desni(nullptr), lijevi(nullptr),
            roditelj(nullptr), balans(0) {}
    };
    Cvor *novi(tipK i = tipK(), tipV u = tipV()) {
        Cvor *temp = new Cvor();
        temp->kljuc = i;
        temp->vrijednost = u;
        temp->desni = temp->lijevi = temp->roditelj = nullptr;
        temp->balans = 0;
        return temp;
    }
    void desno_rotiranje(Cvor *t) {
        Cvor *q = nullptr;
        int o = 0;
        q = t->lijevi->desni;
        t->balans = 0;
        Cvor *l = t->lijevi;
        Cvor *r = t->roditelj;
        l->roditelj = r;
        l->desni = t;
        if (!l->roditelj && !o) {
            p = l;
            o = 1;
        } else if (r->desni == t && !o) {
            l->roditelj->desni = l;
            o = 1;
        }

        if (o == 0) {
            l->roditelj->lijevi = l;
        }
        l->balans = 0;
        if (q)
             q->roditelj = t;
        t->lijevi = q;
        t->roditelj = l;
    }


    void lijevo_rotiranje(Cvor *t) {
        Cvor *q = nullptr;
        int o = 0;
        q = t->desni->lijevi;
        t->balans = 0;
        Cvor *d = t->desni;
        Cvor *r = t->roditelj;
        d->roditelj = r;
        d->lijevi = t;
        if (!d->roditelj && !o) {
            p = d;
            o = 1;
        } else if (r->desni == t && !o) {
            d->roditelj->desni = d;
            o = 1;
        }
        if (o == 0) {
            d->roditelj->lijevi = d;
        }
        d->balans = 0;
        if (q)
            q->roditelj = t;
        t->desni = q;
        t->roditelj = d;
    }

    void popraviBalans(Cvor *t, bool b) {
        if (t == nullptr)
        return;
        else if (b)
        t->balans--;
        else
        t->balans++;
        int blns = t->balans;
        if (t->roditelj) {
            if (t->roditelj->desni == t)
                b = true;
            else
                b = false;
        }
        if (blns == 2) {
            if (t->lijevi->balans== 1)
                desno_rotiranje(t);
            else {
                lijevo_rotiranje(t->lijevi);
                desno_rotiranje(t);
            }
        } else if (blns == -2) {
            if (t->desni->balans == -1)
                lijevo_rotiranje(t);

            else {
                desno_rotiranje(t->desni);
                lijevo_rotiranje(t);
            }
        } else {
            if (blns == 0)
                return;
            popraviBalans(t->roditelj, b);
        }
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
        while (p) {
        if (p->kljuc == i)
            return p;
        else if (i > p->kljuc)
            p = p->desni;
        else
            p = p->lijevi;
        }
        if (!p) {
            Cvor *t = AVLStabloMapa::p, *q = nullptr;
            p = novi(i, u);
            while (t) {
                q = t;
                if (t->kljuc > p->kljuc)
                t = t->lijevi;
                else
                t = t->desni;
            }
            if (!q)
                AVLStabloMapa::p = p;
            else if (q->kljuc > p->kljuc) {
                q->lijevi = p;
                p->roditelj = q;
            } else {
                q->desni = p;
                p->roditelj = q;
            }

            if (!q)
                return p;

            if (q->lijevi && q->desni)
                q->balans = 0;
            else {
                bool b = true;
                if (q->lijevi)
                b = false;
                popraviBalans(q, b);
            }
        }
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

    void obrisiC(Cvor *t, tipK i) {
        Cvor *r = nullptr;
        while (t != nullptr && t->kljuc != i) {
        r = t;
        if (t->kljuc < i)
            t = t->desni;
        else
            t = t->lijevi;
        }
        if (t == nullptr)
        return;
        Cvor *e, *q, *s ;
        if (!t->lijevi){
        e = t->desni;
        s = r;}
        else if (!t->desni){
        e = t->lijevi;
        s = r;}
        else {
        e = t->lijevi;
        q = e->desni;
        s = t;
        while (q) {
            s = e;
            e = q;
            q = e->desni;
        }
        if (t != s) {
            s->desni = e->lijevi;
            if (e->lijevi)
            e->lijevi->roditelj = s;
            e->lijevi = t->lijevi;
            if (t->lijevi)
            t->lijevi->roditelj = e;
        }
        e->desni = t->desni;
        if (t->desni)
            t->desni->roditelj = e;
        }
        if (!r) {
            AVLStabloMapa::p = e;
        } else {
            if (r->lijevi == t) {
                r->lijevi = e;
                r->balans--;
            }else {
                r->desni = e;
                r->balans++;
            }
        }
        if (e)  e->roditelj = r;
        t->desni = t->lijevi = t->roditelj = nullptr;
        delete t;   
    }
    void obrisi_stablo(Cvor *t) {
        if (t == nullptr)
        return;
        obrisi_stablo(t->lijevi);
        obrisi_stablo(t->desni);
        delete t;
    }
    int br_ele;
    Cvor *p;
    public:
    AVLStabloMapa<tipK, tipV>() : br_ele(0), p(nullptr) {}
    ~AVLStabloMapa<tipK, tipV>() { if(p) obrisi_stablo(p); }
    AVLStabloMapa<tipK, tipV>(const AVLStabloMapa<tipK, tipV> &a) {
        br_ele = a.br_ele;
        if (a.br_ele == 0)
        p = nullptr;
        else {
        p = novi(a.p->kljuc, a.p->vrijednost);
        konstruktor(p, a.p);
        }
    }
    AVLStabloMapa<tipK, tipV> &operator=(const AVLStabloMapa<tipK, tipV> &a) {
        if (this == &a)
        return *this;
        obrisi();
        br_ele = a.br_ele;
        if (a.br_ele == 0)
        p = nullptr;
        else {
        if(!p)p = novi(a.p->kljuc,a.p->vrijednost);
        p->kljuc = a.p->kljuc;
        p->vrijednost = a.p->vrijednost;
        konstruktor(p, a.p);
        }
        return *this;
    }
    int brojElemenata() const { return br_ele; }
    void obrisi() {
        obrisi_stablo(p);
        p = nullptr;
        br_ele = 0;
    }
    void obrisi(const tipK &kljuc) {

        obrisiC(p, kljuc);

        br_ele--;
    }
    tipV &operator[](tipK i) {

        if (p == nullptr) {
        p = novi(i);
        br_ele++;
        return p->vrijednost;
        }

        auto c = pretraga(p, i);

        if (c == nullptr) {
        dodaj(p, i);
        br_ele++;
        c = pretraga(p, i);
        }

        return c->vrijednost;
    }

    const tipV operator[](tipK i) const {
        auto c = pretraga(p, i);
        if (c != nullptr)
        return c->vrijednost;
        return tipV();
    }
    void Preorder()const;
};

template <typename tipK, typename tipV>
void AVLStabloMapa<tipK, tipV>::Preorder() const {
    auto current = p;
    std::function<void(Cvor*)> obilazak = [&](Cvor *cvor) {
        if (!cvor) return;
        cout << cvor->kljuc << ","; 
        obilazak(cvor->lijevi);  
        obilazak(cvor->desni);  
    };
    obilazak(p); 
    cout << endl;
}

void t1() {
    AVLStabloMapa<int, int> avl;
    for (int i = 1; i <= 10; i++) {
        avl[i] = i * 10; 
    }
    cout << "t1 - Preorder obilazak:" << endl;
    avl.Preorder();
}

void t2() {
    AVLStabloMapa<int, int> avl;
    for (int i = 10; i >= 1; i--) {
        avl[i] = i * 10; 
    }
    cout << "t2 - Preorder obilazak:" << endl;
    avl.Preorder();
}

void t3() {
    AVLStabloMapa<int, int> avl;
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0)
            avl[i] = i * 10; 
        else
            avl[i] = i;
    }
    cout << "t3 - Preorder obilazak:" << endl;
    avl.Preorder();
}

void t4() {
    AVLStabloMapa<int, int> avl;
    for (int i = 5; i <= 14; i++) {
        avl[i] = i;
    }
    cout << "t4 - Preorder obilazak:" << endl;
    avl.Preorder();
}

void t5() {
    AVLStabloMapa<int, int> avl;
    for (int i = 1; i <= 5; i++) {
        avl[i] = i; 
    }
    for (int i = 6; i <= 10; i++) {
        avl[i] = i * 2;
    }
    cout << "t5 - Preorder obilazak:" << endl;
    avl.Preorder();
}

int main() {
    t1();
    t2();
    t3();
    t4();
    t5();
    return 0;
}
