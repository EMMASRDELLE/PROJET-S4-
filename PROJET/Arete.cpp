#include <iostream>
#include "Arete.h"
#include "svgfile.h"

Arete::Arete( Sommet* ex1, Sommet*ex2,int indice,double poids):m_ex1{ex1},m_ex2{ex2},m_indice{indice}, m_poids{poids}{}

///GETTER
int Arete::getIndice()const
{
    return m_indice;
}

 Sommet*Arete::getEx1() const
{
    return m_ex1;
}
 Sommet*Arete::getEx2() const
{
    return m_ex2;
}
double Arete::getPoids() const
{ return m_poids;
}

///SETTER
void Arete::setPoids(double poids)
{
    m_poids=poids;
}


///AFFICHAGE
void Arete::afficher()const
{
    std::cout<<"<"<<m_indice<<" "<<m_ex1->getNom()<<":"<<m_ex1->getX()<<" "<<m_ex1->getY()<<"  &  "<<m_ex2->getNom()<<":"<<m_ex2->getX()<<" "<<m_ex2->getY()<<">"<<" poids : "<<m_poids<<std::endl;
}
///dessiner arrete
void Arete::Dessiner(Svgfile &svgout) const
{
    svgout.addLine((m_ex1->getX())*100,(m_ex1->getY())*100,(m_ex2->getX())*100,(m_ex2->getY())*100,"black");
    svgout.addText(((m_ex1->getX())*100+(m_ex2->getX())*100)/2,((m_ex1->getY())*100+(m_ex2->getY())*100)/2-20,m_poids,"red");
}

///booleen pour retourner true ou false
bool Arete::CalculDEG( const Sommet*a, Arete*b)
{

    if((a==b->m_ex1)||(a==b->m_ex2)) ///si le sommet a est egale � une extremite de l'arrete
    {
        return true; ///return true
    }
    else
    {
        return false;
    }
}
///attribuer un indice � l'arrete
void Arete::set_indice(int nombre)
{
    m_indice=nombre;
}
