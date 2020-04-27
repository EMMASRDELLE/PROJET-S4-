#include "Sommet.h"

Sommet::Sommet(int indice, char nom, int x, int y):m_indice{indice},m_nom{nom},m_x{x},m_y{y}
{
//ctor
}

Sommet::~Sommet()
{
    //dtor
}


/// GETTER
char Sommet::getNom()const
{
     return m_nom;
}

int Sommet::getX()const
{
    return m_x;
}

int Sommet::getY() const
{
    return m_y;
}
