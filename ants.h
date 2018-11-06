//
//  ants.h
//  Ants
//
//  Created by Jingxian Shi on 12/5/17.
//  Copyright © 2017 Jingxian Shi. All rights reserved.
//

#ifndef ants_h
#define ants_h

#include <cstdlib>
#include <ctime>
#include <vector>
//#include <windows.h>
using namespace std;

void gotoxy(int x, int y);

class World;

enum Direction { UP, DOWN, LEFT, RIGHT};
enum Organism_type { EMPTY, ANTS, DOODLEBUGS};

struct Position
{
    Position(int a, int b) : x(a), y(b) {}
    int x;
    int y;
};

class Organism;
typedef Organism* Organism_ptr;

class Organism
{
public:
    Organism() :position(0, 0) {}
    Organism(int a, int b) : position(a, b) {}
    virtual ~Organism() {}
    Position get_position() { return position; }
    void set_position(Position pos) { position = pos; }
    void position_swap(Organism& other);
    virtual void move(World& w, vector<Organism_ptr>& a) = 0;
    virtual void breed(World& w, vector<Organism_ptr>& a) = 0;
    virtual void starve(World& w, vector<Organism_ptr>& d, int& index) = 0;
    virtual void draw() = 0;
    virtual Organism_type who() = 0;
    int breed_count;
    int starve_count;
private:
    Position position;
};

//-----------------------------------------------------------------------------------------------

class Empty : public Organism
{
public:
    Empty() {}
    Empty(int a, int b) : Organism(a, b) {}
    void draw();
    void move(World& w, vector<Organism_ptr>& a) {}
    void breed(World& w, vector<Organism_ptr>& a) {}
    void starve(World& w, vector<Organism_ptr>& d, int& index) {}
    Organism_type who() {return EMPTY;}
private:
};

//-----------------------------------------------------------------------------------------------

class Ants : public Organism
{
public:
    Ants() {}
    Ants(int a, int b) : Organism(a, b) {breed_count = 0;}
    void draw();
    void move(World& w, vector<Organism_ptr>& ants);
    void breed(World& w, vector<Organism_ptr>& ants);
    void starve(World& w, vector<Organism_ptr>& d, int& index) {}
    Organism_type who() {return ANTS;}
private:
};

//-----------------------------------------------------------------------------------------------

class Doodlebugs : public Organism
{
public:
    Doodlebugs() {}
    Doodlebugs(int a, int b) : Organism(a, b) {breed_count = 0;}
    void draw();
    void move(World& w, vector<Organism_ptr>& ants);
    void breed(World& w, vector<Organism_ptr>& doodlebugs);
    void starve(World& w, vector<Organism_ptr>& doodlebugs, int& index);
    Organism_type who() {return DOODLEBUGS;}
private:
};

//-----------------------------------------------------------------------------------------------

class World
{
public:
    World();
    void critter_swap(Organism_ptr& first, Organism_ptr& second);
    int get_height() { return height; }
    int get_width() { return width; }
    void print_world();
    void simulate_one_time_step(vector<Organism_ptr>& ants, vector<Organism_ptr>& doodlebugs);
private:
    int width;
    int height;
    void shuffle();
    void reset_world();
    void move_organism(Organism_type type);
    void organism_breed(Organism_type type);
public:
    Organism_ptr** grid;
};

//-----------------------------------------------------------------------------------------------

void Organism::position_swap(Organism& other)
{
    swap(position, other.position);
}

//-----------------------------------------------------------------------------------------------

void Empty::draw()
{
    cout << " ";
}

//-----------------------------------------------------------------------------------------------

void Ants::draw()
{
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|
    //                        FOREGROUND_INTENSITY);
    cout << "O";
}

void Ants::move(World& w, vector<Organism_ptr>& ants)
{
    
    Direction direction;
    int x1, y1, x2 = -1, y2 = -1;
    
    direction = static_cast<Direction>(rand() % 4);
    x1 = get_position().x;
    y1 = get_position().y;
    
    breed_count++;
    switch (direction)
    {
        case UP:
            if (y1 > 0)
            {
                if (w.grid[y1 - 1][x1]->who() == EMPTY)
                {
                    x2 = x1;
                    y2 = y1 - 1;
                }
            }
            break;
        case DOWN:
            if (y1 < w.get_height() - 1)
            {
                if (w.grid[y1 + 1][x1]->who() == EMPTY)
                {
                    x2 = x1;
                    y2 = y1 + 1;
                }
            }
            break;
        case LEFT:
            if (x1 > 0)
            {
                if (w.grid[y1][x1 - 1]->who() == EMPTY)
                {
                    x2 = x1 - 1;
                    y2 = y1;
                }
            }
            break;
        case RIGHT:
            if (x1 < w.get_width() - 1)
            {
                if (w.grid[y1][x1 + 1]->who() == EMPTY)
                {
                    x2 = x1 + 1;
                    y2 = y1;
                }
            }
            break;
    }
    
    if (x2 != -1)
    {
        w.critter_swap(w.grid[y1][x1], w.grid[y2][x2]);
    }
}

void Ants::breed(World& w, vector<Organism_ptr>& ants)
{
    int x1, y1, x2 = -1, y2 = -1;
    x1 = get_position().x;
    y1 = get_position().y;
    
    if(breed_count == 3)
    {
        if (y1 > 0 && w.grid[y1 - 1][x1]->who() == EMPTY) //UP
        {
            x2 = x1;
            y2 = y1 - 1;
        }
        else if(y1 < w.get_height() - 1 && w.grid[y1 + 1][x1]->who() == EMPTY) //DOWN
        {
            x2 = x1;
            y2 = y1 + 1;
        }
        else if(x1 > 0 && w.grid[y1][x1 - 1]->who() == EMPTY) //LEFT
        {
            x2 = x1 - 1;
            y2 = y1;
        }
        else if(x1 < w.get_width() - 1 && w.grid[y1][x1 + 1]->who() == EMPTY)
        {
            x2 = x1 + 1;
            y2 = y1;
        }
        
        if(x2 != -1)
        {
            delete w.grid[y2][x2];
            w.grid[y2][x2] = new Ants(x2, y2);
            if(ants.size() < w.get_width()*w.get_height())
            {
                ants.push_back(w.grid[y2][x2]);
            }
        }
        breed_count = 0;
    }
}


//-----------------------------------------------------------------------------------------------

void Doodlebugs::draw()
{
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED |
//                            FOREGROUND_INTENSITY);
    cout << "X";
}

void Doodlebugs::move(World& w, vector<Organism_ptr>& ants)
{
    Direction direction;
    int x1, y1, x2 = -1, y2 = -1;
    x1 = get_position().x;
    y1 = get_position().y;
    
    breed_count++;
    starve_count++;
    //if adjacent tile has an ant
    if (y1 > 0 && w.grid[y1 - 1][x1]->who() == ANTS) //UP
    {
        x2 = x1;
        y2 = y1 - 1;
        int index = 0;
        for(Organism_ptr i : ants)
        {
            if(i->get_position().x == x2 && i->get_position().y == y2)
            {
                break;
            }
            index++;
        }
        ants.erase(ants.begin() + index);
        delete w.grid[y2][x2];
        w.grid[y2][x2] = new Empty(x2, y2);
        starve_count = 0;
    }
    else if(y1 < w.get_height() - 1 && w.grid[y1 + 1][x1]->who() == ANTS) //DOWN
    {
        x2 = x1;
        y2 = y1 + 1;
        int index = 0;
        for(Organism_ptr i : ants)
        {
            if(i->get_position().x == x2 && i->get_position().y == y2)
            {
                break;
            }
            index++;
        }
        ants.erase(ants.begin() + index);
        delete w.grid[y2][x2];
        w.grid[y2][x2] = new Empty(x2, y2);
        starve_count = 0;
    }
    else if(x1 > 0 && w.grid[y1][x1 - 1]->who() == ANTS) //LEFT
    {
        x2 = x1 - 1;
        y2 = y1;
        int index = 0;
        for(Organism_ptr i : ants)
        {
            if(i->get_position().x == x2 && i->get_position().y == y2)
            {
                break;
            }
            index++;
        }
        ants.erase(ants.begin() + index);
        delete w.grid[y2][x2];
        w.grid[y2][x2] = new Empty(x2, y2);
        starve_count = 0;
    }
    else if(x1 < w.get_width() - 1 && w.grid[y1][x1 + 1]->who() == ANTS)
    {
        x2 = x1 + 1;
        y2 = y1;
        int index = 0;
        for(Organism_ptr i : ants)
        {
            if(i->get_position().x == x2 && i->get_position().y == y2)
            {
                break;
            }
            index++;
        }
        ants.erase(ants.begin() + index);
        delete w.grid[y2][x2];
        w.grid[y2][x2] = new Empty(x2, y2);
        starve_count = 0;
    }
    else
    {
        direction = static_cast<Direction>(rand() % 4);
        switch (direction)
        {
            case UP:
                if (y1 > 0)
                {
                    if (w.grid[y1 - 1][x1]->who() == EMPTY)
                    {
                        x2 = x1;
                        y2 = y1 - 1;
                    }
                }
                break;
            case DOWN:
                if (y1 < w.get_height() - 1)
                {
                    if (w.grid[y1 + 1][x1]->who() == EMPTY)
                    {
                        x2 = x1;
                        y2 = y1 + 1;
                    }
                }
                break;
            case LEFT:
                if (x1 > 0)
                {
                    if (w.grid[y1][x1 - 1]->who() == EMPTY)
                    {
                        x2 = x1 - 1;
                        y2 = y1;
                    }
                }
                break;
            case RIGHT:
                if (x1 < w.get_width() - 1)
                {
                    if (w.grid[y1][x1 + 1]->who() == EMPTY)
                    {
                        x2 = x1 + 1;
                        y2 = y1;
                    }
                }
                break;
        }
        
    }
    
    if (x2 != -1)
    {
        w.critter_swap(w.grid[y1][x1], w.grid[y2][x2]);
    }
    
}

void Doodlebugs::breed(World& w, vector<Organism_ptr>& doodlebugs)
{
    int x1, y1, x2 = -1, y2 = -1;
    x1 = get_position().x;
    y1 = get_position().y;
    
    if(breed_count == 8)
    {
        if (y1 > 0 && w.grid[y1 - 1][x1]->who() == EMPTY) //UP
        {
            x2 = x1;
            y2 = y1 - 1;
        }
        else if(y1 < w.get_height() - 1 && w.grid[y1 + 1][x1]->who() == EMPTY) //DOWN
        {
            x2 = x1;
            y2 = y1 + 1;
        }
        else if(x1 > 0 && w.grid[y1][x1 - 1]->who() == EMPTY) //LEFT
        {
            x2 = x1 - 1;
            y2 = y1;
        }
        else if(x1 < w.get_width() - 1 && w.grid[y1][x1 + 1]->who() == EMPTY)
        {
            x2 = x1 + 1;
            y2 = y1;
        }
        
        if(x2 != -1)
        {
            delete w.grid[y2][x2];
            w.grid[y2][x2] = new Doodlebugs(x2, y2);
            if(doodlebugs.size() < w.get_height()*w.get_width())
            {
                doodlebugs.push_back(w.grid[y2][x2]);
            }
        }
        breed_count = 0;
    }
}

void Doodlebugs::starve(World& w, vector<Organism_ptr>& doodlebugs, int& index)
{
//    int x = doodlebugs[index]->get_position().x;
//    int y = doodlebugs[index]->get_position().y;
//    if(w.grid[y][x]->starve_count >= 8)
//    {
//        delete w.grid[y][x];
//        w.grid[y][x] = new Empty(x, y);
//        doodlebugs.erase(doodlebugs.begin() + index);
//        index--;
//    }
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

World::World()
{
    int number_of_ants = 0;
    int number_of_doodlebugs = 1;
    
    width = 20;
    height = 20;
    grid = new Organism_ptr*[height];
    for (int i = 0; i < height; i++)
    {
        grid[i] = new Organism_ptr[width];
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (number_of_ants > 0)
            {
                grid[i][j] = new Ants(j, i);
                number_of_ants--;
            }
            else if (number_of_doodlebugs> 0)
            {
                grid[i][j] = new Doodlebugs(j, i);
                number_of_doodlebugs--;
            }
            else
            {
                grid[i][j] = new Empty(j, i);
            }
        }
    }
    shuffle();
}

void World::print_world()
{
    int num_ants = 0;
    int num_doodlebugs = 0;
    
    for(int i = 0; i < get_height(); i++)
    {
        for(int j = 0; j < get_width(); j++)
        {
            grid[i][j]->draw();
            if(grid[i][j]->who() == ANTS)
            {
                num_ants++;
            }
            else if(grid[i][j]->who() == DOODLEBUGS)
            {
                num_doodlebugs++;
                cout << "breed count: " << grid[i][j]->breed_count << endl;
            }
        }
        cout << endl;
    }
    cout << "Ants: " << num_ants << " Doodlebugs: " << num_doodlebugs << endl << endl;
    
}

void World::shuffle()
{
    for (int i = 0; i < 10000; i++)
    {
        int x1 = rand() % width;
        int y1 = rand() % height;
        int x2 = rand() % width;
        int y2 = rand() % height;
        
        critter_swap(grid[y1][x1], grid[y2][x2]);
    }
}

void World::simulate_one_time_step(vector<Organism_ptr>& ants, vector<Organism_ptr>& doodlebugs)
{
    for(int i = 0; i < doodlebugs.size(); i++)
    {
        doodlebugs[i]->move(*this, ants);
    }
    
    for(int i = 0; i < doodlebugs.size(); i++)
    {
        doodlebugs[i]->starve(*this, doodlebugs, i);
    }
    
    for(int i = 0; i < doodlebugs.size(); i++)
    {
        doodlebugs[i]->breed(*this, doodlebugs);
    }
    
    for(int i = 0; i < ants.size(); i++)
    {
        ants[i]->move(*this, ants);
    }
    
    for(int i = 0; i < ants.size(); i++)
    {
        ants[i]->breed(*this, ants);
    }
}

void World::critter_swap(Organism_ptr& first, Organism_ptr& second)
{
    first->position_swap(*second);
    swap(first, second);
}



#endif /* ants_h */

