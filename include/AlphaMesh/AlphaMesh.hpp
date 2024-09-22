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

      static AllegroFlare::TileMaps::TileMap<bool> process(ALLEGRO_BITMAP* bitmap=nullptr, int num_rows=16, int num_columns=16);
   };
}



