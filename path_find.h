namespace octet {

  /// Scene containing a box with octet.
  class path_find : public resource {

    searchgrid *start_cell;
    searchgrid *goal_cell;
    bool *isvalid;
    float *y_coord;
    std::vector<searchgrid *> openlist;
    std::vector<searchgrid *> visitedlist;
    std::vector<vec3> path_goal;
    
    void setstartandgoal(searchgrid start,searchgrid goal);
    void pathopen(int x,float y,int z, float newcost, searchgrid *parent);
    searchgrid* getnextgrid();
    void continuepath();
    
    public :
      void set_map(bool map[],float _y_coord[])
      {
        isinit=false;
        found_goal=false;
        isvalid = map;        
        y_coord = _y_coord;
      }
      void findpath(vec3 currpos, vec3 tarpos);  
      vec3 nextpathpos(vec3 position,float mindist);
      bool isinit;
      bool found_goal;
  };

  void path_find::findpath(vec3 currpos, vec3 tarpos)
  {
    if (!isinit)
    {
      for (int i=0;i<openlist.size();i++)
      {
        delete openlist[i];
      }
      openlist.clear();

      for (int i=0;i<visitedlist.size();i++)
      {
        delete visitedlist[i];
      }
      visitedlist.clear();
     
      path_goal.clear();

      searchgrid start;
      start.x_coord = currpos.x();
      start.y_coord = currpos.y();
      start.z_coord = currpos.z();
      searchgrid goal;
      goal.x_coord = tarpos.x();
      goal.y_coord = tarpos.y();
      goal.z_coord = tarpos.z();

      setstartandgoal(start,goal);
      isinit=true;
      }
    if (isinit)
    {
      
      continuepath();
    }    
  }

  void path_find::setstartandgoal(searchgrid start, searchgrid goal)
  {
    start_cell = new searchgrid(start.x_coord,start.y_coord,start.z_coord,NULL);
    goal_cell = new searchgrid(goal.x_coord,goal.y_coord,goal.z_coord,&goal);
    start_cell->cumalativedistance =0;
    start_cell->heuristic = start_cell->chebyshevdistance(goal_cell);
    openlist.push_back(start_cell);

  }

  searchgrid* path_find::getnextgrid()
  {
   float bestF= 9999999999.0f;
    int cellidx = -1;
    searchgrid *nextcell = NULL;
    for (int i=0;i<openlist.size();i++)
    {
      if (openlist[i]->getf() <bestF)
      {
        bestF = openlist[i]->getf();
        cellidx = i;
      }
    }

    if (cellidx>=0)
    {
      nextcell = openlist[cellidx];
      visitedlist.push_back(nextcell);
      openlist.erase(openlist.begin() + cellidx);
      }

      return nextcell;
    }

  void path_find::pathopen(int x,float y,int z,float newcost,searchgrid *parent)
  {
    bool is_cell_valid = *(isvalid+x * width_image + z);
    if (!is_cell_valid || x>width_image || z>height_image || x<0 || z<0)
    {
      return;
    }

    int id = x*width_image+z;//width_image+z;
    for (int i=0;i<visitedlist.size();i++)
    {
      if (id==visitedlist[i]->id)
      {
        return;
      }
    }

    searchgrid *newchild = new searchgrid(x, *(y_coord+x * width_image + z), z, parent);
    newchild->cumalativedistance = newcost;
    newchild->heuristic = newchild->chebyshevdistance(goal_cell);
    float newf = newchild->getf();
    for (int i = 0; i < openlist.size(); i++)
    {   
      if (openlist[i]->getf()>newf)
      {
        openlist[i]->cumalativedistance = newchild->cumalativedistance+newcost;
        openlist[i]->parent = newchild;
      }
      else
      {
        delete newchild;
        return;
      }
    }
    openlist.push_back(newchild);
  }

  void path_find::continuepath()
  {
    if (openlist.empty())
    {
      return;
    }

    searchgrid *currentcell = getnextgrid();
    if ( currentcell->id == goal_cell->id)
    {
      goal_cell->parent = currentcell->parent;
      searchgrid *get_path;
      
      for (get_path = goal_cell; get_path!=NULL;get_path=get_path->parent)
      {
        path_goal.push_back(vec3(get_path->x_coord,get_path->y_coord,get_path->z_coord));
      }
      found_goal=true;
    }
    else
    {
      pathopen(currentcell->x_coord+1,currentcell->y_coord,currentcell->z_coord,currentcell->cumalativedistance+1,currentcell);
      pathopen(currentcell->x_coord - 1, currentcell->y_coord, currentcell->z_coord, currentcell->cumalativedistance + 1, currentcell);
      pathopen(currentcell->x_coord , currentcell->y_coord, currentcell->z_coord+1, currentcell->cumalativedistance + 1, currentcell);
      pathopen(currentcell->x_coord , currentcell->y_coord, currentcell->z_coord-1, currentcell->cumalativedistance + 1, currentcell);

      pathopen(currentcell->x_coord + 1, currentcell->y_coord, currentcell->z_coord+1, currentcell->cumalativedistance + 1.414f, currentcell);
      pathopen(currentcell->x_coord + 1, currentcell->y_coord, currentcell->z_coord-1, currentcell->cumalativedistance + 1.414f, currentcell);
      pathopen(currentcell->x_coord - 1, currentcell->y_coord, currentcell->z_coord+1, currentcell->cumalativedistance + 1.414f, currentcell);
      pathopen(currentcell->x_coord - 1, currentcell->y_coord, currentcell->z_coord-1, currentcell->cumalativedistance + 1.414f, currentcell);

      for (int i = 0; i<openlist.size();i++)
      {
        if (currentcell->id == openlist[i]->id)
        {
          openlist.erase(openlist.begin()+i);
        }
      }
      continuepath();
    }   
   }


  vec3 path_find::nextpathpos(vec3 position, float mindist)
  {
    vec3 nextpos;
    nextpos.x() = path_goal[path_goal.size() - 1].x();
    nextpos.y() = path_goal[path_goal.size() - 1].y();
    nextpos.z() = path_goal[path_goal.size() - 1].z();

    vec3 dist = nextpos - position;

    if (path_goal.size()>1)
    {
      if (dist.length()<mindist)
      {
        path_goal.erase(path_goal.end()-1);
      }
    }
    return nextpos;
  }
 }

  