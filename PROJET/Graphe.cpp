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
            int taille;


        for (int i=0; i<ordre; ++i)
        {
        int indice;
        char nom;
        int x,y;
        ifs>>indice>>nom>>x>>y;
        m_sommets.push_back( new Sommet(indice,nom,x,y) );

        }

         ifs >> taille;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture taille du graphe");

        int indice2;
        int num1;
        int num2;
        for (int i=0; i<taille; ++i)
        {
            ifs>>indice2>>num1>>num2;
            if ( ifs.fail() )
                throw std::runtime_error("Probleme lecture arc");
         m_arretes.push_back(new Arrete(indice2,m_sommets[num1-1],m_sommets[num2-1]));

        }

}

Graphe::~Graphe()
{
     for (int i=0; i<m_sommets.size();i++){

        delete m_sommets[i];
     }
     for (int i=0;i<m_arretes.size();i++){
     delete m_arretes [i];}
}

 void Graphe::afficher() const
    {
        std::cout<<std::endl<<"graphe ";
        if(m_orientation)
            std::cout<<"oriente"<<std::endl<<"  ";
        else
        std::cout<<"non oriente"<<std::endl<<"  ";
        std::cout<<"ordre = "<<m_sommets.size()<<std::endl<<"  ";
         std::cout<<"listes d'adjacence :"<<std::endl;
     /*   for (auto s : m_arretes)
        {
            s->afficher();
            std::cout<<std::endl;
        }*/

    }
