#pragma once

#include <vector>
#include "Cell.h"

// Board ansvarar för hela spelplanen och minlogiken
class Board {
public:
    Board(int rader, int kolumner, int antalMinor);

    int hämtaAntalRader() const;
    int hämtaAntalKolumner() const;

     bool ärVunnen() const;  // true om alla säkra rutor är avslöjade

     bool avslöjaRuta(int rad, int kolumn);  // true om spelaren trampade på en mina

     bool växlaMarkering(int rad, int kolumn);  // Markerar/avmarkerar en ruta


    // Tillfällig funktion: skriver ut brädet (kommer att utvecklas)
    void skrivUtFörSpelare() const;
    
    // Flood-fill: används för att auto-avslöja tomma rutor och deras grannar
    void floodFill(int rad, int kolumn);

private:
    int rader_;
    int kolumner_;
    int antalMinor_;
    std::vector<std::vector<Cell>> rutor_;

    void placeraMinorSlumpmässigt();
    void beräknaAngränsandeMinor();
    bool ärInomGräns(int rad, int kolumn) const;


};
