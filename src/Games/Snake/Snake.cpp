//
// Created by super on 17.06.2024.
//

#include "Snake.h"
#include <ctime>
#include "debug.h"
#include "SystemAdapter.h"

float tickSpeed = .4; // s

void Snake::init() {
    glfwSetWindowTitle(getwindow(), "Snake");
    Reset();
    if (map->getField().x <= 0 || map->getField().y <= 0)
    {
        ErrorAbort("field.x <= 0 || field.y <= 0")
    }
    if (map->getField().y != map->getField().x)
    {
        ErrorAbort("field.y != field.x, which is unsupported")
    }
    if (map->getDefaultSnakeSize() < 2)
    {
        ErrorAbort("DefaultSnakeSize < 2")
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glLoadIdentity();
}
void Snake::Reset() {
    isCrashed = false;
    direction = Directions::Down;
    snake.clear();
    for (int i = 1; i < map->getDefaultSnakeSize() + 1; ++i) {
        snake.emplace_back(1, i, Directions::Down);
    }
    ResetApple();
    lastTickTime = 0;
}
void Snake::ResetApple(unsigned short counter) {
    counter--;
    if (counter <= 0)
    {
        Apple = Coords(0);
        Error("Cannot place Apple. The counter is over")
        return;
    }
    Apple = Coords(std::rand() % map->getField().x + 1, std::rand() % map->getField().y + 1);
    for (auto& s : snake) {
        if (Apple == s)
        {
            ResetApple(counter);
            return;
        }
    }
    for (int i = 0; i < map->getMap().size(); ++i) {
        if (map->getMap()[i] == SnakeMap::Wall && Apple == Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1)) {
            ResetApple(counter);
            return;
        }
    }
}


void Snake::MoveSnake(SnakeBody to) {
    auto tryTo = snake.front() + to;
    if (tryTo.x <= 0)
        tryTo.x = map->getField().x;
    else if (tryTo.y <= 0)
        tryTo.y = map->getField().y;
    else if (tryTo.y >= map->getField().y + 1)
        tryTo.y = 1;
    else if (tryTo.x >= map->getField().x + 1)
        tryTo.x = 1;
    auto crash = [=](){
        isCrashed = true;
        switch (boxer::show("The snake crashed into an obstacle", "The snake crashed into an obstacle", boxer::Buttons::OKCancel)) {
            case boxer::Selection::OK: {
                Reset();
                break;
            }
            case boxer::Selection::Cancel: {
                loadMainMenu();
                break;
            }
            default: {
                Warning("You forgot to add case, in crashed snake dialog");
                loadMainMenu();
                break;
            }
        }
        return;
    };
    for (int i = 0; i < map->getMap().size(); ++i) {
        if (map->getMap()[i] == SnakeMap::Wall) {
            if (CheckCollision(tryTo, Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1)))
            {
                crash();
                return;
            }
        }
    }
    for (auto& s : snake) {
        if (CheckCollision(tryTo, s))
        {
            crash();
            return;
        }
    }
    if (tryTo == Apple)
    {
        ResetApple();
        //Eat->Play();
    } else {
        snake.pop_back();
    }
    snake.emplace_front(tryTo.x, tryTo.y, to.direction);
}

void Snake::loop() {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    for (int x = 1; x < map->getField().x + 1; ++x) {
        for (int y = 1; y < map->getField().y + 1; ++y) {
            if (x%2==0)
            {
                if (y%2==0)
                {
                    glColor4d(0., .5, 0., 1.);
                } else {
                    glColor4d(0., 1., 0., 1.);
                }
            } else {
                if (y%2==0)
                {
                    glColor4d(0., 1., 0., 1.);
                } else {
                    glColor4d(0., .5, 0., 1.);
                } //TODO: БРЕД КАКОЙ ТО
            }
            renderTile(Coords(x,y), {});
        }
    }
    for (unsigned int i = 0; i < map->getMap().size(); ++i) {
        switch (map->getMap()[i]) {
            case SnakeMap::None:
                break;
            case SnakeMap::Wall: {
                glColor4d(0, 0, 1., 1.);
                glBindTexture(GL_TEXTURE_2D, 0);
                renderTile(Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1));
                break;
            }
            default:
                Warning("Cannot find tile type in render")
                break;
        }
    }
    if (HeadTexture->initImage == 0) { glColor4d(1.,0,0, 1.); } else {glColor4d(1.,1.,1., 1.);}
    if (isCrashed){glColor4d(1.,1.,0, 1.);}
    glBindTexture(GL_TEXTURE_2D, HeadTexture->initImage);
    switch (snake.front().direction)
    {
        case Up:
            renderTile(snake.front(), HeadTexture->texturecords);
            break;
        case Left:
            renderTile(snake.front(), {HeadTexture->texturecords[3], HeadTexture->texturecords[0], HeadTexture->texturecords[1], HeadTexture->texturecords[2]});
            break;
        case Down:
            renderTile(snake.front(), {HeadTexture->texturecords[2], HeadTexture->texturecords[3], HeadTexture->texturecords[0], HeadTexture->texturecords[1]});
            break;
        case Right:
            renderTile(snake.front(), {HeadTexture->texturecords[1], HeadTexture->texturecords[2], HeadTexture->texturecords[3], HeadTexture->texturecords[0]});
            break;
    }
    if (BodyTexture->initImage == 0)
    {
        glColor4d(1.,0,0, 1.);
    } else {glColor4d(1.,1.,1., 1.);}
    for (int i = 1; i < snake.size() - 1; ++i) {
        if (snake[i].direction != snake[i - 1].direction)
        {
            glBindTexture(GL_TEXTURE_2D, AngleTexture->initImage);
            if ((snake[i].direction == Directions::Left && snake[i - 1].direction == Directions::Up) || (snake[i - 1].direction == Directions::Right && snake[i].direction == Directions::Down))
            {
                renderTile(snake[i], AngleTexture->texturecords);
            } else if ((snake[i].direction == Directions::Right && snake[i - 1].direction == Directions::Up) || (snake[i - 1].direction == Directions::Left && snake[i].direction == Directions::Down))
            {
                renderTile(snake[i], {AngleTexture->texturecords[3], AngleTexture->texturecords[0], AngleTexture->texturecords[1], AngleTexture->texturecords[2]});
            } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Left) || (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Right))
            {
                renderTile(snake[i], {AngleTexture->texturecords[2], AngleTexture->texturecords[3], AngleTexture->texturecords[0], AngleTexture->texturecords[1]});
            } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Right) || (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Left))
            {
                renderTile(snake[i], {AngleTexture->texturecords[1], AngleTexture->texturecords[2], AngleTexture->texturecords[3], AngleTexture->texturecords[0]});
            }
            continue;
        }
        glBindTexture(GL_TEXTURE_2D, BodyTexture->initImage);
        if (snake[i].direction == Directions::Right || snake[i].direction == Directions::Left) {
            renderTile(snake[i], BodyTexture->texturecords);
        } else {
            renderTile(snake[i], {BodyTexture->texturecords[3], BodyTexture->texturecords[0], BodyTexture->texturecords[1], BodyTexture->texturecords[2]});
        }
    }
    if (TailTexture->initImage == 0) {glColor4d(1.,0,0, 1.);} else {glColor4d(1.,1.,1., 1.);}
    glBindTexture(GL_TEXTURE_2D, TailTexture->initImage);
    switch (snake[snake.size() - 2].direction)
    {
        case Up:
            renderTile(snake.back(), TailTexture->texturecords);
            break;
        case Left:
            renderTile(snake.back(), {TailTexture->texturecords[3], TailTexture->texturecords[0], TailTexture->texturecords[1], TailTexture->texturecords[2]});
            break;
        case Down:
            renderTile(snake.back(), {TailTexture->texturecords[2], TailTexture->texturecords[3], TailTexture->texturecords[0], TailTexture->texturecords[1]});
            break;
        case Right:
            renderTile(snake.back(), {TailTexture->texturecords[1], TailTexture->texturecords[2], TailTexture->texturecords[3], TailTexture->texturecords[0]});
            break;
    }
    glColor4d(1.,1.,1., 1.);
    if (AppleTexture->initImage == 0 ) {glColor4d(.3,0,0, 1.);} else {glColor4d(1.,1.,1., 1.);}
    glBindTexture(GL_TEXTURE_2D, AppleTexture->initImage);
    renderTile(Apple, AppleTexture->texturecords);

    // Swap the screen buffers
    glfwSwapBuffers(getwindow());
    if (lastTickTime + tickSpeed <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);

        switch(direction)
        {
            case Up:
                MoveSnake(SnakeBody(0,1, direction));
                break;
            case Left:
                MoveSnake(SnakeBody(-1,0, direction));
                break;
            case Down:
                MoveSnake(SnakeBody(0,-1, direction));
                break;
            case Right:
                MoveSnake(SnakeBody(1,0, direction));
                break;
            default:
                Warning("Unknown direction in SnakeTimer");
                MoveSnake(SnakeBody(0,1, direction));
                break;
        }
    }
}

void Snake::renderTile(Coords coords, std::array<glm::vec2, 4> v) {
    glBegin(GL_QUADS);
    glTexCoord2d(v[0].x, v[0].y);
    glVertex2d(coords.x + 1,coords.y);
    glTexCoord2d(v[1].x, v[1].y);
    glVertex2d(coords.x + 1,coords.y + 1);
    glTexCoord2d(v[2].x, v[2].y);
    glVertex2d(coords.x,coords.y + 1);
    glTexCoord2d(v[3].x, v[3].y);
    glVertex2d(coords.x,coords.y);
    glEnd();
}

void Snake::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
    {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                loadMainMenu();
                break;
            case GLFW_KEY_W:
                direction = Directions::Up;
                break;
            case GLFW_KEY_A:
                direction = Directions::Left;
                break;
            case GLFW_KEY_S:
                direction = Directions::Down;
                break;
            case GLFW_KEY_D:
                direction = Directions::Right;
                break;
            case GLFW_KEY_O: {
                auto path =SystemAdapter::OpenFileDialog({{"Snake map", "smap"}},
                                                         SystemAdapter::GetGameFolderName("Snake"));
                if (path.empty())
                {
                    return;
                }
                glfwSetWindowTitle(getwindow(), ("Snake. Opened map: ..." + path.substr(path.find_last_of('\\'),path.size() - path.find_last_of('\\'))).c_str());
                map = SnakeMap::load(path);
                if (map->getField().x <= 0 || map->getField().y <= 0)
                {
                    ErrorAbort("field.x <= 0 || field.y <= 0")
                }
                if (map->getField().y != map->getField().x)
                {
                    ErrorAbort("field.y != field.x, which is unsupported")
                }
                if (map->getDefaultSnakeSize() < 2)
                {
                    ErrorAbort("DefaultSnakeSize < 2")
                }
                Reset();
                size_callback(getWidth(), getHeight());
                break;
            }
            case GLFW_KEY_I:
                Info("This is my first game in c++; opengl. There are bugs in this game. If you find them, please write, admin@maserplay.ru .  By clicking on the \"O\", you can open the map file (.smap). Specification .smap: [field size defined by 1 number] [initial size of the snake] [a set of numbers from 0 to 1 that define each tile on the map.]")
                break;
        }
    }
}

void Snake::size_callback(int width, int height) {
    glViewport(0,0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    auto screensize = map->getField();
    double Shift = 0;
    if ((float) width >= (float) height) {
        Shift = (((double) (width) / (double) (height)) - (double) screensize.x / (double) screensize.y) - 1;
        glOrtho( -Shift, (double) screensize.x * ((double) width / (double) height) - Shift, 0. + 1, (double) screensize.y + 1, 0., 1.);
    }
    else {
        Shift = (((double) (height) / (double) (width)) - (double) screensize.y / (double) screensize.x) - 1;
        glOrtho(0. + 1, (double) screensize.x + 1, -Shift, (double) screensize.y * ((double) height / (double) width) - Shift, 0., 1.);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool Snake::CheckCollision(Coords c, Coords s) {
    return c == s;
}

Snake::~Snake() {
    delete AppleTexture;
    delete HeadTexture;
    delete BodyTexture;
    delete AngleTexture;
    delete TailTexture;
    delete map;
}
