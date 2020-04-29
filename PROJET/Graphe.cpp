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


        for (int i=0; i<ordre; ++i)
        {
        int indice;
        char nom;
        int x,y;
        ifs>>indice>>nom>>x>>y;
        m_sommets.push_back( new Sommet(indice,nom,x,y) );

        }
         int taille;
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
         m_arretes.push_back(new Arrete(indice2,m_sommets[num1],m_sommets[num2]));

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
        std::cout<<"taille="<< m_arretes.size()<<std::endl<<"  ";
         std::cout<<"listes d'adjacence :"<<std::endl;
       for (auto s : m_arretes)
        {
            std::cout<<"heo";
            s->afficher();
            std::cout<<std::endl;
        }

    }


void Graphe::Dessiner(Svgfile &svgout) const
{
    svgout.addGrid();

    ///on dessine les sommets

    for(int i=0;i<m_sommets.size();++i)
    {
        m_sommets[i]->Dessiner(svgout);
    }

    /// on dessine les arretes
    for(int j=0;j<m_arretes.size();++j)
    {
        m_arretes[j]->Dessiner(svgout);
    }


}


