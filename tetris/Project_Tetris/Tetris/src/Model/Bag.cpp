#include <vector>
#include "Bricks/Brick.h"
#include "Bricks/BrickC.h"
#include "Bricks/BrickJ.h"
#include "Bricks/BrickL.h"
#include "Bricks/BrickI.h"
#include "Bricks/BrickS.h"
#include "Bricks/BrickZ.h"
#include "Bricks/BrickT.h"
#include "Bricks/BrickCustom.h"
#include "Shapes.h"
#include "Bag.h"
#include <cstdlib>
#include <ctime>

// Constructor
Bag::Bag() {
    bricks.push_back(BrickC());
    bricks.push_back(BrickJ());
    bricks.push_back(BrickS());
    bricks.push_back(BrickI());
    bricks.push_back(BrickL());
    bricks.push_back(BrickZ());
    bricks.push_back(BrickT());

}

Brick Bag::getRandomBrick() {
    if (bricks.empty()) {
        refillBag();
    }
    srand(time(NULL));
    int index = rand() % bricks.size() + 0;
    Brick randomBlock = bricks[index];
    bricks.erase(bricks.begin() + index);
    return randomBlock;
}

void Bag::refillBag() {
    bricks.clear();
    bricks.push_back(BrickC());
    bricks.push_back(BrickJ());
    bricks.push_back(BrickS());
    bricks.push_back(BrickI());
    bricks.push_back(BrickL());
    bricks.push_back(BrickZ());
    bricks.push_back(BrickT());
}
void Bag::createCustomBlock(std::vector<std::vector<Shapes>> shape) {
    bricks.push_back(BrickCustom());
}

