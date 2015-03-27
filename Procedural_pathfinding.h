////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh and Himanshu Chablani
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#ifndef TERRAIN_GEN_H_INCLUDED
#define TERRAIN_GEN_H_INCLUDED

#include<iostream>
#include<fstream>

namespace octet {

  /// Scene containing a box with octet.
  class Procedural_pathfinding : public app {

    //bmp_image img;
    perlin pn;
    inputs inputs;
    bmp_image img_gen;
    UI Game_UI;
    sky_box Sky_Box;
    nav_mesh navigation_mesh;
    
    // scene for drawing box
    ref<visual_scene> app_scene;

    struct my_vertex {
      vec3p pos;
      vec3p nor;
      uint32_t color;
    };

   
    static uint32_t make_color(float r, float g, float b) {
      return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
    }
    void setup_camera()
    {
      mat4t &camera_mat = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      camera_mat.translate(150, 150, 150);
      camera_mat.rotateY(-90);
      camera_mat.rotateX(0);
    }
    void input_keys()
    {
      if (inputs.Q_KEY()){
        Game_UI.setup_pop_up(1);
        app_scene->pop_mesh_instance();
        generate(false, true);
        Game_UI.setup_pop_up(4);
      }     
     }
    vec3 matrixmult(mat4t objm, vec3 direction)
    {
      vec3 temp(objm.x().x()*direction.x() + objm.y().x()*direction.y() + objm.z().x()*direction.z(),
        objm.x().y()*direction.x() + objm.y().y()*direction.y() + objm.z().y()*direction.z(),
        objm.x().z()*direction.x() + objm.y().z()*direction.y() + objm.z().z()*direction.z());
      return temp;
    }
    vec3 raycasttomodel()
    {
      vec3 startpoint = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().xyz();
      float dist = (startpoint - vec3(128,75,128)).length();
      vec3 lookdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, -1));
      vec3 endpos = vec3(startpoint.x()+dist*lookdir.x(), startpoint.y()+dist*lookdir.y(), startpoint.z()+dist*lookdir.z());
      return endpos;
    }

    vec3 nav_start_point;
    vec3 nav_end_point;

    void castray()
    {
      if (is_key_going_down(key::key_lmb))
      {
        nav_start_point = raycasttomodel();
        if (nav_start_point.x()<width_image && nav_start_point.x()>0 && nav_start_point.z()<height_image && nav_start_point.z()>0
          && *(isvalid+(int)nav_start_point.x()*width_image + (int)nav_start_point.z()))
        {
         nav_start_point = vec3(nav_start_point.x(), *(y_coord+(int)nav_start_point.x()*width_image + (int)nav_start_point.z()), nav_start_point.z());
        printf("\n\nThe position for start : %f , %f , %f", nav_start_point.x(), nav_start_point.y(), nav_start_point.z());
        Game_UI.setstartpoint(nav_start_point);
        start_point_mesh_node->translate(vec3(nav_start_point.x() - start_point_mesh_node->access_nodeToParent().w().x(), nav_start_point.y() - start_point_mesh_node->access_nodeToParent().w().y(), nav_start_point.z() - start_point_mesh_node->access_nodeToParent().w().z()));
        }      
      }

      if (is_key_going_down(key::key_rmb))
      {
        nav_end_point = raycasttomodel();
        if (nav_end_point.x()<width_image && nav_end_point.x()>0 && nav_end_point.z()<height_image && nav_end_point.z()>0 
        && *(isvalid+(int)nav_end_point.x()*width_image+(int)nav_end_point.z()))
        {
          nav_end_point = vec3(nav_end_point.x(), *(y_coord+(int)nav_end_point.x()*width_image + (int)nav_end_point.z()), nav_end_point.z());
        printf("\n\nThe position for end : %f , %f , %f", nav_end_point.x(), nav_end_point.y(), nav_end_point.z());
        Game_UI.setendpoint(nav_end_point);
        end_point_mesh_node->translate(vec3(nav_end_point.x() - end_point_mesh_node->access_nodeToParent().w().x(), nav_end_point.y() - end_point_mesh_node->access_nodeToParent().w().y(), nav_end_point.z() - end_point_mesh_node->access_nodeToParent().w().z()));
        }      
      }
    }

    void simulate_pathfinding()
      {
        if (is_key_going_down(key::key_f1))
        {
          navigation_started=true;
          //pathfinder.findpath(nav_start_point, nav_end_point);//change
          pathfinder.findpath(nav_start_point, nav_end_point);
          printf("\n\nFinding path");
          temp = nav_start_point;
          ai_bot_mesh_node->translate(vec3(nav_start_point.x() - ai_bot_mesh_node->access_nodeToParent().w().x(),
            nav_start_point.y() - ai_bot_mesh_node->access_nodeToParent().w().y(),
            nav_start_point.z() - ai_bot_mesh_node->access_nodeToParent().w().z()));
        }
                  
        if (pathfinder.found_goal && ((int)temp.z() != (int)nav_end_point.z() || (int)temp.x() != (int)nav_end_point.x() ))//change
        {          
          pathfinder.isinit=false;
          vec3 point_to_go = pathfinder.nextpathpos(ai_bot_mesh_node->access_nodeToParent().w().xyz(), 1.0f);//replace temp with bot pos and move him
          printf("\n\nNext point is : %f , %f , %f", point_to_go.x(), point_to_go.y(), point_to_go.z());
            temp=point_to_go;//remove 
           
            ai_bot_mesh_node->translate(vec3((point_to_go.x() - ai_bot_mesh_node->access_nodeToParent().w().x())/10,
              (point_to_go.y() - ai_bot_mesh_node->access_nodeToParent().w().y())/10,
              (point_to_go.z() - ai_bot_mesh_node->access_nodeToParent().w().z())/10));
            Game_UI.setnextpoint(point_to_go);
         // nav_start_point = point_to_go;
        }

      }
    bool *isvalid;
    float *y_coord;
    float *image;
    vec3 temp = vec3(0,0,1);//only temp
    bool navigation_started=false;
    scene_node *start_point_mesh_node;
    scene_node *end_point_mesh_node;
    scene_node *ai_bot_mesh_node;
    path_find pathfinder;
    
  public:
    /// this is called when we construct the class before everything is initialised.
    Procedural_pathfinding(int argc, char **argv) : app(argc, argv) {
    }

    ~Procedural_pathfinding()
    {
      delete[]isvalid;
      delete[]y_coord;
      delete[]image;
    }
    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(10000);
      
      mesh_sphere *start_point_mesh;
      mesh_sphere *end_point_mesh;
      mesh_cylinder *ai_bot_mesh;
      material *start_mat = new material(vec4(1.0f, 0.0f, 0.0f, 1));
      material *end_mat = new material(vec4(0.0f, 1.0f, 0.0f, 1));
      material *ai_mat = new material(vec4(0.0f, 0.0f, 1.0f, 1));

      setup_camera();
      Sky_Box.InitDayNightCycle(app_scene);

      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      Game_UI.initUI(vx, vy);

    

      start_point_mesh_node = new scene_node();
      start_point_mesh = new mesh_sphere(vec3(0.0f,0.0f,0.0f),5.0f);
      app_scene->add_mesh_instance(new mesh_instance(start_point_mesh_node, start_point_mesh, start_mat));
    
      end_point_mesh_node = new scene_node();
      end_point_mesh = new mesh_sphere(vec3(0.0f, 0.0f, 0.0f), 5.0f);
      app_scene->add_mesh_instance(new mesh_instance(end_point_mesh_node, end_point_mesh, end_mat));


      ai_bot_mesh_node = new scene_node();
      ai_bot_mesh = new mesh_cylinder(vec3(0.0f, 0.0f, 0.0f), 5.0f);
      app_scene->add_mesh_instance(new mesh_instance(ai_bot_mesh_node, ai_bot_mesh, ai_mat));


      inputs.init(this);
      img_gen.init();
      generate(false, false);
      
    }
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);
      // draw the scene
      app_scene->render((float)vx / vy);

      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      //run key_presses loop to check for inputs
      inputs.key_presses(camera);
      inputs.mouse_control(camera);

      Sky_Box.DayNightCycle(app_scene);
      Game_UI.updateUI(vx, vy);
      Game_UI.pop_up_clear();
      input_keys();
      castray();
      simulate_pathfinding();
    }
   
    void generate(bool from_image, bool random_seed){

      //the mesh generatiion
      param_shader *shader = new param_shader("shaders/default.vs", "shaders/simple_color.fs");
      material *red = new material(vec4(0.6f, 0.298f, 0.0f, 1.0f), shader);
      mesh *terrain = new mesh();
      // allocate vertices and indices into OpenGL buffers
      size_t num_vertices = height_image * width_image;
      size_t num_indices = 6 * (num_vertices - height_image) - width_image;
      terrain->allocate(sizeof(my_vertex)* num_vertices, sizeof(uint32_t)* num_indices);
      terrain->set_params(sizeof(my_vertex), num_indices, num_vertices, GL_TRIANGLES, GL_UNSIGNED_INT);
      // describe the structure of my_vertex to OpenGL
      terrain->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
      terrain->add_attribute(attribute_normal, 3, GL_FLOAT, 12);
      terrain->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 24, GL_TRUE);
      gl_resource::wolock vl(terrain->get_vertices());
      my_vertex *vtx = (my_vertex *)vl.u8();
      gl_resource::wolock il(terrain->get_indices());
      uint32_t *idx = idx = il.u32();
      //end of init


      float min = 0.0f, max = 0.0f;
      image = new float[height_image*width_image];     
      isvalid = new bool[height_image*width_image];
      y_coord = new float[height_image*width_image];
        pn.fill_image(image, min, max, 16); 
        
        

      // Visit every pixel of the image and assign a color generated with Perlin noise
      for (int i = 0; i < height_image; ++i) {     // z
        for (int j = 0; j < width_image; ++j) {  // x
          

            vec3 mesh_colour = img_gen.create_colour(*(image + i*width_image + j), min, max);
            vtx->color = make_color(mesh_colour.x(), mesh_colour.y(), mesh_colour.z());
            float vert_height = *(image + i*width_image + j)*40.0f;
            vtx->pos = vec3p((float)j, vert_height, (float)i);
            vtx->nor = vec3p((float)j, vert_height, (float)i);        
                       
          vtx++;
        }
      }
      //make triangles          
      int tempvert = 0;
      for (int i = 0; i < height_image - 1; ++i) {
        for (int j = 0; j < width_image - 1; ++j) {

          idx[0] = tempvert;
          idx[1] = tempvert + 1;
          idx[2] = tempvert + width_image + 1;
          idx += 3;

          idx[0] = tempvert;
          idx[1] = tempvert + width_image + 1;
          idx[2] = tempvert + width_image;
          idx += 3;

          tempvert++;
        }
        tempvert++;
      }

     
      
      
      navigation_mesh.create_nav_mesh(image, min, max , isvalid,y_coord, app_scene);
      
     /* bool *tempvalid = new bool[256] { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, 
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                        true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,
                                        true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,
                                        true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,
                                        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
                                        };
      pathfinder.set_map(tempvalid);*/
      
      pathfinder.set_map(isvalid,y_coord); 

      //pathfinder.findpath(vec3(15,0,30),vec3(800,0,45));
      //set the start and end point for the ai using raycasting on the mesh
      //pass the values to path finding and find the path
      //if found_path == true 
      //get the path move the ai on the path nodes form nextpathpos and move the bot

      scene_node *node = new scene_node();
      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, terrain, red));

    }


  };
}
#endif