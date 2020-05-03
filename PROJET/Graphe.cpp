#include <iostream>
#include <fstream>
#include <algorithm>
#include "Graphe.h"
#include "svgfile.h"
#include <queue>
#include <time.h>
///Chargement Fichier Ponderé
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

        m_arretes[indice]->setPoids(poids); ///on attribu le poids à l'arrete
    }
}
///Constructeur
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
        {
            m_sommets[num2]->ajouterSucc(m_sommets[num1]);
        }


    }
}

///méthode d'affichage
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
///Destructeur
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
/// Calcul de degré
double Graphe::calculDegre(int num)
{
    double deg=0;

    for (auto b: m_arretes) ///on parcourt les arretes
    {
        for (auto a : m_sommets)///on parcourt les sommets
    {
        {

            if (a==m_sommets[num]) ///si le sommet est égale au sommet de l'indice envoyé
            {
                if(b->CalculDEG(a,b)==true) /// si le calculDEG renvoie vrai
                {
                    ++deg; ///on accrémente le compteur de degré
                }
            }
        }
    }

    return deg;
}
}
///Sauvegarde du resultat Calcul degre
void Graphe:: sauvegarde()
{
    std::ofstream ifs{"Resultat1.txt"}; ///ouverture fichier
    for(size_t i=0; i<m_sommets.size(); ++i)
    {

        double deg=calculDegre(i);
        double CG=deg/(m_sommets.size()-1);

        if (ifs.is_open())
            ifs<<m_sommets[i]->getNum()<<m_sommets[i]->getNom()<<" "<<deg<<" "<< CG<<"\n";
    }
    ifs.close();
}
///Affichage du resultat du premier algorithme
void Graphe::affichage_Resultat1(Svgfile &svgout)
{
    Dessiner(svgout);

    int test;
    std::vector<double>vec;
    std::vector <double> bon;
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        double num=calculDegre(i);
        double CG=num/(m_sommets.size()-1);

        vec.push_back(CG);/// on push le resultat normalise dans un vecteur
        std::cout<<m_sommets[i]->getNum()<<" : "<<m_sommets[i]->getNom()<<" "<< "Degre : "<<num<<" Calcul= "<<CG<<std::endl; ///on affiche le resultat
    }

    std::sort (vec.begin(), vec.end(), [](double a1, double a2) ///methode de trie par ordre decroissant
    {
        return a1>a2;
    });

    for(auto s:m_sommets)
    {

        double num=calculDegre(s->getNum());
        double CG=num/(m_sommets.size()-1);

        if(CG==vec[0])/// si CG est egale au plus grand vecteur, on le colorie
        {

            s->colorier(svgout,0);
        }

        if(CG<vec[0]&&CG>0.90*vec[0])///90%-100%
        {
            Dessiner(svgout);
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
///SSPG CALCUL VECTOR PROPRE
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
    double Total=0;
    std::vector<int> somme;
    do
    {
        for(int i=0; i<m_sommets.size(); ++i)
        {
            for (auto succ:m_sommets[i]->getSuccesseurs())///on parcours les successeurs d'un sommet
            {
                Somme= succ->getNum()+Somme; ///on additionne les indices du sommet

            }
            Somme=0;
        }
        for(auto s:m_sommets)
        {
            for(auto succ:s->getSuccesseurs())
            {
                Somme=succ->getNum()+Somme;
                T=Somme*Somme;///au carré
            }
            Somme=0;
            Total =T+Total;
            Lambda=sqrt(Total); ///Racine de la somme totale
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
        Lambdapred=Lambda;///on donne la valeur du lambda actuel au precedent
    }
    while(Lambda>=Lambdapred*1.05||Lambda<=0.95*Lambdapred);///variation du lambda
    return Resultat;

}
///Affichage et sauvegarde du calcul de l'indice du vector propre
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
        ifs<<m_sommets[i]->getNom()<<" "<<Lambda<<" "<< vec[i]<<std::endl; ///sauvegarde dans un fichier
    }

    std::sort (vec2.begin(), vec2.end(), [](double a1, double a2) ///trie decroissant
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
    for (int i=0; i<m_arretes.size(); ++i)
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
///Affichage parcours
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
std::vector<int> Graphe::Djikstra(int num_0, double &Cps, double & somme )
{
    ///Initialisation des variables
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);///Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int actuel;
    int temp2=99999;
    int coumt = 0;
    int poids=0;
    Sommet *p;

    dists[num_0]=0;/// Poids du sommet de départ
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
                temp2=dists[i];

          } /// On prend l'arrete avec la plus petite distance

            else
            {
                coumt++; ///Le compteur s'accremente
            }
        }
     for (int i=0;i<m_sommets.size();++i)
      {
          if(actuel==m_sommets[i]->getNum())
          {
              p=m_sommets[i];
          }
      }

        marquage [p->getNum()]=1; /// On marque les sommets découverts


        for(succ: p->getSuccesseurs())
        {
            poids=poidsSucc(p,succ);

            if(dists[p->getNum()]+poids< dists[succ->getNum()])
            {
                    dists[succ->getNum()]=dists[p->getNum()]+poids;
                    preds[succ->getNum()]=p->getNum();
                    somme= somme+dists[p->getNum()]+poids;
            }
        }

            temp=1;

        if(coumt == m_sommets.size())
        {
            temp=1;
            return preds;
        }

         for( int i=0;i<m_sommets.size();i++)
           {
               if(marquage[i]==0)
               {
                   temp=0;
               }

           }


    }
  std::cout<<std::endl;
    Cps= (m_sommets.size()-1)/somme;
    return preds;
}

void Graphe::sauvegarderProximite(Svgfile&svgout)
{
    std::vector<int> dji;
    std::vector<double> vec2;
    double Cps=0;
    double somme=0;
    std::ofstream ifs{"Resultat3.txt"}; ///ouverture fichier Resultat 3
    for (auto s :m_sommets)
    {
        dji=Djikstra(s->getNum(), Cps, somme);
        vec2.push_back(Cps); ///on stock le resultat normalise
        std::cout<<"Sommet : "<<s->getNom()<<" "<< "Somme :"<<somme<<" "<< " VP :"<< Cps<<std::endl;
        ifs<<s->getNom()<<" "<<somme<<" "<<Cps<<std::endl;

    }
    std::sort (vec2.begin(), vec2.end(), [](double a1, double a2) ///tri decroissant
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
///Afficher la liste des Sommets
void Graphe::afficherListe()
{
    std::cout<<"listes d'adjacence :"<<std::endl;
    for (auto s : m_sommets)
    {
        s->afficher();
        std::cout<<std::endl;
    }
}
///Suppression d'arrete
void Graphe::supprimer_arrete( int indice)
{
    Sommet* s1;
    Sommet* s2;

    for(int j=0; j<m_arretes.size(); ++j) /// on parcours la liste d'arêtes
    {
        if(m_arretes[j]->getIndice()==indice)/// si l indice demandé est trouvé dans la liste arrête
        {
            ///on a les 2 sommets extremités
            s1=m_arretes[j]->getEx1();
            s2=m_arretes[j]->getEx2();

            for(size_t i=0; i<s1->getSuccesseurs().size(); ++i)
            {
                if((s1->getSuccesseurs())[i]->getNum()==s2->getNum()) ///si un des successeurs du sommet 1 de l'arrete est egale au Sommet 2 de l'arrete
                {

                    s1->getSuccesseurs().erase(s1->getSuccesseurs().begin()+i); ///on enleve le successeurs de la liste de succ

                }
            }
            for(size_t i=0; i<s2->getSuccesseurs().size(); ++i)
            {
                if((s2->getSuccesseurs())[i]->getNum()==s1->getNum())///si un des successeurs du sommet 2 de l'arrete est egale au Sommet 1 de l'arrete
                {

                    s2->getSuccesseurs().erase(s2->getSuccesseurs().begin()+i);///on enleve le successeurs de la liste de succ

                }

            }
            delete m_arretes[j]; ///on supprime l'arrete
            m_arretes.erase(m_arretes.begin()+j);///puis on l'erase
        }
    }
    std::cout <<"On affiche la nouvelle liste : "<<std::endl;
    afficherListe();
    for(int i=0; i<m_arretes.size(); ++i)
    {
        m_arretes[i]->set_indice(i); ///on reatribu les bons indices aux arretes
    }

}
///Vulnerabilite Degré
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
        Result.push_back(Result1); ///on push le resultat normalise dans un vecteur
    }

    supprimer_arrete(num);

    ///AVEC SUPPRESSION
    for(auto s: m_sommets)
    {
        deg=calculDegre(s->getNum());
        Result2=deg/(m_sommets.size()-1);
        Result_deux.push_back(Result2);///on push le resultat normalise apres suppression dans un vecteur

    }

    for(int i=0; i<Result_deux.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result_deux[i]- Result[i]; ///difference avt suppression et apres
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl;
    }
}
///Vulnerabilite Vector propre
void Graphe::VulnerabiliteVP( int num)
{
    double Lambda=0;
    std::vector <double> Result=VectorPropre(Lambda); ///on retourne le vecteur de resultats normalisés
    std::vector<double> Result2;
    double diff=0;
    /// SANS SUPPRESSION

    supprimer_arrete(num);

    ///AVEC SUPPRESSION

    Result2=VectorPropre(Lambda); ///on le retourne apres suppression
    std::cout<<"RESULTAT DE LA DIFFERENCE"<<std::endl;
    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i]; ///calcul de la difference
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<":"<<diff<<std::endl;
    }
}
///Vulnerabilite Djikstra
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
        Result.push_back(Cps); ///on stock le resultat dans un vecteur
    }

    supprimer_arrete(num);
    for(auto s:m_sommets)
    {
        dji2=Djikstra(s->getNum(), Cps, somme);
        Result2.push_back(Cps);///on stock le resultat dans un vecteur après suppression
    }

    for(int i=0; i<Result2.size()&&i<Result.size()&&i<m_sommets.size(); ++i)
    {
        diff=Result2[i]- Result[i]; ///Calcul de la diff
        std::cout<<" Sommet"<<m_sommets[i]->getNom()<<"  "<<" "<< "Difference : "<<diff<<std::endl;
    }

}
///BFS
std::vector<int> Graphe::BFS(int num_s0, int & compteur)const
{

    std::queue< Sommet*> file;/// déclaration de la file
    std::vector<int> couleurs((int)m_sommets.size(),0); /// pour le marquage
    std::vector<int> preds((int)m_sommets.size(),-1);///pour noter les prédécesseurs
    ///Initialisation
    file.push(m_sommets[num_s0]);
    couleurs[num_s0]=1;
    Sommet*s;
    compteur=0;

    while(!file.empty()) ///tant que la file n'est pas vide
    {

        s=file.front();
        file.pop();

        for(auto succ:s->getSuccesseurs())///pour chaque successeur du sommet
        {
            if(couleurs[succ->getNum()]==0)
            {
                ///s'il n'est pas marqué
                couleurs[succ->getNum()]=1;///on le marque
                preds[succ->getNum()]=s->getNum();///on marque son prédecesseur
                file.push(succ);///on le met dans la file
            }
        }
        ++compteur; ///on accrémente le compteur
    }
    return preds;
}
///SPGRM qui retourne 1 si connexe, 0 sinon
int Graphe::testConnexe()
{
    std::vector<int> bfs;
    int compteur;
    int test;
    for(auto s: m_sommets)
    {
        bfs=BFS(s->getNum(), compteur);
        std::cout<<std::endl;
    }
    if(compteur==m_sommets.size())
    {
        test=1;
    }
    else
    {
        test=0;
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
    while(test==1&&i<m_sommets.size()); ///tant que le graphe est toujours connexe et i inferieur au nombre de sommet, on supprime
    std::cout<<compteur<<"-arrete connexe "<<std::endl;
}
///Supprimer Sommet
void Graphe::SupprimerSommet(int indice)
{


    for (int k=0;k<m_sommets.size();++k) ///on parcours les sommets
    {
        if (m_sommets[k]->getNum()==indice) ///si 1 des sommets égale à l'indice
        {

            for(int j=0; j<m_arretes.size();++j)///on parcours les arretes
            {
                if(m_arretes[j]->getEx1()->getNum()==m_sommets[k]->getNum()|| m_arretes[j]->getEx2()->getNum()==m_sommets[k]->getNum()) ///si le sommet correspond à un des sommets d'une arrete

                {
                    ///on a les 2 sommets extremités
                    Sommet*s1=m_arretes[j]->getEx1();
                    Sommet*s2=m_arretes[j]->getEx2();

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

                    delete m_arretes[j];///on supprime l'arrete
                    m_arretes.erase(m_arretes.begin()+j);
                }
            }

            delete m_sommets[k]; ///on supprime le sommet
            m_sommets.erase(m_sommets.begin()+k);///on l'erase

        }
    }
}
///K-Sommet connexe
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
    while(test==1&&i<m_sommets.size());///tant que le graphe est connexe, on supprime un sommet
    std::cout<<compteur<<"-Sommet connexe "<<std::endl;
}
///Menu Pour montrer la connexité
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

std::vector<int> Graphe::Intermediarite(unsigned int num0,  std::vector<float> &compt)
{

    ///Initialisation des variables
   std::vector<int> marquage((int)m_sommets.size(),0);//Aucun sommet n'est découvert
    std::vector<int> dists((int)m_sommets.size(),99999);// Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);//Liste des prédecesseurs
    int temp=0;
    int actuel;
    int temp2=9999;
    double somme;
    int cpt=1;
    double cmpt3=0;
    compt[num0]=0;

    dists[num0]=0;///Poids du sommet de départ
    int compteur=0;

    while(temp==0)
    {
        compteur=0;
        temp2=9999;//longueur infinie pour une arrete  non découverte

        for (unsigned int i =0; i<m_sommets.size(); i++)
        {
            if((dists[i] < temp2)&&(marquage[i]==0))
            {
                    actuel=i;
                    temp2=dists[i]; // On prend l'arrete avec la plus petite distance

            }
            else{
                compteur++;
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
      if(compteur==m_sommets.size())
      {
          temp=1;
      }

        }
    return dists;
}
///Calcul Intermediarité
void Graphe::CalculIntermediarite(std::vector<double>&Result1, std::vector<double>&Result2)
{
    for (auto a:m_arretes)
    {
        std::cout<<a->getIndice()<<" "<<a->getEx1()->getNum()<<a->getEx2()->getNum()<<std::endl;
    }
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
        stock=Intermediarite(i,compt); //Intermediaire

        for(unsigned int j=0; j<m_sommets.size(); ++j)
        {
            stock2 = Intermediarite(j,somme);//Depart

            for(unsigned int t=j; t<m_sommets.size(); t++) // Int a arrivée
            {
                if (i!=j&&j!=t&&i!=t)// Arrivée
                {
                    if(stock[j]+stock[t]== stock2[t])
                    {
                        compteur [j]= compt[t]*compt[j];

                        if(somme[t]!=0&&compteur[j]!=0)
                        {
                            centralite[i]=centralite[i]+(compteur[j]/somme[t]); //Compteur a la fin de la boucle for somme j a arrivée
                            normalise[i]=(2*centralite[i])/(n*n-3*n +2);


                        }
                    }
                }

            }

        }
        Result1.push_back(centralite[i]);
        Result2.push_back(normalise[i]);
    }

}
///Sauvegarde et affichage du resultat
void Graphe::SauvegardeIntermediarite(Svgfile&svgout)
{
    ///initialisation
    int i=0;
    std::vector<double> R1;
    std::vector<double> R2;
    std::vector<double>R3;
    CalculIntermediarite(R1,R2);
    std::ofstream ifs{"Intermediarite.txt"}; ///ouverture du fichier
    for(int i =0; i<m_sommets.size(); ++i)
    {

        std::cout<<"Sommet : "<<m_sommets[i]->getNom()<<" Centralite : "<<" "<<R1[i] <<" "<<" Centralite normalisee : "<<R2[i]<<std::endl;
        ifs<<m_sommets[i]->getNum()<<" "<<R1[i]<<" "<<R2[i]<<std::endl; ///on ecrit les valeurs dans un fichier
        R3.push_back(R2[i]); ///on stock les valeurs normalisées dans un vecteur
    }

    std::sort (R3.begin(), R3.end(), [](double a1, double a2)///tri décroissant
    {
        return a1>a2;
    });

    for(auto vec :R2)
    {

        if(vec==R3[0])///Si le resultat est égale à 100% de la valeur la plus grande parmis les résultats
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
void Graphe::VulnerabiliteIntermediarite(int num)
{
    ///INITIALISATION
    std::vector<double> R1;
    std::vector<double> R2;
    std::vector<double> R3;
    std::vector<double> R4;
    std::vector<double>Result;
    std::vector<double>Result2;
    double diff;
    ///Resultat avt suppression
    CalculIntermediarite(R1,R2);
    for (int i=0; i<m_sommets.size(); ++i)
    {
        Result.push_back(R2[i]); ///Result stock les valeurs normalisées
    }

    supprimer_arrete(num); ///on supprime l'arrete
    CalculIntermediarite(R3,R4); ///on récup les valeurs de R4 après suppression

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
///Menu Vulnerabilité
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
    std::vector<int> marquage((int)m_sommets.size(),0);
    std::vector<int> dists((int)m_sommets.size(),99999);///Lorque les sommets ne sont pas découverts on leur attribue une longueur infinie
    std::vector<int> preds((int)m_sommets.size(),-1);

    int temp=0;
    int actuel;
    int temp2=99999;
    int coumt = 0;
    int poids=0;
    Sommet *p;
    dists[num_0]=0;/// Poids du sommet de départ
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
                temp2=dists[i];
          }
            else
            {
                coumt++; ///Le compteur s'accremente
            }
        }

     for (int i=0;i<m_sommets.size();++i)
      {
          if(actuel==m_sommets[i]->getNum())
          {
              p=m_sommets[i];
          }
      }

        marquage [p->getNum()]=1; /// On marque les sommets découverts



        for(succ: p->getSuccesseurs()) ///on parcours les successeurs du sommet
        {
            poids=poidsSucc(p,succ); ///on stock le poids de l'arrete

            if(dists[p->getNum()]+poids< dists[succ->getNum()])
            {
                    dists[succ->getNum()]=dists[p->getNum()]+poids;///nouvelle valeur de dists stockés
                    preds[succ->getNum()]=p->getNum(); ///on marque les predeccesseurs
            }
        }

            temp=1;


         for( int i=0;i<m_sommets.size();i++)
           {
               if(marquage[i]==0)
               {
                   temp=0;
               }

           }
    }
    std::cout<<std::endl;
    afficher_parcours2(num_0,fin,preds);
    return preds;
}
///POUR NOTRE CAS DE LA REUNION
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
///Fonction GPS
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
    }
    while(choix<0&&choix>9);
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
///Menu Centralite
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
    }
    while(choix<1&&choix>5);
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


    ///dessiner en SVG les centralités
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
    ///dessiner en SVG les centralités
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

