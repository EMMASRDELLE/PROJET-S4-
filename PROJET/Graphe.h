#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include "Sommet.h"
#include "Arete.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "svgfile.h"
class Graphe
{
    public:

        /// Méthodes
        Graphe(std::string nomFichier); ///Construction du graphe par lecture de fichier
        virtual ~Graphe();
        void afficher() const;
        void Dessiner(Svgfile &svgout) const;

    protected:

    private:

        ///Initialisation des variables

        int m_orientation;
        std::vector <Sommet*> m_sommets;
        std::vector<Arrete*> m_arretes;
};

#endif // GRAPHE_H_INCLUDED
