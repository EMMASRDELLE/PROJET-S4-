#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graphe.h"
#include "svgfile.h"


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
    int indice;
    double x,y;
    std::string nom;
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

    for (int i=0; i<taille; ++i)
    {
        ifs>>indice2>>num1>>num2;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture arc");
        m_arretes.push_back(new Arrete(m_sommets[num1],m_sommets[num2],indice2));
    }
}
/*méthode d'affichage*/
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


void Graphe::Dessiner(Svgfile &svgout) const
{
    svgout.addGrid();

    ///on dessine les sommets

    for(size_t i=0;i<m_sommets.size();++i)
    {
        m_sommets[i]->Dessiner(svgout);
    }

    /// on dessine les arretes
    for(size_t j=0;j<m_arretes.size();++j)
    {
        m_arretes[j]->Dessiner(svgout);
    }


}
int Graphe::calculDegre()
{
    int deg=0;
    double CG;

std::cout<<"test sommet "<<std::endl;
int num;
std::cin>>num;
//const Sommet*a;
        for (auto b: m_arretes)
        {
           for (auto a : m_sommets)
           {
               if (a==m_sommets[num])
               {
                  if(b->CalculDEG(a,b)==true)
            {
            ++deg;
            }
               }
           }



        }
        CG=deg/(m_arretes.size()-1);
return CG;
}
