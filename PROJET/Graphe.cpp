#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graphe.h"


Graphe::Graphe(std::string nomFichier)
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    ifs >> m_orientation;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture orientation du graphe");
    int ordre;
    ifs >> ordre;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");

    int indice,x,y;
    char nom;
    for (int i=0; i<ordre; ++i)
    {
        ifs>>indice>>nom>>x>>y;
         m_sommets.push_back( new Sommet{indice,nom,x,y} );
    }
        int taille;
    ifs >> taille;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    int indice2,num1,num2;
    double poids;
    for (int i=0; i<taille; ++i)
    {
        ifs>>indice2>>num1>>num2;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture arc");
        m_arretes.push_back(new Arrete(m_sommets[num1-1],m_sommets[num2-1],indice2));
    }
}
/*méthode d'affichage*/
void Graphe::afficher() const
{
    std::cout<<std::endl<<"graphe ";
    if(m_orientation)
        std::cout<<"oriente"<<std::endl<<"  ";
    else
        std::cout<<"non oriente"<<std::endl<<"  ";
    std::cout<<"ordre = "<<m_sommets.size()<<std::endl<<"  ";
    std::cout<<"taille(nombre d'arcs) :"<<m_arretes.size()<<std::endl;
    for (auto a : m_arretes)
    {
        a->afficher();
        std::cout<<std::endl;
    }
}
Graphe::~Graphe()
{
    for(int i=0; i<(int)m_sommets.size(); i++)
    {
        delete m_sommets[i];
    }
    for(int i=0; i<(int)m_arretes.size(); i++)
    {
        delete m_arretes[i];
    }
}
