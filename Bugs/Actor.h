#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"

class StudentWorld;

//Every Actor in BUGS GAME ********************************************************************************
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* swp, int imID, int stX, int stY, Direction dir, int depth);
    virtual ~Actor();
    StudentWorld* getArena();
    virtual void doSomething() = 0;
    bool isDead();
    void setDead();
    void set_moved();
    void set_unmoved();
    bool getMovedStatus();
private:
    StudentWorld* m_swp;
    bool m_dead;
    bool m_moved;
    
};
//POISON **************************************************************************************************
class Poison : public Actor
{
public:
    Poison(StudentWorld* swp, int stX, int stY);
    virtual ~Poison();
    virtual void doSomething();
private:
};
//POOL ****************************************************************************************************
class Pool : public Actor
{
public:
    Pool(StudentWorld* swp, int stX, int stY);
    virtual ~Pool();
    virtual void doSomething();
private:
};
//PEBBLE **************************************************************************************************
class Pebble : public Actor
{
public:
    Pebble(StudentWorld* swp, int stX, int stY);
    virtual ~Pebble();
    virtual void doSomething();
private:
};

//FOOD ****************************************************************************************************
class Food : public Actor
{
public:
    Food(StudentWorld*swp, int stX, int stY, int famt = 6000);
    virtual ~Food();
    virtual void doSomething();
    void addCarcassFood(int fmount);
    int getFoodLeft();
    void getRidFood(int num);
private:
    int m_foodLeft;
};

//ENERGYHOLDER ********************************************************************************************
class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* swp, int imID, int stX, int stY, Direction dir, int depth, int hp);
    virtual ~EnergyHolder();
    virtual void doSomething() = 0;
    virtual bool eat(int x, int y) = 0;
    int getHP();
    void addHP(int num);
    void deHP();
private:
    int m_hp;
};
//Pheromones **************************************************************************************************
class Pheromones : public EnergyHolder
{
public:
    Pheromones(StudentWorld* swp, int pherType, int stX, int stY);
    virtual ~Pheromones();
    virtual void doSomething();
    virtual bool eat(int x, int y);
    int getPherType();
    void addPheromones();
private:
    int m_pherType;
};

//INSECT **************************************************************************************************
class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* swp, int imID, int stX, int stY, Direction dir, int depth, int hp, int enemyNumber);
    virtual ~Insect();
    virtual void doSomething() = 0;
    virtual void move() = 0;
    //Insect Specialties
    virtual bool bite(int enemy);
    virtual bool eat(int x, int y) = 0;
    int getStunStatus();
    bool stunnedState();
    void deStunned();
    void Stunned();
    void resetStun();
    void statusStun();
    void deStatusStun();
    bool isStatusStunned();
    int getEnemyNumber();
private:
    int m_enemyNumber;
    bool m_stunnedByStatus;
    int m_stunCounter;
};
//ANTHILL **************************************************************************************************

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* swp, int colonyNumber, int stX, int stY, Compiler* instruction);
    ~AntHill();
    virtual void doSomething();
    virtual bool eat(int x, int y);
private:
    int m_colony;
    Compiler* m_compiler;
};

//ANT ******************************************************************************************************
class Ant : public Insect
{
public:
    Ant(StudentWorld*swp, int antType, int stX, int stY, Compiler* m_instructions, AntHill* home);
    virtual ~Ant();
    virtual void doSomething();
    virtual void move();
    int getFood();
    virtual bool eat(int x, int y);
    bool Interpreter();
    int getType();
    int generateRandomNumberUpTo(std::string operand);
    int convertToInteger(std::string operand);
    bool conditionTriggered(Compiler::Command cmd);
    void setBit();
    void deBit();
    bool pickUpFood();
    bool hasDangerInFront(int x, int y, int colony);
    bool hasPheromoneInFront(int x, int y);
private:
    Compiler* m_instructions;
    AntHill* m_home;
    bool m_wasBit;
    bool m_wasBlocked;
    int m_type;
    int m_foodAmount;
    int m_lastRandomNumber;
    int m_instructorCounter;
};

//GRASSHOPPER **********************************************************************************************
class GrassHopper : public Insect
{
public:
    GrassHopper(StudentWorld* swp, int imID, int stX, int stY, int hp, int enemyNumber);
    virtual ~GrassHopper();
    virtual void doSomething() = 0;
    virtual void move();
    virtual bool eat(int x, int y);
    int getCurrentDistance();
    void loseDistance();
    void resetDistance();
    void randomDistance();
private:
    int m_currentDistance;
};

//BABYGRASSHOPPER ******************************************************************************************
class BabyGrassHopper : public GrassHopper
{
public:
    BabyGrassHopper(StudentWorld* swp, int stX, int stY, int enemyNumber);
    virtual ~BabyGrassHopper();
    virtual void doSomething();
private:
};

//ADULTGRASSHOPPER *****************************************************************************************
class AdultGrassHopper : public GrassHopper
{
public:
    AdultGrassHopper(StudentWorld* swp, int stX, int stY, int enemyNumber);
    virtual ~AdultGrassHopper();
    virtual void doSomething();
    void jump();
private:
};
#endif // ACTOR_H_
