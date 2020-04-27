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
        void afficher()const;
        ~Graphe();
         void Dessiner(Svgfile &svgout) const;
         int calculDegre();

    protected:

    private:
        int m_orientation;
        std::vector<Sommet*> m_sommets;
        std::vector<Arrete*> m_arretes;
};

#endif // GRAPHE_H
