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
char Sommet::getNom()
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
