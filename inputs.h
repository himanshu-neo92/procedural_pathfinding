////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh and Himanshu Chablani
//

#ifndef INPUTS_H_INCLUDED
#define INPUTS_H_INCLUDED
namespace octet {
  /// Scene containing a box with octet.
  class inputs : public resource {
    // scene for drawing box
    ref<visual_scene> app_scene;
  public:
    /// this is called when we construct the class before everything is initialised.
    inputs() {
    }

    app *the_app;
    void init(app *app){
      this->the_app = app;
    }

    ///these keys are used for geenerating the terrain

    //pressing Q generates the terrain using a random seed 
    bool Q_KEY(){
      if (the_app->is_key_going_down('Q')){
        return true;
      }
      return false;
    }

    //Pressing R generates the terrain reading in from a .PPM file, therefore not reading in from random seed
    bool R_KEY(){
      if (the_app->is_key_going_down('R')){
        return true;
      }
      return false;
    }

    //pressing P generates the terrain using ken perlins pseudo-random permutation table 
    bool P_KEY(){
      if (the_app->is_key_going_down('P')){
        return true;
      }
      return false;
    }

    bool G_KEY(){
      if (the_app->is_key_going_down('G')){
        return true;
      }
      return false;
    }

    bool PLUS_KEY(){
      if (the_app->is_key_going_down(43)){
        return true;
      }
      return false;
    }


    bool MINUS_KEY(){
      if (the_app->is_key_going_down(45)){
        return true;
      }
      return false;
    }
    //misc. key presses such as camera movement and closing the program
    void key_presses(mat4t &camera){

      if (the_app->is_key_down(key_esc)){
        exit(0);
      }

      //camera movement keys (arrow keys and WASD)
      if (the_app->is_key_down(key::key_shift))
      {
        camera.translate(0, 5, 0);
      }
      if (the_app->is_key_down(key::key_ctrl))
      {
        camera.translate(0, -5, 0);
      }
      if (the_app->is_key_down('W'))
      {
        camera.translate(0, 0, -5);
      }
      if (the_app->is_key_down('S'))
      {
        camera.translate(0, 0, 5);
      }
      if (the_app->is_key_down('A'))
      {
        camera.translate(-5, 0, 0);
      }
      if (the_app->is_key_down('D'))
      {
        camera.translate(5, 0, 0);
      }
    }

    //mouse control using x and y pos of mouse
    void mouse_control(mat4t &camera){

      int x, y;
      the_app->get_mouse_pos(x, y);
      int vx, vy;
      the_app->get_viewport_size(vx, vy);

      mat4t modelToWorld;

      modelToWorld.loadIdentity();
      modelToWorld[3] = vec4(camera.w().x(), camera.w().y(), camera.w().z(), 1);
      modelToWorld.rotateY((float)-x*2.0f);
      if (vy / 2 - y < 70 && vy / 2 - y > -70)
        modelToWorld.rotateX((float)vy / 2 - y);
      if (vy / 2 - y >= 70)
        modelToWorld.rotateX(70);
      if (vy / 2 - y <= -70)
        modelToWorld.rotateX(-70);
      camera = modelToWorld;//apply to the node
    }


  };
}

#endif
