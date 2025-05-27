#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename Tip> class DvostraniRed {

  struct Cvor {
    Tip el;
    Cvor *sljedeci;
    Cvor *prethodni;
    Cvor(Tip el, Cvor *sljedeci, Cvor *prethodni)
        : el(el), sljedeci(sljedeci), prethodni(prethodni){};
  };

private:
  int broj_elemenata;
  Cvor *pocetak;
  Cvor *kraj;

public:
  DvostraniRed() : broj_elemenata(0), pocetak(nullptr), kraj(nullptr){};
  ~DvostraniRed() {

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

  DvostraniRed(const DvostraniRed &red) {

    pocetak = new Cvor((red.pocetak)->el, nullptr, nullptr);
    Cvor *p1 = (red.pocetak)->sljedeci;
    for (Cvor *p2 = pocetak; p1 != nullptr; p1 = p1->sljedeci) {
      Cvor *novi = new Cvor(p1->el, nullptr, nullptr);
      novi->prethodni = p2;
      p2->sljedeci = novi;
      p2 = p2->sljedeci;
      kraj = p2;
    }
    broj_elemenata = red.broj_elemenata;
  }

  DvostraniRed &operator=(DvostraniRed red) {
    std::swap(red.pocetak, pocetak);
    std::swap(red.kraj, kraj);
    std::swap(red.broj_elemenata, broj_elemenata);
    return *this;
  }

  int brojElemenata() { return broj_elemenata; };

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

  void staviNaVrh(const Tip &el) {

    Cvor *novi = new Cvor(el, nullptr, nullptr);

    if (pocetak == nullptr) {
      pocetak = novi;
      kraj = novi;
    } else {
      novi->prethodni = kraj;
      kraj->sljedeci = novi;
      kraj = kraj->sljedeci;
    }
    broj_elemenata++;
  }

  Tip skiniSaVrha() {

    if (kraj == nullptr)
      throw std::logic_error("Prazan red");

    Tip x = kraj->el;
    Cvor *p = kraj;
    if (pocetak == kraj) {
      pocetak = nullptr;
      kraj = nullptr;
    } else {
      kraj = kraj->prethodni;
      kraj->sljedeci = nullptr;
    }
    delete p;
    broj_elemenata--;
    return x;
  }

  void staviNaCelo(const Tip &el) {

    Cvor *novi = new Cvor(el, nullptr, nullptr);

    if (pocetak == nullptr) {
      pocetak = novi;
      kraj = novi;
    } else {
      novi->sljedeci = pocetak;
      pocetak->prethodni = novi;
      pocetak = novi;
    }
    broj_elemenata++;
  }

  Tip skiniSaCela() {

    if (pocetak == nullptr)
      throw std::logic_error("Prazan red");

    Tip x = pocetak->el;
    Cvor *p = pocetak;

    if (pocetak == kraj) {
      pocetak = nullptr;
      kraj = nullptr;
    } else {
      pocetak = pocetak->sljedeci;
      pocetak->prethodni = nullptr;
    }
    delete p;
    broj_elemenata--;
    return x;
  }

  Tip vrh() {
    if (kraj == nullptr)
      throw std::logic_error("Prazan red");
    return kraj->el;
  }
  Tip celo() {
    if (pocetak == nullptr)
      throw std::logic_error("Prazan red");
    return pocetak->el;
  }
};

void testStaviNaVrh() {
  DvostraniRed<int> red;
  red.staviNaVrh(2);
  if (red.vrh() == 2)
    std::cout << "OK";
  else
    std::cout << "Nije OK";
}

void testSkiniSaVrha() {
  DvostraniRed<int> red;
  red.staviNaVrh(2);
  red.skiniSaVrha();
  if (red.brojElemenata() == 0)
    std::cout << "OK";
  else
    std::cout << "Nije OK";
}

void testStaviNaCelo() {
  DvostraniRed<int> red;
  red.staviNaCelo(2);
  if (red.celo() == 2)
    std::cout << "OK";
  else
    std::cout << "Nije OK";
}

void testSkiniSaCela() {
  DvostraniRed<int> red;
  red.staviNaCelo(2);
  red.skiniSaCela();
  if (red.brojElemenata() == 0)
    std::cout << "OK";
  else
    std::cout << "Nije OK";
}

template <typename Tip> class Stek {
private:
  Tip *stek;
  int vrh_;
  int kapacitet;

public:
  Stek(int kapacitet = 1000)
      : vrh_(-1), kapacitet(kapacitet), stek(new Tip[kapacitet]){};
  ~Stek() { delete[] stek; };
  Stek(const Stek &kopija) {

    stek = new Tip[kopija.kapacitet];
    Tip *stek2 = stek;
    Tip *kopija2 = kopija.stek;
    vrh_ = kopija.vrh_;
    for (int i = 0; i <= vrh_; i++)
      stek2[i] = kopija2[i];
  };

  Stek<Tip> &operator=(Stek kopija) {
    std::swap(stek, kopija.stek);
    std::swap(vrh_, kopija.vrh_);
    return *this;
  };

  void brisi() { vrh_ = -1; };

  void stavi(const Tip &el) {

    if (vrh_ + 1 == kapacitet) {

      Tip *novi = new Tip[2 * kapacitet];
      kapacitet *= 2;
      Tip *p = stek;
      Tip *novi2 = novi;
      for (int i = 0; i <= vrh_; i++) {
        novi2[i] = p[i];
      }
      delete[] stek;
      stek = novi;
    }
    vrh_++;
    stek[vrh_] = el;
  };

  Tip skini() {

    if (vrh_ == -1)
      throw std::domain_error("Prazak stek");
    Tip x = stek[vrh_];
    vrh_--;
    return x;
  };

  Tip &vrh() {
    if (vrh_ == -1)
      throw std::domain_error("Prazak stek");
    return stek[vrh_];
  };

  int brojElemenata() { return vrh_ + 1; };
};

void pretraga(Stek<std::vector<int>> &s, int trazeni) {

  if (s.brojElemenata() == 0) {
    std::cout << "Nema elementa";
    return;
  }

  else {

    std::vector<int> v = s.skini();

    if (v.size() == 0 || v.at(0) > trazeni || v.at(v.size() - 1) < trazeni) {
      pretraga(s, trazeni);
      s.stavi(v);
    } else {

      // binarna pretraga

      int dno = 0;
      int vrh = v.size() - 1;
      bool pronadjen = false;
      while (vrh >= dno) {
        int srednji = (dno + vrh) / 2;
        if (v.at(srednji) == trazeni) {
          std::cout << srednji << " " << s.brojElemenata();
          pronadjen = true;
          break;
        } else if (v.at(srednji) < trazeni)
          dno = srednji + 1;
        else
          vrh = srednji - 1;
      }
      if (pronadjen == false)
        std::cout << "Nema elementa";
       s.stavi(v);
    }
  }
}

int main() {
 
  testStaviNaVrh();
  testSkiniSaVrha();
  testStaviNaCelo();
  testSkiniSaCela();
  return 0;
}