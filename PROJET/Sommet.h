#ifndef SOMMET_H
#define SOMMET_H


class Sommet
{
    public:
        Sommet(int num, char nom, double x,double y);
        int getNum() const;
        double getX() const;
        double getY() const;
        char getNom() const;

    protected:

    private:
        int m_indice;
        double m_x;
        double m_y;
        char m_nom;
};

#endif // SOMMET_H
