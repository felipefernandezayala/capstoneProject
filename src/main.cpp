#include <iostream>
#include <iostream>
#include <thread>
#include <queue>
#include <future>
#include <mutex>

#include "controller.h"
#include "game.h"
#include "renderer.h"

int main()
{
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    std::vector<std::shared_ptr<FieldObject>> myObjects;
    int nObjects=2;

    // create snake
    std::shared_ptr<Snake> mySnake(new Snake(kGridWidth, kGridHeight));

    // add independent object
    for (int newObj = 0; newObj < nObjects; newObj++)
    {
        myObjects.push_back(std::make_shared<FieldObject>(kGridWidth, kGridHeight));
        myObjects.at(newObj)->setSnake(mySnake);
    }
    
    // simulate objects
    std::for_each(myObjects.begin(), myObjects.end(), [](std::shared_ptr<FieldObject> &thisObject) {
        thisObject->simulate();
    });

     
       
    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    Controller controller;
    Game game(kGridWidth, kGridHeight);
    game.setFieldObjects(myObjects);
    game.setSnake(mySnake);
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.GetScore() << "\n";
    std::cout << "Size: " << game.GetSize() << "\n";
    

    return 0;
}

