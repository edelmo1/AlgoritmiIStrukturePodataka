#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <utility>

using namespace std;

using sat = std::chrono::high_resolution_clock;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {

public:
  Mapa<TipKljuca, TipVrijednosti>() {}
  virtual ~Mapa<TipKljuca, TipVrijednosti>() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti &operator[](TipKljuca i) = 0;
  virtual const TipVrijednosti operator[](TipKljuca i) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {

  struct Cvor {

    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *l_strana, *d_strana, *roditelj;
    int balans;

    Cvor(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti())
        : kljuc(i), vrijednost(u), d_strana(nullptr), l_strana(nullptr),
          roditelj(nullptr), balans(0) {}
  };

  int balanss(Cvor *t) {
    if (t == nullptr)
      return 0;
    return t->balans;
  }

  int max(int a, int b) { return (a > b) ? a : b; }

  // dodat ćemo i novi balans atribut novome čvoru

  Cvor *novi(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti()) {
    Cvor *temp = new Cvor();
    temp->kljuc = i;
    temp->vrijednost = u;
    temp->d_strana = temp->l_strana = temp->roditelj = nullptr;
    temp->balans = 0;
    return temp;
  }

  // rotiranje udesno

  void desno_rotiranje(Cvor *t) {

    Cvor *q = nullptr;
    int o = 0;
    q = t->l_strana->d_strana;
    t->balans = 0;
    Cvor *l = t->l_strana;
    Cvor *r = t->roditelj;
    l->roditelj = r;
    l->d_strana = t;

    if (!l->roditelj && !o) {
      p = l;
      o = 1;
    } else if (r->d_strana == t && !o) {
      l->roditelj->d_strana = l;
      o = 1;
    }

    if (o == 0) {
      l->roditelj->l_strana = l;
    }
    l->balans = 0;

    if (q)
      q->roditelj = t;
    t->l_strana = q;
    t->roditelj = l;
  }

  // rotiranje ulijevo

  void lijevo_rotiranje(Cvor *t) {

    Cvor *q = nullptr;
    int o = 0;
    q = t->d_strana->l_strana;
    t->balans = 0;
    Cvor *d = t->d_strana;
    Cvor *r = t->roditelj;
    d->roditelj = r;
    d->l_strana = t;

    if (!d->roditelj && !o) {
      p = d;
      o = 1;
    } else if (r->d_strana == t && !o) {
      d->roditelj->d_strana = d;
      o = 1;
    }

    if (o == 0) {
      d->roditelj->l_strana = d;
    }

    d->balans = 0;

    if (q)
      q->roditelj = t;
    t->d_strana = q;
    t->roditelj = d;
  }

  // funckija računa faktor balansa (balans lijevog cvora - balans desnog
  // cvora)

  void azurirajBalans(Cvor *t, bool b) {

    if (t == nullptr)
      return;
    else if (b)
      t->balans--;
    else
      t->balans++;

    int blns = t->balans;

    if (t->roditelj) {

      if (t->roditelj->d_strana == t)
        b = true;
      else
        b = false;
    }

    if (blns == 2) {

      if (balanss(t->l_strana) == 1)
        desno_rotiranje(t);
      else {
        lijevo_rotiranje(t->l_strana);
        desno_rotiranje(t);
      }

    } else if (blns == -2) {
      if (balanss(t->d_strana) == -1)
        lijevo_rotiranje(t);

      else {
        desno_rotiranje(t->d_strana);
        lijevo_rotiranje(t);
      }
    } else {

      if (blns == 0)
        return;
      azurirajBalans(t->roditelj, b);
    }
  }

  Cvor *pretraga(Cvor *p, TipKljuca i) const {
    if (p == nullptr || p->kljuc == i)
      return p;
    if (p->kljuc < i)
      return pretraga(p->d_strana, i);
    else if (p->kljuc > i)
      return pretraga(p->l_strana, i);
    return p;
  }

  // u dodaj metodi smo dodali azurirajBalans, kako bi svaki novi dodani bio
  // tamo
  // gdje i treba

  Cvor *dodaj(Cvor *p, TipKljuca i, TipVrijednosti u = TipVrijednosti(),
              Cvor *rod = nullptr) {

    while (p) {

      if (p->kljuc == i)
        return p;
      else if (i > p->kljuc)
        p = p->d_strana;
      else
        p = p->l_strana;
    }

    if (!p) {

      Cvor *t = AVLStabloMapa::p, *q = nullptr;
      p = novi(i, u);
      while (t) {
        q = t;
        if (t->kljuc > p->kljuc)
          t = t->l_strana;
        else
          t = t->d_strana;
      }

      if (!q)
        AVLStabloMapa::p = p;
      else if (q->kljuc > p->kljuc) {
        q->l_strana = p;
        p->roditelj = q;
      } else {
        q->d_strana = p;
        p->roditelj = q;
      }

      if (!q)
        return p;

      if (q->l_strana && q->d_strana)
        q->balans = 0;
      else {
        bool b = true;
        if (q->l_strana)
          b = false;
        azurirajBalans(q, b);
      }
    }

    return p;
  }

  Cvor *nasljednik(Cvor *t) {
    auto ovaj = t;
    while (ovaj && ovaj->l_strana != nullptr)
      ovaj = ovaj->l_strana;
    return ovaj;
  }

  void konstruktor(Cvor *a, Cvor *b) {

    if (b == nullptr)
      return;

    konstruktor(a, b->l_strana);
    konstruktor(a, b->d_strana);
    dodaj(a, b->kljuc, b->vrijednost);
  }

  // također kada brišemo neki element morat ćemo azurirati balans kada to i
  // treba

  void obrisiC(Cvor *t, TipKljuca i) {

    Cvor *r = nullptr;

    while (t != nullptr && t->kljuc != i) {

      r = t;
      if (t->kljuc < i)
        t = t->d_strana;
      else
        t = t->l_strana;
    }

    if (t == nullptr)
      return;

    Cvor *e, *q, *s;

    if (!t->l_strana) {
      e = t->d_strana;
      s = r;
    } else if (!t->d_strana) {
      e = t->l_strana;
      s = r;
    } else {
      e = t->l_strana;
      q = e->d_strana;
      s = t;
      while (q) {
        s = e;
        e = q;
        q = e->d_strana;
      }
      if (t != s) {
        s->d_strana = e->l_strana;
        if (e->l_strana)
          e->l_strana->roditelj = s;
        e->l_strana = t->l_strana;
        if (t->l_strana)
          t->l_strana->roditelj = e;
      }
      e->d_strana = t->d_strana;
      if (t->d_strana)
        t->d_strana->roditelj = e;
    }

    if (!r) {
      AVLStabloMapa::p = e;
    } else {
      if (r->l_strana == t) {
        r->l_strana = e;
        r->balans--;
      } else {
        r->d_strana = e;
        r->balans++;
      }
    }

    if (e)
      e->roditelj = r;

    t->d_strana = t->l_strana = t->roditelj = nullptr;
    delete t;
  }

  void obrisi_stablo(Cvor *t) {

    if (t == nullptr)
      return;

    obrisi_stablo(t->l_strana);
    obrisi_stablo(t->d_strana);
    delete t;
  }

  void PREORDER(Cvor *t, std::string &s) {
    if (t != nullptr) {
      s = s + to_string(t->kljuc) + ",";
      PREORDER(t->l_strana, s);
      PREORDER(t->d_strana, s);
    }
  }
  int br_ele;
  Cvor *p;

public:
  AVLStabloMapa<TipKljuca, TipVrijednosti>() : br_ele(0), p(nullptr) {}

  ~AVLStabloMapa<TipKljuca, TipVrijednosti>() {
    if (p)
      obrisi_stablo(p);
  }
  AVLStabloMapa<TipKljuca, TipVrijednosti>(
      const AVLStabloMapa<TipKljuca, TipVrijednosti> &a) {
    br_ele = a.br_ele;
    if (a.br_ele == 0)
      p = nullptr;
    else {
      p = novi(a.p->kljuc, a.p->vrijednost);
      konstruktor(p, a.p);
    }
  }

  AVLStabloMapa<TipKljuca, TipVrijednosti> &
  operator=(const AVLStabloMapa<TipKljuca, TipVrijednosti> &a) {
    if (this == &a)
      return *this;

    obrisi();
    br_ele = a.br_ele;
    if (a.br_ele == 0)
      p = nullptr;
    else {
      if (!p)
        p = novi(a.p->kljuc, a.p->vrijednost);
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

  void obrisi(const TipKljuca &kljuc) {
    obrisiC(p, kljuc);
    br_ele--;
  }

  TipVrijednosti &operator[](TipKljuca i) {
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

  const TipVrijednosti operator[](TipKljuca i) const {
    auto c = pretraga(p, i);
    if (c != nullptr)
      return c->vrijednost;
    return TipVrijednosti();
  }

  void Preorder() {
    std::string s = "";
    PREORDER(p, s);
    std::cout << s;
  }
};

void test1(){
     AVLStabloMapa<int, int> avl;
    avl[3] = 3;
  avl[5] = 5;
  avl[8] = 8;
  avl[10] = 10;
  avl[9] = 9;
  avl[11] = 11;
   avl[1] = 40;
  avl[2] = 14;
  avl.Preorder();
}

void test2(){
     AVLStabloMapa<int, int> avl;
    avl[2] = 4;
  avl[3] = 12;
  avl[4] = 88;
  avl[8] = 11;
  avl[12] = 22;
  avl[41] = 14;
   avl[32] = 43;
  avl[20] = 32;
  avl.Preorder();
}

void test3(){
     AVLStabloMapa<int, int> avl;
    avl[5] = 1;
  avl[43] = 2;
  avl[41] = 8;
  avl[82] = 31;
  avl[31] = 42;
  avl[22] = 34;
   avl[2] = 23;
  avl[22] = 3;
  avl.Preorder();
}

void test4(){
     AVLStabloMapa<int, int> avl;
    avl[3] = 9;
  avl[32] = 76;
  avl[11] = 6;
  avl[66] = 32;
  avl[54] = 78;
  avl[32] = 43;
   avl[12] = 33;
  avl[32] = 12;
  avl.Preorder();
}

void test5(){
     AVLStabloMapa<int, int> avl;
  avl[4] = 2;
  avl[43] = 6;
  avl[100] = 36;
  avl[23] = 22;
  avl[4] = 26;
  avl[2] = 43;
  avl[2] = 3;
  avl[52] = 12;
  avl.Preorder();
}

int main() {

test1();
std::cout<<std::endl;
test2();
std::cout<<std::endl;
test3();
std::cout<<std::endl;
test4();
std::cout<<std::endl;
test5();
std::cout<<std::endl;
  return 0;
}
