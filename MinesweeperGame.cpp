#include "MinesweeperGame.h"
#include <iostream>
#include <string>
#include <cctype>

// Konstruktor: skapar en Board på heapen via smart pekare
MinesweeperGame::MinesweeperGame(int rader, int kolumner, int antalMinor)
    : board_(std::make_unique<Board>(rader, kolumner, antalMinor))
{
}
void MinesweeperGame::körEnkeltTest() {
    std::cout << "Startlage (ingen markering):\n";
    board_->skrivUtFörSpelare();

    std::cout << "\nMarkerar rutan a1:\n";
    board_->växlaMarkering(0, 0); // a1 = rad 0, kolumn 0
    board_->skrivUtFörSpelare();

    std::cout << "\nAvmarkerar rutan a1 igen:\n";
    board_->växlaMarkering(0, 0); // växla tillbaka
    board_->skrivUtFörSpelare();
}

// Litet test: avslöjar alltid rutan a1
//void MinesweeperGame::körEnkeltTest() {
  //  std::cout << "Fore avslöjaRuta(a1):\n";
    //board_->skrivUtFörSpelare();

 //   bool mina = board_->avslöjaRuta(0, 0); // a1 = rad 0, kolumn 0

//    std::cout << "\nEfter avslöjaRuta(a1):\n";
 //   board_->skrivUtFörSpelare();

 //   if (mina) {
//       std::cout << "\nDet fanns en mina pa a1!\n";
 //   } else {
 //       std::cout << "\nIngen mina pa a1.\n";
 //   }
//}

// Riktiga spel-loopen med användarinput (t.ex. "b3")
void MinesweeperGame::kör() {
    std::cout << "Valkommen till textbaserat Minroj!\n";
    std::cout << "Skriv t.ex. b3 for att undersoka en ruta, eller q for att sluta.\n";


 
    bool gameOver = false;

    while (!gameOver) {
        std::cout << "\nAktuell spelplan:\n";
        board_->skrivUtFörSpelare();

        std::cout << "Vilken ruta vill du undersoka? (t.ex. b3): ";
        std::string input;

           // Markering? Format: f b3 eller f b4
    

        if (!(std::cin >> input)) {
            std::cout << "Kunde inte lasa input. Avslutar.\n";
            break;
        }


        if (input == "f") {
        // Läs nästa input, t.ex. "b3"
        std::string koordinat;
        std::cin >> koordinat;

        // 1) kontroll av format, t.ex. "b3"
    if (koordinat.size() < 2) {
        std::cout << "Ogiltigt format. Anvand t.ex. f b3.\n";
        continue;
    }

    // 2) Bokstav -> rad
    char rowChar = std::tolower(koordinat[0]);
    int rad = rowChar - 'a';

    // 3) Nummer -> kolumn
    int kolumn = 0;
    try {
        kolumn = std::stoi(koordinat.substr(1)) - 1;
    } catch (...) {
        std::cout << "Ogiltigt tal i koordinaten.\n";
        continue;
    }

    // 4) kontroll av gräns
    if (rad < 0 || rad >= board_->hämtaAntalRader() ||
        kolumn < 0 || kolumn >= board_->hämtaAntalKolumner()) {
        std::cout << "Koordinaten ligger utanfor spelplanen.\n";
        continue;
    }

    // 5) Markering/unmarkering
    bool ok = board_->växlaMarkering(rad, kolumn);
    
    if (!ok) {
        std::cout << "Kunde inte markera rutan (kanske avslöjad).\n";
    }

        // (Nästa steg: tolka koordinaten och kalla på växlaMarkering)
        continue;
        }

        // Avsluta spelet
        if (input == "q" || input == "Q" || input == "quit" || input == "QUIT") {
            std::cout << "Avslutar spelet...\n";
            break;
        }

        if (input.size() < 2) {
            std::cout << "Ogiltigt format. Anvand t.ex. b3.\n";
            continue;
        }

        // Forsta tecknet = rad (a, b, c, ...)
        char rowChar = static_cast<char>(std::tolower(static_cast<unsigned char>(input[0])));
        int rad = rowChar - 'a';

        // Resten = kolumn (1-baserad -> 0-baserad)
        int kolumn = 0;
        try {
            kolumn = std::stoi(input.substr(1)) - 1;
        } catch (...) {
            std::cout << "Ogiltigt tal efter bokstaven. Anvand t.ex. b3.\n";
            continue;
        }

        // Kontrollera gränser (bra felmeddelande till spelaren)
        if (rad < 0 || rad >= board_->hämtaAntalRader() ||
            kolumn < 0 || kolumn >= board_->hämtaAntalKolumner()) 
        {
            std::cout << "Koordinaten ligger utanfor spelplanen.\n";
            continue;
        }

        // Forsok avslöja rutan
        bool mina = board_->avslöjaRuta(rad, kolumn);

        if (mina) {
            std::cout << "\nPANG!! Game Over.\n";
            std::cout << "Ruta " << rowChar << (kolumn + 1) << " inneholl en mina.\n";
            board_->skrivUtFörSpelare();
            gameOver = true;
        } else {
            // Ingen mina -> kolla om spelaren har vunnit
            if (board_->ärVunnen()) {
                std::cout << "\nGrattis! Du vann spelet!\n";
                board_->skrivUtFörSpelare();
                gameOver = true;
            }
        }


    }
}
