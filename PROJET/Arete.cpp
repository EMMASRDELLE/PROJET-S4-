#include <iostream>
#include "Arete.h"
#include "svgfile.h"

Arrete::Arrete( Sommet* ex1, Sommet*ex2,int indice,double poids):m_ex1{ex1},m_ex2{ex2},m_indice{indice}, m_poids{poids}{}

///GETTER
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
double Arrete::getPoids() const
{ return m_poids;
}

///SETTER
void Arrete::setPoids(double poids)
{
    m_poids=poids;
}
void Arrete::afficher() const
 {
    std::cout <<"<"<<m_indice <<" "<< m_ex1->getX()<<std::endl;
 }

 void Arrete::Dessiner(Svgfile &svgout) const
 {
   svgout.addLine((m_ex1->getX()),(m_ex1->getY()),(m_ex2->getX()),(m_ex2->getY()),"black");
 }

///AFFICHAGE
void Arrete::afficher()const
{
    std::cout<<"<"<<m_indice<<" "<<m_ex1->getNom()<<":"<<m_ex1->getX()<<" "<<m_ex1->getY()<<"  &  "<<m_ex2->getNom()<<":"<<m_ex2->getX()<<" "<<m_ex2->getY()<<">"<<" poids : "<<m_poids<<std::endl;
}
void Arrete::Dessiner(Svgfile &svgout) const
{
    svgout.addLine((m_ex1->getX())*100,(m_ex1->getY())*100,(m_ex2->getX())*100,(m_ex2->getY())*100,"black");
    svgout.addText(((m_ex1->getX())*100+(m_ex2->getX())*100)/2,((m_ex1->getY())*100+(m_ex2->getY())*100)/2-20,m_poids,"red");
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
