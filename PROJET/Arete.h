#ifndef ARETE_H_INCLUDED
#define ARETE_H_INCLUDED

#include "Sommet.h"

class Arrete
{
    public:
        Arrete(int indice, Sommet* ex1, Sommet* ex2);
        virtual ~Arrete();
       const Sommet* getEx1()const;
       const Sommet* getEx2()const;

    protected:

    private:
        int m_indice;
        Sommet* m_ex1;
        Sommet* m_ex2;

};

#endif // ARRETE_H


#endif // ARETE_H_INCLUDED
