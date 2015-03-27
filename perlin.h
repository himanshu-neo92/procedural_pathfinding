////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh and Himanshu Chablani
//
// Ken Perlin Noise Function is largely sourced and derived from Ken Perlin's Improved Noise paper  
//

#include <time.h>

using namespace std;

namespace octet {

  class perlin : public resource {

    /// Scene containing a box with octet.
    ref<visual_scene> app_scene;
  public:
    perlin() {
    }

    //simple floor function
    int floor(float value)
    {
      return (value >= 0 ? (int)value : (int)value - 1);
    }

    //used to find out the dot product between the gradient vector and the distance vector
    float dotproduct(float grad[], float x, float y)
    {
      return (grad[0] * x + grad[1] * y);
    }

    //linear interpolation between these two points
    float lerp(float t, float a, float b)
    {
      return ((1 - b) * t + b * a);
    }
    //fade or ease curve to produce a smoother interpolation between integral coordinates
    float fade(float t){
      return t * t * t * (t * (t * 6 - 15) + 10);
    }

    //rand number generator
    float random(float max){
      int r;
      float s;

      r = rand();
      s = (float)(r & 0x7fff) / (float)0x7fff;

      return (s * max);
    }

    void fill_image(float *image, float &min, float &max, float _octaves)
    {

      //change this
      int xpos, ypos, grad11, grad12, grad21, grad22;

      //number of times to iterate through the fbm loop
      int octaves = _octaves;

      float result,
        fractional_x, fractional_y,
        noise11, noise12, noise21, noise22,
        interpolatedx1, interpolatedx2, interpolatedxy;

      //set up the gradient table with 8 equally distributed angles around the unit circle
      float gradients[8][2];
      for (int i = 0; i < 8; ++i)
      {
        gradients[i][0] = cos(0.785398163f * (float)i);// 0.785398163 is PI/4.
        gradients[i][1] = sin(0.785398163f * (float)i);
      }

      time_t beginning = time(NULL); //use time for our random seed
      srand((unsigned)beginning);//set the random seed

      //set up the permutation table (not using ken perlin's pseudo random table)
      int permutations[256]; //using 256 numbers though (same as the pseudo random table)
      for (int i = 0; i < 256; ++i)
        permutations[i] = i;//we want 1 of each number for our table

      //randomize the positions of the permutation table (or we'll get the same effect every time)
      for (int i = 0; i < 256; ++i)
      {
        int _rand = (int)random(256);
        int j = permutations[i];
        permutations[i] = permutations[_rand];
        permutations[_rand] = j;
      }

      //set the max and min values that we get
      min = 100000.0f;
      max = -100000.0f;

      //fractional brownian motion variables - change for differen effects
      float amplitude = 0.0f;
      float frequency = 0.0f;
      float gain = 0.65f;
      float lacunarity = 1.8715f;
      // Visit every pixel of the image and assign a color generated with Perlin noise and FBM
      for (int i = 0; i < height_image; ++i)
      {
        for (int j = 0; j < width_image; ++j)
        {
          amplitude = 1.0f;
          frequency = 1.0f / (float)height_image;
          result = 0.0f;
          //depending on the number of octaves depends on how many layers we're putting together
          for (int kk = 0; kk < octaves; ++kk)
          {
            //first we find the unit cube that contains the point
            xpos = floor((float)j * frequency);
            ypos = floor((float)i * frequency);

            fractional_x = (float)j * frequency - (float)xpos;
            fractional_y = (float)i * frequency - (float)ypos;

            //hash coordinates of the 4 cube corner (ken perlin uses 8 usually)
            grad11 = permutations[(xpos + permutations[ypos & 255]) & 255] & 7;
            grad12 = permutations[(xpos + 1 + permutations[ypos & 255]) & 255] & 7;
            grad21 = permutations[(xpos + permutations[(ypos + 1) & 255]) & 255] & 7;
            grad22 = permutations[(xpos + 1 + permutations[(ypos + 1) & 255]) & 255] & 7;//&'ing caps the numbers inclusively

            //check the noise generated at each corner
            noise11 = dotproduct(gradients[grad11], fractional_x, fractional_y);
            noise12 = dotproduct(gradients[grad12], fractional_x - 1.0f, fractional_y);
            noise21 = dotproduct(gradients[grad21], fractional_x, fractional_y - 1.0f);
            noise22 = dotproduct(gradients[grad22], fractional_x - 1.0f, fractional_y - 1.0f);

            //compute the fade curves for x and y pos
            fractional_x = fade(fractional_x);
            fractional_y = fade(fractional_y);

            //blend the results on the x axis and then the y axis
            interpolatedx1 = lerp(noise11, noise12, fractional_x);
            interpolatedx2 = lerp(noise21, noise22, fractional_x);
            interpolatedxy = lerp(interpolatedx1, interpolatedx2, fractional_y);

            //generate the result and update for next octave
            result += interpolatedxy * amplitude;
            amplitude *= gain; //multiply the amplitude by the gain(persistance) each octave
            frequency *= lacunarity; //increase the frequency each octave 
          }

          //put it in our image
          *(image + i*width_image + j) = result;

          if (result < min)
            min = result;

          else if (result > max)
            max = result;
        }
      }

    }
  };
}