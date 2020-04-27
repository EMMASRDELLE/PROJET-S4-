#include <iostream>
#include "Arete.h"

Arrete::Arrete( Sommet* ex1, Sommet*ex2,int indice):m_ex1{ex1},m_ex2{ex2},m_indice{indice} {}

void Arrete::afficher()const
{
    std::cout<<"<"<<m_indice<<" "<<m_ex1->getNom()<<":"<<m_ex1->getX()<<" "<<m_ex1->getY()<<"  &  "<<m_ex2->getNom()<<":"<<m_ex2->getX()<<" "<<m_ex2->getY()<<">"<<std::endl;
}
double Arrete::getIndice()const
{
    return m_indice;
}
const Sommet*Arrete::getEx1() const
{
    return m_ex1;
}
const Sommet*Arrete::getEx2() const
{
    return m_ex2;
}
void Arrete::Dessiner(Svgfile &svgout) const
{
    svgout.addLine((m_ex1->getX())*100,(m_ex1->getY())*100,(m_ex2->getX())*100,(m_ex2->getY())*100,"black");
}
bool Arrete::CalculDEG( const Sommet*a, Arrete*b)
{

    if((a==b->m_ex1)||(a==b->m_ex2))
    {
        return true;
    }
    else
    {
        return false;
    }
}
