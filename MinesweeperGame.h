#pragma once

#include <memory>
#include "Board.h"

// MinesweeperGame hanterar spelets övergripande logik
class MinesweeperGame {
public:
    MinesweeperGame(int rader, int kolumner, int antalMinor);

    void körEnkeltTest();  // Litet test för a1
    void kör();            // Riktiga spel-loopen med användarinput

private:
    std::unique_ptr<Board> board_;
};
