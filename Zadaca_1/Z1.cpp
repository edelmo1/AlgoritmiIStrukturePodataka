#include <iostream>
#include <string>

using namespace std;

template <typename Tip> class Iterator;

template <typename Tip> struct Cvor {
  Tip element;
  Cvor *prethodni;
  Cvor *sljedeci;
  Cvor(Tip el, Cvor *prethodni, Cvor *sljedeci) {
    element = el;
    this->prethodni = prethodni;
    this->sljedeci = sljedeci;
  };
};

template <typename Tip> class Lista {
public:
  Lista() = default;
  virtual ~Lista() = default;
  virtual int brojElemenata() const = 0;
  virtual Tip trenutni() const = 0;
  virtual Tip &trenutni() = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip operator[](int i) const = 0;
  virtual Tip &operator[](int i) = 0;
  virtual void Prikazi() const = 0;
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
public:
  struct Cvor {
    Tip element;
    Cvor *prethodni;
    Cvor *sljedeci;
    Cvor(Tip el, Cvor *prethodni, Cvor *sljedeci) {
      element = el;
      this->prethodni = prethodni;
      this->sljedeci = sljedeci;
    };
  };

private:
  Cvor *pocetni;
  Cvor *krajnji;
  Cvor *tekuci;
  int lduzina, dduzina;

public:
  DvostrukaLista()
      : Lista<Tip>(), lduzina(0), dduzina(0),
        pocetni(new Cvor(Tip(), nullptr, nullptr)),
        krajnji(new Cvor(Tip(), nullptr, nullptr)), tekuci(pocetni) {
    pocetni->sljedeci = krajnji;
    krajnji->prethodni = pocetni;
  };

  DvostrukaLista(const DvostrukaLista<Tip> &lista)
      : dduzina(lista.dduzina), lduzina(lista.lduzina),
        pocetni(new Cvor(0, nullptr, nullptr)),
        krajnji(new Cvor(0, nullptr, nullptr)), tekuci(pocetni) {
    pocetni->sljedeci = krajnji;
    krajnji->prethodni = pocetni;
    Cvor *trenutna = pocetni;
    for (Cvor *p = (lista.pocetni)->sljedeci; p != lista.krajnji;
         p = p->sljedeci) {
      Cvor *novi = new Cvor(p->element, trenutna, trenutna->sljedeci);
      (novi->sljedeci)->prethodni = novi;
      trenutna->sljedeci = novi;
      trenutna = novi;
      // nailazak na tekuci
      if (p == lista.tekuci)
        tekuci = trenutna;
    }
  };

  DvostrukaLista<Tip> &operator=(DvostrukaLista<Tip> l) {
    std::swap(pocetni, l.pocetni);
    std::swap(krajnji, l.krajnji);
    std::swap(tekuci, l.tekuci);
    std::swap(lduzina, l.lduzina);
    std::swap(dduzina, l.dduzina);
    return *this;
  };

  ~DvostrukaLista() {
    for (Cvor *p = pocetni->sljedeci; p != nullptr; p = p->sljedeci) {
      delete pocetni;
      pocetni = p;
    }
    delete pocetni;
    pocetni = nullptr;
    krajnji = nullptr;
    tekuci = nullptr;
    lduzina = 0;
    dduzina = 0;
  };

  int brojElemenata() const { return lduzina + dduzina; };

  Tip trenutni() const {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");
    return (tekuci->sljedeci)->element;
  };

  Tip &trenutni() {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");
    return (tekuci->sljedeci)->element;
  };

  bool prethodni() {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");

    if (tekuci == pocetni)
      return false;

    tekuci = tekuci->prethodni;
    lduzina--;
    dduzina++;
    return true;
  };

  bool sljedeci() {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");

    if ((tekuci->sljedeci)->sljedeci == krajnji)
      return false;

    tekuci = tekuci->sljedeci;
    lduzina++;
    dduzina--;
    return true;
  };

  void pocetak() {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");
    tekuci = pocetni;
    dduzina += lduzina;
    lduzina = 0;
  }

  void kraj() {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");

    tekuci = (krajnji->prethodni)->prethodni;
    lduzina = brojElemenata() - 1;
    dduzina = 1;
  }

  void obrisi() {
    if (pocetni->sljedeci == krajnji)
      throw std::logic_error("Prazna lista");

    Cvor *p = tekuci->sljedeci;
    if ((p)->sljedeci == krajnji) {
      tekuci->sljedeci = krajnji;
      krajnji->prethodni = tekuci;
      tekuci = tekuci->prethodni;
      lduzina--;
    } else {
      tekuci->sljedeci = p->sljedeci;
      (p->sljedeci)->prethodni = tekuci;
      // tekuci = tekuci->sljedeci;
      dduzina--;
    }
    delete p;
  }

  void dodajIspred(const Tip &el) {

    if (pocetni->sljedeci == krajnji) {
      Cvor *novi = new Cvor(el, pocetni, krajnji);
      pocetni->sljedeci = novi;
      krajnji->prethodni = novi;
      tekuci = pocetni;
    } else {
      Cvor *trenutni = tekuci->sljedeci; // pokazuje na stvarni trenutni
      Cvor *novi = new Cvor(el, tekuci, trenutni);
      tekuci->sljedeci = novi;
      trenutni->prethodni = novi;
      tekuci = tekuci->sljedeci;
    }
    dduzina++;
  };

  void dodajIza(const Tip &el) {

    if (pocetni->sljedeci == krajnji) {
      Cvor *novi = new Cvor(el, pocetni, krajnji);
      pocetni->sljedeci = novi;
      krajnji->prethodni = novi;
      tekuci = pocetni;
    } else {
      Cvor *trenutni = tekuci->sljedeci; // pokazuje na stvarni trenutni
      Cvor *novi = new Cvor(el, trenutni, trenutni->sljedeci);
      (trenutni->sljedeci)->prethodni = novi;
      trenutni->sljedeci = novi;
    }
    dduzina++;
  };

  Tip operator[](int i) const {

    if (i < 0)
      throw std::domain_error("Neispravan indeks");

    Cvor *p = pocetni->sljedeci;
    for (int j = 0; j < i; j++) {
      p = p->sljedeci;
    }
    return p->element;
  };

  Tip &operator[](int i) {
    if (i < 0)
      throw std::domain_error("Neispravan indeks");

    Cvor *p = pocetni->sljedeci;
    for (int j = 0; j < i; j++) {
      p = p->sljedeci;
    }
    return p->element;
  };

  void Prikazi() const {

    for (Cvor *p = pocetni->sljedeci; p != krajnji; p = p->sljedeci) {
      std::cout << p->element << " ";
    }
  }

  friend class Iterator<Tip>;
};

// ZADATAK 2
template <typename Tip> class NizLista : public Lista<Tip> {

private:
  int kapacitet, duzina, tekuci;
  Tip **lista; // DODAJ POKAZIVAC NA NIZ POKAZIVACA A NE OVO

public:
  NizLista(int vel = 10000)
      : Lista<Tip>(), kapacitet(vel), duzina(0), tekuci(0),
        lista(new Tip *[vel]) {
    for (int i = 0; i < kapacitet; i++)
      lista[i] = new Tip();
  };

  ~NizLista() {
    if (lista != nullptr) {
      for (int i = 0; i < kapacitet; i++)
        delete lista[i];
      delete[] lista;
      duzina = 0;
      tekuci = 0;
      kapacitet = 0;
    }
  };

  NizLista(const NizLista<Tip> &nova_lista) {

    lista = new Tip *[nova_lista.kapacitet];
    kapacitet = nova_lista.kapacitet;
    for (int i = 0; i < kapacitet; i++)
      lista[i] = new Tip();
    duzina = nova_lista.duzina;
    tekuci = nova_lista.tekuci;
    for (int i = 0; i < duzina; i++)
      *lista[i] = *(nova_lista.lista)[i];
  }

  NizLista<Tip> &operator=(NizLista<Tip> niz) {

    std::swap(kapacitet, niz.kapacitet);
    std::swap(duzina, niz.duzina);
    std::swap(tekuci, niz.tekuci);
    std::swap(lista, niz.lista);
    return *this;
  }

  Tip operator[](int i) const {

    if (i < 0 || i >= duzina)
      throw std::domain_error("Neispravan indeks!\n");
    return *(lista[i]);
  };

  Tip &operator[](int i) {

    if (i < 0 || i >= duzina)
      throw std::domain_error("Neispravan indeks!\n");
    return *(lista[i]);
  };

  int brojElemenata() const { return duzina; };

  Tip trenutni() const {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");
    if (tekuci == duzina)
      throw std::domain_error("Ne postoji tekuci element!\n");
    return *(lista[tekuci]);
  };

  Tip &trenutni() {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");
    if (tekuci == duzina)
      throw std::domain_error("Ne postoji tekuci element!\n");
    return *(lista[tekuci]);
  };

  bool prethodni() {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");
    if (tekuci == 0)
      return false;
    tekuci--;
    return true;
  };

  bool sljedeci() {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");
    if (tekuci == duzina - 1)
      return false;
    tekuci++;

    return true;
  };

  void pocetak() {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");
    tekuci = 0;
  };
  void kraj() {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");
    tekuci = duzina - 1;
  };

  void obrisi() {
    if (duzina == 0)
      throw std::domain_error("Lista je prazna!\n");

    if (tekuci == duzina - 1 && tekuci != 0) {
      tekuci = duzina - 2;
    } else {
      for (int i = tekuci + 1; i < duzina; i++) {
        Tip *p = lista[i - 1];
        lista[i - 1] = lista[i];
        lista[i] = p;
      }
    }
    duzina--;
  };

  void dodajIspred(const Tip &el) {

    if (duzina == kapacitet) {
      Tip **nova_lista = new Tip *[2 * kapacitet];
      kapacitet *= 2;
      for (int i = 0; i < kapacitet; i++)
        nova_lista[i] = new Tip();
      for (int i = 0; i < duzina; i++)
        *(nova_lista[i]) = *(lista[i]);
      for (int i = 0; i < kapacitet / 2; i++)
        delete lista[i];
      delete[] lista;
      lista = nova_lista;
    }
    if (duzina == 0)
      *(lista[0]) = el, tekuci = 0;
    else {
      for (int i = duzina - 1; i >= tekuci; i--) {
        Tip *p = lista[i + 1];
        lista[i + 1] = lista[i];
        lista[i] = p;
      }
      *(lista[tekuci]) = el;
      tekuci++;
    }
    duzina++;
  };

  void dodajIza(const Tip &el) {

    if (duzina == kapacitet) {
      Tip **nova_lista = new Tip *[2 * kapacitet];
      kapacitet *= 2;
      for (int i = 0; i < kapacitet; i++)
        nova_lista[i] = new Tip();
      for (int i = 0; i < duzina; i++)
        *(nova_lista[i]) = *(lista[i]);
      for (int i = 0; i < kapacitet / 2; i++)
        delete lista[i];
      delete[] lista;
      lista = nova_lista;
    }
    if (duzina == 0)
      *(lista[0]) = el, tekuci = 0;
    else {
      for (int i = duzina - 1; i >= tekuci + 1; i--) {
        Tip *p = lista[i + 1];
        lista[i + 1] = lista[i];
        lista[i] = p;
      }
      *(lista[tekuci + 1]) = el;
    }
    duzina++;
  };

  void Prikazi() const {
    std::cout << "Lista glasi:\n";
    for (int i = 0; i < duzina; i++)
      std::cout << lista[i] << " ";

    std::cout << std::endl;
  }

  friend class Iterator<Tip>;
};

// ZADATAK 3

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
private:
  struct Cvor {
    Tip info;
    Cvor *sljedeci;
    Cvor(const Tip &element, Cvor *sljedeci)
        : info(element), sljedeci(sljedeci){};
  };

  Cvor *pocetni;
  Cvor *krajnji;
  Cvor *tekuci;
  int lduzina, dduzina;

  void PostaviKrajnji() {
    for (Cvor *p = pocetni; p != nullptr; p = p->sljedeci) {
      krajnji = p;
    };
  }

public:
  JednostrukaLista()
      : Lista<Tip>(), pocetni(0), krajnji(0), tekuci(0), dduzina(0),
        lduzina(0){};

  ~JednostrukaLista() {
    if (pocetni != nullptr) {
      Cvor *poslije = pocetni->sljedeci;
      while (poslije != nullptr) {
        delete pocetni;
        pocetni = poslije;
        poslije = poslije->sljedeci;
      }
    }
    delete pocetni;
    pocetni = nullptr;
  };

  JednostrukaLista(const JednostrukaLista<Tip> &lista) {

    pocetni = nullptr;
    krajnji = nullptr;
    tekuci = nullptr;
    Cvor *trenutno = nullptr;
    for (Cvor *p = lista.pocetni; p != nullptr; p = p->sljedeci) {

      Cvor *p2 = new Cvor(p->info, p->sljedeci);
      if (pocetni == nullptr) {
        pocetni = p2;
        trenutno = p2;
      } else {
        trenutno->sljedeci = p2;
        trenutno = p2;
      } // za tekuci element
      if (trenutno != nullptr && trenutno->info == lista.trenutni())
        tekuci = trenutno;
    }
    krajnji = trenutno;
    lduzina = lista.lduzina;
    dduzina = lista.dduzina;
  };
  JednostrukaLista<Tip> &operator=(JednostrukaLista<Tip> lista) {
    std::swap(pocetni, lista.pocetni);
    std::swap(tekuci, lista.tekuci);
    std::swap(krajnji, lista.krajnji);
    std::swap(lduzina, lista.lduzina);
    std::swap(dduzina, lista.dduzina);
    return *this;
  };

  int brojElemenata() const { return dduzina + lduzina; }

  Tip trenutni() const {
    if (dduzina == 0)
      throw std::domain_error("Tekuci element ne postoji");
    if (lduzina == 0)
      return pocetni->info;

    return (tekuci->sljedeci)->info;
  };

  Tip &trenutni() {
    if (dduzina == 0)
      throw std::domain_error("Tekuci element ne postoji");
    if (lduzina == 0)
      return pocetni->info;

    return (tekuci->sljedeci)->info;
  };

  bool prethodni() {

    if (lduzina == 0)
      return false;
    if (lduzina == 1)
      tekuci = nullptr;
    else {
      Cvor *p = nullptr;
      for (p = pocetni; p->sljedeci != tekuci; p = p->sljedeci) {
      };
      tekuci = p;
    }
    dduzina++;
    lduzina--;
    return true;
  };

  bool sljedeci() {

    if (dduzina == 0)
      throw std::domain_error("Sljedeci element ne postoji");
    if (dduzina == 1)
      return false;

    if (dduzina == 1)
      // tekuci = pocetni;
      return true;

    else {
      if (tekuci == nullptr)
        tekuci = pocetni;
      else
        tekuci = tekuci->sljedeci;
      lduzina++;
      dduzina--;
    }
    return true;
  };

  void pocetak() {

    tekuci = nullptr;
    dduzina = brojElemenata();
    lduzina = 0;
  };

  void kraj() {

    for (tekuci = pocetni; tekuci->sljedeci != krajnji;
         tekuci = tekuci->sljedeci) {
    };
    lduzina = brojElemenata() - 1;
    dduzina = 1;
  };

  void obrisi() {

    if (dduzina == 0)
      throw std::domain_error("Tekuci element ne postoji");

    if (lduzina == 0) {
      Cvor *p = pocetni;
      pocetni = pocetni->sljedeci;
      delete p;
      dduzina--;
    } else {
      Cvor *p = tekuci->sljedeci; // koji brisemo
      if (p->sljedeci == nullptr) {
        if (pocetni->sljedeci == p) {
          tekuci = nullptr;
          krajnji = pocetni;
          pocetni->sljedeci = nullptr;

        } else {
          Cvor *x;
          for (x = pocetni; x->sljedeci != tekuci; x = x->sljedeci) {
          };
          tekuci->sljedeci = nullptr;
          krajnji = tekuci;
          tekuci = x;
        }
      } else {
        tekuci->sljedeci = p->sljedeci;
      }
      delete p;
      lduzina--;
    }
    PostaviKrajnji();
  };

  void dodajIspred(const Tip &el) {

    if (lduzina + dduzina == 0) {
      pocetni = new Cvor(el, nullptr);
      krajnji = pocetni;
      tekuci = nullptr;
      lduzina = 0, dduzina = 1;
    } else {

      if (lduzina == 0) {
        Cvor *p = new Cvor(el, nullptr);
        p->sljedeci = pocetni;
        pocetni = p;
        tekuci = p;
      } else {
        Cvor *p = new Cvor(el, tekuci->sljedeci);
        tekuci->sljedeci = p;
        tekuci = p;
        if (dduzina == 0) {
          p->sljedeci = nullptr;
          krajnji = p;
        }
      }
      lduzina++;
    }
    PostaviKrajnji();
  };

  void dodajIza(const Tip &el) {

    if (lduzina + dduzina == 0) {

      pocetni = new Cvor(el, nullptr);
      krajnji = pocetni;
      tekuci = nullptr;
      // dduzina = 1, lduzina = 0;
    } else if (dduzina == 0)
      throw std::domain_error("Ne postoji tekuci element");
    else if (lduzina == 0) {
      Cvor *p = new Cvor(el, pocetni->sljedeci);
      pocetni->sljedeci = p;
    } else {
      Cvor *t = tekuci->sljedeci;
      Cvor *p = new Cvor(el, t->sljedeci);
      t->sljedeci = p;
    }
    dduzina++;
    PostaviKrajnji();
  };

  Tip operator[](int i) const {

    if (i < 0 || i > lduzina + dduzina)
      throw std::domain_error("Neispravan indeks");

    int j = 0;
    Cvor *p;
    for (p = pocetni; j != i; j++, p = p->sljedeci)
      ;
    return p->info;
  };

  Tip &operator[](int i) {
    if (i < 0 || i > lduzina + dduzina)
      throw std::domain_error("Neispravan indeks");

    int j = 0;
    Cvor *p;
    for (p = pocetni; j != i; j++, p = p->sljedeci) {
    };
    return p->info;
  };

  friend class Iterator<Tip>;
};

template <typename Tip> class Iterator {
private:
  const DvostrukaLista<Tip> *dvostrukalista;
  typename DvostrukaLista<Tip>::Cvor *trenutni2Lista;

public:
  Iterator(const DvostrukaLista<Tip> &lista)
      : dvostrukalista(lista), trenutni2Lista(0){};

  Iterator(const Lista<Tip> &lista) {
    dvostrukalista = new DvostrukaLista<Tip>(
        *dynamic_cast<const DvostrukaLista<Tip> *>(&lista));
    trenutni2Lista = dvostrukalista->pocetni;
  }

  ~Iterator() {

    if (dvostrukalista != nullptr)
      delete dvostrukalista;
    dvostrukalista = nullptr;
    trenutni2Lista = nullptr;
  }

  Tip trenutni() const { return trenutni2Lista->element; };

  bool prethodni() {

    if (dvostrukalista->trenutni() == false)
      throw std::domain_error("Prazna lista");
    trenutni2Lista = &(dvostrukalista->prethodni());
    dvostrukalista->sljedeci();
    return dvostrukalista->prethodni();
  };

  bool sljedeci() {

    if (dvostrukalista->brojElemenata() == 0)
      throw std::domain_error("Prazna lista");
    if (trenutni2Lista->sljedeci == nullptr)
      return false;
    trenutni2Lista = trenutni2Lista->sljedeci;
    return true;
  };
  void pocetak() {

    if (dvostrukalista->brojElemenata() == 0)
      throw std::domain_error("Prazna lista");
    trenutni2Lista = dvostrukalista->pocetni;
  };

  void kraj() {

    if (dvostrukalista->trenutni() == false)
      throw std::domain_error("Prazna lista");
    trenutni2Lista = dvostrukalista->krajnji;
  };
};

template <typename Tip> Tip dajMaksimum(const Lista<Tip> &lista) {

  Iterator<Tip> it(lista);
  it.pocetak();
  Tip max = it.trenutni();
  int broj = lista.brojElemenata()+1;

  for (int i = 0; i < broj; i++) {

    if (it.trenutni() > max)
      max = it.trenutni();
    it.sljedeci();
  }

  return max;
}

void testDodajIspred() {
  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);

  std::cout << "Ispis nakon dodajIspred: ";
  lista.Prikazi();
}

void testDodajIza() {
  DvostrukaLista<int> lista;
  lista.dodajIza(15);

  std::cout << "Ispis nakon dodajIza: ";
  lista.Prikazi();
}

void testBrojElemenata() {
  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);

  int broj_elemenata = lista.brojElemenata();
  std::cout << "Broj elemenata: " << broj_elemenata << std::endl;
}

void testOperatorIndex() {
  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);

  if (lista.brojElemenata() > 0) {
    int prvi_element = lista[0];
    std::cout << "Prvi element: " << prvi_element << std::endl;
  }
}

void testObrisi() {
  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);

  lista.obrisi();

  std::cout << "Ispis nakon brisanja: ";
  lista.Prikazi();
}

void testTekuci() {

  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);
  if (lista.trenutni() == 5)
    std::cout << "Test tekuci() : OK";
  else
    std::cout << "Test tekuci() : Nije OK";
}

void testPrethodni() {

  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);
  lista.dodajIspred(7);
  lista.prethodni();
  if (lista.trenutni() == 7)
    std::cout << "Test prethodni() : OK";
  else
    std::cout << "Test prethodni() : Nije OK";
}

void testPocetak() {

  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);
  lista.dodajIspred(7);
  lista.pocetak();
  if (lista.trenutni() == 10)
    std::cout << "Test pocetak() : OK";
  else
    std::cout << "Test pocetak() : Nije OK";
}

void testKraj() {

  DvostrukaLista<int> lista;
  lista.dodajIspred(5);
  lista.dodajIspred(10);
  lista.dodajIza(7);
  lista.kraj();
  if (lista.trenutni() == 7)
    std::cout << "Test kraj() : OK";
  else
    std::cout << "Test kraj() : Nije OK";
}

int main() {

  DvostrukaLista<int> l;
  for (int i(0); i <= 100000; i++) {
    l.dodajIza(i);
    l.sljedeci();
  }
  l.pocetak();
  for (int i(0); i < 500; i++)
    l.sljedeci();

  std::cout << l.trenutni() << " ";
  std::cout << dajMaksimum(l) << " ";
  std::cout << l.trenutni();

  testDodajIspred();
  testDodajIza();
  testBrojElemenata();
  testOperatorIndex();
  testObrisi();
  testTekuci();
  testPrethodni();
  testPocetak();
  testKraj();
  return 0;
}