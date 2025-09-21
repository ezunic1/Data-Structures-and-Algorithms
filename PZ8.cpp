#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

template<typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        for (int j = 0; j < vel - i - 1; j++) {
            if (niz[j] > niz[j+1]) {
                auto pom = niz[j];
                niz[j] = niz[j+1];
                niz[j+1] = pom;
            }
        }
    }
}

template<typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i = 0; i < vel - 1; i++){
        Tip min = niz[i];
        int pmin = i;
        for(int j = i+1; j < vel; j++){
            if(niz[j] < min){
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }

}
template <typename Tip> 
int particija(Tip* niz, int prvi, int vel) {
    Tip pivot = niz[prvi];
    int p = prvi + 1;
    while (p <= vel) {
        if (niz[p] <= pivot)
        p++;
        else
        break;
    }
    for (int i = p+1; i <= vel; i++) {
        if (niz[i] < pivot) {
            auto pom = niz[i];
            niz[i] = niz[p];
            niz[p] = pom;
            p++;
        }
    }
    auto pom = niz[prvi];
    niz[prvi] = niz[p-1];
    niz[p-1] = pom;
    return p-1;
}

template<typename Tip>
void quick_sort(Tip* niz, int vel, int p = 0, bool prvi = true){
    if(prvi) vel--;
    if (p < vel) {
        int j = particija(niz, p, vel);
        quick_sort(niz, j - 1, p, false);
        quick_sort(niz, vel, j + 1, false);
    }
}

template <typename Tip>
void merge(Tip* niz, int l, int p, int q, int vel) {
    int i = 0;
    int j = q-l;
    int k = l;
    Tip* nizB = new Tip[vel-l+1]{};
    for(int m = 0; m <= vel-l; m++){
        nizB[m] = niz[l+m];
    }
    while(i <= p - l && j <= vel-l){
        if(nizB[i] < nizB[j]){
            niz[k] = nizB[i++]; 
        }else{
            niz[k] = nizB[j++];
        }
        k++;
    }
    while(i <= p-l){
        niz[k++] = nizB[i++];
        
    }
    while(j <= vel-l){
        niz[k++] = nizB[j++];
        
    }
    delete[] nizB;
}

template <typename Tip>
void merge_sort(Tip* niz, int vel, int l = 0, bool prvi = true) {
    if(prvi) vel--; 
    if(vel > l) {
        int p = (vel+l-1)/2;
        int q = p+1;
        merge_sort(niz, p, l, false);
        merge_sort(niz, vel, q, false);
        merge(niz, l, p, q, vel);
    }
}

void ucitaj(std::string filename, int*& niz, int &vel) {
    std::ifstream ulazniTok;
    ulazniTok.open(filename);
    std::string sadrzaj;
    char znak;
    while((znak = ulazniTok.get()) != EOF) {
        sadrzaj.push_back(znak);
    }
    ulazniTok.close();
    vel = 0;

   for(int i = 0; i < sadrzaj.length(); i++) {
        if(sadrzaj.at(i) != ' ' && sadrzaj.at(i) != '\n') {
            std::string broj = "";
            while(i < sadrzaj.length() && sadrzaj.at(i) != ' ' && sadrzaj.at(i) != '\n' && sadrzaj.at(i) >= '0' && sadrzaj.at(i) <= '9') {
                broj.push_back(sadrzaj.at(i));
                i++;
            }
            vel++;
        }
    }

    int* novi = new int[vel];
    int indeks = 0;
    for(int i = 0; i < sadrzaj.length(); i++) {
        if(sadrzaj.at(i) != ' ' && sadrzaj.at(i) != '\n') {
            std::string broj = "";
        
            while(i < sadrzaj.length() && sadrzaj.at(i) != ' ' && sadrzaj.at(i) != '\n' && sadrzaj.at(i) >= '0' && sadrzaj.at(i) <= '9') {
                broj.push_back(sadrzaj.at(i));
                i++;
            }

            i--;
            novi[indeks++] = std::stoi(broj);
        }
    }
    niz = novi;
    
}

void poziv(std::string naziv, int* niz, int vel) {
    ucitaj(naziv, niz, vel);
    int opcija = 0;
    
    std::cout << "1 - Bubble Sort\n";
    std::cout << "2 - Selection Sort\n";
    std::cout << "3 - Quick Sort\n";
    std::cout << "4 - Merge Sort\n";
    std::cout << "Izaberite algoritam kojim želite da se sortira niz: ";
    std::cin >> opcija;

    clock_t vrijeme1 = clock();
    clock_t vrijeme2;
    if(opcija == 1) {
        bubble_sort(niz, vel);
        vrijeme2 = clock();
    }
    else if(opcija == 2) {
        selection_sort(niz, vel);
        vrijeme2 = clock();
    }
    else if(opcija == 3) {
        quick_sort(niz, vel);
        vrijeme2 = clock(); 
    }
    else if(opcija == 4) {
        merge_sort(niz, vel); 
        vrijeme2 = clock();
    }
    else {
        std::cout << "Opcija koju ste unijeli ne postoji!\n";  
        return;
    }

    std::cout << "\nVrijeme izvršenja: " << (vrijeme2 - vrijeme1)/(CLOCKS_PER_SEC/1000) << " ms" << std::endl;

    bool provjera = true;
    for(int i = 1; i < vel; i++) {
        if(niz[i] < niz[i-1]) provjera = false;
    }

    if(provjera) std::cout << "Uspješno sortiranje!\n";
    else std::cout << "Sortiranje nije uspjelo!\n";

    std::cout << "Unesite naziv datoteke (naziv.txt) u koju želite da se upiše sortirani niz: ";
    std::string izlaz;
    std::cin >> izlaz;

    std::ofstream izlazniTok(izlaz);
    for(int i = 0; i < vel; i++) {
        izlazniTok << niz[i] << " ";
    }
}

void generisi(std::string filename, int vel) {
    std::ofstream izlazniTok(filename);
    for(int i = 0; i < vel; i++) {
        izlazniTok << rand()%10000 << " ";
    }
}

int main(){
    int niz[5] = {3, 2, 8, 1, 5};
    quick_sort(niz, 5);
    for (int i(0); i<5; i++)
        cout << niz[i] << " ";
    return 0;
}
