#ifndef SOMMET_H
#define SOMMET_H

#include <iostream>
#include <string>
#include "svgfile.h"

class Sommet
{
    public:
        Sommet(int num, std::string nom, double x,double y);

        double getX()const ;
        double getY() const;
        int getNum()const;
        std::string getNom() const;
        void Dessiner(Svgfile&svgout)const;

    protected:

    private:

        int m_indice;
        std::string m_nom;
        double m_x;
        double m_y;

};

#endif // SOMMET_H
