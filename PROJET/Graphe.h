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
    void sauvegarde();
    std::vector<double> VectorPropre( double &Lambda);
    void affichage_Resultat1();
    std::vector<int> Djikstra(int debut, double &Cps, double & somme );
    void afficherListe();
    void Calculproximite();
    std::vector<int> Intermediarite(int debut,int Sommet);
    void supprimer_arrete(int num);
    void VulnerabiliteDegre(int num);
    void VulnerabiliteVP(int num);
    void SauvegardeVP();
    void VulnerabiliteDjikstra();
    void sauvegarderProximite();
    std::vector<int> BFS(int num_s0,int & compteur)const;
     void testConnexe();
     void afficher_parcours(size_t num,const std::vector<int>& arbre);
     void kconnexe();
     void SupprimerSommet(int indice);
     void MenuVulnerabilite();

    protected:

        private:
        int m_orientation;
        std::vector<Sommet*> m_sommets;
        std::vector<Arete*> m_arretes;
    };

#endif // GRAPHE_H

