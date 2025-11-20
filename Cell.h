#pragma once

// Cell representerar en ruta på spelplanen
struct Cell {
    bool harMina = false;        // Innehåller rutan en mina?
    bool avslöjad = false;       // Har rutan avslöjats?
    bool markerad = false;       // Är rutan markerad som potentiell mina?
    int angränsandeMinor = 0;    // Antal minor runt omkring rutan
};
