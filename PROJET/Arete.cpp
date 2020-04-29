#include "Arete.h"
#include "svgfile.h"


Arrete::Arrete(int indice, Sommet* ex1, Sommet* ex2):m_indice {indice},m_ex1{ex1},m_ex2{ex2}
{
    //ctor
}

Arrete::~Arrete()
{
    //dtor
}
void Arrete::afficher() const
 {
    std::cout <<"<"<<m_indice <<" "<< m_ex1->getX()<<std::endl;
 }

 void Arrete::Dessiner(Svgfile &svgout) const
 {
   svgout.addLine((m_ex1->getX()),(m_ex1->getY()),(m_ex2->getX()),(m_ex2->getY()),"black");
 }

/// GETTER
Sommet*Arrete::getEx1()const{

 return m_ex1;

 }

Sommet*Arrete::getEx2()const{

 return m_ex2;
 }
