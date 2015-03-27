////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh and Himanshu Chablani
//
// Ken Perlin Noise Function is largely sourced and derived from Ken Perlin's Improved Noise paper 
// Travis Archer has also been sourced where appropriate
//

#ifndef BMP_IMAGE_H_INCLUDED
#define BMP_IMAGE_H_INCLUDED

namespace octet {

#define width_image 256
#define height_image 256

  /// Scene containing a box with octet.
  class bmp_image : public resource {

  public:
    /// this is called when we construct the class before everything is initialised.
    bmp_image() {
    }
    void init(){}
    //colour structre for storing rgb values of each pixel
    struct colour{
      unsigned char v[3];
      colour(unsigned char r, unsigned char g, unsigned char b){
        v[0] = r;
        v[1] = g;
        v[2] = b;
      }
    };
    //SOURCED: TRAVIS ARCHER
    colour lerp(colour c1, colour c2, float value){
      colour tcolor(0, 0, 0);

      for (int g = 0; g < 3; g++){
        if (c1.v[g] > c2.v[g]){
          tcolor.v[g] = c2.v[g] + (unsigned char)(float(c1.v[g] - c2.v[g])*value);
        }
        else{
          tcolor.v[g] = c1.v[g] + (unsigned char)(float(c2.v[g] - c1.v[g])*value);
        }
      }
      return (tcolor);
    }

    vec3 create_colour(float &image, float &_min, float &_max){
      //set up some variables
      float diff = _max - _min,
        flood = 0.35f,//flood level
        rock = 0.65f, //rock level
        mount = 0.85f;//mountain level
      static float rad = 0.0f;
      flood *= diff;
      mount *= diff;
      rock *= diff;

      image -= _min;
      ////these can be changed for interesting results
      colour rocklow(153, 76, 0),
        rockhigh(51, 25, 0),
        grasslow(0, 64, 0),
        grasshigh(133, 182, 116),
        waterlow(0, 0, 55), //done
        waterhigh(0, 53, 106), //done
        mountlow(167, 157, 147), //done
        mounthigh(255, 240, 214); //done
      colour pxl_colour(0, 0, 0);
      if (image < flood)
      {
        pxl_colour = lerp(waterlow, waterhigh, image / flood);
        image = (flood - flood / 10.0f - ((flood / 50.0f)*(cosf(rad) + sinf(rad))));
        rad += 0.5f;
      }//if this is above the mountain line...
      else if (image > mount)
      {
        pxl_colour = lerp(mountlow, mounthigh, (image - mount) / (diff - mount));
      }
      //f this is above the rock line
      else if (image > rock)
      {
        pxl_colour = lerp(rocklow, rockhigh, (image - rock) / (diff - flood));
      }
      //if this is regular land
      else
      {
        pxl_colour = lerp(grasslow, grasshigh, (image - flood) / (mount - flood));
      }
      vec3 mesh_colour = vec3((float)pxl_colour.v[0] / 255.0f, (float)pxl_colour.v[1] / 255.0f, (float)pxl_colour.v[2] / 255.0f);

      return mesh_colour;
    }
  };
}
#endif
