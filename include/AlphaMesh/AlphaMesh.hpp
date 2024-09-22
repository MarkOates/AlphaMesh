#pragma once


#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <allegro5/allegro.h>


namespace AlphaMesh
{
   class AlphaMesh
   {
   private:
      ALLEGRO_BITMAP* bitmap;
      int num_rows;
      int num_columns;
      float cell_width_in_pixels;
      float cell_height_in_pixels;

   protected:


   public:
      AlphaMesh(ALLEGRO_BITMAP* bitmap=nullptr, int num_rows=16, int num_columns=16);
      ~AlphaMesh();

      static bool area_contains_no_pixels(ALLEGRO_BITMAP* bitmap=nullptr, int x=0, int y=0, int w=0, int h=0);
      AllegroFlare::TileMaps::TileMap<bool> build_tile_mask();
   };
}



