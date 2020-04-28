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
   std::vector <int> DJ;
   Graphe g {"Graphe.txt"};
   g.ChargementFichierPond("Pond�ration.txt");
   g.afficher();
   g.Dessiner(svgout);
   //g.affichage_Resultat1();
   g.afficherListe();
    //g.VectorPropre("Resultat2");
    DJ=g.Djikstra(0);

    int id;

    return 0;
}


