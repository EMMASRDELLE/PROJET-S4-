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
}}
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
/*méthode d'affichage*/
void Graphe::afficher()const
{
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
    //on dessine les sommets

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

// Calcul de degré
double Graphe::calculDegre(int num)
{
    double deg=0;

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

    return deg;
}

void Graphe:: sauvegarde(std::string nomFichier)
{
    std::ofstream ifs{nomFichier};
    for(size_t i=0; i<m_sommets.size(); ++i)
    {

        double deg=calculDegre(i);
        double CG=deg/(m_sommets.size()-1);

        if (ifs.is_open())
            ifs<<m_sommets[i]->getNum()<<m_sommets[i]->getNom()<<" "<<deg<<" "<< CG<<"\n";
    }
    ifs.close();
}

void Graphe::affichage_Resultat1()
{
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        double num=calculDegre(i);
        double CG=num/(m_sommets.size()-1);

        std::cout<<m_sommets[i]->getNum()<<" : "<<m_sommets[i]->getNom()<< "Degre : "<<num<<" Calcul= "<<CG<<std::endl;
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
            std::cout<<s->getNom()<<" Lambda : "<<Lambda<<" Calcul : "<<Result<<std::endl;
            ifs<<s->getNum()<<" "<<s->getNom()<<" "<<Lambda<<" "<< Result<<std::endl;;
        }
        Lambdapred=Lambda;
    }
    while((Lambda-Lambdapred>0));
}

std::vector<int> Graphe::Djikstra(int debut, std::string Nomfichier)
{
    /// fichier
    std::ofstream ifs{Nomfichier};
    //Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int s;
    int id;
    int temp2=9999;
    float Cps;
    float somme;

    dists[debut]=0;// Poids du sommet de départ

    while(temp==0)
    {
        temp2=9999;
        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                s=i;
                temp2=dists[i]; // On prend l'arrete avec la plus petite distance

            }
        }
        marquage [s]=1; // On marque les sommets découverts


        for( auto a:m_arretes)
        {
            if (a->getEx1()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<dists[a->getEx2()->getNum()])
                {
                    dists[a->getEx2()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();
                    somme= somme+dists[s]+a->getPoids();

                }
            }
            if(a->getEx2()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();
                    somme= somme+dists[s]+a->getPoids();

                }
            }
        }
        temp=1;
        for(unsigned int i=0; i<m_sommets.size(); ++i)
        {
            if(marquage[i]==0)
                temp=0;
        }
    }

    for( unsigned int i=0; i<m_sommets.size(); ++i)
    {

        std::cout<<std::endl;
        if(i!=debut)
        {
            std::cout<<i;
            id=i;
            while(preds[id]!=-1)
            {
                std::cout<<" <--"<<preds[id];
                id=preds[id];

            }
        }
    } std::cout<<std::endl;
     Cps= (m_sommets.size()-1)/somme;
    for( auto p: m_sommets)
    {
          if(p->getNum()==debut)
          {
             std::cout<< "sommet " << p->getNom() << "Somme : "<<somme<< " a une proximite de " << Cps<<std::endl;
            ifs<<p->getNom()<<" "<<somme<<" "<<Cps;
          }

    }

    return preds;
}

void Graphe::afficherListe()
{
    std::cout<<"listes d'adjacence :"<<std::endl;
        for (auto s : m_sommets)
        {
            s->afficher();
            std::cout<<std::endl;
        }
}


void Graphe::Calculproximite(std::string Nomfichier)
{
    std::vector<int> dji;
    for(auto p:m_sommets)
    {
        dji=Djikstra(p->getNum(), Nomfichier);

}}


/*void Graphe::Calculintermediarite()
{

}*/
std::vector<int> Graphe::Intermediarite(int debut,int Sommet)
{
    //Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int s;
    int id;
    int temp2=9999;
    float somme;
    int cpt=0;

    dists[debut]=0;// Poids du sommet de départ

    while(temp==0)
    {
        temp2=9999;
        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                s=i;
                temp2=dists[i]; // On prend l'arrete avec la plus petite distance

            }
        }
        marquage [s]=1; // On marque les sommets découverts


        for( auto a:m_arretes)
        {
            if (a->getEx1()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<dists[a->getEx2()->getNum()])
                {
                    dists[a->getEx2()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();
                    somme= somme+dists[s]+a->getPoids();

                }
            }
            if(a->getEx2()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();
                    somme= somme+dists[s]+a->getPoids();

                }
            }
        }
        temp=1;
        for(unsigned int i=0; i<m_sommets.size(); ++i)
        {
            if(marquage[i]==0)
                temp=0;
        }
    }

    for( unsigned int i=0; i<m_sommets.size(); ++i)
    {

        std::cout<<std::endl;
        if(i!=debut)
        {
            std::cout<<i;
            id=i;
            while(preds[id]!=-1)
            {
                 std::cout<<" <--"<<preds[id];


                if((preds[id]==Sommet))
                {
                     cpt=cpt+1;

                      std::cout<<"    aFFICHAGE   "<<cpt<<std::endl;

                }
                if(id==Sommet)
                {
                    cpt++;
                    std::cout<<" JNSNJNSV";
                }
                if((id==Sommet)&&(preds[id]==Sommet))
                {
                    cpt=0;
                }
                id=preds[id];



            }
        }

    } std::cout<<std::endl;
  std::cout<<"compteur"<<cpt<<std::endl;

    return preds;
}

