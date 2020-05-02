#ifndef ARETE_H
#define ARETE_H
#include "Sommet.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "svgfile.h"


class Arete
{
public:
    Arete( Sommet*, Sommet*,int indice,double poids);
    void afficher()const ;
    int getIndice() const;
    Sommet*getEx1() const;
    Sommet*getEx2() const;
    void Dessiner(Svgfile &svgout) const;
    bool CalculDEG( const Sommet*a, Arete*b);
    void setPoids(double poids);
    double getPoids()const;
    void set_indice(int nombre);

protected:

private:
     Sommet* m_ex1;
     Sommet* m_ex2;
    int m_indice;
    double m_poids;
};

#endif // ARETE_H
