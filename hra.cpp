#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;

//---------------------------------------------------------------------utilitky----------------------------------------------------------------------------------
void pauza() {
    cout << "\n[Stiskni Enter pro pokracovani...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int nahodne(int minimum, int maximum) {
    return minimum + rand() % (maximum - minimum + 1);
}

void vytiskniCaru(char znak = '-', int delka = 60) {
    for (int i = 0; i < delka; ++i) cout << znak;
    cout << '\n';
}

//-------------------------------------------------statusefekty--------------------------------------------------------------------------------------------------
struct StatusEfekt {
    string nazev;
    int zbyvajiciKola;
    int poskozeniZaKolo;
    bool omracen;
    bool oslepeny;
    bool vystraseny;
};

//-------------------------------------------------------------------hrac------------------------------------------------------------------------------
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

void zobrazHrace(const Hrac& h) {
    vytiskniCaru();
    cout << "  " << h.nazevTridy << "  |  Uroven " << h.uroven
         << "  |  XP " << h.zkusenosti << "/" << h.zkusenostiNaDalsiUroven << "\n";
    cout << "  HP: " << h.zivoty << "/" << h.maxZivoty
         << "  |  Mana: " << h.mana << "/" << h.maxMana
         << "  |  Zlato: " << h.zlato << "\n";
    vytiskniCaru();
}

void zvysUroven(Hrac& h) {
    h.uroven++;
    int ziskZivotu = 2, ziskMany = 1, ziskUtoku = 1;
    h.maxZivoty   += ziskZivotu;
    h.zivoty      = min(h.zivoty + ziskZivotu, h.maxZivoty);
    h.maxMana     += ziskMany;
    h.utok        += ziskUtoku;
    h.zkusenostiNaDalsiUroven = h.zkusenostiNaDalsiUroven + h.zkusenostiNaDalsiUroven / 2;
    h.zkusenosti = 0;
    cout << "\n*** LEVEL UP! Jsi nyni uroven " << h.uroven << "! ***\n";
    cout << "  Max HP +" << ziskZivotu << ", Mana +" << ziskMany
         << ", Utok +" << ziskUtoku << "\n";
    pauza();
}

void pridejZkusenosti(Hrac& h, int mnozstvi) {
    h.zkusenosti += mnozstvi;
    cout << "  (+" << mnozstvi << " XP)\n";
    while (h.zkusenosti >= h.zkusenostiNaDalsiUroven) zvysUroven(h);
}

//-----------------------------------------------------------enemak-----------------------------------------------------------------------------------------------------------------------------------
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

Nepritel vytvorNepritele(string jmeno, int zivoty, int utok,
                         int zlatoMin, int zlatoMax, int odmenaXp,
                         bool mini=false, bool boss=false,
                         vector<string> hlasy={}) {
    Nepritel n;
    n.jmeno = jmeno; n.maxZivoty = zivoty; n.zivoty = zivoty;
    n.utok = utok; n.zlatoMin = zlatoMin; n.zlatoMax = zlatoMax;
    n.odmenaZkusenosti = odmenaXp; n.jeHlavniBoss = boss; n.jeMiniBoss = mini;
    n.hlasy = hlasy;
    n.stav = {"", 0, 0, false, false, false};
    n.jeHexara = false;
    n.posledniKouzloIndex = -1;
    for (int i=0;i<6;i++) n.pouzitaKouzla[i]=false;
    n.hexovaSmrstAktivni = false;
    return n;
}

//--------------------------------------monstra------------------------------------------------------------------------------------------------------------------------------------------
vector<Nepritel> vsechnaMonstra() {
    return {
        vytvorNepritele("Goblin",    5,  1, 1, 4,  8,  false,false, {"Mam te gobline!","Buuu!","Zlaticko!"}),
        vytvorNepritele("Skret",     6,  1, 1, 3,  9,  false,false, {"Gnaf gnaf!","Crush!"}),
        vytvorNepritele("Vlk",       7,  2, 0, 2,  10, false,false, {"Vrrrr!","Auuuu!"}),
        vytvorNepritele("Kostlivec", 6,  1, 0, 3,  9,  false,false, {"Klap klap klap!","Shhhhh..."}),
        vytvorNepritele("Zombie",    8,  2, 0, 2,  11, false,false, {"Mozek!","Hrrrggg..."}),
        vytvorNepritele("Ork",       9,  3, 2, 5,  12, false,false, {"WAAAAAGH!","Smrt lidem!"}),
        vytvorNepritele("Netopyr",   4,  1, 0, 1,   7, false,false, {"Sssss!","Citim krev!"}),
        vytvorNepritele("Pavouk",    5,  1, 0, 2,   8, false,false, {"...","...klik klik klik"}),
        vytvorNepritele("Had",       5,  1, 0, 2,   8, false,false, {"Ssssss!","Jed tece..."}),
        vytvorNepritele("Mumie",     8,  2, 1, 4,  11, false,false, {"Kletba na tebe!","Vecny spanek!"}),
        vytvorNepritele("Banshee",   7,  2, 0, 3,  10, false,false, {"AAAAAAAAAH!","Slysis me?!"}),
        vytvorNepritele("Kyklon",   10,  3, 1, 4,  13, false,false, {"BOOM!","Whuuush!"}),
        vytvorNepritele("Golem",    11,  3, 0, 3,  12, false,false, {"...","GRUMBLE."}),
        vytvorNepritele("Duch",      6,  1, 0, 2,   9, false,false, {"Vidis me?","Buuu!"}),
        vytvorNepritele("Troll",    12,  4, 1, 5,  14, false,false, {"Tamaten hnufak cufak!","Zlaticko-sutry moje!","Do gulase te!"}),
        vytvorNepritele("Harpy",     8,  2, 0, 3,  10, false,false, {"Screeee!","Letaam!"}),
    };
}

Nepritel vytvorMiniHydru() {
    return vytvorNepritele("Hydra", 22, 5, 8, 15, 35, true,false,
                           {"Kolik hlav ces vidет?!","Rrraaah!","Znovu vyrostu!"});
}
Nepritel vytvorMiniLicha() {
    return vytvorNepritele("Lich", 19, 6, 10, 18, 40, true,false,
                           {"Smrt je jen zacatek.","Magie te pohltí!","Mrtvi vstaveji!"});
}
Nepritel vytvorHexaru() {
    Nepritel n = vytvorNepritele("Hexara", 60, 0, 0, 0, 150, false, true,
                                 {"Sest rukou - sest osudu!","Chaos je ma sila!","Citis to? Magie vre!"});
    n.jeHexara = true;
    return n;
}

//------------------------------------------------------------------------------------tridy---------------------------------------------------------------------------------------------------------------------
Hrac vytvorPaladina() {
    Hrac h;
    h.nazevTridy="Paladin"; h.maxZivoty=16; h.zivoty=16;
    h.maxMana=8; h.mana=8; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=5;
    h.nazevSchopnosti[0]="Uder";         h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=5;  h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Uder svetlem"; h.cenaMana[1]=2; h.poskozeniSchopnosti[1]=3;  h.plosnaSchopnost[1]=true;  h.leceniSchopnost[1]=false; h.jedSchopnost[1]=false; h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Oziveni";      h.cenaMana[2]=1; h.poskozeniSchopnosti[2]=2;  h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=true;  h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=false;
    return h;
}
Hrac vytvorLovce() {
    Hrac h;
    h.nazevTridy="Lovec"; h.maxZivoty=12; h.zivoty=12;
    h.maxMana=6; h.mana=6; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=6;
    h.nazevSchopnosti[0]="Strela";          h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=6; h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Jedovata strela"; h.cenaMana[1]=2; h.poskozeniSchopnosti[1]=3; h.plosnaSchopnost[1]=false; h.leceniSchopnost[1]=false; h.jedSchopnost[1]=true;  h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Omraceni";        h.cenaMana[2]=3; h.poskozeniSchopnosti[2]=2; h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=false; h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=true;
    return h;
}
Hrac vytvorMaga() {
    Hrac h;
    h.nazevTridy="Mag"; h.maxZivoty=10; h.zivoty=10;
    h.maxMana=14; h.mana=14; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=4;
    h.nazevSchopnosti[0]="Kouzelny sip";  h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=4; h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Ohniva koule";  h.cenaMana[1]=3; h.poskozeniSchopnosti[1]=6; h.plosnaSchopnost[1]=true;  h.leceniSchopnost[1]=false; h.jedSchopnost[1]=false; h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Absorpce many"; h.cenaMana[2]=0; h.poskozeniSchopnosti[2]=3; h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=true;  h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=false;
    return h;
}
Hrac vytvorWarlocka() {
    Hrac h;
    h.nazevTridy="Warlock"; h.maxZivoty=11; h.zivoty=11;
    h.maxMana=12; h.mana=12; h.zlato=5; h.uroven=1;
    h.zkusenosti=0; h.zkusenostiNaDalsiUroven=20; h.utok=5;
    h.nazevSchopnosti[0]="Temny dotyk";   h.cenaMana[0]=0; h.poskozeniSchopnosti[0]=5; h.plosnaSchopnost[0]=false; h.leceniSchopnost[0]=false; h.jedSchopnost[0]=false; h.omraceniSchopnost[0]=false;
    h.nazevSchopnosti[1]="Prokleti";      h.cenaMana[1]=3; h.poskozeniSchopnosti[1]=3; h.plosnaSchopnost[1]=false; h.leceniSchopnost[1]=false; h.jedSchopnost[1]=true;  h.omraceniSchopnost[1]=false;
    h.nazevSchopnosti[2]="Kradez zivota"; h.cenaMana[2]=4; h.poskozeniSchopnosti[2]=5; h.plosnaSchopnost[2]=false; h.leceniSchopnost[2]=true;  h.jedSchopnost[2]=false; h.omraceniSchopnost[2]=false;
    return h;
}

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
//-----------------------------volba tridy----------------------------------------------------------------
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

//------------------------------------------------souboj-----------------------------------------------------------------------------------
string jmenoKouzlaHexary(int index) {
    const string nazvy[6] = {"Fireball","Ledovy vybuch","Stinovy uder","Vetrny vir","Jedovy oblak","Leciva zare"};
    return nazvy[index];
}

int tahHexary(Nepritel& hexara, Hrac& h, bool& hracMrtvy) {
    int indexKouzla = nahodne(0,5);
    bool kriticke = (indexKouzla == hexara.posledniKouzloIndex) && hexara.posledniKouzloIndex != -1;

    cout << "\n  Hexara chysta: " << jmenoKouzlaHexary(indexKouzla);
    if (kriticke) cout << " [KRITICKE!]";
    cout << "\n";

    hexara.pouzitaKouzla[indexKouzla] = true;

    bool vsechnaPoužita = true;
    for (int i=0;i<6;i++) if (!hexara.pouzitaKouzla[i]) { vsechnaPoužita=false; break; }
    if (vsechnaPoužita && !hexara.hexovaSmrstAktivni) {
        hexara.hexovaSmrstAktivni = true;
        cout << "\n  *** HEXARA AKTIVUJE HEXOVOU SMRST! ***\n";
        cout << "  Masivni vybuch vsech sesti zivlu!\n";
        int poskozeni = 14;
        h.zivoty -= poskozeni;
        cout << "  Beres " << poskozeni << " poskozeni!\n";
        if (h.zivoty <= 0) { hracMrtvy = true; return 0; }
        hexara.posledniKouzloIndex = indexKouzla;
        return 0;
    }

    int poskozeni = 0;
    switch(indexKouzla) {
        case 0:
            poskozeni = kriticke ? 9 : 5;
            cout << "  Ohniva koule te zasahne za " << poskozeni << " poskozeni!\n";
            h.zivoty -= poskozeni;
            break;
        case 1:
            poskozeni = kriticke ? 6 : 3;
            cout << "  Ledovy vybuch te zasahne za " << poskozeni << " poskozeni a zpomali te!\n";
            h.zivoty -= poskozeni;
            break;
        case 2:
            poskozeni = kriticke ? 5 : 2;
            cout << "  Stinovy uder zasahne za " << poskozeni << " poskozeni a oslepi te!\n";
            h.zivoty -= poskozeni;
            h.mana = max(0, h.mana - 1);
            break;
        case 3:
            poskozeni = kriticke ? 6 : 3;
            cout << "  Vetrny vir te odmrsti za " << poskozeni << " poskozeni!\n";
            h.zivoty -= poskozeni;
            break;
        case 4:
            poskozeni = kriticke ? 5 : 2;
            cout << "  Jedovy oblak zasahne za " << poskozeni << " a otravi te na 3 kola!\n";
            h.zivoty -= poskozeni;
            break;
        case 5:
            {int uzdraveni = kriticke ? 10 : 5;
            hexara.zivoty = min(hexara.maxZivoty, hexara.zivoty + uzdraveni);
            cout << "  Hexara se zahoji o " << uzdraveni << " HP! (HP: " << hexara.zivoty << "/" << hexara.maxZivoty << ")\n";}
            break;
    }
    hexara.posledniKouzloIndex = indexKouzla;
    if (h.zivoty <= 0) hracMrtvy = true;
    return 0;
}

bool provedSouboj(Hrac& h, vector<Nepritel> nepratele) {
    vytiskniCaru('*');
    cout << "  *** SOUBOJ! ***\n";
    for (auto& n : nepratele)
        cout << "  -> " << n.jmeno << " (HP: " << n.zivoty << ", ATK: " << n.utok << ")\n";
    vytiskniCaru('*');
    pauza();

    while (true) {
        bool nekdoZije = false;
        for (auto& n : nepratele) if (n.zivoty > 0) { nekdoZije = true; break; }
        if (!nekdoZije) break;
        if (h.zivoty <= 0) return false;


        for (auto& n : nepratele) {
            if (n.zivoty <= 0) continue;
            if (n.stav.zbyvajiciKola > 0) {
                if (n.stav.poskozeniZaKolo > 0) {
                    cout << "  [JED] " << n.jmeno << " bere " << n.stav.poskozeniZaKolo << " poskozeni z jedu!\n";
                    n.zivoty -= n.stav.poskozeniZaKolo;
                }
                n.stav.zbyvajiciKola--;
                if (n.stav.zbyvajiciKola == 0) {
                    cout << "  [STATUS] " << n.jmeno << " se zbavil efektu.\n";
                    n.stav = {"",0,0,false,false,false};
                }
            }
        }


        zobrazHrace(h);
        cout << "\n  Nepratele:\n";
        for (int i=0;i<(int)nepratele.size();i++)
            if (nepratele[i].zivoty > 0)
                cout << "  [" << i+1 << "] " << nepratele[i].jmeno
                     << " HP: " << nepratele[i].zivoty << "/" << nepratele[i].maxZivoty << "\n";

        cout << "\n  Tvoje akce:\n";
        for (int i=0;i<3;i++) {
            cout << "  [" << i+1 << "] " << h.nazevSchopnosti[i];
            if (h.cenaMana[i]>0) cout << " (mana: " << h.cenaMana[i] << ")";
            cout << "\n";
        }

        int akce = -1;
        while (akce<1||akce>3) { cout << "  Volba: "; cin >> akce; }

        int indexAkce = akce-1;
        if (h.cenaMana[indexAkce] > h.mana) {
            cout << "  Nedostatek many! Pouzivas zakladni utok.\n";
            indexAkce = 0;
        }
        h.mana -= h.cenaMana[indexAkce];

        int indexCile = -1;
        if (!h.plosnaSchopnost[indexAkce] && !h.leceniSchopnost[indexAkce]) {
            if (nepratele.size() == 1) {
                indexCile = 0;
            } else {
                cout << "  Na koho? ";
                while (indexCile<0||(size_t)indexCile>=nepratele.size()||nepratele[indexCile].zivoty<=0) {
                    cin >> indexCile; indexCile--;
                }
            }
        }

        int typAkce = h.leceniSchopnost[indexAkce] ? 2
                    : h.plosnaSchopnost[indexAkce]  ? 1 : 0;
        switch (typAkce) {
            case 2: {
                int uzdraveni = h.poskozeniSchopnosti[indexAkce];
                h.zivoty = min(h.maxZivoty, h.zivoty + uzdraveni);
                cout << "  Uzdravujes se o " << uzdraveni << " HP.\n";
                break;
            }
            case 1: {
                for (auto& n : nepratele) {
                    if (n.zivoty <= 0) continue;
                    int poskozeni = h.poskozeniSchopnosti[indexAkce];
                    n.zivoty -= poskozeni;
                    cout << "  " << n.jmeno << " bere " << poskozeni << " poskozeni!\n";
                }
                break;
            }
            default: {
                Nepritel& cil = nepratele[indexCile];
                int poskozeni = h.poskozeniSchopnosti[indexAkce];
                bool zasah = true;
                if (cil.stav.oslepeny && nahodne(1,100) <= 30) {
                    zasah = false; cout << "  Minul sis! (slabost)\n";
                }
                if (zasah) {
                    cil.zivoty -= poskozeni;
                    cout << "  " << cil.jmeno << " bere " << poskozeni << " poskozeni! (HP: "
                         << max(0,cil.zivoty) << "/" << cil.maxZivoty << ")\n";
                    if (h.jedSchopnost[indexAkce] && cil.zivoty > 0) {
                        cil.stav = {"Jed", 3, 2, false, false, false};
                        cout << "  " << cil.jmeno << " je otraven! (3 kola)\n";
                    }
                    if (h.omraceniSchopnost[indexAkce] && cil.zivoty > 0) {
                        cil.stav = {"Omraceni", 1, 0, true, false, false};
                        cout << "  " << cil.jmeno << " je omracen a vynecha svuj tah!\n";
                    }
                }
                break;
            }
        }


        for (auto& n : nepratele) {
            if (n.zivoty <= 0 && n.odmenaZkusenosti > 0) {
                cout << "\n  " << n.jmeno << " byl porazen!\n";
                pridejZkusenosti(h, n.odmenaZkusenosti);
                bool maZlato = n.jeMiniBoss || n.jeHlavniBoss || (nahodne(1,2)==1);
                if (maZlato && n.zlatoMax > 0) {
                    int ziskaneZlato = nahodne(n.zlatoMin, n.zlatoMax);
                    h.zlato += ziskaneZlato;
                    cout << "  Ziskavas " << ziskaneZlato << " zlata!\n";
                }
                n.odmenaZkusenosti = 0;
            }
        }

        nekdoZije = false;
        for (auto& n : nepratele) if (n.zivoty > 0) { nekdoZije = true; break; }
        if (!nekdoZije) break;


        cout << "\n  --- Tah nepratel ---\n";
        for (auto& n : nepratele) {
            if (n.zivoty <= 0) continue;
            if (n.stav.omracen) {
                cout << "  " << n.jmeno << " je omracen a vynechava svuj tah!\n";
                continue;
            }
            if (!n.hlasy.empty() && nahodne(1,100)<=30)
                cout << "  " << n.jmeno << ": \"" << n.hlasy[nahodne(0,(int)n.hlasy.size()-1)] << "\"\n";

            if (n.jeHexara) {
                bool hracMrtvy = false;
                tahHexary(n, h, hracMrtvy);
                if (hracMrtvy) return false;
            } else {
                bool minul = (n.stav.oslepeny && nahodne(1,100)<=40);
                if (minul) {
                    cout << "  " << n.jmeno << " oslepeny miji!\n";
                } else {
                    int poskozeni = n.utok;
                    h.zivoty -= poskozeni;
                    cout << "  " << n.jmeno << " te zasahne za " << poskozeni << " poskozeni!\n";
                }
                if (h.zivoty <= 0) return false;
            }
        }
        pauza();
    }
    cout << "\n  *** Vyhral si! ***\n";
    return true;
}

//--------------------------------------------------------vesnice---------------------------------------------------------------------------
void navstivVesnici(Hrac& h, const string& nazevVesnice) {
    vytiskniCaru('=');
    cout << "  Vesnice: " << nazevVesnice << "\n";
    vytiskniCaru('=');
    while (true) {
        cout << "  Zlato: " << h.zlato << "\n";
        cout << "  HP: " << h.zivoty << "/" << h.maxZivoty << "\n";
        cout << "  Mana: " << h.mana << "/" << h.maxMana << "\n\n";
        cout << "  Co chces udelat?\n";
        cout << "  [1] Doplnit zivoty (5 zlata)\n";
        cout << "  [2] Vylepsit max HP o 3 (10 zlata)\n";
        cout << "  [3] Vylepsit max Manu o 3 (10 zlata)\n";
        cout << "  [4] Vylepsit utok o 2 (15 zlata)\n";
        cout << "  [5] Odejit\n";
        int volba; cin >> volba;
        switch (volba) {
            case 1:
                if (h.zlato<5) { cout << "  Nemas dost zlata!\n"; break; }
                h.zlato-=5; h.zivoty=h.maxZivoty;
                cout << "  HP doplneno na " << h.maxZivoty << ".\n";
                break;
            case 2:
                if (h.zlato<10) { cout << "  Nemas dost zlata!\n"; break; }
                h.zlato-=10; h.maxZivoty+=3; h.zivoty=min(h.zivoty+3,h.maxZivoty);
                cout << "  Max HP nyni: " << h.maxZivoty << ".\n";
                break;
            case 3:
                if (h.zlato<10) { cout << "  Nemas dost zlata!\n"; break; }
                h.zlato-=10; h.maxMana+=3; h.mana=min(h.mana+3,h.maxMana);
                cout << "  Max Mana nyni: " << h.maxMana << ".\n";
                break;
            case 4:
                if (h.zlato<15) { cout << "  Nemas dost zlata!\n"; break; }
                h.zlato-=15; h.utok+=2; h.poskozeniSchopnosti[0]+=2;
                cout << "  Utok nyni: " << h.utok << ".\n";
                break;
            case 5:
                cout << "  Odchazis z vesnice.\n";
                return;
            default:
                cout << "  Neplatna volba.\n";
                break;
        }
    }
}

//-----------------------------------------------------------------------moudry strom-----------------------------------------------------
void mudryStrom(Hrac& h) {
    vytiskniCaru('~');
    cout << "\n  *Prichazis na mytinu, kde stoji osamely strom.*\n";
    cout << "  Strom: \"Zdravim pocestny! Odvazujes se pomerat mysli se mnou?\"\n";
    cout << "  [1] Ano  [2] Ne\n";
    int volba; cin >> volba;
    if (volba != 1) { cout << "  Strom: \"Skoda. Mozna priste.\"\n"; return; }

    cout << "\n  Strom: \"Dobra! Kazdy den na obloze se vznasi,\n";
    cout << "          radost nam prinasi a bubaky strasi. Co to je?\"\n";
    cout << "  [1] Slunce  [2] Hrnec  [3] Mesic\n";
    cin >> volba;
    switch (volba) {
        case 1: {
            int odmena = 3;
            h.zivoty = min(h.maxZivoty, h.zivoty + odmena);
            cout << "\n  Strom: \"Ha, koumak jsi! Magii ti navysuji zivoty o " << odmena << ". Upaluj dal!\"\n";
            break;
        }
        default: {
            int trest = 2;
            h.zivoty = max(1, h.zivoty - trest);
            cout << "\n  Strom: \"Spatne! Za trest ti uberu " << trest << " HP. Priste premyslej!\"\n";
            break;
        }
    }
    vytiskniCaru('~');
    pauza();
}

//------------------------------------------------------hlavni scena--------------------------------------------
void spustHru(Hrac& h) {
    vector<Nepritel> zasobnikMonster = vsechnaMonstra();
    for (int i=(int)zasobnikMonster.size()-1;i>0;i--) {
        int nahodnyIndex = rand()%(i+1);
        swap(zasobnikMonster[i],zasobnikMonster[nahodnyIndex]);
    }
    int indexMonstra = 0;

    auto dalsiMonstrum = [&]() -> Nepritel& {
        return zasobnikMonster[indexMonstra++ % zasobnikMonster.size()];
    };

    auto scena = [&](const string& popis) {
        vytiskniCaru('#');
        cout << "  " << popis << "\n";
        vytiskniCaru('#');
        pauza();
    };

    scena("Vydavas se na svou cestu... Temne lesy te cekaji.");

    navstivVesnici(h, "Kamenny Pristav");

    scena("Temny les - osamely nepritel!");
    if (!provedSouboj(h, {dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Rozcesti - stopy v blatе...");
    if (!provedSouboj(h, {dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Zriceniny - dva pary oci zari tmou!");
    if (!provedSouboj(h, {dalsiMonstrum(), dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Duneni prichazi z dolu... Hydra!");
    if (!provedSouboj(h, {vytvorMiniHydru()})) { cout << "\nGAME OVER\n"; return; }

    navstivVesnici(h, "Zlate Usvite");

    scena("Bazina - necekany utok!");
    if (!provedSouboj(h, {dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Opusteny tabor - partnersky utok!");
    if (!provedSouboj(h, {dalsiMonstrum(), dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Mostek pres rokli - prepadeni!");
    if (!provedSouboj(h, {dalsiMonstrum(), dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Nekromantova vez - Lich ceka!");
    if (!provedSouboj(h, {vytvorMiniLicha()})) { cout << "\nGAME OVER\n"; return; }

    scena("Vyprahla plan - osamely strazce.");
    if (!provedSouboj(h, {dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Krvave doly - dvojice demonu!");
    if (!provedSouboj(h, {dalsiMonstrum(), dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Prokleta kaple - stiny ozivaji!");
    if (!provedSouboj(h, {dalsiMonstrum(), dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    scena("Hradni nadvorí - trojice strazí!");
    if (!provedSouboj(h, {dalsiMonstrum(), dalsiMonstrum(), dalsiMonstrum()})) { cout << "\nGAME OVER\n"; return; }

    mudryStrom(h);

    vytiskniCaru('=');
    cout << "\n  *** BOSS: HEXARA ***\n";
    cout << "  Sestiruká carodejka chaosu se tyci pred tebou!\n";
    cout << "  Hexara: \"Sest rukou - sest osudu! Chaosu podlehej!\"\n";
    vytiskniCaru('=');
    pauza();
    Nepritel hexara = vytvorHexaru();
    if (!provedSouboj(h, {hexara})) { cout << "\nGAME OVER\n"; return; }

    vytiskniCaru('*');
    cout << "\n  *** VITEZSTVI! ***\n";
    cout << "  Hexara padla. Chaos byl porazen.\n";
    cout << "  Tvuj pribeh bude zpivan po generace.\n\n";
    cout << "  Zaverecne statistiky:\n";
    zobrazHrace(h);
    vytiskniCaru('*');
}

//-----------------------------------------------------main--------------------------------------------------------------------------
int main() {
    srand((unsigned)time(nullptr));
    vytiskniCaru('=');
    cout << "      HEXARA'S CURSE - Textove RPG\n";
    vytiskniCaru('=');
    cout << "\nVitej, hrdino. Temne sily se probouzeji...\n\n";
    Hrac hrac = zvolitTridu();
    cout << "\nVydavas se jako " << hrac.nazevTridy << " na svou cestu!\n";
    pauza();
    spustHru(hrac);
    return 0;
}