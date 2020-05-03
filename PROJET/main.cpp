#include <iostream>
#include "Graphe.h"
#include "svgfile.h"
#include<time.h>
#include <windows.h>

void color(int t,int f)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}

void afficher_Arbre(std::vector<Arete*> arretes)
{
    std::cout<<"Liste :"<<std::endl;
    for (auto a: arretes)
    {
        a->afficher();
    }
}
void Menu()
{
    std::cout << "\t\t-------------------------------------PROJET PISCINE-------------------------------------\n"<<std::endl;

    std::cout<<" \t  \t\t \t Yanis CHAMSON --- Shobiya KANDASAMY --- Emma SIRANDELLE "<<std::endl;
    std::cout<<" \t \t \t \t \t \t \tING2 TD7\n\n"<<std::endl;
    color(3,0);
    std::cout<< " \t\t\t   Voici l'ensemble des action que vous pouvez effectuer\n\n"
             "0: Quitter"
             "1: Changer le systeme de ponderation\n"
             "2: Calcul des indices de centralite \n"
             "3: Test de vulnerabilite\n\n"
             <<std::endl;
}

void legende_svg(Svgfile&svgout)
{
svgout.addText(650,330,"Importance des sommets en fonction ","black");
svgout.addText(630,350,"du pourcentage de l'indice de centralite max ","black");

svgout.addDisk(700,400,20,"lime");
svgout.addText(750,405,"100% de la valeur max","black");

svgout.addDisk(700,450,20,"limegreen");
svgout.addText(750,455,"90%-100%","black");

svgout.addDisk(700,500,20,"palegreen");
svgout.addText(750,505,"80%-90%","black");

svgout.addDisk(700,550,20,"coral");
svgout.addText(750,555,"60%-80%","black");

svgout.addDisk(700,600,20,"orange");
svgout.addText(750,605,"50%-60%","black");

svgout.addDisk(700,650,20,"lightsalmon");
svgout.addText(750,655,"0%-50%","black");

svgout.addDisk(700,700,20,"red");
svgout.addText(750,705,"100% de la valeur min","black");
}
int main()
{

    Menu();
    color(15,0);
    Svgfile svgout;

    std::vector <int> DJ;
    int choix;
    int choix2;
    int choix3;
    char* reponse;
  /*  std::string NomFichier;
    std::string NomFic;
    color(11,0);
    std::cout<<"\nEntrez votre choix : ";
    color(5,0);
    std::cin>> choix;


    color(11,0);
    std::cout<< "\n Nom du fichier de topologie (ex: Graphe.txt) : ";
    color(5,0);
    std::cin>>NomFichier;
    std::cout<<std::endl;
    color(11,0);
    std::cout<< "Nom du fichier de ponderation (ex: Ponderation.txt) : ";
    color(5,0);
    std::cin>>NomFic;
    std::cout<<std::endl;
    color(15,0);*/

    Graphe g {"Graphe.txt"};

    srand(time(NULL));
    //g.ChargementFichierPond("Ponderation2.txt");
    g.afficherListe();
    g.MenuVulnerabilite();
    //g.kconnexe();
   // g.SupprimerSommet(0);

    g.Dessiner(svgout);
    legende_svg(svgout);
    //g.MenuConnexe();
//g.affichage_Resultat1(svgout);

std::vector<double>Result1;
std::vector<double>Result2;
   //g.CalculIntermediarite(Result1,Result2);
    std::cout<<std::endl;
    //g.supprimer_arrete(1);
//g.testConnexe();
   //g.SauvegardeIntermediarite(svgout);
  //g.kconnexe();
    //g.MenuVulnerabilite();
   //g.VulnerabiliteDjikstra();
    //g.testConnexe();
    //g.SauvegardeVP(svgout);
  //g.sauvegarderProximite(svgout);
  g.GuideTouristique();

    //g.VectorPropre();



  /* if(choix==1)
    {*/

//g.supprimer_arrete(1);
        //g.VectorPropre();
        //g.afficher();
//g.Djikstra(1, double &Cps, double & somme )
   /*     color(11,0);
        std::cout<<"\nEntrez votre choix : ";
        color(5,0);
        std::cin>> choix;
    }


    if (choix==2)
    {
        color(11,0);
        std::cout<< "Nom du fichier de ponderation (ex: Ponderation.txt) : ";
        color(5,0);
        std::cin>>NomFic;
        g.ChargementFichierPond("Ponderation.txt");
    /*    color(15,0);
        g.afficher();
        color(11,0);
        std::cout<<"\nEntrez votre choix : ";
        color(5,0);
        std::cin>> choix;
    }
   /* if(choix==3)
    {
        color(3,0);;
        std::cout<<" Quel calcul voulez vous affichez?\n"
                 "1: Calcul de degre\n"
                 "2: Calcul de vecteur propre\n"
                 "3: Calcul de proximite\n"
                 "4: Calcul d'intermediarite"
                 <<std::endl;
        color(5,0);
        std::cin>>choix2;

        if (choix2==1)

        {
            color(15,0);*/

        //     g.affichage_Resultat1();
         //    g.sauvegarde("Resultat1");
     /*       color(11,0);
            std::cout<<"Voulez affichez un autre calcul? oui/non";
            color(5,0);
            std::cin>> reponse;

            if (strcmp("oui",reponse)==0)
            {
                color(5,0);
                std::cin>>choix2;
            }
        }

        if (choix2==2)
        {
            color(15,0);*/

         //   g.VectorPropre("Resultat2");
        /*    color(11,0);
            std::cout<<"Voulez affichez un autre calcul? oui/non";
            color(5,0);
            std::cin>> reponse;

            if (strcmp("oui",reponse)==0)
            {
                color(5,0);
                std::cin>>choix2;
            }
        }
        if (choix2==3)

        {
            color(15,0);*/
        // g.Calculproximite("CalculProx.txt");
            color(11,0);
    /*        std::cout<<"Voulez affichez un autre calcul? oui/non";
            color(5,0);
            std::cin>> reponse;

            if (strcmp("oui",reponse)==0)
            {
                color(5,0);
                std::cin>>choix2;
            }
        }

        if (choix2==4)
        {
            color(15,0);
            color(11,0);
            std::cout<<"Voulez affichez un autre calcul? oui/non";
            color(5,0);
            std::cin>> reponse;

            if (strcmp("oui",reponse)==0)
            {
                color(5,0);
                std::cin>>choix2;
            }

        }

        color(11,0);
        std::cout<<"\nEntrez votre choix : ";
        color(5,0);
        std::cin>> choix;
    }
    if (choix==4)
    {
        color(11,0);
        std::cout<<"\nEntrez votre choix : ";
        color(5,0);
        std::cin>> choix;
    }
    color(15,0);*/
    return 0;
}


