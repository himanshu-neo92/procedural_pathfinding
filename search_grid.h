struct searchgrid
{
  int x_coord;
  float y_coord;
  int z_coord;
  unsigned int id;
  searchgrid *parent;
  float cumalativedistance;
  float heuristic;

  searchgrid() : parent(NULL){}
  searchgrid(int x, float y, int z, searchgrid *_parent = NULL)
  {
    x_coord = x;
    y_coord = y;
    z_coord = z;
    parent = _parent;
    id = x*width_image+z;//width_image + z;
    cumalativedistance = 0;
    heuristic = 0;
  }

  float getf()
  {
    return cumalativedistance + heuristic;
  }

  float chebyshevdistance(searchgrid *nodeend)
  {
    float x = (float)(fabs(float(this->x_coord - nodeend->x_coord)));
    float z = (float)(fabs(float(this->z_coord - nodeend->z_coord)));
    return x>z?x:z;
  }
};