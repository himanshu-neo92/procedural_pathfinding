////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Text overlay
//

#include "../../octet.h"
#include "vector"
#include "UI.h"
#include "sky_box.h"
#include "bmp_image.h"
#include "perlin.h"
#include "inputs.h"
#include "search_grid.h"
#include "nav_mesh.h"
#include "path_find.h"
#include "Procedural_pathfinding.h"

/// Create a box with octet
int main(int argc, char **argv) {
  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::Procedural_pathfinding app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}


