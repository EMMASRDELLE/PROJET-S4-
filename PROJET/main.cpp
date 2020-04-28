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
{  int debut;
   Svgfile svgout;
   std::vector <int> DJ;
   Graphe g {"Graphe2.txt"};
   g.ChargementFichierPond("Ponderation2.txt");
   g.afficher();
   g.Dessiner(svgout);
   //g.affichage_Resultat1();
   g.afficherListe();
    //g.VectorPropre("Resultat2");
   std::cout<< "Entrez le sommet de depart pour calculer les chemins les plus courts"<<std::endl;
   std::cin>>debut;
   DJ=g.Djikstra(debut);

    return 0;
}


