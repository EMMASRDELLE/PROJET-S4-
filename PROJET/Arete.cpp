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

     std::cout <<"<"<<m_ex1->getNom(); //<<" "<<m_ex2->getNom()<<"> : " <<m_indice<<"Coords S1 :"<< m_ex1->getX()<< m_ex1->getY()<< "Coords S2 : "<< m_ex2->getX()<<m_ex2->getY()<<std::endl;
      std::cout <<"heo2";
 }

/// GETTER
 Sommet*Arrete::getEx1()const{

 return m_ex1;
 }

  Sommet*Arrete::getEx2()const{

 return m_ex2;
 }
