#include <raylib.h>

//--------------------------------------------------------------------------------------
// Initialization
//--------------------------------------------------------------------------------------
const int CELL_SIZE = 20;
const int LINES = 25;
const int COLUMNS = 25;
const int SCREEN_WIDTH = COLUMNS * CELL_SIZE;
const int SCREEN_HEIGHT = LINES * CELL_SIZE;

int gameSpeed;

Vector2 apple;
Vector2 snake[LINES * COLUMNS];

int snakeLength;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
Direction currentDirection;

void newApple() {
    apple = {(float)GetRandomValue(0, COLUMNS - 1), (float)GetRandomValue(0, LINES - 1)};
}

void setupGame() {
    gameSpeed = 8;

    SetTargetFPS(gameSpeed);

    currentDirection = RIGHT;

    snakeLength = 3;

    snake[0] = {COLUMNS / 2, LINES / 2};

    newApple();

    for (int i = 1; i < snakeLength; i++)
        snake[i] = {snake[0].x - i, snake[0].y};
}

void getInput() {
    // Se usar multiplos 'if' ao invés de 'else if' causa bug
    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && currentDirection != DOWN) {
        currentDirection = UP;
    } else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && currentDirection != UP) {
        currentDirection = DOWN;
    } else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && currentDirection != RIGHT) {
        currentDirection = LEFT;
    } else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && currentDirection != LEFT) {
        currentDirection = RIGHT;
    }
}

void checkCollisions() {
    // Verifica se a cabeça da cobra está fora dos limites
    if (snake[0].x < 0 || snake[0].x >= LINES || snake[0].y < 0 ||
        snake[0].y >= COLUMNS) {
        setupGame();
    }

    // Verifica se a cabeça da cobra colidiu contra o corpo
    for (int i = 1; i < snakeLength - 1; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            setupGame();
        }
    }

    if (snake[0].x == apple.x && snake[0].y == apple.y) {
        snakeLength++;
        snake[snakeLength - 1] = snake[snakeLength - 2];
        if (snakeLength % 2 == 0) {
            gameSpeed++;
            SetTargetFPS(gameSpeed);
        }
        newApple();
    }
}

void updateSnake() {
    // Atualiza a posição do corpo
    for (int i = snakeLength - 1; i > 0; i--)
        snake[i] = snake[i - 1];
    // for (int i = 1; i < snakeLength; i++) {
    //     snake[i] = snake[];
    // }

    // Atualiza a cabeça da cobra
    switch (currentDirection) {
        case UP:
            snake[0].y--;
            break;
        case DOWN:
            snake[0].y++;
            break;
        case LEFT:
            snake[0].x--;
            break;
        case RIGHT:
            snake[0].x++;
            break;
    }

    checkCollisions();
}

void drawSnake() {
    for (int i = 0; i < snakeLength; i++) {
        DrawRectangle(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE,
                      CELL_SIZE, i == 0 ? GREEN : DARKGREEN);
    }
}

void drawApple() {
    DrawRectangle(apple.x * CELL_SIZE, apple.y * CELL_SIZE, CELL_SIZE,
                  CELL_SIZE, RED);
}

void drawGrid() {
    for (int line = 0; line <= SCREEN_WIDTH; line += CELL_SIZE) {
        DrawLine(line, 0, line, SCREEN_HEIGHT, Color{25, 25, 25, 255});
        DrawLine(0, line, SCREEN_WIDTH, line, Color{25, 25, 25, 255});
    }
}

int main() {
    //--------------------------------------------------------------------------------------
    // Window Settings
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Classic Snake Game");
    SetTargetFPS(gameSpeed);
    //--------------------------------------------------------------------------------------

    setupGame();

    //--------------------------------------------------------------------------------------
    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        //--------------------------------------------------------------------------------------
        // Update
        //----------------------------------------------------------------------------------
        getInput();
        updateSnake();
        //----------------------------------------------------------------------------------

        //--------------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        drawSnake();
        drawApple();
        drawGrid();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    //--------------------------------------------------------------------------------------
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
