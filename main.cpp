#include "Player.h"
#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){
    srand(time(NULL));

    Maze m;
    m.NewGame(new Player("Jenn", true), 2);
    cout << m << endl << endl;
    do {
        Player *p = m.GetNextPlayer();
        m.TakeTurn(p);
        cout << m;
    } while (!m.IsGameOver());
    cout << m.GenerateReport();
}