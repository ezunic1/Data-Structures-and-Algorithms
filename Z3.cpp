#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

int dajCifru(int br, int pom){
    return (br / pom) % 10;
}

void countingSort(vector<int> &a, int pom){
    int vel = a.size();
    std::vector<int> b(vel);
    std::vector<int> c(10);
    for(int i = 0; i < vel; i++){
        int cifra = dajCifru(a.at(i), pom);
        c.at(cifra)++;
    }
    for(int i = 1; i < 10; i++){
        c.at(i)+= c.at(i-1);
    }
    for(int i = vel - 1; i >= 0; i --){
        int cifra = dajCifru(a.at(i), pom);
        b.at(c.at(cifra) - 1) = a.at(i);
        c.at(cifra)--;
    }
    a = b;
}

void radixSort(vector<int> &a){
    int max = a.at(0);
    for(int i = 1; i < a.size(); i++){
        if(a.at(i) > max) max = a.at(i);
    }
    for(int i = 1; max / i > 0; i*=10){
        countingSort(a, i);
    } 
}
int LijevoDijete(int i)  { // Pozicija lijevog djeteta
    return 2*i+1;
}
int DesnoDijete(int i) { // Pozicija desnog djeteta
    return 2*i+2;
}
int Roditelj(int i)  { // Pozicija roditelja
    return (i-1)/2;
} 
bool JeLiList(int i, int velicina)  {
    return (i >= velicina/2) && (i < velicina);
} 

void PopraviDolje(vector<int> &v , int i, int velicina){
    while(!JeLiList(i, velicina)){
        int veci = LijevoDijete(i);
        int dd = DesnoDijete(i);
        if((dd <velicina) && v.at(dd) > v.at(veci)) veci = dd;
        if(v.at(i) > v.at(veci)) return;
        swap(v.at(i), v.at(veci));
        i = veci;
    }
}

void PopraviGore(vector<int> &v , int i){
    while((i != 0) && v.at(i) > v.at(Roditelj(i))){
        swap(v.at(i), v.at(Roditelj(i)));
        i = Roditelj(i);
    }
}


void stvoriGomilu(vector<int> &a){
    int velicina = a.size();
    for(int i = velicina / 2 - 1; i >= 0; i--){
        PopraviDolje(a,  i, velicina);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    velicina++;
    PopraviGore(a, velicina - 1);
}
int izbaciPrvi(vector<int> &a, int &velicina){
    if(a.size()==0)std::__throw_domain_error("Gomila je prazna");
    --velicina;
    swap(a.at(0), a.at(velicina));
    if(velicina != 0)
        PopraviDolje(a, 0, velicina);
    return a.at(velicina);
}

void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    int n = a.size();
    for(int i=n-1;i>0;i--){
            swap(a.at(0), a.at(i));
            PopraviDolje(a, 0, i);
        }
}
int main(){
    return 0;
}
