#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "Compiler.h"
#include <cmath>

//ACTOR IMPLEMENTATION ***************************************************************
Actor::Actor(StudentWorld* swp, int imID, int stX, int stY, Direction dir, int depth)
:GraphObject(imID, stX, stY, dir, depth)
{
    m_swp = swp;
    m_dead = false;
    m_moved = false;
}

Actor::~Actor()
{
    //std::cerr <<"Deleting Actor at " << this << std::endl;
}

StudentWorld* Actor::getArena()
{
    return m_swp;
}

bool Actor::isDead()
{
    return m_dead;
}

void Actor::setDead()
{
    m_dead = true;
}

void Actor::set_moved()
{
    m_moved = true;
}
void Actor::set_unmoved()
{
    m_moved = false;
}
bool Actor::getMovedStatus()
{
    return m_moved;
}

//END ACTOR IMPLEMENTATION



//POISON IMPLEMENTATION ***************************************************************
Poison::Poison(StudentWorld* swp, int stX, int stY)
:Actor(swp, IID_POISON, stX, stY, right, 2)
{
    
}

Poison::~Poison()
{
    
}

void Poison::doSomething()
{
    this->getArena()->poisonInsects(this->getX(), this-> getY());
    return;
}
//END POISON IMPLENTATION *************************************************************

//POOL IMPLEMENTATION *****************************************************************
Pool::Pool(StudentWorld* swp, int stX, int stY)
:Actor(swp, IID_WATER_POOL, stX, stY, right, 2)
{
    
}

Pool::~Pool()
{
    
}

void Pool::doSomething()
{
    this->getArena()->stunInsects(this->getX(), this->getY());
    return;
}


//END POOL IMPLENTATION ***************************************************************

//PEBBLE IMPLEMENTATION ***************************************************************
Pebble::Pebble(StudentWorld* swp, int stX, int stY)
:Actor(swp, IID_ROCK, stX, stY, right, 1)
{
    
}

Pebble::~Pebble()
{
    //std::cerr <<"Deleting Pebble at " << this << std::endl;
}

void Pebble::doSomething()
{
    return;
}

//END PEBBLE IMPLEMENTATION

//FOOD IMPLEMENTATION *****************************************************************
Food::Food(StudentWorld*swp, int stX, int stY, int famt)
:Actor(swp, IID_FOOD, stX, stY, right, 2)
{
    m_foodLeft = famt;
}

Food::~Food()
{
    //std::cerr << "Deleting Food at " << this << std::endl;
}

void Food::doSomething()
{
    //std::cerr<<"Food " << m_foodLeft << std::endl;
    return;
}

int Food::getFoodLeft()
{
    return m_foodLeft;
}

void Food::getRidFood(int num)
{
    m_foodLeft = m_foodLeft - num;
    if(m_foodLeft <= 0)
    {
        setDead();
    }
}

void Food::addCarcassFood(int fmount)
{
    m_foodLeft = m_foodLeft + fmount;
}
//END FOOD IMPLEMENTATION *************************************************************

//ENERGYHOLDER IMPLEMENTATION *********************************************************
EnergyHolder::EnergyHolder(StudentWorld* swp, int imID, int stX, int stY, Direction dir, int depth, int hp)
:Actor(swp, imID, stX, stY, dir, depth)
{
    m_hp = hp;
}
EnergyHolder::~EnergyHolder()
{
    
}
void EnergyHolder::doSomething()
{
    return;
}

int EnergyHolder::getHP()
{
    return m_hp;
}

void EnergyHolder::deHP()
{
    m_hp--;
    if (getHP() <= 0)
        setDead();
}

void EnergyHolder::addHP(int num)
{
    m_hp = m_hp + num;
}
//END ENERGYHOLDER IMPLEMENTATION *****************************************************

//PHEROMONES IMPLEMENTATION **********************************
Pheromones::Pheromones(StudentWorld* swp, int pherType, int stX, int stY)
:EnergyHolder(swp, pherType, stX, stY, right, 2, 256)
{
    m_pherType = pherType - 11;
}
Pheromones::~Pheromones()
{
    
}
void Pheromones::doSomething()
{
    deHP();
    if(getHP() <= 0)
    {
        setDead();
        return;
    }
}
int Pheromones::getPherType()
{
    return m_pherType;
}

void Pheromones::addPheromones()
{
    int amount = 256;
    if((getHP()+ amount) > 768)
    {
        addHP(768 - getHP());
        return;
    }
    else
        addHP(256);
    return;
    
}

bool Pheromones::eat(int x, int y)
{
    return false;
}
//INSECT IMPLEMENTATION ***************************************************************
Insect::Insect(StudentWorld* swp, int imID, int stX, int stY, Direction dir, int depth, int hp, int enemyNumber)
:EnergyHolder(swp, imID, stX, stY, dir, depth, hp)
{
    m_stunCounter = 0;
    m_stunnedByStatus = false;
    m_enemyNumber = enemyNumber;
}

Insect::~Insect()
{
    //std::cerr <<"Deleting Insect at " << this << std::endl;
}

int Insect::getStunStatus()
{
    return m_stunCounter;
}

bool Insect::stunnedState()
{
    if(m_stunCounter > 0)
        return true;
    else
        return false;
}

void Insect::deStunned()
{
    m_stunCounter--;
}

void Insect::Stunned()
{
    m_stunCounter = m_stunCounter + 2;
}

void Insect::resetStun()
{
    m_stunCounter = 2;
}

void Insect::statusStun()
{
    m_stunnedByStatus = true;
}

void Insect::deStatusStun()
{
    m_stunnedByStatus = false;
}

bool Insect::isStatusStunned()
{
    return m_stunnedByStatus;
}

int Insect::getEnemyNumber()
{
    return m_enemyNumber;
}

bool Insect::bite(int enemy)
{
    if(this->getArena()->biteAtSpot(this->getX(), this->getY(), 50, this, enemy))
        return true;
    return false;
}
//END INSECT IMPLEMENTATION

//ANTHILL IMPLEMENTATION ***************************************************************

AntHill::AntHill(StudentWorld* swp, int stX, int stY, int colonyNumber, Compiler* comp)
:EnergyHolder(swp, IID_ANT_HILL, stX, stY, right, 2, 8999)
{
    m_colony = colonyNumber;
    m_compiler = comp;
}

AntHill::~AntHill()
{
    
}

void AntHill::doSomething()
{
    deHP(); //Decrese it's queen's hitpoints by 1 unit
    //std::cerr << "AntHill HP " << getHP() << std::endl;

    if(getHP() <= 0)
    {
        setDead();
        return;
        
    }
    if(eat(this->getX(), this->getY()))
    {
        return;
    }
    
    if(getHP()> 2000)
    {
        //std::cerr << "Reached before " << std::endl;
        this->getArena()->allocateInsects(this->getX(), this->getY(), m_colony, m_compiler, this);
        //std::cerr << "Reached After " << std::endl;
        for(int i = 0; i < 1500; i++)
            deHP();
     }
     
    return;
}

bool AntHill::eat(int x, int y)
{
    if(this->getArena()->hasFood(x, y) == false)
        return false;
    int addedhp = this->getArena()->eatFieldFood(x, y, 10000);
    addHP(addedhp);
    return true;
}


//ANT IMPLEMENTATION *******************************************************************
Ant::Ant(StudentWorld* swp, int antType, int stX, int stY, Compiler* compiler, AntHill* home)
:Insect(swp, antType, stX, stY, static_cast<Direction>(randInt(1, 4)), 1, 1500, antType)
{
    m_instructions = compiler;
    m_type = antType;
    m_foodAmount = 0;
    m_lastRandomNumber = 0;
    m_home = home;
    m_wasBit = false;
    m_wasBlocked = false;
    m_instructorCounter = 0;
}

Ant::~Ant()
{
    
}
void Ant::doSomething()
{
    deHP();
    //std::cerr << "Ant HP " << getHP() << std::endl;
    
    if(getHP() <= 0)
    {
        this->getArena()->addFood(this->getX(), this->getY(), 100);
        setDead();
        return;
    }
    if(stunnedState() == true)
    {
        deStunned();
        return;
    }
    if(Interpreter() == false)
    {
        this->getArena()->addFood(this->getX(), this->getY(), 100);
        setDead();
        return;
    }
    return;
}

int Ant::getType()
{
    return m_type;
}
bool Ant::Interpreter()
{
    Compiler c = *m_instructions;
    Compiler::Command cmd;
    for(int i = 0; i < 10 ; i++)
    {
        if (!c.getCommand(m_instructorCounter, cmd) )
            return false; // error - no such instruction!
        switch(cmd.opcode)
        {
            case Compiler::emitPheromone:
                this->getArena()->placePheromones(getX(), getY(),getType());
                m_instructorCounter++;
                //std::cerr <<"Placing Pheromones " << std::endl;
                return true;
                break;
            case Compiler::faceRandomDirection:
                this->setDirection(static_cast<Direction>(randInt(1, 4)));
                m_instructorCounter++;
                //std::cerr <<"Setting Direction" << std::endl;
                return true;
                break;
            case Compiler::moveForward:
                // cause the ant to move forward by // updating its x,y coordinates
                deStatusStun();
                move();
                m_instructorCounter++; // advance to next instruction break;
                
                //std::cerr <<"Moving Forward" << std::endl;
                return true;
                break;
            case Compiler::generateRandomNumber:
                m_lastRandomNumber = generateRandomNumberUpTo(cmd.operand1);
                m_instructorCounter++; // advance to next instruction break;
                //std::cerr <<"Generating Random Number" << std::endl;
                break;
            case Compiler::if_command:
                // if the condition of the if command is
                // is true, then go to the target position // in the vector; otherwise fall through to // the next position
                if (conditionTriggered(cmd))
                    m_instructorCounter = convertToInteger(cmd.operand2);
                
                else
                    m_instructorCounter++; // just advance to the next line break;
                break;
            case Compiler::goto_command:
                // just set ic the specified position // in operand1
                m_instructorCounter = convertToInteger(cmd.operand1);
                break;
            case Compiler::eatFood:
                if(m_foodAmount < 0)
                {
                    m_instructorCounter++;
                    break;
                }
                eat(this->getX(), this->getY());
                m_instructorCounter++;
                //std::cerr <<"Eating" << std::endl;
                return true;
                break;
            case Compiler::dropFood:
                if(m_foodAmount < 0)
                {
                    m_instructorCounter++;
                    break;
                }
                this->getArena()->addFood(this->getX(), this->getY(), this->getFood());
                m_foodAmount = 0;
                m_instructorCounter++;
                //std::cerr <<"Dropping Food" << std::endl;
                return true;
                break;
            case Compiler::pickupFood:
                if(pickUpFood())
                    return true;
                return false;
                break;
            case Compiler::bite:
                this->bite(m_type);
                m_instructorCounter++;
                //std::cerr <<"Biting Ant" << std::endl;
                return true;
                break;
            case Compiler::rotateClockwise:
                if(getDirection() == right)
                    setDirection(down);
                else if(getDirection() == down)
                    setDirection(left);
                else if(getDirection() == left)
                    setDirection(up);
                else if(getDirection() == up)
                    setDirection(right);
                m_instructorCounter++;
                //std::cerr <<"Rotating Clockwise" << std::endl;
                return true;
                break;
            case Compiler::rotateCounterClockwise:
                if(getDirection() == right)
                    setDirection(up);
                else if(getDirection() == down)
                    setDirection(right);
                else if(getDirection() == left)
                    setDirection(down);
                else if(getDirection() == up)
                    setDirection(left);
                m_instructorCounter++;
                //std::cerr <<"Rotating Counter Clockwise" << std::endl;
                return true;
                break;
        }
    }
    return true;
}
bool Ant::pickUpFood()
{
    if(this->getArena()->hasFood(getX(), getY()) == false)
    {
        m_instructorCounter++;
        return false;
    }
    int food = this->getArena()->eatFieldFood(this->getX(), this->getY(), 400);
    if((m_foodAmount + food) > 1800)
    {
        int leftOvers = (m_foodAmount + food - 1800);
        m_foodAmount = 1800;
        this->getArena()->addFood(getX(), getY(), leftOvers);
        m_instructorCounter++;
        //std::cerr <<"Picking Up Food" << std::endl;

        return true;
    }
    m_foodAmount = m_foodAmount + food;
    m_instructorCounter++;
    //std::cerr <<"Picking Up Food" << std::endl;

    return true;
}
bool Ant::eat(int x, int y)
{
    if(getFood() < 0)
        return false;
    if(getFood() > 100)
    {
        addHP(100);
        m_foodAmount = m_foodAmount - 100;
    }
    else
    {
        addHP(getFood());
        m_foodAmount = 0;
    }
    
    return true;
}
int Ant::generateRandomNumberUpTo(std::string operand)
{
    int max = convertToInteger(operand);
    return randInt(0, max);
}
int Ant::convertToInteger(std::string operand)
{
    return atoi(operand.c_str());
}
int Ant::getFood()
{
    return m_foodAmount;
}
bool Ant::conditionTriggered(Compiler::Command cmd)
{
    /*
    i_smell_danger_in_front_of_me,		// 0
    i_smell_pheromone_in_front_of_me,	// 1
    i_was_bit,							// 2
    i_am_carrying_food,					// 3
    i_am_hungry,						// 4
    i_am_standing_on_my_anthill,		// 5
    i_am_standing_on_food,				// 6
    i_am_standing_with_an_enemy,		// 7
    i_was_blocked_from_moving,			// 8
    last_random_number_was_zero			// 9
    */
    switch(convertToInteger(cmd.operand1))
    {
        case 0:
            if(hasDangerInFront(getX(), getY(), m_type))
                return true;
            else
                return false;
            break;
        case 1:
            if(hasPheromoneInFront(getX(), getY()))
                return true;
            else
                return false;
            break;
        case 2:
            if(m_wasBit == true)
                return true;
            else
                return false;
            break;
        case 3:
            if(getFood() > 0)
                return true;
            else
                return false;
            break;
        case 4:
            if(getHP() <= 25)
                return true;
            else
                return false;
            break;
        case 5:
            if(this->getX() == m_home->getX() && this->getY() == m_home->getY())
                return true;
            else
                return false;
            break;
        case 6:
            if(this->getArena()->hasFood(this->getX(), this->getY()))
                return true;
            else
                return false;
            break;
        case 7:
            if(this->getArena()->hasEnemyInsects(this->getX(), this->getY(), this->getEnemyNumber()))
                return true;
            else
                return false;
            break;
        case 8:
            if(m_wasBlocked == true)
                return true;
            else
                return false;
            break;
        case 9:
            if(m_lastRandomNumber == 0)
                return true;
            else
                return false;
            break;
    }
    return false;
}
void Ant::move()
{
    switch(this->getDirection())
    {
        case 0: //NONE
            return;
            break;
        case 1: //UP
            if(this->getArena()->hasPebble(this->getX(), this->getY() + 1) == false)
            {
                this->moveTo(this->getX(), this->getY() + 1);
                this->set_moved();
                m_wasBlocked = false;
                m_wasBit = false;
            }
            else
                m_wasBlocked = true;
            break;
        case 2: //Right
            if(this->getArena()->hasPebble(this->getX() + 1, this->getY()) == false)
            {
                this->moveTo(this->getX() + 1, this->getY());
                this->set_moved();
                m_wasBlocked = false;
                m_wasBit = false;
            }
            else
                m_wasBlocked = true;
            break;
        case 3: //Down
            if(this->getArena()->hasPebble(this->getX(), this->getY() - 1) == false)
            {
                this->moveTo(this->getX(), this->getY() - 1);
                this->set_moved();
                m_wasBlocked = false;
            }
            else
                m_wasBlocked = true;
            break;
            
        case 4: //Left
            if(this->getArena()->hasPebble(this->getX() - 1, this->getY()) == false)
            {
                this->moveTo(this->getX() - 1, this->getY());
                this->set_moved();
                m_wasBlocked = false;
            }
            else
                m_wasBlocked = true;
            break;
    }

}
void Ant::setBit()
{
    m_wasBit = true;
}

void Ant::deBit()
{
    m_wasBit = false;
}

bool Ant::hasDangerInFront(int x, int y, int colony)
{
    switch(this->getDirection())
    {
        case 0: //NONE
            return false;
            break;
        case 1: //UP
            if(this->getArena()->hasPebble(this->getX(), this->getY() + 1) == false)
            {
                if(this->getArena()->dangerInFront(this->getX(), this->getY() + 1, m_type))
                    return true;
                return false;
            }
            break;
        case 2: //Right
            if(this->getArena()->hasPebble(this->getX() + 1, this->getY()) == false)
            {
                if(this->getArena()->dangerInFront(this->getX() + 1, this->getY(), m_type))
                    return true;
                return false;
            }
            break;
        case 3: //Down
            if(this->getArena()->hasPebble(this->getX(), this->getY() - 1) == false)
            {
                if(this->getArena()->dangerInFront(this->getX(), this->getY() - 1, m_type))
                    return true;
                return false;
            }
            break;
        case 4: //Left
            if(this->getArena()->hasPebble(this->getX() - 1, this->getY()) == false)
            {
                if(this->getArena()->dangerInFront(this->getX() - 1, this->getY(), m_type))
                    return true;
                return false;

            }
            break;
    }
    return false;
}

bool Ant::hasPheromoneInFront(int x, int y)
{
    switch(this->getDirection())
    {
        case 0: //NONE
            return false;
            break;
        case 1: //UP
            if(this->getArena()->hasPebble(this->getX(), this->getY() + 1) == false)
            {
                if(this->getArena()->pheroInFront(this->getX(), this->getY() + 1))
                    return true;
                return false;
            }
            break;
        case 2: //Right
            if(this->getArena()->hasPebble(this->getX() + 1, this->getY()) == false)
            {
                if(this->getArena()->pheroInFront(this->getX() + 1, this->getY()))
                    return true;
                return false;
            }
            break;
        case 3: //Down
            if(this->getArena()->hasPebble(this->getX(), this->getY() - 1) == false)
            {
                if(this->getArena()->pheroInFront(this->getX(), this->getY() - 1))
                    return true;
                return false;
            }
            break;
        case 4: //Left
            if(this->getArena()->hasPebble(this->getX() - 1, this->getY()) == false)
            {
                if(this->getArena()->pheroInFront(this->getX() - 1, this->getY()))
                    return true;
                return false;
            }
            break;
    }
    return false;
}


//END ANT IMPLEMENTATION ***************************************************************

//GRASSHOPPER IMPLEMENTATION ***********************************************************
GrassHopper::GrassHopper(StudentWorld*swp, int imID, int stX, int stY, int hp, int enemyNumber)
:Insect(swp, imID, stX, stY, static_cast<Direction>(randInt(1, 4)), 0, hp, enemyNumber )
{
    m_currentDistance = randInt(2, 10);
}

GrassHopper::~GrassHopper()
{
    //std::cerr <<"Deleting GrassHopper at " << this << std::endl;
}

bool GrassHopper::eat(int x, int y)
{
    if(this->getArena()->hasFood(x, y) == false)
        return false;
    int addedhp = this->getArena()->eatFieldFood(x, y, 200);
    addHP(addedhp);
    return true;
}

int GrassHopper::getCurrentDistance()
{
    return m_currentDistance;
}

void GrassHopper::loseDistance()
{
    m_currentDistance--;
}

void GrassHopper::resetDistance()
{
    m_currentDistance = 1;
}

void GrassHopper::randomDistance()
{
    m_currentDistance = randInt(2, 10);
}

void GrassHopper::move()
{
    switch(this->getDirection())
    {
        case 0: //NONE
            return;
            break;
        case 1: //UP
            if(this->getArena()->hasPebble(this->getX(), this->getY() + 1) == false)
            {
                this->moveTo(this->getX(), this->getY() + 1);
                this->set_moved();
            }
            else
            {resetDistance();}
            break;
        case 2: //Right
            if(this->getArena()->hasPebble(this->getX() + 1, this->getY()) == false)
            {
                this->moveTo(this->getX() + 1, this->getY());
                this->set_moved();
            }
            else
            {resetDistance();}
            break;
        case 3: //Down
            if(this->getArena()->hasPebble(this->getX(), this->getY() - 1) == false)
            {
                this->moveTo(this->getX(), this->getY() - 1);
                this->set_moved();
            }
            else
            {resetDistance();}
            break;
        case 4: //Left
            if(this->getArena()->hasPebble(this->getX() - 1, this->getY()) == false)
            {
                this->moveTo(this->getX() - 1, this->getY());
                this->set_moved();
            }
            else
            {resetDistance();}
            break;
    }
    loseDistance();
    if(getCurrentDistance() == 0)
    {
        randomDistance();
        this->setDirection(static_cast<Direction>(randInt(1, 4)));
    }
    resetStun();
}

//END GRASSHOPPER IMPLEMENTATION

//BABYGRASSHOPPER IMPLEMENTATION ********************************************************
BabyGrassHopper::BabyGrassHopper(StudentWorld* swp, int stX, int stY, int enemyNumber)
:GrassHopper(swp, IID_BABY_GRASSHOPPER, stX, stY, 500, enemyNumber)
{
    
}

BabyGrassHopper::~BabyGrassHopper()
{
    //std::cerr << "Deleting new GrassHopper at " << this << std::endl;
}

void BabyGrassHopper::doSomething() //NOT FINISHED
{
    
    deHP();
    
    //std::cerr << this << ":" << getHP() << std::endl;
    if(getHP() <= 0)
    {
        this->getArena()->addFood(this->getX(), this->getY(), 100);
        setDead();
        return;
    }
    
    if(stunnedState() == true)
    {
        deStunned();
        return;
    }
    
    deStunned();
    if(getStunStatus() == -1)
        resetStun();
    if(getHP() >= 1600)
    {
        this->getArena()->createAdultGrassHopper(this->getX(), this->getY());
        this->getArena()->addFood(this->getX(), this->getY(), 100);
        setDead();
        return;
    }
    if(isStatusStunned() == false)
    {
        if(eat(this->getX(), this->getY()))
        {
            statusStun();
            if(randInt(0, 1) == 1)
            {
                resetStun();
                return;
            }
        }
    }
    if(this->isStatusStunned())
    {
        resetStun();
        deStunned();
        deStunned();
        this->deStatusStun();
    }

    this->move();
}

//END BABYGRASSHOPPER IMPLEMENTATION

//ADULTGRASSHOPPER IMPLEMENTATION ********************************************************
AdultGrassHopper::AdultGrassHopper(StudentWorld* swp, int stX, int stY, int enemyNumber)
:GrassHopper(swp, IID_ADULT_GRASSHOPPER, stX, stY, 1600, enemyNumber)
{
    
}

AdultGrassHopper::~AdultGrassHopper()
{
    //std::cerr << "Deleting new GrassHopper at " << this << std::endl;
}

void AdultGrassHopper::doSomething() //NOT FINISHED
{
    
    deHP();
    //std::cerr << getHP() << std::endl;
    if(getHP() < 0)
    {
        this->getArena()->addFood(this->getX(), this->getY(), 100);
        setDead();
        return;
    }
    
    if(stunnedState() == true)
    {
        deStunned();
        return;
    }

    deStunned();
    if(getStunStatus() == -1)
        resetStun();
    
    if(randInt(1, 3) == 1)
    {
        if(this->bite(this->getEnemyNumber()))
        {
            resetStun();
            return;
        }
    }
    if(this->isStatusStunned())
    {
        resetStun();
        deStunned();
        deStunned();
        this->deStatusStun();
    }
    if(randInt(1, 10) == 1)//10% Chance - Every Time
    {
        this->jump();
        resetStun();
        return;
    }
    
    
    if(eat(this->getX(), this->getY()))
    {
        if(randInt(0, 1) == 1)
        {
            resetStun();
            return;
        }
    }

    this->move();
}

void AdultGrassHopper::jump()
{
    for(int i = 0; i < 10; i++)
    {
        int spaces = randInt(1, 10);
        int degrees = randInt(0, 360);
        double radians = degrees * (M_PI/180);
        if((this->getY() + spaces*sin(radians) < VIEW_HEIGHT) && (this->getY() + spaces*sin(radians) >= 0) && (this->getX() + spaces*cos(radians) < VIEW_WIDTH) && (this->getX() + spaces*cos(radians) >= 0))
        {
            if(this->getArena()->hasPebble(this->getX() + spaces*cos(radians), this->getY() + spaces*sin(radians)) == false)
            {
                this->moveTo(this->getX() + spaces*cos(radians), this->getY() + spaces*sin(radians));
                if(degrees > 45 && degrees <= 135)
                    this->setDirection(up);
                else if(degrees > 135 && degrees <= 225)
                    this->setDirection(left);
                else if(degrees > 225 && degrees <=315)
                    this->setDirection(down);
                else
                    this->setDirection(right);
                return;
            }
        }
    }
}




//END ADULTGRASSHOPPER IMPLEMENTATION
