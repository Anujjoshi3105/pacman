// ANUJ JOSHI 2K22/CO/74
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

// Characters types
enum EntityType
{
    PLAYER = 0,
    ENEMY = 1,
    TARGET = 2,
    BLOCKADE = 3,
};

// Entity structure for linked list (Player and Snake)
typedef struct Entity
{
    int x, y;
    chtype icon;
    struct Entity *next;
} Entity;

// Block structure
typedef struct Block
{
    Entity *head;
    Entity *tail;
} Block;

// To free Block
void freeLinkedList(Block *list)
{
    while (list->head)
    {
        Entity *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    list->tail = NULL;
}

// To add entity to the linked list
void addEntityToLinkedList(Block *list, Entity *entity)
{
    Entity *newEntity = (Entity *)malloc(sizeof(Entity));
    if (!newEntity)
    {
        printf("Failed to allocate memory for a new entity.");
        exit(EXIT_FAILURE);
    }
    *newEntity = *entity;
    newEntity->next = NULL;

    if (!list->head)
    {
        list->head = list->tail = newEntity;
    }
    else
    {
        list->tail->next = newEntity;
        list->tail = newEntity;
    }
}

// To read the game board from file
int **readfile(char filename[], Entity *enemy, Entity *playermc, Entity *target, int *s)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Failed to open the file.\n");
        exit(EXIT_FAILURE);
    }

    int num[3];
    if (fscanf(file, "%d %d %d \n", &num[0], &num[1], &num[2]) != 3)
    {
        printf("Invalid file format.\n");
        exit(EXIT_FAILURE);
    }

    int **arr = (int **)malloc(sizeof(int *) * num[0]);
    if (!arr)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int k = 0; k < num[0]; k++)
    {
        arr[k] = (int *)malloc(sizeof(int) * 2);
        if (!arr[k])
        {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    arr[0][0] = num[1];
    arr[0][1] = num[2];

    int size = 1;

    while (fscanf(file, "%d %d %d \n", &num[0], &num[1], &num[2]) == 3)
    {
        switch (num[2])
        {
        case PLAYER:
            playermc->y = num[0];
            playermc->x = num[1];
            playermc->icon = '^';
            break;
        case ENEMY:
            enemy->y = num[0];
            enemy->x = num[1];
            enemy->icon = '~';
            break;
        case TARGET:
            target->y = num[0];
            target->x = num[1];
            target->icon = 'x';
            break;
        case BLOCKADE:
            arr[size][0] = num[0];
            arr[size][1] = num[1];
            size++;
            break;
        default:
            printf("Invalid entity type in file.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    *s = size;
    return arr;
}

// To add character on game board
void addAt(WINDOW *board, int x, int y, chtype ch)
{
    mvwaddch(board, y, x, ch);
}

// To add blockades on game board
void addBlocks(WINDOW *board, int **arr, int size)
{
    for (int i = 1; i < size; i++)
    {
        addAt(board, arr[i][1], arr[i][0], 'o');
    }
}

// To remove player and snake
void removeEntities(WINDOW *board, Entity *enemy, Entity *mc)
{
    addAt(board, enemy->x, enemy->y, ' ');
    addAt(board, mc->x, mc->y, ' ');
}

// To add player and snake
void addEntities(WINDOW *board, Entity *enemy, Entity *mc)
{
    addAt(board, enemy->x, enemy->y, enemy->icon);
    addAt(board, mc->x, mc->y, mc->icon);
}

// To check player and snake valid blocks
int checkBlock(Entity *entity, int **blocks, int size)
{
    int x = entity->x;
    int y = entity->y;

    if ((x > 0 && x < blocks[0][1] - 1) && (y > 0 && y < blocks[0][0] - 1))
    {
        for (int i = 1; i < size; i++)
        {
            if (x == blocks[i][1] && y == blocks[i][0])
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

// To update the snake's position
void updateSnake(Entity *enemy, Entity *mc, int **blockades, int size, Block *snakePos)
{
    addEntityToLinkedList(snakePos, enemy);

    int flag = 0;

    if (mc->x < enemy->x && flag == 0)
    {
        enemy->x--;
        flag++;
        if (checkBlock(enemy, blockades, size) == 0)
        {
            enemy->x++;
            flag--;
        }
    }

    if (mc->x > enemy->x && flag == 0)
    {
        enemy->x++;
        flag++;
        if (checkBlock(enemy, blockades, size) == 0)
        {
            enemy->x--;
            flag--;
        }
    }

    if (mc->y < enemy->y && flag == 0)
    {
        enemy->y--;
        flag++;
        if (checkBlock(enemy, blockades, size) == 0)
        {
            enemy->y++;
            flag--;
        }
    }

    if (mc->y > enemy->y && flag == 0)
    {
        enemy->y++;
        flag++;
        if (checkBlock(enemy, blockades, size) == 0)
        {
            enemy->y--;
            flag--;
        }
    }
}

// To update player's position
void updatePlayer(WINDOW *board, Entity *mc, int **blockades, int size, Entity *enemy, Block *characterPrev, Block *snakePrev)
{
    chtype c = wgetch(board);
    removeEntities(board, enemy, mc);

    switch (c)
    {
    case 'w':
    case 'W':
    case KEY_UP:
        addEntityToLinkedList(characterPrev, mc);
        mc->y--;
        mc->icon = '^';
        if (checkBlock(mc, blockades, size) == 0)
        {
            mc->y++;
        }
        updateSnake(enemy, mc, blockades, size, snakePrev);
        break;
    case 's':
    case 'S':
    case KEY_DOWN:
        addEntityToLinkedList(characterPrev, mc);
        mc->y++;
        mc->icon = 'v';
        if (checkBlock(mc, blockades, size) == 0)
        {
            mc->y--;
        }
        updateSnake(enemy, mc, blockades, size, snakePrev);
        break;
    case 'a':
    case 'A':
    case KEY_LEFT:
        addEntityToLinkedList(characterPrev, mc);
        mc->x--;
        mc->icon = '<';
        if (checkBlock(mc, blockades, size) == 0)
        {
            mc->x++;
        }
        updateSnake(enemy, mc, blockades, size, snakePrev);
        break;
    case 'd':
    case 'D':
    case KEY_RIGHT:
        addEntityToLinkedList(characterPrev, mc);
        mc->x++;
        mc->icon = '>';
        if (checkBlock(mc, blockades, size) == 0)
        {
            mc->x--;
        }
        updateSnake(enemy, mc, blockades, size, snakePrev);
        break;
    case 'u':
    case 'U':
        if (characterPrev->tail)
        {
            mc->x = characterPrev->tail->x;
            mc->y = characterPrev->tail->y;
            mc->icon = characterPrev->tail->icon;

            Entity *curr = characterPrev->head;
            Entity *prev = curr;
            while (curr->next != NULL)
            {
                prev = curr;
                curr = curr->next;
            }
            prev->next = NULL;
            characterPrev->tail = prev;
        }
        if (snakePrev->tail)
        {
            enemy->x = snakePrev->tail->x;
            enemy->y = snakePrev->tail->y;

            Entity *curr1 = snakePrev->head;
            Entity *prev1 = curr1;
            while (curr1->next != NULL)
            {
                prev1 = curr1;
                curr1 = curr1->next;
            }
            prev1->next = NULL;
            snakePrev->tail = prev1;
        }
        break;
    case 'z':
    case 'Z':
        endwin();
        clear();
        exit(0);
    default:
        break;
    }
}

// To determine state of game
int gameState(Entity *mc, Entity *target, Entity *enemy)
{
    if (mc->x == target->x && mc->y == target->y)
    {
        return 1; // Player won
    }
    if (mc->x == enemy->x && mc->y == enemy->y)
    {
        return -1; // Player lost
    }
    return 0; // Game continues
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Entity *playermc = (Entity *)malloc(sizeof(Entity));
    Entity *enemy = (Entity *)malloc(sizeof(Entity));
    Entity *target = (Entity *)malloc(sizeof(Entity));

    int state = 0;

    int **blockades;
    int size;

    blockades = readfile(argv[1], enemy, playermc, target, &size);

    initscr();
    cbreak();
    refresh();

    WINDOW *board = newwin(blockades[0][0], blockades[0][1], 0, 0);
    curs_set(0);
    wborder(board, '|', '|', '-', '-', '#', '#', '#', '#');
    keypad(board, true);
    noecho();

    addBlocks(board, blockades, size);
    addAt(board, target->x, target->y, target->icon);

    Block *characterPrev = (Block *)malloc(sizeof(Block));
    Block *snakePrev = (Block *)malloc(sizeof(Block));

    characterPrev->head = characterPrev->tail = NULL;
    snakePrev->head = snakePrev->tail = NULL;

    while (state == 0)
    {
        addEntities(board, enemy, playermc);
        wrefresh(board);

        updatePlayer(board, playermc, blockades, size, enemy, characterPrev, snakePrev);

        addEntities(board, enemy, playermc);
        wrefresh(board);
        state = gameState(playermc, target, enemy);
    }

    board = newwin(blockades[0][0], blockades[0][1], 0, 0);
    wborder(board, '|', '|', '-', '-', '#', '#', '#', '#');
    move(blockades[0][0] + 1, 0);

    if (state == 1)
    {
        printw("You Win!");
    }
    else if (state == -1)
    {
        printw("You Lost, try again.");
    }

    wrefresh(board);
    getch();
    endwin();

    freeLinkedList(characterPrev);
    freeLinkedList(snakePrev);

    free(playermc);
    free(enemy);
    free(target);

    for (int i = 0; i < size; i++)
    {
        free(blockades[i]);
    }
    free(blockades);

    return 0;
}
