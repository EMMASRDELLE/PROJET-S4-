#ifndef GRAPHE_H
#define GRAPHE_H
#include <string>
#include <vector>
#include "Sommet.h"
#include "Arete.h"

class Graphe
{
    public:
        Graphe(std::string nomFic);
        void afficher() const;
        ~Graphe();

    protected:

    private:
        int m_orientation;
        std::vector<Sommet*> m_sommets;
        std::vector<Arrete*> m_arretes;
};

#endif // GRAPHE_H
