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
//--------------------------------monstra-------------------------------------------------------------------------------------------------------------------------------------------------------
vector<Nepritel> vsechnaMonstra() {
        return {
            vytvorNepritele("Goblin",    6,  2, 1, 4,  8,  false,false, {"Mam te gobline!","Buuu!","Zlaticko!"}),
            vytvorNepritele("Skret",     7,  2, 1, 3,  9,  false,false, {"Gnaf gnaf!","Crush!"}),
            vytvorNepritele("Vlk",       8,  3, 0, 2,  10, false,false, {"Vrrrr!","Auuuu!"}),
            vytvorNepritele("Kostlivec", 7,  2, 0, 3,  9,  false,false, {"Klap klap klap!","Shhhhh..."}),
            vytvorNepritele("Zombie",    9,  3, 0, 2,  11, false,false, {"Mozek!","Hrrrggg..."}),
            vytvorNepritele("Ork",      10,  4, 2, 5,  12, false,false, {"WAAAAAGH!","Smrt lidem!"}),
            vytvorNepritele("Netopyr",   5,  2, 0, 1,   7, false,false, {"Sssss!","Citim krev!"}),
            vytvorNepritele("Pavouk",    6,  2, 0, 2,   8, false,false, {"...","...klik klik klik"}),
            vytvorNepritele("Had",       6,  2, 0, 2,   8, false,false, {"Ssssss!","Jed tece..."}),
            vytvorNepritele("Mumie",     9,  3, 1, 4,  11, false,false, {"Kletba na tebe!","Vecny spanek!"}),
            vytvorNepritele("Banshee",   8,  3, 0, 3,  10, false,false, {"AAAAAAAAAH!","Slysis me?!"}),
            vytvorNepritele("Kyklon",   11,  4, 1, 4,  13, false,false, {"BOOM!","Whuuush!"}),
            vytvorNepritele("Golem",    12,  4, 0, 3,  12, false,false, {"...","GRUMBLE."}),
            vytvorNepritele("Duch",      7,  2, 0, 2,   9, false,false, {"Vidis me?","Buuu!"}),
            vytvorNepritele("Troll",    13,  5, 1, 5,  14, false,false, {"Tamaten hnufak cufak!","Zlaticko-sutry moje!","Do gulase te!"}),
            vytvorNepritele("Harpy",     9,  3, 0, 3,  10, false,false, {"Screeee!","Letaam!"}),
        };
    }

//--------------------------------------miniboss+hexara----------------------------------------------------------------------------------------------------------------------------

Nepritel vytvorMiniHydru() {
        return vytvorNepritele("Hydra", 25, 6, 8, 15, 35, true,false,
                               {"Kolik hlav ces vidет?!","Rrraaah!","Znovu vyrostu!"});
    }
Nepritel vytvorMiniLicha() {
        return vytvorNepritele("Lich", 22, 7, 10, 18, 40, true,false,
                               {"Smrt je jen zacatek.","Magie te pohltí!","Mrtvi vstaveji!"});
    }
Nepritel vytvorHexaru() {
        Nepritel n = vytvorNepritele("Hexara", 60, 0, 0, 0, 150, false, true,
                                     {"Sest rukou - sest osudu!","Chaos je ma sila!","Citis to? Magie vre!"});
        n.jeHexara = true;
        return n;
    }

//----------------------------------------------------------------------

int main(){

}
