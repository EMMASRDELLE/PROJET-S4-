#include "Sommet.h"

Sommet::Sommet(int indice, std::string nom,  double x, double y):m_indice{indice},m_nom{nom}, m_x{x},m_y{y}
{
    //ctor
}

std::string Sommet::getNom()const
{
    return m_nom;
}
double Sommet::getX()const
{
    return m_x;
}
double Sommet::getY()const
{
    return m_y;
}

void Sommet::Dessiner(Svgfile&svgout)const
{
    svgout.addDisk(m_x*100,m_y*100,10,"black");
    svgout.addText(m_x,(m_y-30),m_nom,"green");
}
