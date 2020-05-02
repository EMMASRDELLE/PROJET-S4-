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
    void affichage_Resultat1(Svgfile &svgout);
    std::vector<int> Djikstra(int num0, double &Cps, double & somme );
    void afficherListe();
    std::vector<int> Intermediarite(int debut,int Sommet);
    void supprimer_arrete(int num);
    void VulnerabiliteDegre(int num);
    void VulnerabiliteVP(int num);
    void SauvegardeVP(Svgfile &svgout);
    void VulnerabiliteDjikstra(int num);
    void sauvegarderProximite(Svgfile &svgout);
    std::vector<int> BFS(int num_s0,int & compteur)const;
     void testConnexe();
     void afficher_parcours(size_t num,const std::vector<int>& arbre);
     void kconnexe();
     void SupprimerSommet(int indice);
     void MenuVulnerabilite();
     void CalculIntermediarite(std::vector<double>&Result1, std::vector<double>&Result2);
     std::vector<int> Intermediarite(unsigned int num0,  std::vector<float> &compt);
     void SauvegardeIntermediarite();
     void VulnerabiliteIntermediarite(int num);
     int poidsSucc(Sommet* a, Sommet* b);


    protected:

        private:
        int m_orientation;
        std::vector<Sommet*> m_sommets;
        std::vector<Arete*> m_arretes;
    };

#endif // GRAPHE_H

