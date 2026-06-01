#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;
//----------------utilitky-------------------------------------------------------
    void pauza() {
        cout << "\n[Stiskni Enter pro pokracovani...]";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
}
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

//----------------------------------zobrazeniHrace-------------------------------------------------------------

void zobrazHrace(const Hrac& h) {
    vytiskniCaru();
    cout << "  " << h.nazevTridy << "  |  Uroven " << h.uroven
         << "  |  XP " << h.zkusenosti << "/" << h.zkusenostiNaDalsiUroven << "\n";
    cout << "  HP: " << h.zivoty << "/" << h.maxZivoty
         << "  |  Mana: " << h.mana << "/" << h.maxMana
         << "  |  Zlato: " << h.zlato << "\n";
    vytiskniCaru();
}

//-------------------------uroven----------------------------------

void zvysUroven(Hrac& h) {
    h.uroven++;
    int ziskZivotu = 2, ziskMany = 1, ziskUtoku = 1;
    h.maxZivoty += ziskZivotu;
    h.zivoty = min(h.zivoty + ziskZivotu, h.maxZivoty);
    h.maxMana += ziskMany;
    h.utok += ziskUtoku;
    h.zkusenostiNaDalsiUroven = h.zkusenostiNaDalsiUroven + h.zkusenostiNaDalsiUroven / 2;
    h.zkusenosti = 0;
    cout << "\n*** LEVEL UP! Jsi nyni uroven " << h.uroven << "! ***\n";
    cout << "  Max HP +" << ziskZivotu << ", Mana +" << ziskMany
         << ", Utok +" << ziskUtoku << "\n";
    pauza();
}

//------------------------xpcka--------------------------------------------------------------------------

void pridejZkusenosti(Hrac& h, int mnozstvi) {
    h.zkusenosti += mnozstvi;
    cout << "  (+" << mnozstvi << " XP)\n";
    while (h.zkusenosti >= h.zkusenostiNaDalsiUroven) zvysUroven(h);
}

//------------------------tridy----------------------------------------------------------------------------------------------------------------------------------------
Hrac vytvorPaladina() {
    Hrac h;
    h.nazevTridy="Paladin"; h.maxZivoty=14; h.zivoty=14;
    h.maxMana=8; h.mana=8; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=4;
    h.nazevSchopnosti[0]="Uder";         h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=4;  h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Uder svetlem"; h.cenaMana[1]=2; h.poskozeniSchopnosti[1]=2;  h.plosnaSchopnost[1]=true;  h.leceniSchopnost[1]=false; h.jedSchopnost[1]=false; h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Oziveni";      h.cenaMana[2]=1; h.poskozeniSchopnosti[2]=2;  h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=true;  h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=false;
    return h;
}

Hrac vytvorLovce() {
    Hrac h;
    h.nazevTridy="Lovec"; h.maxZivoty=11; h.zivoty=11;
    h.maxMana=6; h.mana=6; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=5;
    h.nazevSchopnosti[0]="Strela";          h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=5; h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Jedovata strela"; h.cenaMana[1]=2; h.poskozeniSchopnosti[1]=3; h.plosnaSchopnost[1]=false; h.leceniSchopnost[1]=false; h.jedSchopnost[1]=true;  h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Omraceni";        h.cenaMana[2]=3; h.poskozeniSchopnosti[2]=2; h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=false; h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=true;
    return h;
}

Hrac vytvorMaga() {
    Hrac h;
    h.nazevTridy="Mag"; h.maxZivoty=9; h.zivoty=9;
    h.maxMana=14; h.mana=14; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=3;
    h.nazevSchopnosti[0]="Kouzelny sip";  h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=3; h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Ohniva koule";  h.cenaMana[1]=3; h.poskozeniSchopnosti[1]=4; h.plosnaSchopnost[1]=true;  h.leceniSchopnost[1]=false; h.jedSchopnost[1]=false; h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Absorpce many"; h.cenaMana[2]=0; h.poskozeniSchopnosti[2]=3; h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=true;  h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=false;
    return h;
}
Hrac vytvorWarlocka() {
    Hrac h;
    h.nazevTridy="Warlock"; h.maxZivoty=10; h.zivoty=10;
    h.maxMana=12; h.mana=12; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=4;
    h.nazevSchopnosti[0]="Temny dotyk";   h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=4; h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Prokleti";      h.cenaMana[1]=3; h.poskozeniSchopnosti[1]=3; h.plosnaSchopnost[1]=false; h.leceniSchopnost[1]=false; h.jedSchopnost[1]=true;  h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Kradez zivota"; h.cenaMana[2]=4; h.poskozeniSchopnosti[2]=5; h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=true;  h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=false;
    return h;
}

//-------------------------------------------------------------------------------zobrazeniTridy-------------------------------------------------------------------------------------------------------------------------------

void zobrazInfoTridy(const Hrac& h) {
    vytiskniCaru('=');
    cout << "  Class: " << h.nazevTridy << "\n";
    vytiskniCaru();
    cout << "  Statistiky:\n";
    cout << "    HP:   " << h.zivoty << "/" << h.maxZivoty << "\n";
    cout << "    Utok: " << h.utok << "\n";
    cout << "    Mana: " << h.mana << "/" << h.maxMana << "\n";
    cout << "\n  Schopnosti:\n";
    for (int i=0;i<3;i++) {
        cout << "    [" << i+1 << "] " << h.nazevSchopnosti[i];
        if (h.cenaMana[i]>0) cout << " (cena: " << h.cenaMana[i] << " many)";
 
        int typSchopnosti = (h.leceniSchopnost[i]   ? 8 : 0)
                          | (h.plosnaSchopnost[i]    ? 4 : 0)
                          | (h.jedSchopnost[i]       ? 2 : 0)
                          | (h.omraceniSchopnost[i]  ? 1 : 0);
        switch (typSchopnosti) {
        case 8:  cout << " [LECENI +" << h.poskozeniSchopnosti[i] << " HP]";       break;
        case 4:  cout << " [AOE dmg: " << h.poskozeniSchopnosti[i] << "]";         break;
        case 2:  cout << " [JED + dmg: " << h.poskozeniSchopnosti[i] << "]";       break;
        case 1:  cout << " [OMRACENI + dmg: " << h.poskozeniSchopnosti[i] << "]";  break;
        default: cout << " [dmg: " << h.poskozeniSchopnosti[i] << "]";             break;
        }
        cout << "\n";
    }
    vytiskniCaru('=');
}

//-------------------------------------------------------vybertridy-----------------------------------------------------------------------------------------------

Hrac zvolitTridu() {
    vector<Hrac> tridy = { vytvorPaladina(), vytvorLovce(), vytvorMaga(), vytvorWarlocka() };
    while (true) {
        cout << "\nVyber si classu:\n";
        for (int i=0;i<(int)tridy.size();i++)
            cout << "  [" << i+1 << "] " << tridy[i].nazevTridy << "\n";
        int volba; cin >> volba;
        if (volba<1||volba>(int)tridy.size()) { cout << "Neplatna volba.\n"; continue; }
        Hrac& vybranaTrida = tridy[volba-1];
        zobrazInfoTridy(vybranaTrida);
        cout << "Chces hrat za " << vybranaTrida.nazevTridy << "? [1=Ano / 2=Jina class]: ";
        int potvrzeni; cin >> potvrzeni;
        if (potvrzeni==1) return vybranaTrida;
    }
}

//------------------------------enemak---------------------------------------------------------------------------------
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