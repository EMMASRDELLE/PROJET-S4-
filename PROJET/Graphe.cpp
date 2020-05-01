#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graphe.h"
#include "svgfile.h"
#include <queue>
#include <time.h>


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
///m�thode d'affichage
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
///DessinerSVG
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
void Graphe::affichage_Resultat1(Svgfile &svgout)
{
        int test;
    std::vector<double>vec;
    std::vector <double> bon;
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        double num=calculDegre(i);
        double CG=num/(m_sommets.size()-1);
        vec.push_back(CG);


        std::cout<<m_sommets[i]->getNum()<<" : "<<m_sommets[i]->getNom()<< "Degre : "<<num<<" Calcul= "<<CG<<std::endl;
    }


std::sort (vec.begin(), vec.end(), [](double a1, double a2)
    {
        return a1>a2;
    });
    for(auto s:m_sommets)
 {
      double num=calculDegre(s->getNum());
    double CG=num/(m_sommets.size()-1);

     if(CG==vec[0])///100%
     {
         s->colorier(svgout,0);


     }

     if(CG<vec[0]&&CG>0.90*vec[0])///90%-100%
     {
         s->colorier(svgout,1);
     }
     if(CG>vec[0]*0.80&&CG<vec[0]*0.90)///80%-90%
     {
         s->colorier(svgout,2);
     }
      if(CG>vec[0]*0.60&&CG<vec[0]*0.80)///60%-80%
     {

         s->colorier(svgout,3);
     }

      if(CG>vec[0]*0.50&&CG<vec[0]*0.60)///50%-60%
     {
         s->colorier(svgout,4);
     }

     if(CG>vec[vec.size()-1]&&CG<vec[0]*0.50)///<50%
     {
         s->colorier(svgout,5);
     }

     if( CG==vec[vec.size()-1])/// egal a la plus petite
     {
         s->colorier(svgout,6);
     }
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


        }
        std::cout<<std::endl;
        Lambdapred=Lambda;
    }
    while(Lambda>=Lambdapred*1.05||Lambda<=0.95*Lambdapred);
    return Resultat;

}
void Graphe::SauvegardeVP(Svgfile&svgout)
{
    double Lambda=0;
    std::vector<double> vec=VectorPropre(Lambda);
    Lambda=0;
    std::vector<double>vec2=VectorPropre(Lambda);
    Lambda=0;
    std::ofstream ifs{"Resultat2.txt"};

    for(int i=0; i< vec.size()&&i<m_sommets.size(); ++i)
    {
        std::cout<<m_sommets[i]->getNom()<<" "<<"Lambda :"<< Lambda<<" "<< "ResultatVP :"<< vec[i]<<std::endl;
        ifs<<m_sommets[i]->getNom()<<" "<<Lambda<<" "<< vec[i]<<std::endl;
    }
std::sort (vec2.begin(), vec2.end(), [](double a1, double a2)
    {
        return a1>a2;
    });
    for(int i=0;i<m_sommets.size();++i)
    {
       std::vector<double> vec=VectorPropre(Lambda);

        if(vec[i]==vec2[0])///100%
     {
         m_sommets[i]->colorier(svgout,0);

     }

     if(vec[i]<vec2[0]&&vec[i]>0.90*vec2[0])///90%-100%
     {
         m_sommets[i]->colorier(svgout,1);
     }
     if(vec[i]>vec2[0]*0.80&&vec[i]<vec2[0]*0.90)///80%-90%
     {
         m_sommets[i]->colorier(svgout,2);
     }
      if(vec[i]>vec2[0]*0.60&&vec[i]<vec2[0]*0.80)///60%-80%
     {

         m_sommets[i]->colorier(svgout,3);
     }

      if(vec[i]>vec2[0]*0.50&&vec[i]<vec2[0]*0.60)///50%-60%
     {
         m_sommets[i]->colorier(svgout,4);
     }

     if(vec[i]>vec2[vec2.size()-1]&&vec[i]<vec2[0]*0.50)///<50%
     {
         m_sommets[i]->colorier(svgout,5);
     }

     if( vec[i]==vec2[vec2.size()-1])/// egal a la plus petite
     {
         m_sommets[i]->colorier(svgout,6);
     }
    }
    }
std::vector<int> Graphe::Djikstra(int debut, double &Cps, double & somme )
{
    ///Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int s;
    int id;
    int temp2=9999;

    dists[debut]=0;/// Poids du sommet de d�part

    while(temp==0)
    {
        temp2=9999;
        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                s=i;

                temp2=dists[i]; /// On prend l'arrete avec la plus petite distance
            }
        }
        marquage [s]=1; /// On marque les sommets d�couverts


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

    /*for( unsigned int i=0; i<m_sommets.size(); ++i)
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
    }*/
    std::cout<<std::endl;
    Cps= (m_sommets.size()-1)/somme;


    return preds;
}
void Graphe::sauvegarderProximite(Svgfile&svgout)
{
    std::vector<int> dji;
     std::vector<int> vec;
     std::vector<int> vec2;
    double Cps=0;
    double somme=0;
    std::ofstream ifs{"Resultat3.txt"};
    for (auto s :m_sommets)
    {
        dji=Djikstra(s->getNum(), Cps, somme);
        vec2.push_back(Cps);
        std::cout<<"Sommet : "<<s->getNom()<<" "<< "Somme :"<<somme<<" "<< " VP :"<< Cps<<std::endl;
        ifs<<s->getNom()<<" "<<somme<<" "<<Cps<<std::endl;

    }
    std::sort (vec2.begin(), vec2.end(), [](double a1, double a2)
    {
        return a1>a2;
    });
    Cps=0;
    somme=0;

    for(int i=0;i<m_sommets.size();++i)
    {
        Djikstra(m_sommets[i]->getNum(),Cps,somme);
         if(Cps==vec2[0])///100%
     {
         m_sommets[i]->colorier(svgout,0);

     }

     if(Cps<vec2[0]&&vec[i]>0.90*vec2[0])///90%-100%
     {
         m_sommets[i]->colorier(svgout,1);
     }
     if(Cps>vec2[0]*0.80&&vec[i]<vec2[0]*0.90)///80%-90%
     {
         m_sommets[i]->colorier(svgout,2);
     }
      if(Cps>vec2[0]*0.60&&vec[i]<vec2[0]*0.80)///60%-80%
     {

         m_sommets[i]->colorier(svgout,3);
     }

      if(Cps>vec2[0]*0.50&&vec[i]<vec2[0]*0.60)///50%-60%
     {
         m_sommets[i]->colorier(svgout,4);
     }

     if(Cps>vec2[vec2.size()-1]&&vec[i]<vec2[0]*0.50)///<50%
     {
         m_sommets[i]->colorier(svgout,5);
     }

     if( Cps==vec2[vec2.size()-1])/// egal a la plus petite
     {
         m_sommets[i]->colorier(svgout,6);
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

std::vector<int> Graphe::Intermediarite(int debut,int Sommet)
{
    ///Initialisation des variables
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

void Graphe::supprimer_arrete( int indice)
{
    Sommet* s1;
    Sommet* s2;

    for(int j=0; j<m_arretes.size(); ++j) /// on parcours la liste d'ar�tes
    {
        if(m_arretes[j]->getIndice()==indice)/// si l indice demand� est trouv� dans la liste arr�te
        {
            ///on a les 2 sommets extremit�s
            s1=m_arretes[j]->getEx1();
            s2=m_arretes[j]->getEx2();

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


            delete m_arretes[j];
            m_arretes.erase(m_arretes.begin()+j);
        }
    }
    std::cout <<"On affiche la nouvelle liste : "<<std::endl;
    afficherListe();
    for(int i=0; i<m_arretes.size(); ++i)
    {
        m_arretes[i]->set_indice(i);
        std::cout<<m_arretes[i]->getIndice()<<" "<<i<<std::endl;
    }



}

void Graphe::VulnerabiliteDegre(int num)
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

    supprimer_arrete(num);

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
void Graphe::VulnerabiliteVP( int num)
{
    double Lambda=0;
    std::vector <double> Result=VectorPropre(Lambda);
    std::vector<double> Result2;
    double diff=0;
    /// SANS SUPPRESSION

    supprimer_arrete(num);

    ///AVEC SUPPRESSION

    Result2=VectorPropre(Lambda);
    std::cout<<"RESULTAT DE LA DIFFERENCE"<<std::endl;
    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i];
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl;
    }
}

void Graphe ::VulnerabiliteDjikstra(int num)
{
    std::vector<double> Result;
    std::vector<double> Result2;
    std::vector<int> dji;
    std::vector<int> dji2;
    double Cps;
    double somme;
    double diff;

    for (auto s :m_sommets)
    {
        dji=Djikstra(s->getNum(), Cps, somme);
        Result.push_back(Cps);
    }
    for (auto v : Result)
    {
        std::cout<<v<<std::endl;
    }

    supprimer_arrete(num);
    for(auto s:m_sommets)
    {
        std::cout<<"eh ouais";
        dji2=Djikstra(s->getNum(), Cps, somme);
        std::cout<<"mabelle";
        Result2.push_back(Cps);

    }
    std::cout<<"ouais";
    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i];
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<"  "<<" "<< "Difference : "<<diff<<std::endl;
    }

}
void Graphe::MenuVulnerabilite()
{
    ///MENU
    int choix=0;
    int num=0;
    std::cout<<"Choisissez une arrete a supprimer"<<std::endl;
    std::cin>>num;
    std::cout<<"Que voulez vous comparer"<<std::endl;
    std::cout<<"1) Centralite de degre"<<std::endl;
    std::cout<<"2) Centralite de vector propre"<<std::endl;
    std::cout<<"3) Centralite de proximite"<<std::endl;
    std::cout<<"4) Centralite Intermediarite"<<std::endl;
    do
    {
        std::cin>>choix;
    }
    while(choix<0&&choix>4);
    std::cout<<"RESULTAT DE LA DIFFERENCE"<<std::endl;

    if(choix==1)
    {
        Graphe {"Graphe.txt"};
        VulnerabiliteDegre(num);
    }

    if(choix==2)
    {
        Graphe {"Graphe.txt"};
        VulnerabiliteVP(num);
    }
    if(choix==3)
    {
        Graphe {"Graphe.txt"};
        VulnerabiliteDjikstra(num);

    }
    if(choix==4)
    {

    }

}
std::vector<int> Graphe::BFS(int num_s0, int & compteur)const
{
    /// d�claration de la file
    std::queue< Sommet*> file;
    /// pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);
    ///pour noter les pr�d�cesseurs : on note les num�ros des pr�d�cesseurs (on pourrait stocker des pointeurs sur ...)
    std::vector<int> preds((int)m_sommets.size(),-1);
    ///�tape initiale : on enfile et on marque le sommet initial
    file.push(m_sommets[num_s0]);
    couleurs[num_s0]=1;
    Sommet*s;
    compteur=0;
    ///tant que la file n'est pas vide
    while(!file.empty())
    {
        ///on d�file le prochain sommet
        s=file.front();
        file.pop();
        ///pour chaque successeur du sommet d�fil�
        for(auto succ:s->getSuccesseurs())
        {
            if(couleurs[succ->getNum()]==0)
            {
                ///s'il n'est pas marqu�
                couleurs[succ->getNum()]=1;///on le marque
                preds[succ->getNum()]=s->getNum();///on note son pr�decesseur (=le sommet d�fil�)
                file.push(succ);///on le met dans la file

            }
        }
        ++compteur;
    }
    return preds;
}
void Graphe::testConnexe()
{
    std::vector<int> bfs;
    int compteur;
    for(auto s: m_sommets)
    {
        bfs=BFS(s->getNum(), compteur);
        std::cout<<std::endl;

    }

    if(compteur==m_sommets.size())
    {
        std::cout<<" CE GRAPHE EST CONNEXE"<<std::endl;
        std::cout<<std::endl;
    }
    else
    {
        std::cout<<" CE GRAPHE EST NON CONNEXE"<<std::endl;
        std::cout<<std::endl;
    }

}
void Graphe::afficher_parcours(size_t num,const std::vector<int>& arbre)
{
    for(size_t i=0; i<arbre.size(); ++i)
    {
        if(i!=num)
        {
            if(arbre[i]!=-1)
            {
                std::cout<<i<<" <-- ";
                size_t j=arbre[i];
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

void Graphe::kconnexe()
{
    std::vector<int> bfs;
    int compteur;
    int test=0;
    int alea=0;
    do
    {
        alea=rand()%(m_sommets.size());

        for(auto s: m_sommets)
        {
            bfs=BFS(s->getNum(), compteur);
            std::cout<<std::endl;

        }
        std::cout<<compteur;
        if(compteur==m_sommets.size())
        {
            test=test+1;
        }

    }
    while (compteur==m_sommets.size());
    std::cout<<test<<"-arrete connexe "<<std::endl;
}



void Graphe::SupprimerSommet(int indice)
{
    for (s :m_sommets)
    {
        if (s->getNum()==indice)
        {
            for(a :m_arretes)
            {
                if(a->getEx1()->getNum()==s->getNum()|| a->getEx2()->getNum()==s->getNum())
                {
                    ///on a les 2 sommets extremit�s
                    Sommet*s1=a->getEx1();
                    Sommet*s2=a->getEx2();

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
            delete s;
        }
    }
}
