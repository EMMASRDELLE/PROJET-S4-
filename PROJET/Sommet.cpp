#include "Sommet.h"


Sommet::Sommet(int indice, std::string nom,  double x, double y):m_indice{indice},m_nom{nom}, m_x{x},m_y{y}
{
    //ctor
}
int Sommet::getNum()const
{
    return m_indice;
}


/// GETTER

void Sommet::set_indice(int nombre)
{
    m_indice=nombre;
}
void Sommet::ajouterSucc(const Sommet*s)
{
    m_successeurs.push_back(s);
}
std::string Sommet::getNom()const
{
    return m_nom;
}
double Sommet::getX()const
{
    return m_x;
}
double Sommet::getY()const
{
    return m_y;
}

void Sommet::Dessiner(Svgfile&svgout)const
{

    svgout.addDisk(m_x*100,m_y*100,10,"black");
    svgout.addText(m_x*100,(m_y*100)-20,m_nom,"blue");
    svgout.addText((m_x*100),(m_y*100)-50,m_indice,"green");
}



void Sommet::afficher() const
    {
        std::cout<<"     sommet "<<m_nom<<" : ";
        for (auto s : m_successeurs)
            std::cout<<s->getNom()<<" ";
    }

void Sommet::supp_succ(int indice)
{
    for(auto s: m_successeurs)
    {
        if(s->getNum()==indice)
        {
            delete s;
        }
    }
}
