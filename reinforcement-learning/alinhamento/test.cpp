#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "State.hpp"
#include "Agent.hpp"
#include "Global.hpp"

using namespace std;

int main(){
    State current_state = State('1', '2');
    Agent p1 = Agent(&current_state, '1', 0.2, 0.7, 0.3);
    Agent p2 = Agent(&current_state, '2', 0.2, 0.7, 0.3);
    Global setup = Global(&p1, &p2, &current_state);
    

    cout << "First player: " << setup.state->player_symbol << endl;
    cout << "Initial current board hash: " << "\"" << setup.p1->current_state->getCurrentHash() << "\"" << endl;
    cout << "Initial last board hash: " << "\"" << setup.p1->current_state->getLastHash() << "\"" << endl;
    cout << "Initial transition hash: " << "\"" << setup.p1->current_state->getTransitionHash() << "\"" << endl;
    setup.state->show_board();

    cin.get();

    cout << "Training..." << endl;
    setup.train(3);
    cout << "Finished!" << endl;

    return 0;
}