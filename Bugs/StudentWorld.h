#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "Field.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <list>
#include <string>

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    //Helper Functions
    std::string getWinningAntsName();
    Insect* pickRandomActor(int x, int y, Insect* other, int enemyNumber);
    int getTicks();
    int eatFieldFood(int x, int y, int num);
    bool loadyField();
    void dropFood(int x, int y, int fmount);
    bool hasPebble(int x, int y);
    bool hasFood(int x, int y);
    bool hasEnemyInsects(int x, int y, int enemyNumber);
    bool theSimulationIsOver();
    bool dangerInFront(int x, int y, int colony);
    bool pheroInFront(int x, int y);
    bool weHaveAWinningAnt();
    void allocateInsects(int x, int y, int colony, Compiler* comp, AntHill* home);
    void stunInsects(int x, int y);
    void poisonInsects(int x, int y);
    void updateTickCount();
    void createAdultGrassHopper(int x, int y);
    void addFood(int x, int y, int fmount);
    void increaseColonyNumber(int colonyN);
    bool biteAtSpot(int x, int y, int dam, Insect* other, int enemyNumber);
    void updateDisplayText();
    void pickUpFood(Ant* ant, int x, int y);
    void placePheromones(int x, int y, int colony);
    int winningAntColonyNumber();
    void setWinningColony(int w);
    int getWinningColony();
    bool compareToWinner(int colony);
private:
    std::list<Actor*> m_array[64][64];
    Field m_field;
    int m_tickCount;
    std::vector<int> colonySize;
    int m_colonyCount;
    std::vector<std::string> compilerN;
    Compiler *compilerForEntrant0, *compilerForEntrant1,
    *compilerForEntrant2, *compilerForEntrant3;
    int m_winningAnt;
    int m_antsinW;
};

#endif // STUDENTWORLD_H_
