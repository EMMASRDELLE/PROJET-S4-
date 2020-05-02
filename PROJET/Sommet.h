
#ifndef SOMMET_H
#define SOMMET_H


#include <iostream>
#include <string>
#include "svgfile.h"
#include <vector>
class Sommet
{
    public:

        Sommet(int num, std::string nom, double x,double y);
        double getX()const ;
        double getY() const;
        int getNum()const;
        std::string getNom() const;
        void Dessiner(Svgfile&svgout)const;
        void ajouterSucc( Sommet*s);
        void set_indice(int nombre);
        std::vector < Sommet*>& getSuccesseurs() {return m_successeurs; };
        int setNum(int num);
        void afficher() const;
        void supp_succ( int indice);
        void colorier(Svgfile&svgout, int num);


    protected:

    private:

        int m_indice;
        std::string m_nom;
        double m_x;
        double m_y;
        std::vector < Sommet*> m_successeurs;


};

#endif // SOMMET_H
