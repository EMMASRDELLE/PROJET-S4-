#ifndef ARETE_H_INCLUDED
#define ARETE_H_INCLUDED

#include "Sommet.h"
#include <vector>
#include <iostream>
#include <fstream>
class Arrete
{
    public:
        Arrete(int indice, Sommet* ex1, Sommet* ex2);
        virtual ~Arrete();
       void afficher () const;
        Sommet* getEx1()const;
        Sommet* getEx2()const;
         int getXX();


    protected:

    private:
        int m_indice;
        Sommet* m_ex1;
        Sommet* m_ex2;

};




#endif // ARETE_H_INCLUDED
