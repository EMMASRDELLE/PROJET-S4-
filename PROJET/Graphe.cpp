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
            m_sommets[num1]->ajouterSucc(m_sommets[num2]);
            if((!m_orientation)&&(num1<num2)) m_sommets[num2]->ajouterSucc(m_sommets[num1]);
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


double Graphe::calculDegre(int num)
{
    double deg=0;
    double CG=0;

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
        CG=deg/(m_sommets.size()-1);
return CG;
}

void Graphe:: sauvegarde(std::string nomFichier)
{
    std::ofstream ifs{nomFichier};
    for(size_t i=0;i<m_sommets.size();++i)
    {

        double num=calculDegre(i);
        if (ifs.is_open())
        ifs<<m_sommets[i]->getNum()<<m_sommets[i]->getNom()<<num<<"\n";
    }
    ifs.close();
}

void Graphe::affichage_Resultat1()
{
    for(size_t i=0;i<m_sommets.size();++i)
    {
        double num=calculDegre(i);
        std::cout<<m_sommets[i]->getNum()<<" : "<<m_sommets[i]->getNom()<<" degre = "<<num<<std::endl;
    }
}

void Graphe::VectorPropre()
{
    ///initialisation
    for (size_t i=0;i<m_sommets.size();++i)
    {
        m_sommets[i]->set_indice(1);
    }
    int L=0;
    do {
        for(auto s: m_sommets)
        {
            for (auto succ:s->getSuccesseurs())
            {
                L= succ->getNum()+L;

            }

        }
         double C=sqrt(L*L);
          for(auto s: m_sommets)
        {
            for (auto succ:s->getSuccesseurs())
            {
                L= (succ->getNum()+L)/C;
               std::cout<<L<<std::endl;
            }

        }
    }while(L>0&& L<500);
}
