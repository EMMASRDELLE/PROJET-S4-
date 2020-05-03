#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graphe.h"
#include "svgfile.h"
#include <queue>
#include <time.h>

///Chargement du fichier Pond�r�
void Graphe::ChargementFichierPond(std::string nomFichier)
{
    std::ifstream ifs{nomFichier}; ///ouverture du fichier en mode lecture
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    std::vector <int> Dji;
    int taille;
    int indice;
    double poids;
    ifs >> taille; ///on lit la taille
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    for (int i=0; i<taille; ++i)
    {
        ifs>>indice>>poids;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture arc");

        m_arretes[indice]->setPoids(poids); ///on attribue le poids � l'arrete
    }
}
///Constructeur de Graphe
Graphe::Graphe(std::string nomFichier)
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );

    ifs >> m_orientation;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture orientation du graphe");
    int ordre;
    ifs >> ordre; ///on lit l'ordre
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");
    int indice;
    double x,y;
    std::string nom;
    for (int i=0; i<ordre; ++i)
    {
        ifs>>indice>>nom>>x>>y;
        m_sommets.push_back( new Sommet{indice,nom,x,y} ); ///on cr�e un nouveau sommet
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

        m_arretes.push_back(new Arete(m_sommets[num1],m_sommets[num2],indice2,1)); ///on cr�e une arrete
        m_sommets[num1]->ajouterSucc(m_sommets[num2]); ///on ajoute les successeurs du sommet 1
        if((!m_orientation)&&(num1<num2)) /// si graphe orient�
        {
            m_sommets[num2]->ajouterSucc(m_sommets[num1]);
        }

    }
}

///m�thode d'affichage
void Graphe::afficher()const
{
    if(m_orientation==1)
        std::cout<<"Le graphe est oriente"<<std::endl<<"  ";
    else
        std::cout<<"Le graphe est non oriente "<<std::endl<<"\n";
    std::cout<<"  - Ordre : "<<m_sommets.size()<<std::endl<<"\n";
    std::cout<<"  - Taille :"<<m_arretes.size()<<std::endl<<"\n";
    for (auto a : m_arretes)
    {
        a->afficher();
        std::cout<<std::endl;
    }
}
///methode destructeur
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

    for (auto b: m_arretes) ///on parcours les arretes
    {
        for (auto a : m_sommets) ///on parcours les sommets
        {

            if (a==m_sommets[num]) ///si un sommet est �gale au sommet de r�f�rence
            {
                if(b->CalculDEG(a,b)==true) ///si CalculDeg renvoie vrai
                {
                    ++deg; ///on agremente
                }
            }
        }
    }

    return deg;
}
///Sauvegarde du Resultat du calcul de degr�
void Graphe:: sauvegarde()
{
    std::ofstream ifs{"Resultat1.txt"}; ///ouverture du fichier en mode �criture
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        double deg=calculDegre(i);
        double CG=deg/(m_sommets.size()-1);
        if (ifs.is_open())
            ifs<<m_sommets[i]->getNum()<<" "<<m_sommets[i]->getNom()<<" "<<deg<<" "<< CG<<"\n";///on ecrit dans le fichier
    }
    ifs.close();
}
///Affichage du resultat du calcul de degr�
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
        std::cout<<m_sommets[i]->getNum()<<" : "<<m_sommets[i]->getNom()<<" "<< "Degre : "<<num<<" Calcul= "<<CG<<std::endl;

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

        if(CG>vec[vec.size()-1]&&CG<=vec[0]*0.50)///<50%
        {
            s->colorier(svgout,5);
        }

        if( CG==vec[vec.size()-1])/// egal a la plus petite
        {
            s->colorier(svgout,6);
        }


    }

}
///Calcul de l'indice Vector Propre
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
                Somme= succ->getNum()+Somme; ///on additionne les indices de tous les successeurs du sommet
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
            Resultat.push_back(Result); ///on push le result normalis� dans un vecteur
        }
        std::cout<<std::endl;
        Lambdapred=Lambda; ///le lambda precedent prend la variation de lambda
    }
    while(Lambda>=Lambdapred*1.05||Lambda<=0.95*Lambdapred); ///les variations de lambda
    return Resultat; ///return le vecteur de Resultat comprenant l'ensemble des valeurs normalis�es

}
///Sauvegarde et affichage Vector Propre
void Graphe::SauvegardeVP(Svgfile&svgout)
{
    double Lambda=0;
    std::vector<double> vec=VectorPropre(Lambda);
    Lambda=0;
    std::vector<double>vec2=VectorPropre(Lambda);
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

    Lambda=0;
    for(int i=0; i<m_sommets.size(); ++i)
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

        if(vec[i]>vec2[vec2.size()-1]&&vec[i]<=vec2[0]*0.50)///<=50%
        {
            m_sommets[i]->colorier(svgout,5);
        }

        if( vec[i]==vec2[vec2.size()-1])/// egal a la plus petite
        {
            m_sommets[i]->colorier(svgout,6);
        }
    }
}

int Graphe::poidsSucc(Sommet* a, Sommet* b)
{
    for (int i=0;i<m_arretes.size();++i)
    {
        if(m_arretes[i]->getEx1()==a&&m_arretes[i]->getEx2()==b)
            {
                return m_arretes[i]->getPoids();
            }
        if(m_arretes[i]->getEx1()==b&&m_arretes[i]->getEx2()==a)
            {
                return m_arretes[i]->getPoids();
            }

    }
}
///Affichage Parcours
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
///CALCUL INDICE DE PROXIMITE
std::vector<int> Graphe::Djikstra(int num0, double &Cps, double & somme )
{
    ///Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);///Aucun sommet n'est marqu� au d�part
    std::vector<int> dists((int)m_sommets.size(),99999);/// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int actuel;
    int temp2=99999;///Longueur infinie pour les distance non d�couverte
    int coumt = 0;
    int poids=0;
    Sommet *p;/// Sommet de parcours

    dists[num0]=0;/// Poids du sommet de d�part
    actuel=num0;

    while(temp==0)
    {
        coumt = 0;
        temp2=99999;
        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                actuel=i;
                temp2=dists[i]; /// On prend l'arrete avec la plus petite distance
            }
            else
            {
                coumt++; ///On compte le nombre fois ou il n'y a pas d'arrete plus petite
            }
        }
      for (int i=0;i<m_sommets.size();++i)
      {
          if(actuel==m_sommets[i]->getNum())
          {
              p=m_sommets[i]; ///on attribue le sommet qui a un indice �gal � actuel � p
          }
      }
        marquage [p->getNum()]=1; /// On marque les sommets d�couverts


        for(succ: p->getSuccesseurs())
        {
            poids=poidsSucc(p,succ); /// Calcul du poids entre le poids actuel et ses successeurs

            if(dists[p->getNum()]+ poids< dists[succ->getNum()])
            {
                    dists[succ->getNum()]=dists[p->getNum()]+poids;
                    preds[succ->getNum()]=p->getNum(); ///On defile
                    somme= somme+dists[p->getNum()]+poids;
            }
        }
            temp=1;
        if(coumt == m_sommets.size()) /// Il n'y a plus de pcc Dikjstra s'arrete
        {
            temp=1;
            return preds;
        }

          for( int i=0;i<m_sommets.size();i++) /// Lorsque le graphe est connexe Dikjstra s'arrete lorsque tous les sommets sont marqu�s
           {
               if(marquage[i]==0)
               {
                   temp=0;
               }

           }
    }

    Cps= (m_sommets.size()-1)/somme; /// Calcul du Cps

     std::cout<<std::endl;
    return preds;
}
///Sauvegarde  et affichage de l'indice de proximite
void Graphe::sauvegarderProximite(Svgfile&svgout)
{
    std::vector<int> dji;
    std::vector<double> vec2;
    double Cps=0;
    double somme=0;
    std::ofstream ifs{"Resultat3.txt"};
    for (auto s :m_sommets)
    {
        dji=Djikstra(s->getNum(), Cps, somme);
        vec2.push_back(Cps); ///on stock la valeur normalis�e dans un vecteur
        std::cout<<"Sommet : "<<s->getNom()<<" "<< "Somme :"<<somme<<" "<< " VP :"<< Cps<<std::endl;
        ifs<<s->getNom()<<" "<<somme<<" "<<Cps<<std::endl; ///ecriture dans un fichier

    }
    std::sort (vec2.begin(), vec2.end(), [](double a1, double a2) ///tri d�croissant
    {
        return a1>a2;
    });
    Cps=0;
    somme=0;


    for(int i=0; i<m_sommets.size(); ++i)
    {
        Djikstra(m_sommets[i]->getNum(),Cps,somme);


        if(Cps==vec2[0])///100%
        {

            m_sommets[i]->colorier(svgout,0);

        }

        if(Cps<vec2[0]&&Cps>0.90*vec2[0])///90%-100%
        {
            m_sommets[i]->colorier(svgout,1);

        }
        if(Cps>vec2[0]*0.80&&Cps<vec2[0]*0.90)///80%-90%
        {
            m_sommets[i]->colorier(svgout,2);

        }
        if(Cps>vec2[0]*0.60&&Cps<vec2[0]*0.80)///60%-80%
        {
            m_sommets[i]->colorier(svgout,3);

        }

        if(Cps>vec2[0]*0.50&&Cps<vec2[0]*0.60)///50%-60%
        {
            m_sommets[i]->colorier(svgout,4);

        }

        if(Cps>vec2[vec2.size()-1]&&Cps<=vec2[0]*0.50)///<50%
        {
            m_sommets[i]->colorier(svgout,5);

        }

        if( Cps==vec2[vec2.size()-1])/// egal a la plus petite
        {
            m_sommets[i]->colorier(svgout,6);

        }

    }
}
///afficher la liste d'adjacence des sommets
void Graphe::afficherListe()
{
    std::cout<<"listes d'adjacence :"<<std::endl;
    for (auto s : m_sommets)
    {
        s->afficher();
        std::cout<<std::endl;
    }
}
///supprimer une arrete
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
                    s1->getSuccesseurs().erase(s1->getSuccesseurs().begin()+i); ///on erase le successeur

                }
            }
            for(size_t i=0; i<s2->getSuccesseurs().size(); ++i)
            {
                if((s2->getSuccesseurs())[i]->getNum()==s1->getNum())
                {
                    s2->getSuccesseurs().erase(s2->getSuccesseurs().begin()+i); ///on erase le successeur

                }

            }
            delete m_arretes[j]; ///on supprime l'arrete
            m_arretes.erase(m_arretes.begin()+j); ///on l'erase
        }
    }
    std::cout <<"On affiche la nouvelle liste : "<<std::endl;
    afficherListe();
    for(int i=0; i<m_arretes.size(); ++i)
    {
        m_arretes[i]->set_indice(i); ///on reattribut les indices
    }

}
///VULNERABILITE DEGRE
void Graphe::VulnerabiliteDegre(int num)
{
    ///initialisation
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
        Result.push_back(Result1); ///on stock la valeur normalis�e dans un vecteur

    }

    supprimer_arrete(num);

    ///AVEC SUPPRESSION
    for(auto s: m_sommets)
    {
        deg=calculDegre(s->getNum());
        Result2=deg/(m_sommets.size()-1);
        Result_deux.push_back(Result2);///on stock la valeur normalis�e dans un vecteur apr�s suppression

    }

    for(int i=0; i<Result_deux.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result_deux[i]- Result[i];
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl; ///affichage du resultat de la diff
    }
}
///VULNERABILITE Vector Propre
void Graphe::VulnerabiliteVP( int num)
{
    double Lambda=0;
    std::vector <double> Result=VectorPropre(Lambda); ///on retourne l'indice de vector propre normalis�
    std::vector<double> Result2;
    double diff=0;
    /// SANS SUPPRESSION

    supprimer_arrete(num);

    ///AVEC SUPPRESSION

    Result2=VectorPropre(Lambda);///on retourne l'indice de vector propre normalis�
    std::cout<<"RESULTAT DE LA DIFFERENCE"<<std::endl;
    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i]; ///calcul de la difference avant et apr�s suppression
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl;
    }
}
///Vulnerabilite de l'indice de proximite
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
        Result.push_back(Cps);///on stock la valeur normalis�e dans un vecteur
    }
    ///Suppression d'arretes
    supprimer_arrete(num);
    for(auto s:m_sommets)
    {
        dji2=Djikstra(s->getNum(), Cps, somme);
        Result2.push_back(Cps); ///on stock la valeur normalis�e apr�s suppression
    }

    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i];///calcul de la difference
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<"  "<<" "<< "Difference : "<<diff<<std::endl;
    }

}
///BFS
std::vector<int> Graphe::BFS(int num_s0, int & compteur)const
{
    /// d�claration de la file
    std::queue< Sommet*> file;
    /// pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);
    ///pour noter les pr�d�cesseurs
    std::vector<int> preds((int)m_sommets.size(),-1);
    file.push(m_sommets[num_s0]);
    couleurs[num_s0]=1;
    Sommet*s;
    compteur=0;
    ///tant que la file n'est pas vide
    while(!file.empty())
    {
        s=file.front();
        file.pop();
        ///on parcout les successeurs
        for(auto succ:s->getSuccesseurs())
        {
            if(couleurs[succ->getNum()]==0) ///s'il n'est pas marqu�
            {
                couleurs[succ->getNum()]=1;///on le marque
                preds[succ->getNum()]=s->getNum();///on note son pr�decesseur
                file.push(succ);///on le met dans la file

            }
        }
        ++compteur; ///on aggr�mente le compteur
    }
    return preds;
}
///TEST CONNEXE
int Graphe::testConnexe()
{
    std::vector<int> bfs;
    int compteur;
    int test;
    for(auto s: m_sommets)
    {
        bfs=BFS(s->getNum(), compteur);///parcours bfs pour chaque sommet
        std::cout<<std::endl;
    }
    if(compteur==m_sommets.size())
    {
        test=1; ///return 1 si le graphe est toujours connexe
    }
    else
    {
        test=0; ///return 1 si le graphe est non connexe
    }
    return test;
}
///K-ARRETE CONNEXE
void Graphe::kconnexe()
{
    std::vector<int> bfs;
    int compteur=0;
    int test;
    int i=0;
    do
    {
        supprimer_arrete(i);
        test=testConnexe();
        i++;
        ++compteur;
    }
    while(test==1&&i<m_sommets.size()); ///on supprime 1 arrete tant que le graphe est toujours connexe
    std::cout<<compteur<<"-arrete connexe "<<std::endl;
}
///Supprimer un sommet
void Graphe::SupprimerSommet(int indice)
{

    for (int k=0; k<m_sommets.size(); ++k)
    {
        if (m_sommets[k]->getNum()==indice) /// si le sommet est celui de l'indice recherche
        {

            for(int j=0; j<m_arretes.size(); ++j) ///on parcours les arretes
            {
                if(m_arretes[j]->getEx1()->getNum()==m_sommets[k]->getNum()|| m_arretes[j]->getEx2()->getNum()==m_sommets[k]->getNum()) ///si une des extremites de l'arrete est la m�me que le sommet recherche

                {
                    ///on a les 2 sommets extremit�s
                    Sommet*s1=m_arretes[j]->getEx1();
                    Sommet*s2=m_arretes[j]->getEx2();

                    for(size_t i=0; i<s1->getSuccesseurs().size(); ++i)
                    {
                        if((s1->getSuccesseurs())[i]->getNum()==s2->getNum())
                        {

                            s1->getSuccesseurs().erase(s1->getSuccesseurs().begin()+i); ///on libere le successeur

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
                    m_arretes.erase(m_arretes.begin()+j); ///on supprime l'arrete
                }
            }

            delete m_sommets[k];  ///on supprime le sommet
            m_sommets.erase(m_sommets.begin()+k);

        }
    }
}
///K-Sommet Connexe
void Graphe::kconnexeSommet()
{
    std::vector<int> bfs;
    int compteur=0;
    int test;
    int i=0;
    do
    {
        SupprimerSommet(i);
        test=testConnexe();
        i++;
        ++compteur;
    }
    while(test==1&&i<m_sommets.size()); ///on supprime un sommet tant que le graphe est connexe
    std::cout<<compteur<<"-Sommet connexe "<<std::endl;
}
void Graphe::MenuConnexe()
{
    int test;
    int choix;
    std::cout<<"MENU CONNEXITE"<<std::endl;
    std::cout<<"1) Tester la connexite du graphe"<<std::endl;
    std::cout<<"2) K arrete"<<std::endl;
    std::cout<<"3) K sommet"<<std::endl;
    do
    {
        std::cin>>choix;
    }
    while (choix<1||choix>3);
    int num;
    if(choix==1)
    {
        std::cout<<"Quelle arrete veux tu supprimer"<<std::endl;
        std::cin>>num;
        supprimer_arrete(num);
        test=testConnexe();
        if(test==1)
        {
            std::cout<<" CE GRAPHE EST CONNEXE"<<std::endl;
            std::cout<<std::endl;
        }
        else if(test==0)
        {
            std::cout<<" CE GRAPHE EST NON CONNEXE"<<std::endl;
            std::cout<<std::endl;
        }

    }
    if(choix==2)
    {
        kconnexe();
    }
    if(choix==3)
    {
        kconnexeSommet();
    }
}
///Djikstra remodel� pour Intermediarite
std::vector<int> Graphe::Intermediarite(unsigned int num0,  std::vector<float> &compt)
{
    ///Initialisation des variables
   std::vector<int> marquage((int)m_sommets.size(),0);///Aucun sommet n'est d�couvert
    std::vector<int> dists((int)m_sommets.size(),99999);/// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);///Liste des pr�decesseurs
    int temp=0;
    int actuel;
    int temp2=99999;

    Sommet* p;/// Sommet de parcours
    int poids=0;
    int compteur=0;

    compt[num0]=0;
    dists[num0]=0;///Poids du sommet de d�part
    actuel=num0;

    while(temp==0)
    {
        compteur=0;
        temp2=9999; ///longueur infinie pour une arrete  non d�couverte

        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                    actuel=i;
                    temp2=dists[i]; /// On prend l'arrete avec la plus petite distance

            }
            else{
                compteur++; /// Si il n'ya plus de longueur plus petite alors le compteur s'incr�mente
            }
        }
        for (int i=0;i<m_sommets.size();++i)
      {
          if(actuel==m_sommets[i]->getNum())
          {
              p=m_sommets[i];/// On attribue le sommet correspondant a p (celui qui a le meme indice que actuel)
          }
      }
        marquage [p->getNum()]=1; /// On marque les sommets d�couverts


           for(succ: p->getSuccesseurs())
        {
            poids=poidsSucc(p,succ);/// Recherche du poids entre le sommet actuel et ses successeurs

            if(dists[p->getNum()]+poids< dists[succ->getNum()])
            {
                    dists[succ->getNum()]=dists[p->getNum()]+poids;
                    preds[succ->getNum()]=p->getNum(); /// Defilement

                     if(actuel!=num0)
                    {
                        compt[succ->getNum()]=compt[p->getNum()];
                    }
                    else
                    {
                        compt[succ->getNum()]=1;
                    }

            }
            else if (dists[p->getNum()]+poids == dists[succ->getNum()]) ///Plusieurs pcc ayant le meme poids
            {
                    dists[succ->getNum()]=dists[p->getNum()]+poids;
                    preds[succ->getNum()]=p->getNum();

                    compt[succ->getNum()]=compt[succ->getNum()]+compt[p->getNum()];
            }
        }

      if(compteur==m_sommets.size()) /// Lorsque tous les sommets sont marqu�s
      {
          temp=1;

      }

        }
    return dists;
}
///Calcul de l'indice d'intermediarit�
void Graphe::CalculIntermediarite(std::vector<double>&Result1, std::vector<double>&Result2)
{

    ///Initialisation des variables
    std::vector<int> stock((int)m_sommets.size(),1);
    std::vector<int> stock2((int)m_sommets.size(),1);
    std::vector<float> compt((float)m_sommets.size(),1);
    std::vector<float> compteur((float)m_sommets.size(),0);
    std::vector<float>centralite((float)m_sommets.size(),0);
    std::vector<float>normalise((float)m_sommets.size(),0);
    std::vector<float>somme((float)m_sommets.size(),0);

    int n = m_sommets.size();
    for(unsigned int i=0; i<m_sommets.size(); i++)
    {
        stock=Intermediarite(i,compt); /// Calcul des chemins a partir du sommet Intermediaire

        for(unsigned int j=0; j<m_sommets.size(); ++j)
        {
            stock2 = Intermediarite(j,somme);///Calcul des chemins a partir du sommetDepart

            for(unsigned int t=j; t<m_sommets.size(); t++) /// Intermediaire a arriv�e
            {
                if (i!=j&&j!=t&&i!=t)
                {
                    if(stock[j]+stock[t]== stock2[t])
                    {
                        compteur [j]= compt[t]*compt[j];///Chemin entre depart et intermediaire et intermediaire � arriv�e

                        if(somme[t]!=0&&compteur[j]!=0)
                        {
                            centralite[i]=centralite[i]+(compteur[j]/somme[t]); ///Compteur a la fin de la boucle for somme j a arriv�e
                            normalise[i]=(2*centralite[i])/(n*n-3*n +2);


                        }
                    }
                }

            }

        }
        ///On enregistre les resultats
        Result1.push_back(centralite[i]);
        Result2.push_back(normalise[i]);
    }

}
///Affichage et sauvegarde de l'indice d'intermediarit�
void Graphe::SauvegardeIntermediarite(Svgfile&svgout)
{
    int i=0;
    std::vector<double> R1;
    std::vector<double> R2;
    std::vector<double>R3;
    CalculIntermediarite(R1,R2);
    std::ofstream ifs{"Intermediarite.txt"};
    for(int i =0; i<m_sommets.size(); ++i)
    {
        std::cout<<"Sommet : "<<m_sommets[i]->getNom()<<" Centralite : "<<" "<<R1[i] <<" "<<" Centralite normalisee : "<<R2[i]<<std::endl;
        ifs<<m_sommets[i]->getNum()<<" "<<R1[i]<<" "<<R2[i]<<std::endl;
        R3.push_back(R2[i]); ///on stock la valeur normalis�e � chaque tour dans un vecteur
    }

    std::sort (R3.begin(), R3.end(), [](double a1, double a2) ///tri decroissant
    {
        return a1>a2;
    });

    for(auto vec :R2)
    {

        if(vec==R3[0])///100%
        {

            m_sommets[i]->colorier(svgout,0);


        }

        if(vec<R3[0]&&vec>0.90*R3[0])///90%-100%
        {

            m_sommets[i]->colorier(svgout,1);


        }
        if(vec>R3[0]*0.80&&vec<R3[0]*0.90)///80%-90%
        {
            m_sommets[i]->colorier(svgout,2);


        }
        if(vec>R3[0]*0.60&&vec<R3[0]*0.80)///60%-80%
        {

            m_sommets[i]->colorier(svgout,3);


        }

        if(vec>R3[0]*0.50&vec<R3[0]*0.60)///50%-60%
        {
            m_sommets[i]->colorier(svgout,4);


        }

        if(vec>R3[R3.size()-1]&&vec<=R3[0]*0.50)///<50%
        {
            m_sommets[i]->colorier(svgout,5);

        }

        if( vec==R3[R3.size()-1])/// egal a la plus petite
        {
            m_sommets[i]->colorier(svgout,6);

        }

        i++;

    }

}
///VULNERABILITE D INTERMEDIARITE
void Graphe::VulnerabiliteIntermediarite(int num)
{

    std::vector<double> R1;
    std::vector<double> R2;
    std::vector<double> R3;
    std::vector<double> R4;
    std::vector<double>Result;
    std::vector<double>Result2;
    double diff;
    CalculIntermediarite(R1,R2); ///on recupere le vecteur  R2
    for (int i=0; i<m_sommets.size(); ++i)
    {
        Result.push_back(R2[i]);
    }
   ///on supprime une arrete
    supprimer_arrete(num);
    CalculIntermediarite(R3,R4); ///on recupere R4

    for(int i=0; i<m_sommets.size(); ++i)
    {
        Result2.push_back(R4[i]);
    }

    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i]; ///calcul de la difference
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<"  "<<" "<< "Difference : "<<diff<<std::endl;
    }
}
///MENU VULNERABILITE
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
    while(choix<0||choix>4);
    std::cout<<"RESULTAT DE LA DIFFERENCE"<<std::endl;

    if(choix==1)
    {
        VulnerabiliteDegre(num);
    }

    if(choix==2)
    {

        VulnerabiliteVP(num);
    }
    if(choix==3)
    {
        VulnerabiliteDjikstra(num);

    }
    if(choix==4)
    {
        VulnerabiliteIntermediarite(num);
    }

}

std::vector<int> Graphe::Djikstra2(int num_0, int fin,int &somme)
{
    ///Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);///Aucun sommet n'est marqu� au d�part
    std::vector<int> dists((int)m_sommets.size(),99999);/// Lorque les sommets ne sont pas d�couverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int actuel;
    int temp2=99999;///Longueur infinie pour les distance non d�couverte
    int coumt = 0;
    int poids=0;
    Sommet *p;/// Sommet de parcours

    dists[num_0]=0;/// Poids du sommet de d�part
    actuel=num_0;

    while(temp==0)
    {
        coumt = 0;
        temp2=99999;
        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                actuel=i;
                temp2=dists[i]; /// On prend l'arrete avec la plus petite distance
            }
            else
            {
                coumt++; ///On compte le nombre fois ou il n'y a pas d'arrete plus petite
            }
        }
      for (int i=0;i<m_sommets.size();++i)
      {
          if(actuel==m_sommets[i]->getNum())
          {
              p=m_sommets[i]; ///on attribue le sommet qui a un indice �gal � actuel � p
          }
      }
        marquage [p->getNum()]=1; /// On marque les sommets d�couverts


        for(succ: p->getSuccesseurs())
        {
            poids=poidsSucc(p,succ); /// Calcul du poids entre le poids actuel et ses successeurs

            if(dists[p->getNum()]+ poids< dists[succ->getNum()])
            {
                    dists[succ->getNum()]=dists[p->getNum()]+poids;
                    preds[succ->getNum()]=p->getNum(); ///On defile
                    somme= somme+dists[p->getNum()]+poids;
            }
        }
            temp=1;
        if(coumt == m_sommets.size()) /// Il n'y a plus de pcc Dikjstra s'arrete
        {
            temp=1;
            return preds;
        }

          for( int i=0;i<m_sommets.size();i++) /// Lorsque le graphe est connexe Dikjstra s'arrete lorsque tous les sommets sont marqu�s
           {
               if(marquage[i]==0)
               {
                   temp=0;
               }

           }
    }

    afficher_parcours2(num_0,fin,preds);
    std::cout<<std::endl;
    return preds;
}
///Affichage Parcours pour l'ile de la Reunion
void Graphe::afficher_parcours2(size_t num, int fin, const std::vector<int>& arbre)
{
    for(size_t i=0; i<arbre.size(); ++i)
    {

        if(i!=num&&i==fin)
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
///GPS DE L ILE DE LA REUNION
void Graphe::GuideTouristique()
{
    int choix;

    std::cout<<"BIENVENUE SUR VOTRE GUIDE TOURISTIQUE"<<std::endl;
    std::cout<<"QUELLE EST VOTRE LOCALISATION ACTUELLE DANS L ILE DE LA REUNION ? "<<std::endl;
    std::cout<<"O)Saint-Denis"<<std::endl;
    std::cout<<"1)Saint-Paul"<<std::endl;
    std::cout<<"2)Saint-Andre"<<std::endl;
    std::cout<<"3)Saint-Leu "<<std::endl;
    std::cout<<"4)Saint-Benoit"<<std::endl;
    std::cout<<"5)Saint-Louis"<<std::endl;
    std::cout<<"6) Sainte-Rose"<<std::endl;
    std::cout<<"7)Saint-Pierre"<<std::endl;
    std::cout<<"8)Saint-Philippe"<<std::endl;
    std::cout<<"9) Saint-Joseph"<<std::endl;

    do
    {
        std::cin>>choix;
    }while(choix<0||choix>9);
    std::cout<<"VOICI LES CHEMINS POSSIBLES"<<std::endl;
    double Cps, somme;
    int choix2;
    std::vector<int> dji;
    dji=Djikstra(choix,Cps, somme);
    afficher_parcours(choix,dji);
    std::cout<<"CHOISISSEZ VOTRE DESTINATION "<<std::endl;
    std::cout<<"O)Saint-Denis"<<std::endl;
    std::cout<<"1)Saint-Paul"<<std::endl;
    std::cout<<"2)Saint-Andre"<<std::endl;
    std::cout<<"3)Saint-Leu "<<std::endl;
    std::cout<<"4)Saint-Benoit"<<std::endl;
    std::cout<<"5)Saint-Louis"<<std::endl;
    std::cout<<"6) Sainte-Rose"<<std::endl;
    std::cout<<"7)Saint-Pierre"<<std::endl;
    std::cout<<"8)Saint-Philippe"<<std::endl;
    std::cout<<"9) Saint-Joseph"<<std::endl;
    std::cin>>choix2;
    std::cout<<"VOICI LE TRAJET A PRENDRE "<<std::endl;
    int s;
    Djikstra2(choix, choix2,s);

}

void Graphe::MenuIndiceCentralite( Svgfile&svgout)
{
    int choix;

    std::cout<<"1) Centralite de degre"<<std::endl;
    std::cout<<"2)Centralite de vector propre"<<std::endl;
    std::cout<<"3)Centralite de proximite"<<std::endl;
    std::cout<<"4) Centralite d'intermediarite"<<std::endl;
    do
    {
        std::cout<<"Quel indice veux tu calculer"<<std::endl;
        std::cin>>choix;

    }while(choix<1||choix>5);

    if(choix==1)
    {
        affichage_Resultat1(svgout);
        sauvegarde();
    }
    if(choix==2)
    {
        SauvegardeVP(svgout);
    }
    if(choix==3)
    {
        sauvegarderProximite(svgout);
    }
    if(choix==4)
    {
        SauvegardeIntermediarite(svgout);
    }
}
void Graphe::Menu_afficher_centralite(Svgfile&svgout)
{

    int choix;
    std::cout<<"1) Centralite de degre"<<std::endl;
    std::cout<<"2)Centralite de vector propre"<<std::endl;
    std::cout<<"3)Centralite de proximite"<<std::endl;
    std::cout<<"4) Centralite d'intermediarite"<<std::endl;

    do
    {

        std::cout<<"Quel indice veux tu afficher sur le graphe ? "<<std::endl;
        std::cin>>choix;

    }while(choix<1||choix>5);
    if(choix==1)
    {
        Dessiner_centralite_degre(svgout);
        sauvegarde();
    }

    if(choix==2)

    {
        Dessiner_centralite_vect_propre(svgout);
        SauvegardeVP(svgout);
    }

    if(choix==3)

    {
        Dessiner_centralite_prox(svgout);
        sauvegarderProximite(svgout);
    }

    if(choix==4)
    {

        Dessiner_centralite_intermediaire(svgout);
        SauvegardeIntermediarite(svgout);

    }

}
void Graphe::Dessiner_centralite_vect_propre(Svgfile &svgout)
{

    double Lambda=0;
    std::vector<double> vec=VectorPropre(Lambda);

    for(int i=0; i< vec.size()&&i<m_sommets.size(); ++i)
    {
        svgout.addText((m_sommets[i]->getX())*100-10,(m_sommets[i]->getY())*100-30,vec[i],"red");
        Dessiner(svgout);
    }

}





void Graphe::Dessiner_centralite_degre(Svgfile&svgout)
{
    ///dessiner en SVG les centralites
    for(size_t i=0; i<m_sommets.size(); ++i)

    {
        double deg=calculDegre(i);
        double CG=deg/(m_sommets.size()-1);

        svgout.addText((m_sommets[i]->getX())*100-10,(m_sommets[i]->getY())*100-45,CG,"blue");
        Dessiner(svgout);

    }

}
void Graphe::Dessiner_centralite_prox(Svgfile&svgout)

{

    std::vector<int >dji;
    double Cps=0;
    double somme=0;

    ///dessiner en SVG les centralites
    for(size_t i=0; i<m_sommets.size(); ++i)

    {
        dji=Djikstra(m_sommets[i]->getNum(), Cps, somme);
        svgout.addText((m_sommets[i]->getX())*100-10,(m_sommets[i]->getY())*100-45,Cps,"blue");
        Dessiner(svgout);

    }

}



void Graphe::Dessiner_centralite_intermediaire(Svgfile&svgout)
{
    int i=0;
    std::vector<double> R1;
    std::vector<double> R2;
    CalculIntermediarite(R1,R2);

    for(int i =0; i<m_sommets.size(); ++i)

    {
        svgout.addText((m_sommets[i]->getX())*100-10,(m_sommets[i]->getY())*100-45,R2[i],"blue");

        Dessiner(svgout);

    }


}

