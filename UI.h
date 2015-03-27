#ifndef UI_INCLUDED
#define UI_H_INCLUDED

namespace octet {

  class UI : public resource{

    text_overlay *overlay;
    /** @var  text_overlay *overlay
    *   @brief text overlay for the UI.
    */
    mesh_text *UI_top;
    /** @var   mesh_text *UI_top
    *   @brief text mesh for the top UI.
    */
    mesh_text *UI_bot;
    /** @var   mesh_text *UI_bot
    *   @brief text mesh for the bottom UI.
    */
    mesh_text *UI_popup;
    /** @var  mesh_text *UI_popup
    *   @brief text mesh for the popup UI.
    */
    mesh_text *target;
    /** @var  mesh_text *target
    *   @brief text mesh for the target UI.
    */

    vec3 startpoint;
    vec3 endpoint;
    vec3 nextpoint;
  public:
    int pop_up_var = 0;
    bool clear_popup = false;
    float temp_popup_time;
    float total_popup_time;
 
    UI() {
    }
    void initUI(int vx, int vy)
    {
      printf(" x %i , y %i", vx, vy);
      overlay = new text_overlay();
      // get the defualt font.
      bitmap_font *font = overlay->get_default_font();
      // create a box containing text (in pixels)
      aabb bb_bot(vec3(vx * 7 / 8, -500, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_bot = new mesh_text(font, "", &bb_bot);

      aabb bb_top(vec3(vx * 6 / 8, 200, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_top = new mesh_text(font, "", &bb_top);

      aabb bb_popup(vec3(vx * 7 / 8, -240, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_popup = new mesh_text(font, "", &bb_popup);

      aabb bb_target(vec3(vx, -100, 0.0f), vec3(vx, 100.0f, 0.0f));
      target = new mesh_text(font, "+", &bb_target);
      // add the mesh to the overlay.

      overlay->add_mesh_text(UI_bot);
      overlay->add_mesh_text(UI_top);
      overlay->add_mesh_text(UI_popup);
      overlay->add_mesh_text(target);
    }
    /** @fn  void initUI()
    *   @brief This function is used to init the UI variables ie overlay textmesh....
    */


    void setstartpoint(vec3 _startpoint)
    {
      startpoint=_startpoint;
    }

    void setendpoint(vec3 _endpoint)
    {
      endpoint = _endpoint;
    }

    void setnextpoint(vec3 _nextpoint)
    {
      nextpoint = _nextpoint;
    }
    void setup_pop_up(int num)
    {
      total_popup_time = 2.0f;
      temp_popup_time = 0.0f;
      pop_up_var = num;
      clear_popup = true;
    }
    void pop_up_clear()
    {
      if (clear_popup)
      {
        if (total_popup_time - temp_popup_time <= 0)
        {
          clear_popup = false;
          pop_up_var = 0;
        }
        else
        {
          temp_popup_time += 0.03f;
        }
      }
    }
    void updateUI(int vx, int vy)
    {
      
      UI_bot->clear();
      UI_bot->format(
        "Press Q to generate a random height map!  Press F1 to start pathfinding");
      // convert it to a mesh.
      UI_bot->update();


      UI_top->clear();
      UI_top->format(
        "Start point: %f,%f,%f ,End point: %f,%f,%f ,next point: %f,%f,%f ",startpoint.x(),startpoint.y(),startpoint.z(),endpoint.x(),endpoint.y(),endpoint.z(),nextpoint.x(),nextpoint.y(),nextpoint.z());
      // convert it to a mesh.
      UI_top->update();


      UI_popup->clear();
      switch (pop_up_var)
      {
      case 0:UI_popup->format("");
        break;
      case 1:UI_popup->format("Please wait...generating random terrain. This may take a while.");
       break;
      case 4:UI_popup->format("Terrain successfully generated.");
        break;
      }
      UI_popup->update();
      // draw the text overlay
      overlay->render(vx, vy);
    }

  };
}
#endif