#ifndef MAPARRAY_H
#define MAPARRAY_H
#include <assert.h>
#include <QDebug>

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

int **getMapPointerFromStoryMapID(int id);
int **getMapPointerFromBattleID(int id);

#endif // MAPARRAY_H
