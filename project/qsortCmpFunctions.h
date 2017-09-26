#include "vertex.h"

typedef struct ColAmountSt { 
        u32 color;
        u32 amount;
} *colAmount;

int cmpNaturalOrder(const void *p, const void *q);
int cmpWelshPowell(const void *p, const void *q);
int cmpMinToMaxColAmount(const void *p, const void *q);
int cmpMaxToMinColAmount(const void *p, const void *q);
int cmpMaxToMinByColor(const void *p, const void *q);
int cmpMinToMax(const void *p, const void *q);