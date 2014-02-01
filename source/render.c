#include <PA9.h>       // Include for PA_Lib
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "data.h"
#include "render.h"

//---Init sprites + background
void initImages() {
	PA_EasyBgLoad(1,3,map);
	PA_LoadSpritePal(0,0,(void*)sprite0_Pal);
	PA_CreateSprite(0,0,(void*)Elude_Sprite,OBJ_SIZE_32X64,1,0,32,32);
}
//---Background---
void drawScene() {
  glPushMatrix();
  glBegin(GL_QUAD);
		
  glColor3b(0,255,-.1);
	PA_Vertex3D(-5,-5,-.1);
  PA_Vertex3D(-5,-3,-.1);
  PA_Vertex3D(5,3,-.1);
	PA_Vertex3D(5,-5,-.1);

  glColor3b(0,0,255);
	PA_Vertex3D(-5,-3,-.1);
	PA_Vertex3D(-5,5,-.1);
	PA_Vertex3D(5,5,-.1);
	PA_Vertex3D(5,-3,-.1);

  glEnd();
	glPopMatrix(1);
}
//---Render Temporary Draw Shape---
void renderTemp(body *draw,s16 s) {
  s16 i;
  glPushMatrix();  //draw the temporary shape
  glBegin(GL_TRIANGLES);

  glColor3b(200,200,50);
  for (i=0;i<s-2;i++) {
    PA_Vertex3D(draw->verts[i][0],draw->verts[i][1],0);
    PA_Vertex3D(draw->verts[i+1][0],draw->verts[i+1][1],0);
    PA_Vertex3D(draw->verts[i+2][0],draw->verts[i+2][1],0);
  }
  if (s>1) {
    PA_Vertex3D(draw->verts[s-2][0],draw->verts[s-2][1],0);
    PA_Vertex3D(draw->verts[s-1][0],draw->verts[s-1][1],0);
    PA_Vertex3D(draw->verts[0][0],draw->verts[0][1],0);
  }

  glEnd();
	glPopMatrix(1); //end of temporary shape
  glFlush();
}
//---Render Free Bodies---
void renderBodies(body *b[],s16 s) {
  s16 i,j;
  double dx,dy;
  for (j=0;j<s;j++) {
      
    if (b[j]->size==3) { //--draw us some Triangles--
      glPushMatrix();
      glBegin(GL_TRIANGLES);
      glColor3b(200,0,0);

      PA_Vertex3D(b[j]->midX+b[j]->verts[0][0],     b[j]->midY+b[j]->verts[0][1],0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[1][0],     b[j]->midY+b[j]->verts[1][1],0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[2][0],     b[j]->midY+b[j]->verts[2][1],0);

      glEnd();
		  glPopMatrix(1);
    }else
    if (b[j]->size==4) { //--draw us some rectangles--
      glPushMatrix();
      glBegin(GL_QUADS);
      glColor3b(200,0,0);

      PA_Vertex3D(b[j]->midX+b[j]->verts[0][0],     b[j]->midY+b[j]->verts[0][1],0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[1][0],     b[j]->midY+b[j]->verts[1][1],0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[2][0],     b[j]->midY+b[j]->verts[2][1],0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[3][0],     b[j]->midY+b[j]->verts[3][1],0);

      glEnd();
		  glPopMatrix(1);
    }else{ //--draw us some whatever the hell we want--
      glPushMatrix();
      glBegin(GL_QUADS);
      glColor3b(200,0,0);

      for (i=0;i<(b[j]->size)-1;i++) {
        dx = (b[j]->verts[i+1][0] - b[j]->verts[i][0]);
        dy = (b[j]->verts[i+1][1] - b[j]->verts[i][1]);
        dx = .05*dx/(dx<0?-dx:dx);
        dy = .05*dy/(dy<0?-dy:dy);

        PA_Vertex3D(b[j]->midX+b[j]->verts[i][0] - dy,         b[j]->midY+b[j]->verts[i][1] + dx,0);
        PA_Vertex3D(b[j]->midX+b[j]->verts[i][0] + dy,         b[j]->midY+b[j]->verts[i][1] - dx,0);
        PA_Vertex3D(b[j]->midX+b[j]->verts[i+1][0] + dy,       b[j]->midY+b[j]->verts[i+1][1] - dx,0);
        PA_Vertex3D(b[j]->midX+b[j]->verts[i+1][0] - dy,       b[j]->midY+b[j]->verts[i+1][1] + dx,0);
      }
      dx = (b[j]->verts[0][0] - b[j]->verts[b[j]->size-1][0]);
      dy = (b[j]->verts[0][1] - b[j]->verts[b[j]->size-1][1]);
      dx = .05*dx/(dx<0?-dx:dx);
      dy = .05*dy/(dy<0?-dy:dy);

      PA_Vertex3D(b[j]->midX+b[j]->verts[0][0] - dy,           b[j]->midY+b[j]->verts[0][1] + dx,0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[0][0] + dy,           b[j]->midY+b[j]->verts[0][1] - dx,0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[b[j]->size-1][0] + dy,b[j]->midY+b[j]->verts[b[j]->size-1][1] - dx,0);
      PA_Vertex3D(b[j]->midX+b[j]->verts[b[j]->size-1][0] - dy,b[j]->midY+b[j]->verts[b[j]->size-1][1] + dx,0);

      glEnd();
		  glPopMatrix(1);
    }
  }
}
/*void renderBodies(body *b[],s16 s) {
    s16 i,j;
		glPushMatrix();
    glBegin(GL_TRIANGLES);

    glColor3b(200,0,0);
    for (j=0;j<s;j++) {

      for (i=0;i<(b[j]->size)-2;i++) {
        PA_Vertex3D(b[j]->verts[i][0],b[j]->verts[i][1],0);
        PA_Vertex3D(b[j]->verts[i+1][0],b[j]->verts[i+1][1],0);
        PA_Vertex3D(b[j]->verts[i+2][0],b[j]->verts[i+2][1],0);
      }
      if (b[j]->size>1) {
        PA_Vertex3D(b[j]->verts[b[j]->size-2][0],b[j]->verts[b[j]->size-2][1],0);
        PA_Vertex3D(b[j]->verts[b[j]->size-1][0],b[j]->verts[b[j]->size-1][1],0);
        PA_Vertex3D(b[j]->verts[0][0],b[j]->verts[0][1],0);
      }

    }

    glEnd();
		glPopMatrix(1);
}*/
