#include "Sommet.h"

Sommet::Sommet(int indice, char nom, double x, double y):m_indice{indice},m_nom{nom},m_x{x},m_y{y}
{
   std::cout<<"le sommet a été crée"; //ctor
}

Sommet::~Sommet()
{
    //dtor
}


/// GETTER
char  Sommet::getNom()
{
     return m_nom;
}

int Sommet::getX()
{
    return m_x;
}

int Sommet::getY()
{
    return m_y;
}

void Sommet::Dessiner(Svgfile&svgout)const
{
    svgout.addDisk(m_x,m_y,10,"black");
}
