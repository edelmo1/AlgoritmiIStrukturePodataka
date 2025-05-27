#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;


template <typename TipKljuca, typename TipVrijednosti> class Mapa {

public:
  Mapa() = default;
  virtual ~Mapa(){};
  virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
  virtual TipVrijednosti operator[](TipKljuca kljuc) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {

private:
  std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>>> niz;
  int brojEl;
  unsigned int (*funkcija)(TipKljuca, unsigned int);

  int Trazi(std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>>> v,
            TipKljuca k) const {
    int i = funkcija(k, 1); // 1 je prvi pokusaj
    // std::list<std::pair<TipKljuca, TipVrijednosti>> lista = v[i];
    if (v[i].size() == 0)
      return -1;
    for (std::pair<TipKljuca, TipVrijednosti> par : v[i]) {
      if (par.first == k)
        return i;
    }
    return -1;
  }

  int Dodaj(std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>>> &v,
            TipKljuca k) {

    int i = funkcija(k, 1);
    std::list<std::pair<TipKljuca, TipVrijednosti>> lista = v[i];
    for (std::pair<TipKljuca, TipVrijednosti> par : lista) {
      if (par.first == k)
        return -1; //-1 znaci da vec postoji
    }

    (v[i]).push_back(std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti()));
    /*lista.sort([](const auto &a, const auto &b) {
            return a.first < b.first;
        }); //??*/
    return 1;
  }

public:
  // konstruktor
  HashMapaLan() {
    funkcija = nullptr;
    niz = std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>>>(1000);
    niz.resize(1000);
    brojEl = 0;
  }

  // destruktor
  ~HashMapaLan() {
    niz.resize(0);
    brojEl = 0;
    funkcija = 0;
  }

  // kopirajuci konstruktor
  HashMapaLan(const HashMapaLan &m) {

    brojEl = m.brojEl;
    funkcija = m.funkcija;
    niz = m.niz; // vidi hoce li raditi
  }

  // operator dodjele
  HashMapaLan &operator=(HashMapaLan m) {
    std::swap(funkcija, m.funkcija);
    std::swap(brojEl, m.brojEl);
    std::swap(niz, m.niz);
    return *this;
  }

  TipVrijednosti &operator[](TipKljuca kljuc) {

    if (funkcija == nullptr)
      throw std::logic_error("Nema funkcije");

    int provjera = Trazi(niz, kljuc);
    if (provjera == -1) { // kljuc ne postoji u tabeli
      int provjera2 = Dodaj(niz, kljuc);
      brojEl++;
      provjera = Trazi(niz, kljuc);
    }
    // std::list<std::pair<TipKljuca,TipVrijednosti>>::iterator it =
    // niz[provjera].begin();
    typename std::list<std::pair<TipKljuca, TipVrijednosti>>::iterator it =
        niz[provjera].begin();
    for (; it != niz[provjera].end(); it++) {
      if ((*it).first == kljuc)
        return (*it).second;
    }
    return (*it).second; // ovde ne bi trebao nikada doci
  }

  TipVrijednosti operator[](TipKljuca kljuc) const {

    if (funkcija == nullptr)
      throw std::logic_error("Nema funkcije");

    int provjera = Trazi(niz, kljuc);
    if (provjera == -1) { // kljuc ne postoji u tabeli
      return TipVrijednosti();
    }
    std::list<std::pair<TipKljuca, TipVrijednosti>> lista = niz[provjera];
    for (std::pair<TipKljuca, TipVrijednosti> par : lista) {
      if (par.first == kljuc)
        return par.second;
    }
    return TipVrijednosti(); // ne bi trebao nikada doci
  }

  int brojElemenata() const { return brojEl; }

  void obrisi() {
    // niz.resize(0);
    brojEl = 0;
    for (int i = 0; i < niz.size(); i++) {
      niz[i].erase(niz[i].begin(), niz[i].end());
     // niz[i].resize(0);
    }
  }
  void obrisi(const TipKljuca &kljuc) {
    int provjera = Trazi(niz, kljuc);
    // std::list<std::pair<TipKljuca, TipVrijednosti>> lista = niz[provjera];
    for (std::pair<TipKljuca, TipVrijednosti> par : niz[provjera]) {
      if (par.first == kljuc) {
        niz[provjera].remove(par);
        brojEl--;
        return;
      }
    }
  }

  void definisiHashFunkciju(unsigned int (*func)(TipKljuca, unsigned int)) {
    funkcija = func;
    return;
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
UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 0, 1.2);
    g->dodajGranu(1, 2, 0.1);
    g->dodajGranu(0, 0, 3.14);
    for (GranaIterator<bool> iter = g->dajGranePocetak();
         iter != g->dajGraneKraj(); ++iter)
      cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
  delete g;

  return 0;
}
