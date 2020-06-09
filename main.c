#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


#define FILENAME_X    ".\\Cases\\casox.txt"
#define HEIGHT_X      30
#define FILENAME_A    ".\\Cases\\casoa.txt"
#define HEIGHT_A      100
#define FILENAME_B    ".\\Cases\\casob.txt"
#define HEIGHT_B      200
#define FILENAME_C    ".\\Cases\\casoc.txt"
#define HEIGHT_C      500
#define FILENAME_D    ".\\Cases\\casod.txt"
#define HEIGHT_D      1000
#define FILENAME_E    ".\\Cases\\casoe.txt"
#define HEIGHT_E      2000
#define FILENAME_F    ".\\Cases\\casof.txt"
#define HEIGHT_F      5000
#define FILENAME_G    ".\\Cases\\casog.txt"
#define HEIGHT_G      10000
#define FILENAME_H    ".\\Cases\\casoh.txt"
#define HEIGHT_H      20000

#define FILENAME      FILENAME_A
#define HEIGHT        HEIGHT_A
#define WIDTH         HEIGHT + 1

/* ------------------------------------------------------*/
// A structure to represent a queue
typedef struct{
    int x;
    int y;
    long farFromOrigin;
} Compass;

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    Compass* array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned long capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (Compass*) malloc(queue->capacity * sizeof(Compass));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, Compass item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
Compass dequeue(struct Queue* queue)
{
    Compass item;
    item.x = -1;
    if (isEmpty(queue))
        return item;
    item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
Compass front(struct Queue* queue)
{
    Compass item;
    item.x = -1;
    if (isEmpty(queue))
        return item;
    return queue->array[queue->front];
}

// Function to get rear of queue
Compass rear(struct Queue* queue)
{
    Compass item;
    item.y = -1;
    if (isEmpty(queue))
        return item;
    return queue->array[queue->rear];
}
/* ------------------------------------------------------*/

typedef struct{
    char *x;
} Scenario;

Compass findHeroes    (Scenario *y);
Compass findCharacter (Scenario *y, char find);
Compass walk          (Scenario *y, struct Queue *queue);
Compass depthWalk     (Scenario *y, struct Queue *queue);
void    queuePos      (Scenario *y, struct Queue *queue, int xs, int ys, int steps);

int main(int argc, char const *argv[]) {
    printf("SEARCHING THROW FILE: %s :: WIDTH %d, HEIGHT %d\n", FILENAME, WIDTH-1, HEIGHT);
    clock_t begin = clock();
    struct Queue* queue = createQueue(((HEIGHT * 2) + (WIDTH * 2)));

    Scenario *y = malloc(sizeof (Scenario) * HEIGHT);
        y[0].x = malloc(sizeof (char) * WIDTH);


    // Read File
    FILE *file;
    if ((file = fopen(FILENAME, "r")) == NULL) {
        printf("Error! Could not open file");
        exit(1);
    } else {
        int row = 0;
        while(fscanf(file, "%s", y[row].x) != EOF){
            row++;
                y[row].x = malloc(sizeof (char) * WIDTH);
        }
        fclose(file);
    }

    // Get hero position
        Compass pos = findHeroes(y);
        pos.farFromOrigin = 0;
        printf("Heroes position -> x:%d y:%d\n", pos.x, pos.y);
        queuePos(y, queue, pos.x, pos.y, pos.farFromOrigin);


    // // Search throw the enemy
        pos = walk(y, queue);
        printf("\nThe hero gave %ld steps\n", pos.farFromOrigin);
        printf("Until get to Thanos on x:%d y:%d\n\n", pos.x, pos.y);



    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Program runtime: %.4f seconds\n", time_spent);


    // Fre Memories
    free(queue->array);
    free(queue);

    for(int row = 0; row < HEIGHT; row++){
        free(y[row].x);
    }
    free(y);

    return 0;
}


Compass walk(Scenario *y, struct Queue *queue){
    Compass pos = dequeue(queue);
    queuePos(y, queue, pos.x, pos.y, pos.farFromOrigin);

    do{
        pos = depthWalk(y, queue);
    }while(!isEmpty(queue) && pos.x == 0);

    return pos;
}

Compass depthWalk(Scenario *y, struct Queue *queue){
    Compass pos = dequeue(queue);
    if(y[pos.y].x[pos.x] == '*'){
        pos.x = 0;
        return pos;
    }

    if(y[pos.y].x[pos.x] == 'B')
        return pos;

    y[pos.y].x[pos.x] = '*';
    queuePos(y, queue, pos.x, pos.y, pos.farFromOrigin);

    pos.x = 0;
    return pos;
}

void queuePos (Scenario *y, struct Queue *queue, int xs, int ys, int steps){
    Compass pos;
    pos.farFromOrigin = steps + 1;

    pos.x = xs;
    for(int row = -1; row < 2; row = row + 2){
        pos.y = ys + row;
        if(pos.y > 0 && pos.y < HEIGHT)
            if(y[pos.y].x[pos.x] == '.' || y[pos.y].x[pos.x] == 'B')
                enqueue(queue, pos);
    }

    pos.y = ys;
    for(int column = -1; column < 2; column = column + 2){
        pos.x = xs + column;
        if(pos.x > 0 && pos.x < WIDTH)
            if(y[pos.y].x[pos.x] == '.' || y[pos.y].x[pos.x] == 'B')
                enqueue(queue, pos);
    }
}

Compass findHeroes(Scenario *y){
    return findCharacter(y, 'A');
}
Compass findCharacter(Scenario *y, char find){
    int stopHeight = HEIGHT - 1, stopWidth = WIDTH - 1;
    Compass pos;
    pos.x = -1;

    for(int row = 1; row < stopHeight; row++)
        for(int column = 1; column < stopWidth; column++)
            if(y[row].x[column] == find){
                pos.x = column;
                pos.y = row;
                return pos;
            }
    return pos;
}
