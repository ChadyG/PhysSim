#include "data.h"

//--Free Body initialisation---
void bodyInit(body *n,s16 s) {
  s16 i;
  n->size = s;
  n->type = 0;
  n->width = 0;
  n->height = 0;
  n->midX = 0;
  n->midY = 0;
  n->verts = malloc(s*sizeof(double *));
  n->veloc = calloc(s,sizeof(double *));
  for (i=0;i<s;i++) {
    n->verts[i] = malloc(sizeof(double)*2);
    n->veloc[i] = calloc(2,sizeof(double));
  }
}
//---Free Body copy---
void bodyCopy(body *b,const body *a) {
  s16 i;
  b->type = a->type;
  b->width = a->width;
  b->height = a->height;
  b->midX = a->midX;
  b->midY = a->midY;
  for (i=0;i<b->size;i++) {
    b->verts[i][0] = a->verts[i][0];
    b->verts[i][1] = a->verts[i][1];
  }
}
//---free up memory used by bodies---
void bodyDestruct(body **b,s16 *s) {
  s16 i;
  for (i=0;i<*s;i++) 
    free(b[i]);
  *s = 0;
}
