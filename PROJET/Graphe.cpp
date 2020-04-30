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

        m_arretes.push_back(new Arete(m_sommets[num1],m_sommets[num2],indice2,1));
        m_sommets[num1]->ajouterSucc(m_sommets[num2]);
        if((!m_orientation)&&(num1<num2))
            m_sommets[num2]->ajouterSucc(m_sommets[num1]);

    }
}
/*m�thode d'affichage*/
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

// Calcul de degr�
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

void Graphe:: sauvegarde()
{
    std::ofstream ifs{"Resultat1.txt"};
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

std::vector<double> Graphe::VectorPropre( double &Lambda)
{
    std::vector<double >Resultat;
    ///initialisation
    for (size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->set_indice(1);
    }
    double Somme=0;
    double Lambdapred=0;
   Lambda=0;
    double T=0;
    std::vector<int> somme;
    do
    {

        for(int i=0; i<m_sommets.size(); ++i)
        {
            for (auto succ:m_sommets[i]->getSuccesseurs())
            {
                Somme= succ->getNum()+Somme;


            }
            Somme=0;
        }
        double Total=0;
        for(auto s:m_sommets)
        {
            for(auto succ:s->getSuccesseurs())
            {
                Somme=succ->getNum()+Somme;
                T=Somme*Somme;
            }
            Somme=0;
            Total =T+Total;
            Lambda=sqrt(Total);
        }


        double G=0;
        for(auto s: m_sommets)

        {
            for (auto succ:s->getSuccesseurs())
            {
                G= succ->getNum()+G;

            }

            double Result=G/Lambda;
            G=0;
            Resultat.push_back(Result);


        } std::cout<<std::endl;
        Lambdapred=Lambda;
    }
    while(Lambda>=Lambdapred*1.05||Lambda<=0.95*Lambdapred);
    return Resultat;

}
void Graphe::SauvegardeVP()
{
    double Lambda=0;
    std::vector<double> vec=VectorPropre(Lambda);
     std::ofstream ifs{"Resultat2.txt"};

    for(int i=0; i< vec.size()&&i<m_sommets.size();++i)
   {
        std::cout<<m_sommets[i]->getNom()<<" "<<"Lambda :"<< Lambda<<" "<< "ResultatVP :"<< vec[i]<<std::endl;
         ifs<<m_sommets[i]->getNom()<<" "<<Lambda<<" "<< vec[i]<<std::endl;
   }

}
std::vector<int> Graphe::Djikstra(int debut, double &Cps, double & somme )
{
    /// fichier
    std::ofstream ifs{"CalculProximite.txt"};
    ///Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);



    int temp=0;
    int s;
    int id;
    int temp2=9999;


    dists[debut]=0;// Poids du sommet de d�part

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
        marquage [s]=1; // On marque les sommets d�couverts


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
    }
    std::cout<<std::endl;
    Cps= (m_sommets.size()-1)/somme;
    for( auto p: m_sommets)
{


        if(p->getNum()==debut)
        {
            std::cout<< "sommet " << p->getNom() << "Somme : "<<somme<< " a une proximite de " << Cps<<std::endl;

           // RESULT.push_back(Cps);
        }

    }

    return preds;
}
void Graphe::sauvegarderProximite()
{
    std::vector<int> dji;
    std::vector<double> resultat;
    double Cps=0;
    double somme=0;
   std::ofstream ifs{"Resultat2.txt"};
    for (auto s :m_sommets)
    {
        dji=Djikstra(s->getNum(), Cps, somme);
        std::cout<<"Sommet : "<<s->getNom()<<" "<< "Somme :"<<somme<<" "<< " VP :"<< Cps<<std::endl;
         ifs<<s->getNom()<<" "<<somme<<" "<<Cps<<std::endl;
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
/*void Graphe::Calculproximite()
{
    std::vector<int> dji;
    for(auto p:m_sommets)
    {
        dji=Djikstra(p->getNum());
    }
}
*/
std::vector<int> Graphe::Intermediarite(int debut,int Sommet)
{
    //Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int s;
    int id;
    int temp2=9999;
    float somme;
    int var;
    int cpt=0;

    dists[debut]=0;// Poids du sommet de d�part

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
        marquage [s]=1; // On marque les sommets d�couverts


        for( auto a:m_arretes)
        {
            if (a->getEx1()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<=dists[a->getEx2()->getNum()])
                {
                    dists[a->getEx2()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx2()->getNum()]=a->getEx1()->getNum();
                    somme= somme+dists[s]+a->getPoids();
                    var=dists[s]+a->getPoids();

                    std::cout<<"Sommet "<<a->getEx2()->getNom()<<" : Poids = "<<var<<std::endl;

                }
            }
            if(a->getEx2()->getNum()==s)
            {
                if((dists[s]+a->getPoids())<=dists[a->getEx1()->getNum()])
                {
                    dists[a->getEx1()->getNum()]=dists[s]+a->getPoids();
                    preds[a->getEx1()->getNum()]=a->getEx2()->getNum();
                    somme= somme+dists[s]+a->getPoids();
                    var=dists[s]+a->getPoids();
                    std::cout<<"Sommet2 "<<a->getEx1()->getNom()<<": Poids = "<<var<<std::endl;;

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
    double cmpt3=0;
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
                if(id==Sommet)
                {
                    cpt++;

                    //if(m_sommets[id]<m_sommets)
                    {
                        ++cmpt3;
                    }

                }
                id=preds[id];


            }
        }

    }
    std::cout<<std::endl;
    std::cout<<"compteur"<<cpt<<" "<<cmpt3<<std::endl;

    return preds;
}

void Graphe::supprimer_arrete()
{
    int indice;
    std::cout<<"selectionner l'indice de l'arrete "<<std::endl;
    std::cin>>indice;
    Sommet* s1;
    Sommet* s2;

    for(auto a: m_arretes)/// on parcours la liste d'ar�tes
    {
        if(a->getIndice()==indice)/// si l indice demand� est trouv� dans la liste arr�te
        {
            ///on a les 2 sommets extremit�s
            s1=a->getEx1();
            s2=a->getEx2();


            for(size_t i=0; i<s1->getSuccesseurs().size(); ++i)
            {
                if((s1->getSuccesseurs())[i]->getNum()==s2->getNum())
                {

                    s1->getSuccesseurs().erase(s1->getSuccesseurs().begin()+i);

                }
            }
            for(size_t i=0; i<s2->getSuccesseurs().size(); ++i)
            {
                if((s2->getSuccesseurs())[i]->getNum()==s1->getNum())
                {

                    s2->getSuccesseurs().erase(s2->getSuccesseurs().begin()+i);

                }

            }


            delete a;


        }

    }
    afficherListe();

}

void Graphe::VulnerabiliteDegre()
{
    double deg2,deg=0;
    std::vector <double> Result;
    std::vector<double> Result_deux;
    double Result1=0;
    double Result2=0;
    double diff=0;
    /// SANS SUPPRESSION

    for(auto s: m_sommets)
    {
        deg=calculDegre(s->getNum());
        Result1=deg/(m_sommets.size()-1);
        Result.push_back(Result1);

    }

    supprimer_arrete();

    ///AVEC SUPPRESSION
    for(auto s: m_sommets)
    {
        deg=calculDegre(s->getNum());
        Result2=deg/(m_sommets.size()-1);
        Result_deux.push_back(Result2);

    }

    for(int i=0; i<Result_deux.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result_deux[i]- Result[i];
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl;
    }


}
void Graphe::VulnerabiliteVP()
{
    double deg2,deg=0;
    double Lambda=0;
    std::vector <double> Result=VectorPropre(Lambda);
    std::vector<double> Result2;
    double Result1=0;

    double diff=0;
    /// SANS SUPPRESSION


    supprimer_arrete();

    ///AVEC SUPPRESSION

   Result2=VectorPropre(Lambda);
 std::cout<<"RESULTAT DE LA DIFFERENCE"<<std::endl;
     for(int i=0;i<Result2.size()&&i<Result.size()&&i<m_sommets.size();++i)
     {
         diff=Result2[i]- Result[i];
       std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl;
     }
}


