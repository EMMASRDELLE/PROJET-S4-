#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED
#include <vector>
#include <iostream>
#include <fstream>

class Sommet
{
    public:
        Sommet(int indice, char nom, int x, int y);
        virtual ~Sommet();
        char getNom();
        int getX() ;
        int getY () ;


    protected:

    private:

        int m_indice;
        char m_nom;
        int m_x;
        int m_y;
};

#endif // SOMMET_H_INCLUDED
