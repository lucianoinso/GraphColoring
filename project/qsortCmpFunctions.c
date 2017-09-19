#include "qsortCmpFunctions.h"

int cmpNaturalOrder(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1name = get_vertex_name(v1);
    u32 v2name = get_vertex_name(v2);
    return (v1name > v2name) - (v1name < v2name);
    // Para el return se opera con los resultados booleanos 
    // de las funciones de la forma:
    // si v1 > v2
    // return (1) - (0) = return 1
    // si v2 > v1
    // return (0) - (1) = return -1
    // si v1 == v2
    // return (0) - (0) = return 0
}

int cmpWelshPowell(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1grade = get_vertex_grade(v1);
    u32 v2grade = get_vertex_grade(v2);
    // Ordenamiento segun el grado en orden decreciente
    return (v2grade > v1grade) - (v2grade < v1grade);
}

int cmpMinToMaxColAmount(const void *p, const void *q){
    colAmount ca1 = *(colAmount const *) p;
    colAmount ca2 = *(colAmount const *) q;
    u32 c1ColAmount = ca1->amount;
    u32 c2ColAmount = ca2->amount;

    if(c1ColAmount < c2ColAmount) {
        return -1;
    } else if(c1ColAmount > c2ColAmount) {
        return 1;
    } else {  // c1ColAmount == c2ColAmount
        u32 c1color = ca1->color;
        u32 c2color = ca2->color;
        if (c1color < c2color) return -1;
        else if (c1color > c2color) return 1;
        else return 0;  // (c1color == c2color)
    }
    return 0;
}

int cmpMaxToMinColAmount(const void *p, const void *q){
    colAmount ca1 = *(colAmount const *) p;
    colAmount ca2 = *(colAmount const *) q;
    u32 c1ColAmount = ca1->amount;
    u32 c2ColAmount = ca2->amount;

    if(c1ColAmount < c2ColAmount) {
        return 1;
    } else if(c1ColAmount > c2ColAmount) {
        return -1;
    } else {  // c1ColAmount == c2ColAmount
        u32 c1color = ca1->color;
        u32 c2color = ca2->color;
        if (c1color < c2color) return -1;
        else if (c1color > c2color) return 1;
        // este último caso lo mas probable es que no exista pero para abarcar todos los casos
        else return 0;  // (c1color == c2color)
    }
}

int cmpMaxToMinByColor(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1color = get_vertex_color(v1);
    u32 v2color = get_vertex_color(v2);

    if(v1color < v2color) {
        return 1;
    } else if(v1color > v2color) {
        return -1;
    } else {  // c1ColAmount == c2ColAmount
        u32 v1name = v1->name;
        u32 v2name = v2->name;
        if (v1name < v2name) return -1;
        else if (v1name > v2name) return 1;
        // Este caso lo mas probable es que no exista pero para abarcar todos los casos
        else return 0;  // (v1name == v2name)
    }
}

int cmpMinToMax(const void *p, const void *q){
    u32 name1 = *(u32 const *) p;
    u32 name2 = *(u32 const *) q;
    return (name1 > name2) - (name1 < name2);
}

