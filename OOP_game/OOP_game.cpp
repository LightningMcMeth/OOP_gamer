#include <SFML/Graphics.hpp>
#include <vector>

class SnakeSegment {
public:

    SnakeSegment(int x, int y) : position(x, y) {}
    sf::Vector2i position;
};

class Snake {
public:

    enum Direction { UP, DOWN, LEFT, RIGHT };

    Snake(int l, int s, int x, int y) : length(l), speed(s) {

        for (int i = 0; i < l; i++)
        {
            segments.push_back(SnakeSegment(x - i, y));
        }
    }

    const std::vector<SnakeSegment>& getSegments() const {
        return segments;
    }

    void setDirection(Direction dir) {
        direction = dir;
    }

    Direction getDirection() const {
        return direction;
    }

    void move() {

        switch (direction) {

        case UP:
            moveBy(0, -speed);
            break;
        case DOWN:
            moveBy(0, speed);
            break;
        case LEFT:
            moveBy(-speed, 0);
            break;
        case RIGHT:
            moveBy(speed, 0);
            break;
        }
    }


private:
    std::vector<SnakeSegment> segments;
    int length;
    int speed;
    Direction direction = RIGHT;

    void moveBy(int dx, int dy) {

        for (int i = segments.size() - 1; i > 0; --i) {

            segments[i].position = segments[i - 1].position;
        }

        segments[0].position.x += dx;
        segments[0].position.y += dy;
    }
};



class SnakeGaming {
public:

    char (&getMatrix())[50][50]{
        return matrix;
    }

private:
    char matrix[50][50];
};



class Renderer {
public:

    Renderer(SnakeGaming& game) : game(game), snake(5, 1, 25, 25) {
        window.create(sf::VideoMode(500, 500), "Snake gaming");
    }

    void draw() {
        window.clear();

        char(&matrix)[50][50] = game.getMatrix();

        for (int i = 0; i < 50; ++i) {

            for (int j = 0; j < 50; ++j) {

                if (matrix[i][j] != 0) {

                    sf::RectangleShape rectangle(sf::Vector2f(10, 10));
                    rectangle.setPosition(j * 10, i * 10);

                    rectangle.setFillColor(sf::Color::Green);

                    window.draw(rectangle);
                }
            }
        }

        window.display();
    }

    void renderSnake() {

        auto segments = snake.getSegments();

        for (auto segment : segments) {

            sf::RectangleShape rect(sf::Vector2f(10, 10));

            rect.setOrigin(5, 5);
            rect.setPosition(segment.position.x * 10, segment.position.y * 10);
            rect.setFillColor(sf::Color::Green);

            window.draw(rect);

        }
    }

    void run(SnakeGaming& game) {
        
        sf::Clock clock;
        float timeSinceLastMove = 0.0f;
        float moveInterval = 1.0f / 10.0f;

        while (window.isOpen()) {

            sf::Event event;

            while (window.pollEvent(event)) {

                if (event.type == sf::Event::Closed) {

                    window.close();
                }
            }

            handleInput();

            float deltaTime = clock.restart().asSeconds();
            timeSinceLastMove += deltaTime;

            if (timeSinceLastMove >= moveInterval) {

                update();
                timeSinceLastMove = 0.0f;
            }

            window.clear();
            renderSnake();
            window.display();
        }
    }

private:
    sf::RenderWindow window;
    SnakeGaming& game;
    Snake snake;

    void handleInput() {

        Snake::Direction currentDirection = snake.getDirection();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && currentDirection != Snake::DOWN) {

            snake.setDirection(Snake::UP);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && currentDirection != Snake::UP) {

            snake.setDirection(Snake::DOWN);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && currentDirection != Snake::RIGHT) {

            snake.setDirection(Snake::LEFT);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && currentDirection != Snake::LEFT) {

            snake.setDirection(Snake::RIGHT);
        }
    }

    void update() {

        snake.move();
    }
};

int main() {
    SnakeGaming game;
    Renderer renderer(game);
    renderer.run(game);

    return 0;
}