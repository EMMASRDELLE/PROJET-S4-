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
        m_arretes.push_back(new Arrete(m_sommets[num1],m_sommets[num2],indice2));
    }
}
/*m�thode d'affichage*/
void Graphe::afficher()const
{ std::cout << "------------------------------PROJET PISCINE-------------------------------------\n"<<std::endl;

    std::cout<<" \t  Yanis CHAMSON --- Shobiya KANDASAMY --- Emma SIRANDELLE "<<std::endl;
    std::cout<<" \t \t \t \t ING2 TD7\n\n"<<std::endl;


    if(m_orientation)
        std::cout<<"Le graphe est oriente"<<std::endl<<"  ";
    else
        std::cout<<"Le graphe est non oriente "<<std::endl<<"\n";
    std::cout<<"  - Ordre : "<<m_sommets.size()<<std::endl<<"";
    std::cout<<"  - Taille :"<<m_arretes.size()<<std::endl<<"\n";
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

