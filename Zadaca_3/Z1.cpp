#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <typename Tip> class Red {

  struct Cvor {
    Tip el;
    Cvor *sljedeci;

  public:
    Cvor(Tip el, Cvor *cvor) {
      this->el = el;
      sljedeci = cvor;
    };
  };

private:
  Cvor *pocetak;
  Cvor *kraj;
  int broj_elemenata;

public:
  Red() : pocetak(nullptr), kraj(nullptr), broj_elemenata(0){};

  ~Red() {
    if (pocetak != nullptr) {
      // pomjeramo pocetak, a on vuce p
      for (Cvor *p = pocetak; p != nullptr; p = pocetak) {
        pocetak = pocetak->sljedeci;
        delete p;
      }
    }
    pocetak = nullptr;
    kraj = nullptr;
    broj_elemenata = 0;
  }

  Red(const Red &red) {

    pocetak = new Cvor((red.pocetak)->el, nullptr);
    Cvor *p1 = (red.pocetak)->sljedeci;
    for (Cvor *p2 = pocetak; p1 != nullptr; p1 = p1->sljedeci) {
      Cvor *novi = new Cvor(p1->el, nullptr);
      p2->sljedeci = novi;
      p2 = p2->sljedeci;
      kraj = p2;
    }
    broj_elemenata = red.broj_elemenata;
  }

  Red &operator=(Red red) {

    std::swap(red.pocetak, pocetak);
    std::swap(red.kraj, kraj);
    std::swap(red.broj_elemenata, broj_elemenata);
    return *this;
  }

  void brisi() {

    if (pocetak != nullptr) {
      // pomjeramo pocetak, a on vuce p
      for (Cvor *p = pocetak; p != nullptr; p = pocetak) {
        pocetak = pocetak->sljedeci;
        delete p;
      }
    }
    pocetak = nullptr;
    kraj = nullptr;
    broj_elemenata = 0;
  }

  void stavi(const Tip &el) {

    Cvor *novi = new Cvor(el, nullptr);
    if (pocetak == nullptr) {
      pocetak = novi;
      kraj = novi;
    } else {
      kraj->sljedeci = novi;
      kraj = kraj->sljedeci;
    }
    broj_elemenata++;
  }
  Tip skini() {

    if (pocetak == nullptr)
      throw std::logic_error("Prazan red");
    Tip x = pocetak->el;
    Cvor *p = pocetak;
    if (pocetak == kraj) {
      pocetak = nullptr;
      kraj = nullptr;
    } else
      pocetak = pocetak->sljedeci;
    delete p;
    broj_elemenata--;
    return x;
  }

  Tip &celo() {
    if (pocetak == nullptr)
      throw std::logic_error("Prazan red");
    return pocetak->el;
  }

  int brojElemenata() { return broj_elemenata; }
};

int brojCifara(int n) {

  int broj = 0;
  while (n != 0) {
    n = n / 10;
    broj++;
  }
  return broj;
}

void radixSort(vector<int> &a) {

  // nadjimo  duzinu najveceg broja
  int maxBrojCifara = brojCifara(a.at(0));
  for (int i = 1; i < a.size(); i++) {
    if (maxBrojCifara < brojCifara(a.at(i)))
      maxBrojCifara = brojCifara(a.at(i));
  }

  // napravimo vektor stringova koji su svi iste duzine
  std::vector<std::string> v;
  for (int i = 0; i < a.size(); i++) {

    std::string s = "0";
    int brojNula = maxBrojCifara - brojCifara(a.at(i));

    if (brojNula == 0)
      v.push_back(to_string(a.at(i)));
    else {
      for (int j = 0; j < brojNula - 1; j++)
        s += "0";
      v.push_back(s + to_string(a.at(i)));
    }
  }

  std::vector<Red<std::string>> redovi;
  redovi.resize(10);

  for (int i = 0; i < maxBrojCifara; i++) {

    for (int j = 0; j < v.size(); j++) {

      char br = v.at(j).at(v.at(j).size() - 1 - i);
      int broj = br - '0';
      //??
      redovi.at(broj).stavi(v.at(j));
    }

    v.resize(0);

    for (int r = 0; r < redovi.size(); r++) {
      int vel = redovi.at(r).brojElemenata();
      for (int k = 0; k < vel; k++)
        v.push_back(redovi.at(r).skini());
    }
  }

  // na kraju ih vratimo u originalni vektor
  a.resize(0);

  for (int i = 0; i < v.size(); i++) {
    a.push_back(stoi(v.at(i)));
  }
}

// ZADATAK 2

int lijevoDijete(int i) { return 2 * i + 1; }

int desnoDijete(int i) { return 2 * i + 2; }

int roditelj(int i) { return floor((i - 1) / 2); }

bool daLiJeList(vector<int> &a, int i) {
  if (i < a.size() && i >= floor(a.size() / 2))
    return true;
  return false;
}

void popraviDole(vector<int> &a, int i) {

  while (daLiJeList(a, i) == false) {

    int veci = lijevoDijete(i);
    int desno = desnoDijete(i);
    if (desno < a.size() && a.at(veci) < a.at(desno))
      veci = desno;

    if (a.at(veci) < a.at(i))
      return;

    int temp = a.at(veci);
    a.at(veci) = a.at(i);
    a.at(i) = temp;
    i = veci;
  }
}

void stvoriGomilu(vector<int> &a) {

  for (int i = floor(a.size() / 2) - 1; i >= 0; i--) {
    popraviDole(a, i);
  }
}

void popraviGore(vector<int> &a, int i) {

  while (i != 0 && a.at(i) >= a.at(roditelj(i))) {
    int temp = a.at(i);
    a.at(i) = a.at(roditelj(i));
    a.at(roditelj(i)) = temp;
    i = roditelj(i);
  }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
  velicina++;
  a.resize(velicina);
  a[velicina - 1] = umetnuti;
  // a.push_back(umetnuti);
  popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {

  if (velicina == 0)
    throw std::domain_error("Gomila prazna!");
  int prvi = a.at(0);
  velicina--;
  int temp = a.at(0);
  a.at(0) = a[velicina];
  a[velicina] = temp;
  vector<int> kraj;
  for (int i = velicina; i < a.size(); i++)
    kraj.push_back(a.at(i));
  a.resize(velicina);
  if (velicina != 0)
    popraviDole(a, 0);
  // a+kraj
  for (int i = 0; i < kraj.size(); i++)
    a.push_back(kraj.at(i));
  return prvi;
}

void gomilaSort(vector<int> &a) {

  vector<int> rez;
  stvoriGomilu(a);
  int v = a.size();
  int vel = a.size();
  for (int i = 0; i < v - 1; i++) {
    rez.insert(rez.begin(), izbaciPrvi(a, vel));
  }
  rez.insert(rez.begin(), a.at(0));
  a.resize(0);
  a = rez;
}

int main() {
  vector<int> a;
  int velicina = 0;
  umetniUGomilu(a, 546, velicina);
  umetniUGomilu(a, 435435, velicina);
  umetniUGomilu(a, 12, velicina);
  umetniUGomilu(a, 546, velicina);
  umetniUGomilu(a, 675767, velicina);
  umetniUGomilu(a, 33, velicina);
  umetniUGomilu(a, 555, velicina);
  umetniUGomilu(a, 1, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  izbaciPrvi(a, velicina);
  for (int i(0); i < a.size(); i++) {
    cout << a[i] << velicina;
  }
  return 0;
}