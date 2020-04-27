#ifndef SOMMET_H
#define SOMMET_H

#include <iostream>
#include <string>
#include "svgfile.h"
class Sommet
{
    public:
        Sommet(int num, char nom, double x,double y);

        double getX()const ;
        double getY() const;
        std::string getNom() const;
        void Dessiner(Svgfile&svgout)const;

    protected:

    private:
        int m_indice;
        double m_x;
        double m_y;
        std::string m_nom;
};

#endif // SOMMET_H
