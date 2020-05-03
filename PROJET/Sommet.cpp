#include "Sommet.h"


Sommet::Sommet(int indice, std::string nom,  double x, double y):m_indice{indice},m_nom{nom}, m_x{x},m_y{y}
{
    //ctor
}
/// GETTER
int Sommet::getNum()const
{
    return m_indice;
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
///ajouter un successeur
void Sommet::ajouterSucc( Sommet*s)
{
    m_successeurs.push_back(s);
}

///Attribuer à l'indice un nombre
int Sommet::setNum(int num)
{
    m_indice=num;
}
void Sommet::set_indice(int nombre)
{
    m_indice=nombre;
}
void Sommet::Dessiner(Svgfile&svgout)const
{
    svgout.addDisk(m_x*100,m_y*100,10,"black");
    svgout.addText(m_x*100,(m_y*100)-20,m_nom,"blue");
    svgout.addText((m_x*100),(m_y*100)-50,m_indice,"green");
}
///Afficher un sommet
void Sommet::afficher() const
    {
        std::cout<<"     sommet "<<m_nom<<" : ";
        for (auto s : m_successeurs)
            std::cout<<s->getNom()<<" ";
    }
///supprimer un successeur
void Sommet::supp_succ(int indice)
{
    for(auto s: m_successeurs)
    {
        if(s->getNum()==indice)
        {
            delete s;///supprimer un succcesseurs
        }
    }
}
///Coloriage
void Sommet::colorier(Svgfile&svgout, int num)
{
    if(num==0)/// full important
    {
        svgout.addDisk(m_x*100,m_y*100,10,"lime");
    }

    if(num==1)
        svgout.addDisk(m_x*100,m_y*100,10,"limegreen");

    if(num==2)
    {
        svgout.addDisk(m_x*100,m_y*100,10,"palegreen");
    }

    if(num==3)
    {
        svgout.addDisk(m_x*100,m_y*100,10,"coral");
    }

    if(num==4)
    {
        svgout.addDisk(m_x*100,m_y*100,10,"orange");
    }
    if(num==5)
    {
        svgout.addDisk(m_x*100,m_y*100,10,"lightsalmon");
    }

    if(num==6)/// quand c est vraiment pas important
    {

        svgout.addDisk(m_x*100,m_y*100,10,"red");
    }

    svgout.addText((m_x*100)-5,(m_y*100)+5,m_indice,"black");
}
