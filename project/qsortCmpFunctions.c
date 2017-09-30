#include "qsortCmpFunctions.h"

int cmpNaturalOrder(const void *p, const void *q) {
    vertexOrd vOrd1 = *(vertexOrd const *) p;
    vertexOrd vOrd2 = *(vertexOrd const *) q;
    u32 v1name = vOrd1->order;
    u32 v2name = vOrd2->order;
    /*   
      Para el return se opera con los resultados booleanos 
      de las funciones de la forma:
      si v1 > v2
      return (1) - (0) = return 1
      si v2 > v1
      return (0) - (1) = return -1
      si v1 == v2
      return (0) - (0) = return 0
    
      Como los nombres de los vertices no se repiten no hace falta
      dar un criterio extra de ordenamiento cuando estos son iguales para
      que la funcion sea deterministica
    */
    return (v1name > v2name) - (v1name < v2name);
}

// Ordenamiento segun el grado en orden decreciente
int cmpWelshPowell(const void *p, const void *q) {
    vertexOrd vOrd1 = *(vertexOrd const *) p;
    vertexOrd vOrd2 = *(vertexOrd const *) q;
    u32 v1grade = vOrd1->order;
    u32 v2grade = vOrd2->order;

    if (v1grade < v2grade) {
        return 1;
    } else if (v1grade > v2grade) {
        return -1;
    } else {  // v1grade == v2grade
              // ordeno en orden creciente segun el tag a los
              // que tengan el mismo grado
        u32 v1tag = vOrd1->tag;
        u32 v2tag = vOrd2->tag;
        if (v1tag < v2tag) {
            return -1;
        } else if (v1tag > v2tag) {
            return 1;
        } else {
            return 0;  // (v1tag == v2tag)
        }
    }
    return 0;
}

int cmpMinToMaxColAmount(const void *p, const void *q) {
    colAmount ca1 = *(colAmount const *) p;
    colAmount ca2 = *(colAmount const *) q;
    u32 c1ColAmount = ca1->amount;
    u32 c2ColAmount = ca2->amount;

    if (c1ColAmount < c2ColAmount) {
        return -1;
    } else if (c1ColAmount > c2ColAmount) {
        return 1;
    } else {  // c1ColAmount == c2ColAmount
        u32 c1color = ca1->color;
        u32 c2color = ca2->color;
        if (c1color < c2color) {
            return -1;
        } else if (c1color > c2color) {
            return 1;
        } else {
            return 0;  // (c1color == c2color)
        }
    }
    return 0;
}

int cmpMaxToMinColAmount(const void *p, const void *q) {
    colAmount ca1 = *(colAmount const *) p;
    colAmount ca2 = *(colAmount const *) q;
    u32 c1ColAmount = ca1->amount;
    u32 c2ColAmount = ca2->amount;

    if (c1ColAmount < c2ColAmount) {
        return 1;
    } else if (c1ColAmount > c2ColAmount) {
        return -1;
    } else {  // c1ColAmount == c2ColAmount
        u32 c1color = ca1->color;
        u32 c2color = ca2->color;
        if (c1color < c2color) {
            return -1;
        } else if (c1color > c2color) {
            return 1;
        } else {
            return 0;  // (c1color == c2color)
        }
    }
}

int cmpMaxToMinByColor(const void *p, const void *q) {
    vertexOrd vOrd1 = *(vertexOrd const *) p;
    vertexOrd vOrd2 = *(vertexOrd const *) q;
    u32 v1color = vOrd1->order;
    u32 v2color = vOrd2->order;

    if (v1color < v2color) {
        return 1;
    } else if (v1color > v2color) {
        return -1;
    } else {  // v1color == color
              // ordeno en orden creciente segun el tag a los
              // que tengan el mismo color
        u32 v1tag = vOrd1->tag;
        u32 v2tag = vOrd2->tag;
        if (v1tag < v2tag) {
            return -1;
        } else if (v1tag > v2tag) {
            return 1;
        } else {
            return 0;  // (v1tag == v2tag)
        }
    }
    return 0;
}

int cmpMinToMax(const void *p, const void *q) {
    u32 tag1 = *(u32 const *) p;
    u32 tag2 = *(u32 const *) q;

    return (tag1 > tag2) - (tag1 < tag2);
    /*
      Como los tags de los vertices no se repiten no hace falta
      dar un criterio extra de ordenamiento cuando estos son iguales para
      que la funcion sea deterministica
    */
}
