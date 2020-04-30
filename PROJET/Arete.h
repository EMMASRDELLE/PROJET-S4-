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
        Arete(int indice, Sommet* ex1, Sommet* ex2);
        virtual ~Arete();
        void afficher ()const;
        Sommet* getEx1()const;
        Sommet* getEx2()const;
        void Dessiner(Svgfile &svgout) const;

    protected:

    private:
        int m_indice;
        Sommet* m_ex1;
        Sommet* m_ex2;



class Arete
{
public:
    Arete( Sommet*, Sommet*,int indice,double poids);
    void afficher()const ;
    double getIndice() const;
    const Sommet*getEx1() const;
    const Sommet*getEx2() const;
    void Dessiner(Svgfile &svgout) const;
    bool CalculDEG( const Sommet*a, Arete*b);
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
