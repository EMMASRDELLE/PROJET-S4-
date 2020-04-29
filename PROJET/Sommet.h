#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED
#include <vector>
#include <iostream>
#include <fstream>
#include "svgfile.h"

class Sommet
{
    public:
        Sommet(int indice, char nom, double x, double y);
        virtual~Sommet();
        char getNom();
        int getX() ;
        int getY ();
        void Dessiner(Svgfile &svgout) const;


    protected:

    private:

        int m_indice;
        char m_nom;
        int m_x;
        int m_y;
};

#endif // SOMMET_H_INCLUDED
