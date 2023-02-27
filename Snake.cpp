#include "Snake.h"
#include <iostream>

Snake::Snake(int initialLength, int rowColumnSize){
    if (initialLength < 1){
        initialLength = 1;
    }
    
    this->initialLength = initialLength;
    if (rand() % 2 % 2 == 0){
        speed.x = rowColumnSize;
    }else{
        speed.y = rowColumnSize;
    }
     
    segmentTexture.loadFromFile("sprites/redblock.png");
    segmentSprite.setTexture(segmentTexture);
    segmentSprite.setScale(
        rowColumnSize / segmentSprite.getGlobalBounds().width, 
        rowColumnSize / segmentSprite.getGlobalBounds().height
    );

    for (size_t i = 0; i < initialLength; i++){
        growSnake();
    }
}   

void Snake::resetSnake(){
    for (size_t i = 1; i < body.size(); i++){
        if (body[0].x == body[i].x && body[0].y == body[i].y){
            body.resize(initialLength);
            break;
        }
    }
}

bool Snake::eat(const sf::Vector2f &food){
    if (body[0].x == food.x && body[0].y == food.y){
        growSnake();
        return true;
    }
    
    return false;
}

void Snake::growSnake(){ 
    if (body.size() == 0){
        body.push_back({0.f, 0.f});
    }else{
        body.push_back({body.back().x, body.back().y});
    }
}

void Snake::updateSnake(int horizontalBoundary, int verticalBoundary){  
    if (body.size() > 1){
        //Start from the back of the vector, and have the current index copy the element in front of it.
        for (int i = body.size() - 1; i > 0; i--){
            body[i] = body[i - 1];
        }
    }
    
    body[0] += speed;

    if (body[0].x < 0){
       body[0].x = horizontalBoundary;
    }else if(body[0].x > horizontalBoundary){
        body[0].x = 0;
    }else if(body[0].y < 0){
        body[0].y = verticalBoundary;
    }else if(body[0].y > verticalBoundary){
        body[0].y = 0;
    }
}

void Snake::controlSnake(int unitsToMove){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && currentDirection != Direction::LEFT){
        speed.x = unitsToMove, speed.y = 0;
        currentDirection = Direction::RIGHT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && currentDirection != Direction::RIGHT){
        speed.x = -unitsToMove, speed.y = 0;
        currentDirection = Direction::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && currentDirection != Direction::DOWN){
        speed.x = 0, speed.y = -unitsToMove;
        currentDirection = Direction::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && currentDirection != Direction::UP){
        speed.x = 0, speed.y = unitsToMove;
        currentDirection = Direction::DOWN;
    }
}

void Snake::drawSnake(sf::RenderWindow &window){
    for (const sf::Vector2f &segment : body){ 
        segmentSprite.setPosition(segment);
        window.draw(segmentSprite);
    }
}