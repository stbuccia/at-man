#ifndef PERSONAGGIO_HPP_INCLUDED
#define PERSONAGGIO_HPP_INCLUDED
#define AZIONE 5
#include <string>
#include "Arma.hpp"
#include <stdlib.h>
#include <time.h>
class Personaggio
{
    protected:
        int str, dex, con, luck, hp, hpmax;    //forza, destrezza, costituzione, fortuna, punti vita, punti vita massimi
        int lev, act, exp;                               //livello, punti azione per turno, punti esperienza attuali
        int posx;
        int posy;
        Arma equip;                     //arma equipaggiata
    public:
        Personaggio();
        void setEquip(Arma e);
        void setStr(int x);
        void setDex(int x);
        void setCon(int x);
        void setLuck(int x);
        void setHp(int x);
        void setAct(int x);
        void setPosX(int x);
        void setPosY(int y);
        bool actMuovi();
        bool actUsa();
        bool actApri();
        bool actVend();
        bool actAttacca();
        int getPosX();
        int getPosY();
        int getStr();
        int getDex();
        int getCon();
        int getLuck();
        int getHp();
        int getHpmax();
        int getAct();
        int getLev();
        int getExp();
        Arma getEquip();
        int attacca(Personaggio *p);

};

#endif // PERSONAGGIO_HPP_INCLUDED
