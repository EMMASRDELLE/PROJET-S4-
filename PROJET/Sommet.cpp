#include "Sommet.h"

Sommet::Sommet(int num, char nom, int x,int y):m_num{num},m_nom{nom}, m_x{x},m_y{y}
{
    //ctor
}
int Sommet::getNum() const{
    return m_num;
}
char Sommet::getNom() const
{
    return m_nom;
}
int Sommet::getX() const
{
    return m_x;
}
int Sommet::getY() const
{
    return m_y;
}
