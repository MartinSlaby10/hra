#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
//----------------utilitky-------------------------------------------------------
    void vytiskniCaru(char znak = '-', int delka = 60) {
    for (int i = 0; i < delka; ++i) cout << znak;
    cout << '\n';
}
    int nahodne(int minimum, int maximum) {
    return minimum + rand() % (maximum - minimum + 1);
}
//-------------------------------------efekty------------------------------------------
struct StatusEfekt {
        string nazev;
        int zbyvajiciKola;
        int poskozeniZaKolo;
        bool omracen;
        bool oslepeny;
        bool vystraseny;
    };
//--------------------------hrac--------------------------------------------------------
    struct Hrac {
        string nazevTridy;
        int maxZivoty, zivoty;
        int maxMana, mana;
        int zlato;
        int uroven;
        int zkusenosti, zkusenostiNaDalsiUroven;
        int utok;
        string nazevSchopnosti[3];
        int cenaMana[3];
        int poskozeniSchopnosti[3];
        bool plosnaSchopnost[3];
        bool leceniSchopnost[3];
        bool jedSchopnost[3];
        bool omraceniSchopnost[3];
};
//------------------------------enemak-----------------------------------------
    struct Nepritel {
        string jmeno;
        int maxZivoty, zivoty;
        int utok;
        int zlatoMin, zlatoMax;
        int odmenaZkusenosti;
        bool jeHlavniBoss;
        bool jeMiniBoss;
        vector<string> hlasy;
        StatusEfekt stav;
        bool jeHexara;
        int posledniKouzloIndex;
        bool pouzitaKouzla[6];
        bool hexovaSmrstAktivni;
};
    Nepritel vytvorNepritele(string jmeno, int zivoty, int utok, int zlatoMin, int zlatoMax, int odmenaXp, bool mini = false, bool boss = false, vector<string> hlasy = {}) {
        Nepritel n;
        n.jmeno = jmeno; n.maxZivoty = zivoty; n.utok = utok; n.zlatoMin = zlatoMin; n.zlatoMax = zlatoMax; n.odmenaZkusenosti = odmenaXp; n.jeHlavniBoss = boss; n.jeMiniBoss = mini; n.hlasy = hlasy;
        n.stav = {"", 0, 0, false, false, false};
        n.jeHexara = false; n.posledniKouzloIndex = -1;
        for (int i = 0; i < 6; ++i) n.pouzitaKouzla[i] = false;
        n.hexovaSmrstAktivni = false;
        return n;
    }

//----------------------------------------------------------------------

int main(){

}
