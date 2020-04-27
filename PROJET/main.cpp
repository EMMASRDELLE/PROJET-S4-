#include <iostream>
#include "Graphe.h"

void afficher_Arbre(std::vector<const Arrete*> arretes)
{
    std::cout<<"Liste :"<<std::endl;
    for (auto a:arretes)
    {
        a->afficher();
    }
}

int main()
{
   size_t num;
   Graphe g {"Graphe.txt"};
   g.afficher();
   Sommet a(1,"damso",2,3);
    g.afficher();

    return 0;
}


