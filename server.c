#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>

#define MAX 3
#define ARRIVE 0
#define DEPART 1

// A linked list (LL) node to store a queue entry
struct Plane
{
    int id;
    int time;
    struct QNode *next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue
{
    int count;
    struct Plane *front, *rear;
};

// A utility function to create a new linked list node.
struct Plane *newPlane(int id, int time)
{
    struct Plane *temp = (struct Plane *)malloc(sizeof(struct Plane));
    temp->id = id;
    temp->time = time;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    q->count = 0;
    return q;
}

// The function to add a key k to q
void enQueue(struct Queue *q, struct Plane *temp)
{
    // Create a new LL node

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        q->count++;
        return NULL;
    }
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
    q->count++;
}

// Function to remove a key from given queue q
struct Plane *deQueue(struct Queue *q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
    {
        q->count--;
        return NULL;
    }

    // Store previous front and move front one node ahead
    struct Plane *temp = q->front;
    q->front = q->front->next;
    q->count--;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    return temp;
}

int size(struct Queue *q)
{
    return q->count;
}
int empty(struct Queue *q)
{
    return (q->count <= 0);
}

int full(struct Queue *q)
{
    return (q->count >= MAX);
}

struct airport
{

    struct Queue *R1_takeoff;
    struct Queue *R2_takeoff;
    struct Queue *R1_landing;
    struct Queue *R2_landing;
    int idletime;
    int landwait, takeoffwait;
    int nland, nplanes, nrefuse, ntakeoff;
    struct Plane *pln;
};

void initairport(struct airport *ap);
void start(int *endtime, double *expectarrive, double *expectdepart);
void newplane(struct airport *ap, int curtime, int action);
void refuse(struct airport *ap, int action);
void land(struct airport *ap, struct Plane *pl, int curtime, int runway);
void fly(struct airport *ap, struct Plane *pl, int curtime, int runway);
void idle(struct airport *ap, int curtime);
void conclude(struct airport *ap, int endtime);
int randomnumber(double expectedvalue);
void apaddqueue(struct airport *ap, char type, int runway);
struct Plane *apdelqueue(struct airport *ap, char type, int runway);
int apsize(struct airport *ap, char type);
int runwayFull(struct airport *ap, char type, int runway);
int runwayEmpty(struct airport *ap, char type, int runway);
void myrandomize();

void initairport(struct airport *ap)
{

    ap->R1_landing = createQueue();
    ap->R1_takeoff = createQueue();
    ap->R2_landing = createQueue();
    ap->R2_takeoff = createQueue();
    ap->nplanes = ap->nland = ap->ntakeoff = ap->nrefuse = 0;
    ap->landwait = ap->takeoffwait = ap->idletime = 0;
}

void start(int *endtime, double *expectarrive, double *expectdepart)
{
    int flag = 0;
    char wish;
    printf("\n\n\t\t WELCOME TO AIRPORT SIMULATOR \n\n");
    printf("\n\t->Program that simulates an airport with 2 runways.\n");
    printf("\t->One plane can land or depart from each runway in each unit of time.\n");
    printf("\t->Runway 1 is for LANDING and Runway 2 is for TAKEOFF in general.\n");
    printf("\t->Up to %d planes can be waiting to land or take off at any time.\n\n", MAX);
    printf("\t->How many units of time will the simulation run?");
    scanf("%d", endtime);
    myrandomize();
    do
    {
        printf("\n\t Expected number of arrivals per unit time ? ");
        scanf("%lf", expectarrive);
        printf("\n\tExpected number of departures per unit time ? ");
        scanf("%lf", expectdepart);

        if (*expectarrive < 0.0 || *expectdepart < 0.0)
        {
            printf("These numbers must be greater than 0.\n");
            flag = 0;
        }
        else
        {
            if (*expectarrive + *expectdepart > 5.0)
            {
                printf("The airport will become saturated. Read new numbers(Enter y )? ");
                scanf(" %c", &wish);
                if (tolower(wish) == 'y')
                    flag = 0;
                else
                    flag = 1;
            }
            else
                flag = 1;
        }
    } while (flag == 0);
}

void newplane(struct airport *ap, int curtime, int action)
{
    (ap->nplanes)++;
    ap->pln->id = ap->nplanes;
    ap->pln->time = curtime;

    switch (action)
    {
    case ARRIVE:
        printf("\t\tPlane %d ready to land.\n", ap->nplanes);
        break;

    case DEPART:
        printf("\t\tPlane %d ready to take off.\n", ap->nplanes);
        break;
    }
}
void refuse(struct airport *ap, int action)
{
    switch (action)
    {
    case ARRIVE:
        printf("\t\tplane  %d directed to another airport.\n", ap->pln->id);
        break;

    case DEPART:
        printf("\t\tplane %d told to try later.\n", ap->pln->id);
        break;
    }
    (ap->nrefuse)++;
}
void land(struct airport *ap, struct Plane *pl, int curtime, int runway)
{
    int wait;
    wait = curtime - pl->time;
    printf("%d: Plane %d landed on Runway-%d  ", curtime, pl->id, runway);
    printf("in queue %d units \n", wait);
    (ap->nland)++;
    (ap->landwait) += wait;
}
void fly(struct airport *ap, struct Plane *pl, int curtime, int runway)
{
    int wait;
    wait = curtime - pl->time;
    printf("\n%d: Plane %d took off from Runway-%d  ", curtime, pl->id, runway);
    printf("in queue %d units \n", wait);
    (ap->ntakeoff)++;
    (ap->takeoffwait) += wait;
}
void idle(struct airport *ap, int curtime)
{
    printf("%d: Runway is idle.\n", curtime);
    ap->idletime++;
}
void conclude(struct airport *ap, int endtime)
{
    printf("\tSimulation has concluded after %d units.\n", endtime);
    printf("\tTotal number of planes processed: %d\n", ap->nplanes);
    printf("\tNumber of planes landed: %d\n", ap->nland);
    printf("\tNumber of planes taken off: %d\n", ap->ntakeoff);
    printf("\tNumber of planes refused use: %d\n", ap->nrefuse);
    // printf("\tNumber left ready to land: %d\n", apsize(ap, 'l'));
    // printf("\tNumber left ready to take off: %d\n", apsize(ap, 't'));

    if (endtime > 0)
        printf("\tPercentage of time runway idle: %.3lf \n", ((double)ap->idletime / endtime) * 100.0);

    if (ap->nland > 0)
        printf("\tAverage wait time to land: %.3lf \n", ((double)ap->landwait / ap->nland));

    if (ap->ntakeoff > 0)
        printf("\tAverage wait time to take off: %.3lf \n", ((double)ap->takeoffwait / ap->ntakeoff));
}

int randomnumber(double expectedvalue)
{   
    
    int n = 0; //counter of iteration
    double limit;
    double x; //pseudo random number
    //limit = exp(-expectedvalue);  
    limit=0.13509;  
    x = rand() / (double)RAND_MAX;
    while (x > limit)
    {
        n++;
        x *= rand() / (double)RAND_MAX;
    }

    return n;
}
void apaddqueue(struct airport *ap, char type, int runway)
{
    switch (tolower(type))
    {
    case 'l':
        switch (runway)
        {
        case 1:
            enQueue(ap->R1_landing, ap->pln);
            break;
        case 2:
            enQueue(ap->R2_landing, ap->pln);
            break;
        }
        break;
    case 't':
        switch (runway)
        {
        case 1:
            enQueue(ap->R1_takeoff, ap->pln);
            break;
        case 2:
            enQueue(ap->R2_takeoff, ap->pln);
            break;
        }
        break;
    }
}

struct Plane *apdelqueue(struct airport *ap, char type, int runway)
{
    struct Plane *p1 = (struct Plane *)malloc(sizeof(struct Plane));
    switch (tolower(type))
    {
    case 'l':
        switch (runway)
        {
        case 1:
            p1 = deQueue(ap->R1_landing);
            break;
        case 2:
            p1 = deQueue(ap->R2_landing);
            break;
        }
        break;
    case 't':
        switch (runway)
        {
        case 1:
            p1 = deQueue(ap->R1_takeoff);
            break;
        case 2:
            p1 = deQueue(ap->R2_takeoff);
            break;
        }
        break;
    }

    return p1;
}
int apsize(struct airport *ap, char type)
{
    switch (tolower(type))
    {
    case 'l':
        return (size(ap->R1_landing) + size(ap->R2_landing));

    case 't':
        return (size(ap->R1_takeoff) + size(ap->R2_takeoff));
    }

    return 0;
}
int runwayFull(struct airport *ap, char type, int runway)
{
    switch (tolower(type))
    {
    case 'l':
        switch (runway)
        {
        case 1:
            return (full(ap->R1_landing));
        case 2:
            return (full(ap->R2_landing));
        }
        break;
    case 't':
        switch (runway)
        {
        case 1:
            return (full(ap->R1_takeoff));
        case 2:
            return (full(ap->R2_takeoff));
            break;
        }
        break;
        return 0;
    }
}
int runwayEmpty(struct airport *ap, char type, int runway)
{
    switch (tolower(type))
    {
    case 'l':
        switch (runway)
        {
        case 1:
            return (empty(ap->R1_landing));
        case 2:
            return (empty(ap->R2_landing));
        }
        break;
    case 't':
        switch (runway)
        {
        case 1:
            return (empty(ap->R1_takeoff));
        case 2:
            return (empty(ap->R2_takeoff));
            break;
        }
        break;
        return 0;
    }
}
void myrandomize()
{
    srand((unsigned int)(time(NULL) % 10000));
}

void printQueue(struct Queue *temp)
{
    printf("data -> ");
    while (temp != NULL)
    {
        printf("%d->", temp->front->id);
    }
    temp = temp->front;
}