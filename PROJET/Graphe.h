#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include "Sommet.h"
#include "Arrete.h"
#include <vector>
#include <iostream>
#include <fstream>
class Graphe
{
    public:

        /// M�thodes

        Graphe(std::string nomFichier); ///Construction du graphe par lecture de fichier
        virtual ~Graphe();
        void afficher() const;

    protected:

    private:

        ///Initialisation des variables

        int m_orientation;
        std::vector <Sommet*> m_sommets;
        std::vector<Arrete*> m_arretes;
};

#endif // GRAPHE_H_INCLUDED
