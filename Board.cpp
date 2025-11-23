#include "Board.h"
#include <iostream>
#include <random>

// Konstruktor
Board::Board(int rader, int kolumner, int antalMinor)
    : rader_(rader),
      kolumner_(kolumner),
      antalMinor_(antalMinor),
      rutor_(rader, std::vector<Cell>(kolumner))
{
    placeraMinorSlumpmässigt();
    beräknaAngränsandeMinor();
}

int Board::hämtaAntalRader() const {
    return rader_;
}

int Board::hämtaAntalKolumner() const {
    return kolumner_;
}

void Board::skrivUtFörSpelare() const {
    std::cout << "   ";
    for (int c = 0; c < kolumner_; ++c) {
        std::cout << " " << (c + 1) << " ";
    }
    std::cout << "\n";

    std::cout << "   +";
    for (int c = 0; c < kolumner_; ++c) {
        std::cout << "--+";
    }
    std::cout << "\n";

    for (int r = 0; r < rader_; ++r) {
        char radLabel = static_cast<char>('a' + r);
        std::cout << " " << radLabel << " |";

        for (int c = 0; c < kolumner_; ++c) {
            const Cell& cell = rutor_[r][c];

            char tecken = ' ';

            if (!cell.avslöjad) {
                if (cell.markerad) {
                    tecken = 'F';
                } else {
                    tecken = ' ';
                }
            } else {
                if (cell.harMina) {
                    tecken = '*';
                } else if (cell.angränsandeMinor > 0) {
                    tecken = static_cast<char>('0' + cell.angränsandeMinor);
                } else {
                    tecken = '.';
                }
            }

            std::cout << " " << tecken << "|";
        }

        std::cout << "\n   +";
        for (int c = 0; c < kolumner_; ++c) {
            std::cout << "--+";
        }
        std::cout << "\n";
    }
}

void Board::placeraMinorSlumpmässigt() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> radDist(0, rader_ - 1);
    std::uniform_int_distribution<> kolumnDist(0, kolumner_ - 1);

    int placerade = 0;
    while (placerade < antalMinor_) {
        int r = radDist(generator);
        int c = kolumnDist(generator);

        if (!rutor_[r][c].harMina) {
            rutor_[r][c].harMina = true;
            ++placerade;
        }
    }
}

void Board::beräknaAngränsandeMinor() {
    for (int r = 0; r < rader_; ++r) {
        for (int c = 0; c < kolumner_; ++c) {
            if (rutor_[r][c].harMina) {
                rutor_[r][c].angränsandeMinor = -1;
                continue;
            }

            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (ärInomGräns(nr, nc) && rutor_[nr][nc].harMina) {
                        ++count;
                    }
                }
            }
            rutor_[r][c].angränsandeMinor = count;
        }
    }
}

bool Board::ärVunnen() const {
    for (const auto& rad : rutor_) {
        for (const auto& cell : rad) {
            if (!cell.harMina && !cell.avslöjad) {
                // Minfri ruta som inte är avslöjad -> inte vinst än
                return false;
            }
        }
    }
    // Alla minfria rutor är avslöjade -> vinst
    return true;
}


bool Board::avslöjaRuta(int rad, int kolumn) {
    // Kolla först att koordinaten ligger inom spelplanen
    if (!ärInomGräns(rad, kolumn)) {
        return false; // Ogiltig ruta, men ingen mina
    }

    // Hämta referens till den aktuella rutan
    Cell& cell = rutor_[rad][kolumn];

    // Om rutan är markerad (F), ska den inte avslöjas
    if (cell.markerad) {
        return false;
    }

    // Om rutan redan är avslöjad, gör inget
    if (cell.avslöjad) {
        return false;
    }

    // Markera rutan som avslöjad
    cell.avslöjad = true;

    // Om rutan har en mina har spelaren förlorat
    if (cell.harMina) {
        return true;  // Trampade på en mina
    }

    // Om rutan har 0 minor runt sig, kör flood-fill
    if (cell.angränsandeMinor == 0) {
        floodFill(rad, kolumn);
    }



    // Ingen mina på den här rutan
    return false;
}


bool Board::växlaMarkering(int rad, int kolumn) {
    // Kolla först att koordinaten ligger inom spelplanen
    if (!ärInomGräns(rad, kolumn)) {
        return false; // Ogiltig ruta
    }

    Cell& cell = rutor_[rad][kolumn];

    // Om rutan redan är avslöjad ska vi inte kunna markera den
    if (cell.avslöjad) {
        return false;
    }

    // Växla markeringsläge: om den var markerad -> avmarkera, annars markera
    cell.markerad = !cell.markerad;

    return true;
}



bool Board::ärInomGräns(int rad, int kolumn) const {
    return rad >= 0 && rad < rader_ &&
           kolumn >= 0 && kolumn < kolumner_;
}


// Hjälpfunktion för flood-fill: öppnar tomma rutor och sprider sig till grannar
void Board::floodFill(int rad, int kolumn) {
    // Gå igenom alla 8 grannar runt rutan
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            // Hoppa över rutan själv
            if (dr == 0 && dc == 0) {
                 continue;
            }

            int nr = rad + dr;
            int nc = kolumn + dc;

            // Ligger grannen inom spelplanen?
            if (!ärInomGräns(nr, nc)) {
                continue;
            }

            Cell& granne = rutor_[nr][nc];

            // Öppna aldrig en mina automatiskt
            if (granne.harMina) {
                continue;
            }

            // Om rutan redan är avslöjad eller markerad, hoppa över
            if (granne.avslöjad || granne.markerad) {
                continue;
            }

            // Avslöja grannen
            granne.avslöjad = true;

            // Om grannen också har 0 angränsande minor, fortsätt flood-fill därifrån
            if (granne.angränsandeMinor == 0) {
                floodFill(nr, nc);
            }
        }
    }
}



