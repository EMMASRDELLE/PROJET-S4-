#include <iostream>
#include "Graphe.h"
#include "svgfile.h"

void afficher_Arbre(std::vector<const Arrete*> arretes)
{
    std::cout<<"Liste :"<<std::endl;
    for (auto a: arretes)
    {
        a->afficher();
    }
}

int main()
{
   Svgfile svgout;
   Graphe g {"Graphe.txt"};

   g.afficher();
   g.Dessiner(svgout);
   int a =g.calculDegre();
    std::cout <<a;

    return 0;
}


