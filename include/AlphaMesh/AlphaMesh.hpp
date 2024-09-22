#pragma once


#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <allegro5/allegro.h>


namespace AlphaMesh
{
   class AlphaMesh
   {
   private:

   protected:


   public:
      AlphaMesh();
      ~AlphaMesh();

      static bool area_contains_no_pixels(ALLEGRO_BITMAP* bitmap=nullptr, int x=0, int y=0, int w=0, int h=0);
      static AllegroFlare::TileMaps::TileMap<bool> process(ALLEGRO_BITMAP* bitmap=nullptr, int num_rows=16, int num_columns=16);
   };
}



