#ifndef GRAPHE_H
#define GRAPHE_H
#include <string>
#include <vector>
#include "Sommet.h"
#include "Arete.h"
#include "svgfile.h"

class Graphe
{
    public:
         Graphe(std::string nomFic);
         void ChargementFichierPond(std::string nomFichier);
         void afficher()const;
         ~Graphe();
         void Dessiner(Svgfile &svgout) const;
         double calculDegre(int num);
         void sauvegarde(std::string nomFichier);
         void VectorPropre(std::string nomFichier);
         void affichage_Resultat1();
         void Intermediarite();

    protected:

    private:
        int m_orientation;
        std::vector<Sommet*> m_sommets;
        std::vector<Arrete*> m_arretes;
};

#endif // GRAPHE_H
