/*
  Physics Module
  we will only interface the doPhysics() function
  it will handle the rest
*/
void doPhysics(body **,s16);
void doPhysicsCircle(body **,s16,s16);  //reference
void APosterioriCollDet(body **,s16,s16); //bodies, current index, total bodies
void APrioriCollDet(body **,s16,s16);
int CollDetCircle(body *,body *,s16);
int CollDetRectangle(body *,body *);
int CollDetTriangle(body *,body *);
int CollDetLooseBody(body *,body *);

void transRotation(double [2],const double [2],const double [2]);
void intersect(double [2],double [4],double [4]);
double determinant(double,double,double,double);

//double findLowest (double,double,double);
//double findHighest (double,double,double);
