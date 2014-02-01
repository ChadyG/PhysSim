/*
  Rendering Module

  here we will put all functions that will draw to the screen
  or use any graphics data

  drawScene will draw a background
  renderBodies will draw the mass systems
*/
void initImages();
void drawScene();
void renderTemp(body *,s16);
void renderBodies(body **,s16);
