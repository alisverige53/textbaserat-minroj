#include <memory>
#include "MinesweeperGame.h"

int main() {
    int rader = 6;
    int kolumner = 6;
    int antalMinor = 6;

    auto spel = std::make_unique<MinesweeperGame>(rader, kolumner, antalMinor);


    //spel->körEnkeltTest(); 

    // spel->körEnkeltTest();  // om du vill testa a1
     spel->kör();               // starta riktiga spelet

    return 0;
}
