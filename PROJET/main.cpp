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
   g.affichage_Resultat1();
    g.VectorPropre("Resultat2");

    return 0;
}


