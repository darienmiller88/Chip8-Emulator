#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Snake{
    enum class Direction{
        UP, DOWN, LEFT, RIGHT
    };

    public:
        Snake(int initialLength, int unitLength);
        void updateSnake(int horizontalBoundary, int verticalBoundary);
        void drawSnake(sf::RenderWindow &Window);
        void controlSnake(int unitsToMove);
        bool eat(const sf::Vector2f &food);
        void resetSnake();
    private:
        void growSnake();
    private:
        std::vector<sf::Vector2f> body;
        sf::Texture segmentTexture;
        sf::Sprite segmentSprite;
        sf::Vector2f speed;
        int initialLength;
        Direction currentDirection;
};