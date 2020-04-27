#ifndef ARETE_H
#define ARETE_H
#include "Sommet.h"

class Arrete
{
    public:
        Arrete(const Sommet*,const Sommet*,int indice);
        void afficher() const;
        double getIndice() const;
        const Sommet*getEx1() const;
        const Sommet*getEx2() const;

    protected:

    private:
        const Sommet* m_ex1;
        const Sommet* m_ex2;
        int m_indice;
};

#endif // ARETE_H
