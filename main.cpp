//
//  main.cpp
//  Ants
//
//  Created by Jingxian Shi on 12/4/17.
//  Copyright © 2017 Jingxian Shi. All rights reserved.
//

#include <iostream>
#include <vector>
#include "ants.h"
using namespace std;

int main(int argc, const char * argv[])
{
    srand(time(0));
    World w;
    vector<Organism_ptr> Ants;
    vector<Organism_ptr> Doodlebugs;
    char ch;
    
    for (int i = 0; i < w.get_height(); i++)
    {
        for (int j = 0; j < w.get_width(); j++)
        {
            if (w.grid[i][j]->who() == ANTS)
            {
                Ants.push_back(w.grid[i][j]);
            }
            if (w.grid[i][j]->who() == DOODLEBUGS)
            {
                Doodlebugs.push_back(w.grid[i][j]);
            }
        }
    }
    
    w.print_world();
    int turn = 1;
    cout << "turn: " << turn << endl;
    
    while(cin.get(ch))// && Ants.size() != 0 && Doodlebugs.size() != 0)
    {
        w.simulate_one_time_step(Ants, Doodlebugs);
        w.print_world();
        cout << "turn: " << turn << endl;
        turn++;
    }

    cout << " One or both species has been eliminated." << endl;
    
    return 0;
}
