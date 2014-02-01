#include <PA9.h>       // Include for PA_Lib
#include <math.h>
#include "data.h"#include "phys.h"
#define SOFT 0
#define RECTANGLE 1
#define CIRCLE 2
#define TRIANGLE 3
#define LIMIT .5

//---Run Physics calculations on Hash Table---
void doPhysics(body *b[],s16 s) {
  s16 i,j;
  double lowest = 0;

  for (j=0;j<s;j++) {
    lowest = b[j]->verts[0][1];

//probably going to want to split up the collision detection and response 
//so that we can make use of simple detections to keep the complexity down
    APosterioriCollDet(b,j,s);

    //find generic low
    for (i=1;i<(b[j]->size);i++) {
      if (lowest >  b[j]->verts[i][1])
        lowest = b[j]->verts[i][1];
    }
    lowest += b[j]->midY;

    //above ground else below ground
    if (lowest > -3) 
      b[j]->veloc[0][1] -= .025;
    else if ((b[j]->veloc[0][1])<0)
      b[j]->veloc[0][1] = -.75*(b[j]->veloc[0][1]);

    //on ground (friction stuff)
    if (lowest>-3.05 && lowest<-2.95) {
      b[j]->veloc[0][0] *= .9;
      if ((b[j]->veloc[0][0])>-.05 && (b[j]->veloc[0][0])<.05)
        b[j]->veloc[0][0] = 0;
      if ((b[j]->veloc[0][1])>-.05 && (b[j]->veloc[0][1])<.05)
        b[j]->veloc[0][1] = 0;
    }

    //left and right walls
    if (b[j]->midX < -5) {
      b[j]->midX = -5;
      if (b[j]->veloc[0][0] < 0)
        b[j]->veloc[0][0] = -.75*(b[j]->veloc[0][0]);
    }
    if (b[j]->midX > 5) {
      b[j]->midX = 5;
      if (b[j]->veloc[0][0] > 0)
        b[j]->veloc[0][0] = -.75*(b[j]->veloc[0][0]);
    }

    //ceiling
    if (b[j]->midY > 5) {
      b[j]->midY = 5;
      b[j]->veloc[0][1] = -.025;
    }

    //min/max on velocity
    b[j]->veloc[0][0] = b[j]->veloc[0][0]>LIMIT?LIMIT:b[j]->veloc[0][0]<-LIMIT?-LIMIT:b[j]->veloc[0][0];
    b[j]->veloc[0][1] = b[j]->veloc[0][1]>LIMIT?LIMIT:b[j]->veloc[0][1]<-LIMIT?-LIMIT:b[j]->veloc[0][1];

    //apply velocity to verteces
    b[j]->midX += b[j]->veloc[0][0];
    b[j]->midY += b[j]->veloc[0][1];
  }
}
//---Collision Detection---
//---Before Collision
void APrioriCollDet(body *b[],s16 j,s16 s) {
  s16 i,v,w,inside;
//A = line on body, P = line to point, d = comparison determinant
  /*double Ax,Ay,Px,Py,d,e;*/
  //loop through other bodies
  for (i=0;i<s;i++) {
    if (i!=j) {

      //if a point in other body lies within our bounding box
      if (CollDetCircle(b[j],b[i],.1)) {
        for (v=0;v<b[j]->size;v++) {
        //find point of intersection of point to this moving edge (plane)

          for (w=0;w<b[i]->size-1;w++) {
      //determine if intersection point is inside actual movement
//  test for dot product of AB Ap, BC Bp, CD Cp, and DA Dp
//  if <0 (to the right) point is inside

          }
          if (inside) {//collision

// begin temp final velocity calc
        //transform Xp to XV(by negative supplement of XV)
        //body impact = dx of Xp'
        //rotation = dy of Xp'
        //transform these back to world (center along velocity XV)
// end temp final velocity calc

        //add to current velocities
          }
        }
      }
    }
  }
}
//---After Collision---
void APosterioriCollDet(body *b[],s16 j,s16 s) {
  s16 i,/*v,w,*/inside;
//A = line on body, P = line to point, d = comparison determinant
  double /*Ax,Ay,Px,Py,e,*/d,Vo1,Vo2,V1,V2,nx,ny,tx,ty,m1,m2;
  //loop through other bodies
  for (i=0;i<s;i++) {
    //if a point in other body lies within this body 
//  test for dot product of AB Ap, BC Bp, ... NA Np,
//  if <0 (to the right) point is inside
    if (i!=j) {
      inside = 0;
      if (CollDetCircle(b[j],b[i],.1)) {
        /*for (v=0;v<b[i]->size;v++) {
          inside = 1;
          w = b[j]->size-1;
          Ax = b[j]->verts[0][0]-b[j]->verts[w][0];
          Ay = +b[j]->verts[0][1]-b[j]->verts[w][1];
          Px = (b[i]->midX+b[i]->verts[v][0])-(b[j]->midX+b[j]->verts[w][0]);
          Py = (b[i]->midY+b[i]->verts[v][1])-(b[j]->midY+b[j]->verts[w][1]);
          d = determinant(Ax,  Ay,
                          Px,  Py);
          for (w=0;w<b[j]->size-1 && inside;w++) {
            Ax = b[j]->verts[0][0]-b[j]->verts[w][0];
            Ay = +b[j]->verts[0][1]-b[j]->verts[w][1];
            Px = (b[i]->midX+b[i]->verts[v][0])-(b[j]->midX+b[j]->verts[w][0]);
            Py = (b[i]->midY+b[i]->verts[v][1])-(b[j]->midY+b[j]->verts[w][1]);
            e = determinant(Ax,  Ay,
                            Px,  Py);
            if ((d>0 && e<0) || (d<0 && e>0))
              inside = 0;
          }
          if (inside) {*/
          if (b[j]->type == b[i]->type) {
            if (b[j]->type == RECTANGLE)
              inside = CollDetRectangle(b[j],b[i]);
            if (b[j]->type == CIRCLE)
              inside = CollDetCircle(b[j],b[i],0);
            if (b[j]->type == TRIANGLE)
              inside = CollDetTriangle(b[j],b[i]);
          }else
              inside = CollDetLooseBody(b[j],b[i]);
          if (inside) {//collision

              //dx = b[j]->midX - b[i]->midX;
              //dy = b[j]->midY - b[i]->midY;
              //b[j]->veloc[0][0] = dx*.05;
              //b[j]->veloc[0][1] = dy*.05;
              //b[i]->veloc[0][0] = -dx*.05;
              //b[i]veloc[0][1] = -dy*.05;
//--end temp collision calculations

//--elastic collision calculations

  m1 = b[j]->size * (4*b[j]->width);
  m2 = b[i]->size * (4*b[i]->width);
  //find normal and tangent vectors of impact
    //fuck it, we'll use midpoints for this one 
  //normal and tangent vector
  nx = b[j]->midX - b[i]->midX;
  ny = b[j]->midY - b[i]->midY;
  ty = nx;
  tx = -ny;
  d = (nx*nx)+(ny*ny);
  d = d/fabs(d);
  nx = nx/d;
  ny = ny/d;
  //project velocities of the bodies onto these vectors
  Vo1 = b[j]->veloc[0][0]*nx + b[j]->veloc[0][1]*ny;
  Vo2 = b[i]->veloc[0][0]*nx + b[i]->veloc[0][1]*ny;

  //do one dimensional collision calculations on normal vector
  V1 = Vo1*(m1 - m2)/(m1 + m2) + Vo2*(2*m1)/(m1 + m2);
  V2 = Vo1*(2*m1)/(m1 + m2) + Vo2*(m2 - m1)/(m1 + m2);

  //multiply by the unit normal, then add that with the tangential to get final velocities
  b[j]->veloc[0][0] = .75*(V1*nx + tx);
  b[j]->veloc[0][1] = .75*(V1*ny + ty);

  //still need to add velocity buffer and collision pairings
  //b[i]->veloc[0][0] = V2*nx + tx;
  //b[i]->veloc[0][1] = V2*ny + ty;

//--end perfectly elastic
          }//--end collision
        //}//--end for loops
      }
    }//--end not same body
  }
}
//--detect Circle collision---
int CollDetCircle(body *b,body *a,s16 r) {
  double dx,dy;
  //circle to circle
  dx = a->midX - b->midX;
  dy = a->midY - b->midY;
  if ((dx*dx+dy*dy) < (r+b->width+a->width)*(r+b->width+a->width)) {
    return 1;
  }

  return 0;
}
//---Run collisions on Circle---
//keep for now just for reference in pushing bodies off one another
void doPhysicsCircle(body *b[],s16 j,s16 s) {
  s16 i;
  double dx,dy,dist,radii;
  //circle to circle
  for (i=0;i<s;i++) {
    if (i!=j && b[i]->type==CIRCLE) {
      dx = b[j]->midX - b[i]->midX;
      dy = b[j]->midY - b[i]->midY;
      dist = (dx*dx+dy*dy);
      radii = (b[j]->width+b[i]->width)*(b[j]->width+b[i]->width);
      if (dist < radii) {
        dist = sqrt(dist);
        radii = sqrt(radii);
        b[j]->veloc[0][0] = (dx/fabs(dx))*(.025+radii-dist)*.75*((b[i]->width*.75+b[j]->width*.25)/b[j]->width);
        b[j]->veloc[0][1] = (dy/fabs(dy))*(.025+radii-dist)*.75*((b[i]->width*.75+b[j]->width*.25)/b[j]->width);
      }
    }
  }
}
//---Run collisions on Triangle---
int CollDetTriangle(body *b,body *a) {
	s16 v;
  double ab,bc,ca,Ax,Ay,Bx,By;
  for (v=0;v<a->size;v++) {
    Ax = (b->midX + b->verts[1][0])-(b->midX + b->verts[0][0]);
    Ay = (b->midY + b->verts[1][1])-(b->midY + b->verts[0][1]);
    Bx = (a->midX + a->verts[v][0])-(b->midX + b->verts[0][0]);
    By = (a->midY + a->verts[v][1])-(b->midY + b->verts[0][1]);
    ab = determinant(Ax,  Ay,
                      Bx,  By);

    Ax = (b->midX + b->verts[2][0])-(b->midX + b->verts[1][0]);
    Ay = (b->midY + b->verts[2][1])-(b->midY + b->verts[1][1]);
    Bx = (a->midX + a->verts[v][0])-(b->midX + b->verts[1][0]);
    By = (a->midY + a->verts[v][1])-(b->midY + b->verts[1][1]);
    bc = determinant(Ax,  Ay,
                      Bx,  By);

    Ax = (b->midX + b->verts[0][0])-(b->midX + b->verts[2][0]);
    Ay = (b->midY + b->verts[0][1])-(b->midY + b->verts[2][1]);
    Bx = (a->midX + a->verts[v][0])-(b->midX + b->verts[2][0]);
    By = (a->midY + a->verts[v][1])-(b->midY + b->verts[2][1]);
    ca = determinant(Ax,  Ay,
                      Bx,  By);

    if ((ab>0 && bc>0 && ca>0) || (ab<0 && bc<0 && ca<0)) {
      return 1;
    }
  }
  return 0;
}
//---Run collisions on Rectangle---
int CollDetRectangle(body *b,body *a) {
  double xo,yo,xt,yt,wt,ht,wo,ho;
  //rectangle to rectangle
  if (a->type==RECTANGLE) {
    xo = b->midX;
    xt = a->midX;
    yo = b->midY;
    yt = a->midY;

    wo = b->width>0?b->width:-b->width;
    wt = a->width>0?a->width:-a->width;
    ho = b->height>0?b->height:-b->height;
    ht = a->height>0?a->height:-a->height;
    if ((xt >= xo - (wt + wo)/2) &&
        (xt <= xo + (wt + wo)/2) &&
        (yt >= yo - (ht + ho)/2) &&
        (yt <= yo + (ht + ho)/2)) {
      return 1;
    }
  }
  return 0;
}
//---Run Physics calculations on Loose System---
int CollDetLooseBody(body *b,body *a) {
  s16 v,w,inside;
//A = line on body, P = line to point, d = comparison determinant
  double Ax,Ay,Px,Py,d,e;
  //loop through other bodies
    //if a point in other body lies within this body 
//  test for dot product of AB Ap, BC Bp, ... NA Np,
//  if <0 (to the right) point is inside
  inside = 0;
  for (v=0;v<b->size;v++) {
    inside = 1;
    w = b->size-1;
    Ax = b->verts[0][0] - b->verts[w][0];
    Ay = +b->verts[0][1] - b->verts[w][1];
    Px = (a->midX + a->verts[v][0])-(b->midX + b->verts[w][0]);
    Py = (a->midY + a->verts[v][1])-(b->midY + b->verts[w][1]);
    d = determinant(Ax,  Ay,
                    Px,  Py);
    for (w=0;w<b->size-1 && inside;w++) {
      Ax = b->verts[0][0] - b->verts[w][0];
      Ay = +b->verts[0][1] - b->verts[w][1];
      Px = (a->midX + a->verts[v][0])-(b->midX + b->verts[w][0]);
      Py = (a->midY + a->verts[v][1])-(b->midY + b->verts[w][1]);
      e = determinant(Ax,  Ay,
                      Px,  Py);
      if ((d>0 && e<0) || (d<0 && e>0))
        inside = 0;
    }
    if (inside) {
      return 1;
    }
  }
  return 0;
}
//---Transformations---
void transRotation(double n[2],const double p[2],const double rot[2]) {
  n[0] = p[0];
  n[1] = p[1];
  n[0] = p[0]*rot[0] - p[1]*rot[1];
  n[1] = p[0]*rot[1] + p[1]*rot[0];
}
//--Line Line Intersection---
//n => point of intersection, a => line 1, b => line 2
void intersect(double p[2],double a[4],double b[4]) {
  p[0] = determinant(determinant(a[0], a[1], a[2], a[3]), a[0] - a[2],
            determinant(b[0], b[1], b[2], b[3]), b[0] - b[2])
            / determinant( a[0] - a[2], a[1] - a[3], b[0] - b[2], b[1] - b[3] );

  p[1] = determinant(determinant(a[0], a[1], a[2], a[3]), a[1] - a[3],
            determinant(b[0], b[1], b[2], b[3]), b[1] - b[3])
            / determinant( a[0] - a[2], a[1] - a[3], b[0] - b[2], b[1] - b[3] );
}
//to find dot product, do (x1,-y1,-y2,x2)
double determinant(double a,double b,double c,double d) {
  return a*d-b*c;
}
