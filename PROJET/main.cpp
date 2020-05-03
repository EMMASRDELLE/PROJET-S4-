///Source: Code et cours de madame Palasi
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

    std::cout<< " \t\t\t   Voici l'ensemble des action que vous pouvez effectuer\n\n"
             "0: Quitter\n"
             "1: Ajouter un systeme de ponderation\n"
             "2: Calcul des indices de centralite \n"
             "3: Test de vulnerabilite\n"
             "4 : Afficher les resultats de calcul sur le graphe\n"
             "5 : Guide Touristique\n"
             "6 : Tester la k-connexite\n"
             "7 : Supprimer une arrete\n"
             "8 : Supprimer un sommet\n"
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
    Svgfile svgout;
    int choix;

    std::string NomFichier;

color(15,0);
    std::cout << "\t\t-------------------------------------PROJET PISCINE-------------------------------------\n"<<std::endl;

    std::cout<<" \t  \t\t \t Yanis CHAMSON --- Shobiya KANDASAMY --- Emma SIRANDELLE "<<std::endl;
    std::cout<<" \t \t \t \t \t \t \tING2 TD7\n\n"<<std::endl;
    color(3,0);
    std::cout<<"VOICI LES GRAPHES A CHARGER"<<std::endl;
    std::cout<<"1) Graphe.txt\n"
             <<"2) Graphe2.txt\n"
             <<"3) Graphe3.txt\n"
             <<"4) Grapheoriente.txt\n"
             <<std::endl;
    std::cout<< "\n Nom du fichier de topologie (ex: Graphe.txt) : ";
    color(5,0);
    std::cin>>NomFichier;
    Graphe g{NomFichier};
    g.afficherListe();
    std::cout<<std::endl;
    do
    {

        color(11,0);
        Menu();
        color(5,0);
        do
        {
            std::cout<<"\nEntrez votre choix : "<<std::endl;
            std::cin>> choix;
        }
        while (choix<0||choix>8);

        if(choix==1)
        {
            std::string NomFic;
            std::cout<< " Saisir Nom du fichier de ponderation (ex: Ponderation.txt) : ";
            color(5,0);
            std::cin>>NomFic;
            g.ChargementFichierPond(NomFic);
            std::cout<<std::endl;
            color(15,0);
        }
        if(choix==2)
        {
            g.Dessiner(svgout);
            g.MenuIndiceCentralite(svgout);
        }
        if(choix==3)
        {

            g.MenuVulnerabilite();
        }
        if(choix==4)
        {

        }
        if(choix==5)
        {
            Graphe g {"Reunion.txt"};
            g.ChargementFichierPond("PonderationReseau.txt");
            g.GuideTouristique();
        }
        if(choix==6)
        {
            g.MenuConnexe();
        }
        if(choix==7)
        {
            int num;
            std::cout<<"Supprime une arrete"<<std::endl;
            std::cin>>num;
            g.supprimer_arrete(num);
        }
        if(choix==8)
        {
            int num;
            std::cout<<"Supprime un sommet"<<std::endl;
            std::cin>>num;
            g.SupprimerSommet(num);
        }

    }
    while(choix!=0);
    return 0;
}


