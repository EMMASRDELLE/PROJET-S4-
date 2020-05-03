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
    std::vector<int> Djikstra(int num_0, double &Cps, double & somme );
    void afficherListe();
    std::vector<int> Intermediarite(int debut,int Sommet);
    void supprimer_arrete(int num);
    void VulnerabiliteDegre(int num);
    void VulnerabiliteVP(int num);
    void SauvegardeVP(Svgfile &svgout);
    void VulnerabiliteDjikstra(int num);
    void sauvegarderProximite(Svgfile &svgout);
    std::vector<int> BFS(int num_s0,int & compteur)const;
     int testConnexe();
     void afficher_parcours(size_t num,const std::vector<int>& arbre);
     void kconnexe();
     void kconnexeSommet();
     void SupprimerSommet(int indice);
     void MenuVulnerabilite();
     void CalculIntermediarite(std::vector<double>&Result1, std::vector<double>&Result2);
     std::vector<int> Intermediarite(unsigned int num0,  std::vector<float> &compt);
     void SauvegardeIntermediarite(Svgfile&svgout);
     void VulnerabiliteIntermediarite(int num);
     int poidsSucc(Sommet* a, Sommet* b);
     void MenuConnexe();
     void GuideTouristique();
     std::vector<int> Djikstra2(int num_0, int fin,int &somme);
     void afficher_parcours2(size_t num, int fin, const std::vector<int>& arbre);
     void MenuIndiceCentralite(Svgfile&svgout);
     void Dessiner_centralite_prox(Svgfile&svgout);
     void Dessiner_centralite_intermediaire(Svgfile&svgout);
     void Dessiner_centralite_degre(Svgfile&svgout);
     void Dessiner_centralite_vect_propre(Svgfile &svgout);
     void Menu_afficher_centralite(Svgfile&svgout);

    protected:

        private:
        int m_orientation;
        std::vector<Sommet*> m_sommets;
        std::vector<Arete*> m_arretes;
    };

#endif // GRAPHE_H

