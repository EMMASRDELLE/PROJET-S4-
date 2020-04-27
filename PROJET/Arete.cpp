#include <iostream>
#include "Arete.h"

Arrete::Arrete(const Sommet* ex1,const Sommet*ex2,int indice):m_ex1{ex1},m_ex2{ex2},m_indice{indice}{}

void Arrete::afficher() const{
    std::cout<<"<"<<m_indice;
    std::cout<<""<<m_ex1->getNom()<<"  "<<m_ex2->getNom()<<"> : "<<std::endl;
}
double Arrete::getIndice() const{
    return m_indice;
}
const Sommet*Arrete::getEx1() const{
    return m_ex1;
}
const Sommet*Arrete::getEx2() const{
    return m_ex2;
}
