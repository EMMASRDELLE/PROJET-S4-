#ifndef SOMMET_H
#define SOMMET_H


class Sommet
{
    public:
        Sommet(int num, char nom, int x,int y);
        int getNum() const;
        int getX() const;
        int getY() const;
        char getNom() const;

    protected:

    private:
        int m_num;
        int m_x;
        int m_y;
        char m_nom;
};

#endif // SOMMET_H
