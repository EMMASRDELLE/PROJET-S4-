#ifndef ARETE_H
#define ARETE_H
#include "Sommet.h"

class Arrete
{
public:
    Arrete( Sommet*, Sommet*,int indice,int poids);
    void afficher()const ;
    double getIndice() const;
    const Sommet*getEx1() const;
    const Sommet*getEx2() const;
    void Dessiner(Svgfile &svgout) const;
    bool CalculDEG( const Sommet*a, Arrete*b);
    void setPoids(double poids);
    double getPoids()const;

protected:

private:
    const Sommet* m_ex1;
    const Sommet* m_ex2;
    int m_indice;
    double m_poids;
};

#endif // ARETE_H
