struct Plane
{
    int id;
    int time;
    struct QNode *next;
};
struct Queue
{
    int count;
    struct Plane *front, *rear;
};
struct Plane *newPlane(int id, int time);
struct Queue *createQueue();
void enQueue(struct Queue *q, struct Plane *temp);
struct Plane *deQueue(struct Queue *q);
int size(struct Queue *q);
int empty(struct Queue *q);
int full(struct Queue *q);
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
