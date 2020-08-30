#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include"header.h"

#define MAX 3
#define ARRIVE 0
#define DEPART 1


int main()
{
    //srand(time(0));
    struct airport a;
    int i, pri, curtime, endtime;
    double expectarrive, expectdepart;
    struct Plane *temp;
    int test;
    test = randomnumber(0.47);
    printf("%d", test);
    initairport(&a);
    start(&endtime, &expectarrive, &expectdepart);

    for (curtime = 1; curtime <= endtime; curtime++)
    {
        pri = randomnumber(expectarrive);
        for (i = 1; i <= pri; i++)
        {
            newplane(&a, curtime, ARRIVE);
            if (runwayFull(&a, 'l', 1))
                refuse(&a, ARRIVE);
            else
                apaddqueue(&a, 'l', 1);
        }

        pri = randomnumber(expectdepart);

        for (i = 1; i <= pri; i++)
        {
            newplane(&a, curtime, DEPART);
            if (runwayFull(&a, 't', 2))
                refuse(&a, DEPART);
            else
                apaddqueue(&a, 't', 2);
        }

        if (!(runwayEmpty(&a, 'l', 1))) //landing queue is not empty 
        {
            temp = apdelqueue(&a, 'l', 1);
            land(&a, temp, curtime, 1);
        }// only if landing queue is empty take off is allowed
        if (!(runwayEmpty(&a, 't', 2))) //takeoff queue is not empty 
        {
            temp = apdelqueue(&a, 't', 2);
            fly(&a, temp, curtime, 2);
        }
        else 
        {
                idle(&a, curtime);
        }
    }

    conclude(&a, endtime);
    return 0;
}
