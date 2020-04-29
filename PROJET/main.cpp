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
   size_t num;
   Svgfile svgout;
   Graphe g {"Graphe.txt"};
   g.afficher();
   g.Dessiner(svgout);

   Sommet a (0,'A',23,3);





    return 0;
}


