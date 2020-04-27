#include "Arete.h"


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
     std::cout <<"<"<<m_ex1->getNum()<<" "<<m_ex2->getNum()<<"> : " <<m_poids<<std::endl;
 }

/// GETTER
const Sommet*Arrete::getEx1()const{

 return m_ex1;
 }

 const Sommet*Arrete::getEx2()const{

 return m_ex2;
 }
