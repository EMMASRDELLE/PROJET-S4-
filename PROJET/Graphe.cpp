#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graphe.h"
#include "svgfile.h"
#include <queue>


void Graphe::ChargementFichierPond(std::string nomFichier)
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    std::vector <int> Dji;
    int taille;
    ifs >> taille;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    int indice;
    double poids;

    for (int i=0; i<taille; ++i)
    {
        ifs>>indice>>poids;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture arc");

        m_arretes[indice]->setPoids(poids);


    }
    Dji=Djikstra(4);
}
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

        m_arretes.push_back(new Arrete(m_sommets[num1],m_sommets[num2],indice2,1));
        m_sommets[num1]->ajouterSucc(m_sommets[num2]);
        if((!m_orientation)&&(num1<num2))
            m_sommets[num2]->ajouterSucc(m_sommets[num1]);

    }
}
/*m�thode d'affichage*/
void Graphe::afficher()const
{
    std::cout << "------------------------------PROJET PISCINE-------------------------------------\n"<<std::endl;

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

    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->Dessiner(svgout);
    }

    /// on dessine les arretes
    for(size_t j=0; j<m_arretes.size(); ++j)
    {
        m_arretes[j]->Dessiner(svgout);
    }


}

/// Calcul de degr�
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
    for(size_t i=0; i<m_sommets.size(); ++i)
    {

        double num=calculDegre(i);
        if (ifs.is_open())
            ifs<<m_sommets[i]->getNum()<<m_sommets[i]->getNom()<<num<<"\n";
    }
    ifs.close();
}

void Graphe::affichage_Resultat1()
{
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        double num=calculDegre(i);
        std::cout<<m_sommets[i]->getNum()<<" : "<<m_sommets[i]->getNom()<<" = "<<num<<std::endl;
    }
}


void Graphe::VectorPropre(std::string Nomfichier)
{
    std::ofstream ifs{Nomfichier};
    ///initialisation
    for (size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->set_indice(1);
    }
    double Somme=0;
    double Lambdapred=0;
    double Lambda=0;
    double T=0;

    do
    {

        for(auto s: m_sommets)
        {
            for (auto succ:s->getSuccesseurs())
            {

                Somme= succ->getNum()+Somme;


            }
            T=T+Somme;

            Somme=0;
        }

        Lambda=sqrt(T*T);
        double G=0;
        for(auto s: m_sommets)

        {
            for (auto succ:s->getSuccesseurs())
            {
                G= succ->getNum()+G;

            }

            double Result=G/Lambda;
            G=0;
            std::cout<<s->getNom()<<" "<<Result<<std::endl;
            ifs<<s->getNum()<<" "<<s->getNom()<<" "<<Result<<std::endl;;
        }
        Lambdapred=Lambda;
    }
    while((Lambda-Lambdapred>0));
}

std::vector<int> Graphe::Djikstra(int debut)
{
    ///Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);/// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int s;
    int id;
    int temp2=9999;

    dists[debut]=0;/// Poids du sommet de d�part

    while(temp==0)
    {
        temp2=9999;
        for (int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                s=i;
                temp2=dists[i]; /// On prend l'arrete avec la plus petite distance

            }
        }
        marquage [s]=1; /// On marque les sommets d�couverts


        for( auto a: m_arretes)
        {
            if (a->getEx1()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<dists[a->getEx2()->getNum()])
                {
                    dists[a->getEx2()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();
                    std::cout<<"Poids"<<dists[s]+a->getPoids();
                }
            }
            if(a->getEx2()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();
                    std::cout<<"Poids:  "<<dists[s]+a->getPoids();
                }
            }

        }
        temp=1;
        for(int i=0; i<m_sommets.size(); ++i)
        {
            if(marquage[i]==0)
                temp=0;
        }
    }

    for(int i=0; i<m_sommets.size(); ++i)
    {

        std::cout<<std::endl;
        if(i!=debut)
        {
            std::cout<<i;
            id=i;
            while(preds[id]!=-1)
            {
                std::cout<<" -->"<<preds[id];
                id=preds[id];

            }
        }
    }

    return preds;
}

