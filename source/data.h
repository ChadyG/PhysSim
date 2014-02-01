#include <PA9.h>       // Include for PA_Lib

/*
  This module will manage all game data including;

  mass systems
  iconic objects
*/

typedef struct {
  int size;
  int type;
  double width,height,midX,midY;
  double **verts;
  double **veloc;
} body;

void bodyInit(body *,s16);
void bodyCopy(body *,const body *);
void bodyDestruct(body **,s16 *);
