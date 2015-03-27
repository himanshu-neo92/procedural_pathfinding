#ifndef SKY_BOX_H_INCLUDED
#define SKY_BOX_H_INCLUDED

namespace octet {


  class sky_box : public resource{


  public:
    sky_box()
    {}
    scene_node *daynightnode;
    void InitDayNightCycle(ref<visual_scene> app_scene)
    {
      mat4t modelToWorld;
      mat4t modelToWorld_sun;
      mat4t modelToWorld_moon;
      mat4t modelToWorld_sky;
      mat4t modelToWorld_sunlight;

      modelToWorld_sunlight.loadIdentity();
      modelToWorld_sunlight.translate(4900, 0, 0);
      modelToWorld_sunlight.rotateY(-90);

      modelToWorld_sky.loadIdentity();
      modelToWorld.loadIdentity();
      modelToWorld_sun.loadIdentity();
      modelToWorld_sun.translate(5000, 0, 0);
      modelToWorld_moon.loadIdentity();
      modelToWorld_moon.translate(-5000, 0, 0);
      daynightnode = new scene_node(modelToWorld, atom_);
      app_scene->add_child(daynightnode);
      scene_node *sky_node = new scene_node(modelToWorld_sky, atom_);
      scene_node *sun_node = new scene_node(modelToWorld_sun, atom_);
      scene_node *moon_node = new scene_node(modelToWorld_moon, atom_);
      scene_node *sunlight_node = new scene_node(modelToWorld_sunlight, atom_);

      mesh_sphere *sky = new mesh_sphere(vec3(0, 0, 0), 5100);
      mesh_sphere *sun = new mesh_sphere(vec3(5000, 0, 0), 200);
      mesh_sphere *moon = new mesh_sphere(vec3(-5000, 0, 0), 200);
      material *sun_mat = new material(vec4(0.992, 0.721, 0.074, 1));
      material *moon_mat = new material(vec4(0.266, 0.388, 0.956, 1));
      material *sky_mat = new material(vec4(0.247, 0.3039, 0.580, 1));
      app_scene->add_mesh_instance(new mesh_instance(sun_node, sun, sun_mat));
      app_scene->add_mesh_instance(new mesh_instance(moon_node, moon, moon_mat));
      app_scene->add_mesh_instance(new mesh_instance(sky_node, sky, sky_mat));
      daynightnode->add_child(sun_node);
      daynightnode->add_child(moon_node);
    }
    /** @fn  void InitDayNightCycle()
    *   @brief This function is used to init the day night cycle
    */
    void DayNightCycle(ref<visual_scene> app_scene)
    {
      daynightnode->access_nodeToParent().rotateZ(0.1);
      app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateX(-0.1);
    }

  };
}
#endif