#include "Sommet.h"

Sommet::Sommet(int indice, char nom,  double x,double y):m_indice{indice},m_nom{nom}, m_x{x},m_y{y}
{
    //ctor
}

char Sommet::getNom()const
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
