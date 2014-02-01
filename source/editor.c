#include <PA9.h>       // Include for PA_Lib
#include "data.h"#include "editor.h"
#include "phys.h"
#include "render.h"
#define MAX_BODIES 10
#define CIRCLESIZE 16  //vertices of our unit circle
#define ARC 32     //ARC = 256/(CIRCLE/2)
#define SOFT 0
#define RECTANGLE 1
#define CIRCLE 2
#define TRIANGLE 3

//middle is 130,100, Stylus*25

//---Begin Editor control--
void editor() { 
  s16 i, obj_end = 0, bods = 0, drawType = 0;
  s32 oldStyX = 0,oldStyY = 0,scrollX=0,scrollY=0;
  body draw, *new, *bodies[MAX_BODIES];
  bodyInit(&draw,100); //constructor

  double circle[CIRCLESIZE][2];
  for (i=128;i<=384;i+=ARC) {
    circle[(i-128)/ARC][0] = PA_Cos(i)/256.0;
    circle[(i-128)/ARC][1] = PA_Sin(i)/256.0;
    if (i > 128 && i <384){
      circle[CIRCLESIZE-(i-128)/ARC][0] = -circle[(i-128)/ARC][0];
      circle[CIRCLESIZE-(i-128)/ARC][1] = circle[(i-128)/ARC][1];
    }
  }
  /*for (i=0;i<=3.2;i+=.2) {
    circle[(int)(i/.2)][0] = cos(i);
    circle[(int)(i/.2)][1] = sin(i);
    if (i > 0 && i <3.2){
      circle[16-(int)(i/.2)][0] = -circle[(int)(i/.2)][0];
      circle[16-(int)(i/.2)][1] = circle[(int)(i/.2)][1];
    }
  }*/
	
	PA_Init3D();
	PA_Init3DDrawing (0,0,5,0,0,0);

	initImages();

  PA_InitText(1, 0);

	while (1) { // Infinite loop to keep the program running
    scrollX += (Pad.Held.Right - Pad.Held.Left)*4.0;
	  scrollY += (Pad.Held.Down - Pad.Held.Up)*4.0;   //background scrolling :w-hat:

    PA_OutputText(1, 1, 1, "Stylus X: %d   Y:%d  ", Stylus.X,Stylus.Y);
    PA_OutputText(1, 1, 2, "3d X: %f   Y:%f  ", (Stylus.X-130)/25.0,(100-Stylus.Y)/25.0);
    PA_OutputText(1, 1, 3, "obj_end %d ", obj_end);
    PA_OutputText(1, 1, 4, "body size %d       ", bods>0?bodies[bods-1]->size:0);
    PA_OutputText(1, 1, 5, "Draw type %s      ", drawType==0?"body":drawType==1?"square":drawType==2?"circle":"triangle");
	  PA_EasyBgScrollXY(1,3,scrollX,scrollY);

//--set the drawing mode--
    if (Pad.Newpress.A)
      drawType = SOFT;
    if (Pad.Newpress.B)
      drawType = RECTANGLE;
    if (Pad.Newpress.X)
      drawType = CIRCLE;
    if (Pad.Newpress.Y)
      drawType = TRIANGLE;

    if (Pad.Newpress.Select)
      bodyDestruct(bodies,&bods);

    if (bods==MAX_BODIES) {
      obj_end = 0;
    }else{ //allow player to start drawing new shape
      if (Stylus.Newpress)
        draw.type = drawType;

      drawTemp(&draw,&obj_end,&oldStyX,&oldStyY,circle);

      if (!Stylus.Held && obj_end>0) {  //Add to our Hash table-----
        new = malloc(sizeof(body));
        if (draw.type==SOFT) {
          new->midX /= obj_end-1;
          new->midY /= obj_end-1;
          new->width = PA_TrueDistance(new->midX,new->midY,new->verts[0][0],new->verts[0][1]);
          for (i=1;i<obj_end;i++) {
            new->height = PA_TrueDistance(new->midX,new->midY,new->verts[0][0],new->verts[0][1]);
            if (new->height > new->width)
              new->width = new->height;
          }
        }
        bodyInit(new,obj_end);
        bodyCopy(new,&draw);
        makeLocal(new);
        bodies[bods] = new;
        bods++;
        obj_end = 0;
      }
    }  //end of new shape draw and add

    if (bods>0)  doPhysics(bodies,bods); //Run Physics calculations


		PA_WaitForVBL(); //now we can start doing GL stuff
		
    drawScene();  //background
    if (bods>0) renderBodies(bodies,bods); 

    renderTemp(&draw,obj_end);

	}
  //deallocate editor specific data when switching to sim field and back to menu
}//---End Editor Control---

//---Center Calculation---
void makeLocal(body *bod) {
  s16 i;
  bod->midX = 0;
  bod->midY = 0;
  for (i=0;i < bod->size;i++) {
    bod->midX += bod->verts[i][0];
    bod->midY += bod->verts[i][1];
  }
  bod->midX = (bod->midX)/(bod->size);
  bod->midY = (bod->midY)/(bod->size);
  
  for (i=0;i < bod->size;i++) {
    bod->verts[i][0] = bod->verts[i][0] - bod->midX;
    bod->verts[i][1] = bod->verts[i][1] - bod->midY;
  }
}

//---Scrolling---
void drawTemp(body *draw,s16 *obj_end,s32 *oldStyX,s32 *oldStyY,double circle[CIRCLESIZE][2]) {
  s16 i;
  double styX,styY;
  if (draw->type==SOFT) {  //Free bodies----------------------
    if (Stylus.Newpress) {
      styX = (Stylus.X-130)/25.0;
      styY = (100-Stylus.Y)/25.0;
      draw->verts[0][0] = styX;
      draw->verts[0][1] = styY;
      draw->midX = styX;
      draw->midY = styY;
      *obj_end = 1;

      *oldStyX = Stylus.X;
      *oldStyY = Stylus.Y;
   }
    if (*obj_end>0 && *obj_end<draw->size && Stylus.Held &&
        (Stylus.X - *oldStyX)*(Stylus.X - *oldStyX) > 4 &&
        (Stylus.Y - *oldStyY)*(Stylus.Y - *oldStyY) > 4) {
      styX = (Stylus.X-130)/25.0;
      styY = (100-Stylus.Y)/25.0;
      draw->verts[*obj_end][0] = styX;
      draw->verts[*obj_end][1] = styY;
      draw->midX += styX;
      draw->midY += styY;
      *obj_end = *obj_end+1;

      *oldStyX = Stylus.X;
      *oldStyY = Stylus.Y;
    }
  }
  if (draw->type==RECTANGLE) {  //Rectangles----------------------
    if (Stylus.Newpress) {
      draw->midX = (Stylus.X-130)/25.0;
      draw->midY = (100-Stylus.Y)/25.0;

      *obj_end = 4;
    }
    if (Stylus.Held) {
      draw->width = (Stylus.X-130)/25.0 - draw->midX;
      draw->height = (100-Stylus.Y)/25.0 - draw->midY;
      draw->width = draw->width>1.5?1.5:draw->width<-1.5?-1.5:draw->width;
      draw->height = draw->height>1.5?1.5:draw->height<-1.5?-1.5:draw->height;

      draw->verts[0][0] = draw->midX + draw->width - (draw->height)/2;
      draw->verts[0][1] = draw->midY + draw->height + (draw->width)/2;
      draw->verts[1][0] = draw->midX + draw->width + (draw->height)/2;
      draw->verts[1][1] = draw->midY + draw->height - (draw->width)/2;
      draw->verts[2][0] = draw->midX - draw->width + (draw->height)/2;
      draw->verts[2][1] = draw->midY - draw->height - (draw->width)/2;
      draw->verts[3][0] = draw->midX - draw->width - (draw->height)/2;
      draw->verts[3][1] = draw->midY - draw->height + (draw->width)/2;
    }
  }
  if (draw->type==CIRCLE) { // Circles------------------------
    if (Stylus.Newpress) {
      *oldStyX = Stylus.X;
      *oldStyY = Stylus.Y;
      draw->midX = (Stylus.X-130)/25.0;
      draw->midY = (100-Stylus.Y)/25.0;
      *obj_end = CIRCLESIZE;
     }
    if (Stylus.Held) {
      draw->width = PA_TrueDistance(Stylus.X,Stylus.Y,*oldStyX,*oldStyY)/25.0;
      draw->width = draw->width>1.5?1.5:draw->width;
      for (i=0;i<*obj_end;i++) {
        draw->verts[i][0] = draw->midX + circle[i][0]*draw->width;
        draw->verts[i][1] = draw->midY + circle[i][1]*draw->width;
      }
    }
  }
  if (draw->type==TRIANGLE) { // Triangles------------------------
    if (Stylus.Newpress) {
      draw->midX = (Stylus.X-130)/25.0;
      draw->midY = (100-Stylus.Y)/25.0;
      *obj_end = 3;
    }
    if (Stylus.Held) {
      draw->width = (Stylus.X-130)/25.0 - draw->midX;
      draw->height = (100-Stylus.Y)/25.0 - draw->midY;
      draw->width = draw->width>1.5?1.5:draw->width<-1.5?-1.5:draw->width;
      draw->height = draw->height>1.5?1.5:draw->height<-1.5?-1.5:draw->height;

      draw->verts[0][0] = draw->midX + draw->width;
      draw->verts[0][1] = draw->midY + draw->height;
      draw->verts[1][0] = draw->midX - draw->height;
      draw->verts[1][1] = draw->midY + draw->width;
      draw->verts[2][0] = draw->midX + draw->height;
      draw->verts[2][1] = draw->midY - draw->width;
    }
  }
}

/*
    if (Stylus.Newpress) {
      draw->verts[0][0] = (Stylus.X-130)/25.0;
      draw->verts[0][1] = (100-Stylus.Y)/25.0;

      draw->verts[1][1] = (100-Stylus.Y)/25.0;


      draw->verts[3][0] = (Stylus.X-130)/25.0;

      *obj_end = 4;
    }
    if (Stylus.Held) {
      draw->width = (Stylus.X-130)/25.0 - draw->verts[0][0];
      draw->height = (100-Stylus.Y)/25.0 - draw->verts[0][1];
      draw->width = draw->width>1.5?1.5:draw->width<-1.5?-1.5:draw->width;
      draw->height = draw->height>1.5?1.5:draw->height<-1.5?-1.5:draw->height;


      draw->verts[1][0] = draw->width + draw->verts[0][0];

      draw->verts[2][0] = draw->width + draw->verts[0][0];
      draw->verts[2][1] = draw->height + draw->verts[0][1];

      draw->verts[3][1] = draw->height + draw->verts[0][1];
    }
*/
