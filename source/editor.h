/*
  Editor Module
  we will only interface the editor() function
  it will handle the rest
*/
#define CIRCLESIZE 16  //vertices of our unit circle
void editor();
void drawTemp(body *,s16 *,s32 *,s32 *,double [CIRCLESIZE][2]);
void makeLocal(body *);
