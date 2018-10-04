#include "StudentWorld.h"
#include "Compiler.h"
#include <sstream>
#include <iomanip>
using namespace std;

#include <vector>

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

//START IMPLEMENTATION*****************************************************************
StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_colonyCount = 0;
    m_winningAnt = -1;
    m_antsinW = 5;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{

    compilerForEntrant0 = nullptr;
    compilerForEntrant1 = nullptr;
    compilerForEntrant2 = nullptr;
    compilerForEntrant3 = nullptr;
    
    // get the names of all of the ant program source files
    // we provide the getFilenamesOfAntPrograms() function for
    // you in our GameWorld class.
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    int files = static_cast<int>(fileNames.size());
    //std::cerr << files << std::endl;
    m_colonyCount = files;
    std::string error;
    
        switch(files)
        {
            case 4:
                compilerForEntrant3 = new Compiler();
                // compile the source file... If the compile function returns // false, there was a syntax error during compilation!
                if ( ! compilerForEntrant3->compile(fileNames[3], error) )
                {
                    // entrant 0’s source code had a syntax error!
                    // send this error to our framework to warn the user. // do it JUST like this!
                    setError(fileNames[3] + " " + error);
                    // return an error to tell our simulation framework
                    // that something went wrong, and it’ll inform the user
                    return GWSTATUS_LEVEL_ERROR;
                }
                colonySize.push_back(0);
            case 3:
                compilerForEntrant2 = new Compiler;
                // compile the source file... If the compile function returns // false, there was a syntax error during compilation!
                if ( ! compilerForEntrant2->compile(fileNames[2], error) )
                {
                    // entrant 0’s source code had a syntax error!
                    // send this error to our framework to warn the user. //    do it JUST like this!
                    setError(fileNames[2] + " " + error);
                    // return an error to tell our simulation framework
                    // that something went wrong, and it’ll inform the user
                    return GWSTATUS_LEVEL_ERROR;
                }
                colonySize.push_back(0);
            case 2:
                compilerForEntrant1 = new Compiler;
                // compile the source file... If the compile function returns // false, there was a syntax error during compilation!
                if ( ! compilerForEntrant1->compile(fileNames[1], error) )
                {
                    // entrant 0’s source code had a syntax error!
                    // send this error to our framework to warn the user. // do it JUST like this!
                    setError(fileNames[1] + " " + error);
                    // return an error to tell our simulation framework
                    // that something went wrong, and it’ll inform the user
                    return GWSTATUS_LEVEL_ERROR;
                }
                colonySize.push_back(0);
            case 1:
                compilerForEntrant0 = new Compiler;
                // compile the source file... If the compile function returns // false, there was a syntax error during compilation!
                if ( ! compilerForEntrant0->compile(fileNames[0], error) )
                {
                    // entrant 0’s source code had a syntax error!
                    // send this error to our framework to warn the user. // do it JUST like this!
                    setError(fileNames[0] + " " + error);
                    // return an error to tell our simulation framework
                    // that something went wrong, and it’ll inform the user
                    return GWSTATUS_LEVEL_ERROR;
                }
                colonySize.push_back(0);
                break;
    }
    
    m_tickCount = 0;
    
    if(this->loadyField() == false) //Loads Field
        
        return GWSTATUS_LEVEL_ERROR;
    
    for(int x = 0; x < 64; x++) //Iterates through entire board
    {
        for(int y = 0; y < 64; y++)
        {
            if(m_field.getContentsOf(x,y) == Field::FieldItem::rock)
            {
                m_array[x][y].push_front(new Pebble(this,x,y));
            }
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::food)
            {
                m_array[x][y].push_front(new Food(this,x,y));
            }
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::water)
            {
                m_array[x][y].push_front(new Pool(this,x,y));
            }
            else if(m_field.getContentsOf(x, y) == Field::FieldItem::poison)
            {
                m_array[x][y].push_front(new Poison(this,x,y));
            }
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::grasshopper)
            {
                m_array[x][y].push_front(new BabyGrassHopper(this,x,y, 4));
                //std::cerr << "Created New GrassHopper at " << m_array[y][x].front() << std::endl;
            }
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::anthill0 && compilerForEntrant0 != nullptr)
            {
                compilerN.push_back(compilerForEntrant0->getColonyName());
                m_array[x][y].push_front(new AntHill(this, x, y, 0, compilerForEntrant0));
            }
            
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::anthill1 && compilerForEntrant1 != nullptr)
            {
                compilerN.push_back(compilerForEntrant1->getColonyName());
                m_array[x][y].push_front(new AntHill(this, x, y, 1, compilerForEntrant1));
            }
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::anthill2 && compilerForEntrant2 != nullptr)
            {
                compilerN.push_back(compilerForEntrant2->getColonyName());
                m_array[x][y].push_front(new AntHill(this, x, y, 2, compilerForEntrant2));
            }
            else if(m_field.getContentsOf(x,y) == Field::FieldItem::anthill3 && compilerForEntrant3 != nullptr)
            {
                compilerN.push_back(compilerForEntrant3->getColonyName());
                m_array[x][y].push_front(new AntHill(this, x, y, 3, compilerForEntrant3));
            }
            
            //else
            //{
                //m_array[y][x].push_front(nullptr);
            //}
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    updateTickCount(); // Update the current tick # in the simulation
    
    // The term "actors" refers to all ants, anthills, poison, pebbles,
    // baby and adult grasshoppers, food, pools of water, etc.
    
    for(int x = 0; x < 64; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            if(m_array[x][y].empty() == false)
            {
                for(std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
                {
                    if((*p)->getMovedStatus() == false)
                    {
                        // Get the actor’s current location
                        int oldX = (*p)->getX();
                        int oldY = (*p)->getY();
                        
                        (*p)->doSomething(); //Make every actor do something
                        //std::cerr << "REACHED" << std::endl;
                        
                        if((*p)->isDead()) //Check death condition
                        {
                            std::list<Actor*>::iterator temp = p;
                            delete *p;
                            p= m_array[x][y].erase(temp);
                            p--;
                            //std::cerr << "REACHED1" << std::endl;
                            continue;
                        }
                        
                            if(oldX != (*p)->getX() || oldY != (*p)->getY()) //Update Data Structure
                            {
                                //std::cerr << "REACHED1" << std::endl;
                                Actor* ap = *p;
                                //std::cerr << "REACHED2" << std::endl;
                                m_array[ap->getX()][ap->getY()].push_back(ap);
                                //std::cerr << "REACHED3" << std::endl;
                                p = m_array[oldX][oldY].erase(p);
                                //std::cerr << "REACHED4" << std::endl;
                                p--;
                                //std::cerr << "REACHED5" << std::endl;
                            }
                    }
                    
                }
            }
        }
    }
    // Remove newly-dead actors after each tick
    
    
        for(int x = 0; x < 64; x++)
        {
            for(int y = 0; y < 64; y++)
            {
                if(m_array[x][y].empty() == false)
                {
                    for(std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
                    {
                        if(*p != nullptr)
                        {
                            (*p)->set_unmoved();
                        }
                    }
                }
            }
        }
    
       // Update the simulation Status Line
    updateDisplayText(); // Update the ticks/ant stats text at screen top
    // If the simulation’s over (ticks == 2000) then see if we have a winner
    if (theSimulationIsOver()) {
        if (weHaveAWinningAnt()) {
            setWinner(getWinningAntsName());
            return GWSTATUS_PLAYER_WON; }
        else
            return GWSTATUS_NO_WINNER;
    }
    // The simulation is not yet over, continue!
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
    for(int x = 0; x < 64; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            if(m_array[x][y].empty() == false)
            {
                for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
                {
                    if(*p!= nullptr)
                    {
                        std::list<Actor*>::iterator temp = p;
                        delete *p;
                        *p = nullptr;
                        p= m_array[x][y].erase(temp);
                        p--;//
                    }
                }
            }
        }
    }
    if(compilerForEntrant0!=nullptr)
    {
        delete compilerForEntrant0;
        compilerForEntrant0 = nullptr;
    }
    if(compilerForEntrant1!=nullptr)
    {
        delete compilerForEntrant1;
        compilerForEntrant1 = nullptr;
    }
    if(compilerForEntrant2!=nullptr)
    {
        delete compilerForEntrant2;
        compilerForEntrant2 = nullptr;
    }
    if(compilerForEntrant3!=nullptr)
    {
        delete compilerForEntrant3;
        compilerForEntrant3 = nullptr;
    }

}

//STUDENTWORLD HELPER FUNCTIONS *********************************************************

void StudentWorld::allocateInsects(int x, int y, int colony, Compiler* comp, AntHill* home)
{
    switch(colony)
    {
        case 0:
            //std::cerr << "MAking Ant" << std::endl;
            m_array[x][y].push_back(new Ant(this, IID_ANT_TYPE0,x, y, comp, home));
            increaseColonyNumber(colony);
            //std::cerr << "Making Ants" << std::endl;
            //std::cerr<<"New Ant created at " << this << std::endl;
            break;
        case 1:
            m_array[x][y].push_back(new Ant(this, IID_ANT_TYPE1,x, y,  comp, home));
            increaseColonyNumber(colony);
            //std::cerr<<"New Ant created at " << this << std::endl;
            break;
        case 2:
            m_array[x][y].push_back(new Ant(this, IID_ANT_TYPE2,x, y,  comp, home));
            increaseColonyNumber(colony);
            //std::cerr<<"New Ant created at " << this << std::endl;
            break;
        case 3:
            m_array[x][y].push_back(new Ant(this, IID_ANT_TYPE3, x, y,  comp, home));
            increaseColonyNumber(colony);
            //std::cerr<<"New Ant created at " << this << std::endl;
            break;
    }
    if(colonySize[colony] > m_antsinW)
    {
        m_antsinW = colonySize[colony];
        setWinningColony(colony);
    }
        
    
}

void StudentWorld::increaseColonyNumber(int colonyN)
{
    switch(colonyN)
    {
        case 0:
            colonySize[0] = colonySize[0] + 1;
            break;
        case 1:
            colonySize[1] = colonySize[1] + 1;
            break;
        case 2:
            colonySize[2] = colonySize[2] + 1;
            break;
        case 3:
            colonySize[3] = colonySize[3] + 1;
            break;
    }
}

std::string StudentWorld::getWinningAntsName()
{
    int i = winningAntColonyNumber();
    return compilerN[i];
}

int StudentWorld::getTicks()
{
    return m_tickCount;
}

bool StudentWorld::loadyField()
{
    Field f;
    std::string fieldFile = getFieldFilename();
    std::string error;
    if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) {
        setError(fieldFile + " " + error);
        //std::cerr << error << std::endl;
        return false;
    }
    m_field = f;
    return true;
}

bool StudentWorld::hasPebble(int x, int y)
{
    if(m_field.getContentsOf(x,y) == Field::FieldItem::rock)
        return true;
    else
        return false;
}

bool StudentWorld::hasFood(int x, int y)
{
    
    for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
    {
        Actor* ap = *p;
        if (ap != nullptr)
        {
            Food* pp = dynamic_cast<Food*>(ap);
            if (pp != nullptr)
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::hasEnemyInsects (int x, int y, int enemyNumber)
{
    if(m_array[x][y].empty() == true)
        return false;
    
    for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
    {
        Actor* ap = *p;
        if (ap != nullptr)
        {
            Insect* ag = dynamic_cast<Insect*>(ap);
            if (ag != nullptr)
            {
                if(ag->getEnemyNumber() != enemyNumber || enemyNumber == 4)
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::theSimulationIsOver()
{
    if(m_tickCount == 2000)
        return true;
    else
        return false;
}

bool StudentWorld::weHaveAWinningAnt()
{
    for(int i = 0; i < colonySize.size(); i++)
    {    if(colonySize[i] >= 6)
            return true;
    }
    return false;
}

int StudentWorld::eatFieldFood(int x, int y, int num)
{
    for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
    {
        Actor* ap = *p;
        if (ap != nullptr)
        {
            Food* pp = dynamic_cast<Food*>(ap);
            if (pp != nullptr)
            {
                if(pp->getFoodLeft() < num)
                {
                    int temp = pp->getFoodLeft();
                    pp->getRidFood(pp->getFoodLeft());
                    return temp;
                }
                else
                {
                    pp->getRidFood(num);
                    return num;
                }
            }
        }
    }
    return 0;
}
void StudentWorld::poisonInsects(int x, int y)
{
    if(hasEnemyInsects(x, y, 10))
    {
        for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
        {
            Actor* ap = *p;
            if (ap != nullptr)
            {
                AdultGrassHopper* agh = dynamic_cast<AdultGrassHopper*>(ap);
                if(agh != nullptr)
                {
                    return;
                }
                Insect* ag = dynamic_cast<Insect*>(ap);
                if (ag != nullptr)
                {
                    for(int i = 0; i < 150; i++)
                    {
                        ag->deHP();
                    }
                    return;
                }
            }
        }
    }
    return;

}

void StudentWorld::stunInsects(int x, int y)
{
    if(hasEnemyInsects(x, y, 10))
    {
        for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
        {
            Actor* ap = *p;
            if (ap != nullptr)
            {
                AdultGrassHopper* agh = dynamic_cast<AdultGrassHopper*>(ap);
                if(agh != nullptr)
                {
                    return;
                }
                Insect* ag = dynamic_cast<Insect*>(ap);
                if (ag != nullptr && ag->isStatusStunned() == false)
                {
                    
                    ag->resetStun();
                    ag->statusStun();
                    return;
                }
            }
        }
    }
    return;
}

void StudentWorld::createAdultGrassHopper(int x, int y)
{
    m_array[x][y].push_back(new AdultGrassHopper(this, x, y, 4));
}

void StudentWorld::addFood(int x, int y, int fmount)
{
    
    if(hasFood(x, y) == false)
    {
        Actor* ap = m_array[x][y].front();
        if (ap != nullptr)
        {
            Food* pp = dynamic_cast<Food*>(ap);
            if (pp != nullptr)
            {
                pp->addCarcassFood(fmount);
                return;
            }
        }
    }
    m_array[x][y].push_front(new Food(this, x, y, fmount));
}

void StudentWorld::placePheromones(int x, int y, int colony)
{
    if(m_array[x][y].empty() == false)
    {
        Actor* ap = m_array[x][y].front();
        if (ap != nullptr)
        {
            Pheromones* pp = dynamic_cast<Pheromones*>(ap);
            if (pp != nullptr)
            {
                if(pp->getPherType() != colony)
                    pp->addPheromones();
                return;
            }
        }
    }
    m_array[x][y].push_back(new Pheromones(this, (colony + 11), x, y));
}

Insect* StudentWorld::pickRandomActor(int x, int y, Insect* other, int enemyNumber)
{
    std::vector<Insect*> enemies;
    for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
    {
        Actor* ap = *p;
        if (ap != nullptr)
        {
            Insect* ag = dynamic_cast<Insect*>(ap);
            if (ag != nullptr && ag !=other)
            {
                if(ag->getEnemyNumber() != enemyNumber || enemyNumber == 4)
                    enemies.push_back(ag);
            }
        }
    }
    int ins = randInt(0, static_cast<int>(enemies.size()-1));
    if(enemies.empty() == true)
        return other;
    return enemies[ins];
}

bool StudentWorld::biteAtSpot(int x, int y, int dam, Insect* other, int enemyNumber)
{
    if(hasEnemyInsects(x, y, enemyNumber))
    {
        Insect* ip = dynamic_cast<Insect*>(pickRandomActor(x, y, other, enemyNumber));
        if(ip != other)
        {
            for(int i = 0; i < dam; i++)
                ip->deHP();
            if(ip->getHP() < 0)
            {
                ip->setDead();
            }
            //BITE BACK
            if(ip->isDead() == true)
                return false;
            Ant* ant = dynamic_cast<Ant*>(ip);
            if(ant != nullptr)
            {
                ant->setBit();
                return true;
            }
            if(randInt(1,2) == 1) //50% chance it bites back
            {
                AdultGrassHopper* ap = dynamic_cast<AdultGrassHopper*>(ip);
                if(ap != nullptr)
                {
                    ap->getArena()->biteAtSpot(ap->getX(), ap->getY(), 50, ap, ap->getEnemyNumber());
                    return true;
                }
            }
            return true;
        }
    }
    return false;
}

void StudentWorld::updateTickCount()
{
    m_tickCount++;
}

int StudentWorld::winningAntColonyNumber()
{
    int max = m_winningAnt;
    for(int i = 0; i < colonySize.size(); i++)
    {
        if(colonySize[i] > colonySize[max])
        {
            max = i;
        }
    }
    return max;
}


void StudentWorld::updateDisplayText()
{
    std::ostringstream oss;
    oss << "Ticks:" << std::setw(5) << (2000 - m_tickCount);
    if(m_colonyCount > 0)
    {
        oss << " - ";
        for(int i = 0; i < m_colonyCount; i++)
        {
            if(i != 0)
                oss << "  ";
            oss << compilerN[i];
            if(i == m_winningAnt)
                oss << "*";
            oss << ": ";
            oss << std::setw(2) << std::setfill('0') << colonySize[i];
            oss << " ants";
        }
    }
    setGameStatText(oss.str());
    return;
}


bool StudentWorld::dangerInFront(int x, int y, int colony)
{
    if(hasEnemyInsects(x, y, colony))
       return true;
    if(m_field.getContentsOf(x,y) == Field::FieldItem::poison || m_field.getContentsOf(x,y) == Field::FieldItem::water)
        return true;
    return false;
}

bool StudentWorld::pheroInFront(int x, int y)
{
    if(m_array[x][y].empty() == true)
        return false;
    
    for (std::list<Actor*>::iterator p = m_array[x][y].begin(); p != m_array[x][y].end();p++)
    {
        Actor* ap = *p;
        if (ap != nullptr)
        {
            Pheromones* ag = dynamic_cast<Pheromones*>(ap);
            if (ag != nullptr)
            {
                return true;
            }
        }
    }
    return false;
}


void StudentWorld::setWinningColony(int w)
{
    if(colonySize[w] <= 5)
    {
        return;
    }
    m_winningAnt = w;
}

int StudentWorld::getWinningColony()
{
    return m_winningAnt;
}

bool StudentWorld::compareToWinner(int colony)
{
    if(colonySize[colony] > colonySize[m_winningAnt])
        return true;
    return false;
}
