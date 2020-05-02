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

void Graphe::afficher()const
{
    if(m_orientation==1)
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

std::vector<int> Graphe::Djikstra(int num0, std::string Nomfichier)
{
    /// fichier
    std::ofstream ifs{Nomfichier};
    //Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int actuel;
    int temp2=9999;
    double Cps;
    double somme;

    dists[num0]=0;// Poids du sommet de départ

    while(temp==0)
    {
        temp2=9999;
        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                actuel=i;
                temp2=dists[i]; // On prend l'arrete avec la plus petite distance
            }
        }
        marquage [actuel]=1; // On marque les sommets découverts


        for( auto a: m_arretes)
        {
            if (a->getEx1()->getNum()==actuel)
            {
                if((dists[actuel]+a->getPoids())<dists[a->getEx2()->getNum()])
                {
                    dists[a->getEx2()->getNum()]=dists[actuel]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();
                    somme= somme+dists[actuel]+a->getPoids();

                }
            }
            if(a->getEx2()->getNum()==actuel)
            {
                if((dists[actuel]+a->getPoids())<dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[actuel]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();
                    somme= somme+dists[actuel]+a->getPoids();

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
    afficherparcours(num0,preds);


    Cps= (m_sommets.size()-1)/somme;
    for( auto p: m_sommets)
    {
        if(p->getNum()==num0)
        {
            std::cout<< "sommet " << p->getNom() << "Somme : "<<somme<< " a une proximite de " << Cps<<std::endl;
            ifs<<p->getNom()<<" "<<somme<<" "<<Cps;
        }
    }

    return preds;
}
void Graphe::afficherparcours(unsigned int num,std::vector<int> &arbre)
{
    for(unsigned int i=0; i<arbre.size(); ++i)
    {
        if(i!=num)
        {
            if(arbre[i]!=-1)
            {
                std::cout<<i<<" <-- ";
                unsigned j=arbre[i];
                while(j!=num)
                {
                    std::cout<<j<<" <-- ";
                    j=arbre[j];
                }
                std::cout<<j<<std::endl;
            }
        }
    }
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
        dji=Djikstra(p->getNum(),Nomfichier);
    }
}

void Graphe::CalculIntermediarite()
{

    std::vector<int> stock((int)m_sommets.size(),1);
    std::vector<int> stock2((int)m_sommets.size(),1);
    std::vector<float> compt((float)m_sommets.size(),1);
    std::vector<float> compteur((float)m_sommets.size(),0);
    std::vector<float>centralite((float)m_sommets.size(),0);
    std::vector<float>normalise((float)m_sommets.size(),0);
    std::vector<float>compt2((float)m_sommets.size(),0);

int n = m_sommets.size();
    for(unsigned int i=0; i<m_sommets.size(); i++)
    {   //On stock tous les poids du premier parcours
        stock=Intermediarite(i,compt); //Intermediaire

        for(unsigned int j=0; j<m_sommets.size(); ++j)
        {   // On stocke les poids a partir du depart
            stock2 = Intermediarite(j,compt2);//Depart

            for(unsigned int t=j; t<m_sommets.size(); t++) // Intermediaire a arrivée
            {
                if (i!=j&&j!=t&&i!=t)// Arrivée
                {
                    if(stock[j]+stock[t]== stock2[t])
                    {
                        compteur [j]= compt[t]*compt[j];// On multiplie les pcc de depart a int puis de int a arrivée

                        if(compt2[t]!=0&&compteur[j]!=0)
                        {
                            centralite[i]=centralite[i]+(compteur[j]/compt2[t]); //Compteur a la fin de la boucle for somme j a arrivée
                            normalise[i]=(2*centralite[i])/(n*n-3*n +2); //On normalise

                        }
                    }
                }

            }

        }

    }std::cout<< "Indice de centralite non normalise "<< std::endl;
    for(unsigned int i =0; i<m_sommets.size(); ++i)
    {
        std::cout<<i<<" centralite "<< centralite [i] <<std::endl;
    }
    std::cout<< "\nIndice de centralite normalise "<< std::endl;
     for(unsigned int i =0; i<m_sommets.size(); ++i)
    {
        std::cout<<i<<" centralite normalisee "<<normalise[i]<<std::endl;
    }
}


std::vector<int> Graphe::Intermediarite(unsigned int num0,  std::vector<float> &compt)
{
    //Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);//Aucun sommet n'est découvert
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);//Liste des prédecesseurs
    int temp=0;
    int actuel;
    int temp2=9999;

    compt[num0]=0;
    dists[num0]=0;// Poids du sommet de départ

    while(temp==0)
    {
        temp2=9999;//longueur infinie pour une arrete  non découverte

        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2))
            {
                if((marquage[i]==0))
                {
                    actuel=i;
                    temp2=dists[i]; // On prend l'arrete avec la plus petite distance
                }
            }
        }
        marquage [actuel]=1; // On marque les sommets découverts

        for( auto a:m_arretes)
        {
            if (a->getEx1()->getNum()==actuel)
            {
                if((dists[actuel]+a->getPoids())<dists[a->getEx2()->getNum()])
                {
                    dists[a->getEx2()->getNum()]=dists[actuel]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();

                    if(actuel!=num0)
                    {
                        compt[a->getEx2()->getNum()]=compt[actuel];
                    }
                    else
                    {
                        compt[a->getEx2()->getNum()]=1;
                    }
                }
                else  if((dists[actuel]+a->getPoids())==dists[a->getEx2()->getNum()])
                {

                    dists[a->getEx2()->getNum()]=dists[actuel]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();

                    compt[a->getEx2()->getNum()]=compt[a->getEx2()->getNum()]+compt[actuel];


                }
            }
            if(a->getEx2()->getNum()==actuel)
            {
                if((dists[actuel]+a->getPoids())<dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[actuel]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();

                    if(actuel!=num0)
                    {
                        compt[a->getEx1()->getNum()]=compt[actuel];
                    }

                    else
                    {
                        compt[a->getEx1()->getNum()]=1;
                    }

                }
                else  if((dists[actuel]+a->getPoids())==dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[actuel]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();
                    compt[a->getEx1()->getNum()]=compt[a->getEx1()->getNum()]+compt[actuel];
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
    // afficherparcours(num0,preds);

    return dists;
}
