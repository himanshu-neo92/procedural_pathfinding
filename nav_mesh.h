namespace octet {

  /// Scene containing a box with octet.
  class nav_mesh : public resource {
    struct nav_vertex
    {
      vec3p pos;
      uint32_t temp_color;
    };

    static uint32_t temp_make_color(float r, float g, float b) {
      return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
    }
  public:    
  
    void create_nav_mesh(float *image,float min,float max,bool *isvalid,float *y_coord,visual_scene *app_scene)
    { 
      int count = 0;
      float diff = max - min,
        flood = 0.35f,//flood level
        rock = 0.65f; //rock level        
      flood *= diff;
      rock *= diff;
      float *temp = image;
      /*for (int i = 0; i<height_image*width_image; ++i, temp++)
      {
        if (*temp>flood && *temp<rock)
        {
          count++;
        }
      }*/

      //////the mesh generatiion
      ////param_shader *shader = new param_shader("shaders/default.vs", "shaders/simple_color.fs");
      ////material *red = new material(vec4(0.6f, 0.298f, 0.0f, 1.0f), shader);
      ////mesh *terrain = new mesh();
      ////// allocate vertices and indices into OpenGL buffers
      ////size_t num_vertices = count;
      ////size_t num_indices =0;
      ////terrain->allocate(sizeof(nav_vertex)* num_vertices, 0);
      ////terrain->set_params(sizeof(nav_vertex), num_indices, num_vertices, GL_POINTS, NULL);
      ////// describe the structure of my_vertex to OpenGL
      ////terrain->add_attribute(attribute_pos, 3, GL_FLOAT, 0);      
      ////terrain->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);
      ////gl_resource::wolock vl(terrain->get_vertices());
      ////nav_vertex *vtx = (nav_vertex *)vl.u8();
      ////gl_resource::wolock il(terrain->get_indices());
      ////uint32_t *idx = idx = il.u32();
      //////end of init
      
      for (int i = 0; i < height_image; ++i) {     // z
        for (int j = 0; j < width_image; ++j) {  // x
          

          if (*(image + i*width_image + j)>flood && *(image + i*width_image + j)<rock)
          {
              /*float vert_height = *(image + i*width_image + j)*100.0f;
             vtx->pos = vec3p((float)j, vert_height, (float)i);
             vtx->temp_color = temp_make_color(0.00f, 1.0f, 0.22f);             
             vtx++;*/
             *(isvalid+i*width_image + j) = true;
          }
          else
          {
            *(isvalid + i*width_image + j) = false;
          }

          *(y_coord + i*width_image + j) = *(image + i*width_image + j)*40.0f;
        }
      }


    /*scene_node *nav_node = new scene_node();
      app_scene->add_child(nav_node);
      app_scene->add_mesh_instance(new mesh_instance(nav_node, terrain, red));
    */}

  };
  }
  