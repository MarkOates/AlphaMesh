#pragma once


#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>


namespace AlphaMesh
{
   class AlphaMesh
   {
   private:
      ALLEGRO_BITMAP* bitmap;
      int num_rows;
      int num_columns;
      int width;
      int height;
      float cell_width;
      float cell_height;

   protected:


   public:
      AlphaMesh(ALLEGRO_BITMAP* bitmap=nullptr, int num_rows=16, int num_columns=16);
      ~AlphaMesh();

      int get_width() const;
      int get_height() const;
      float get_cell_width() const;
      float get_cell_height() const;
      static bool area_contains_no_pixels(ALLEGRO_BITMAP* bitmap=nullptr, int x=0, int y=0, int w=0, int h=0);
      static std::vector<ALLEGRO_VERTEX> assemble_quad(float x=0, float y=0, float x2=0, float y2=0);
      std::vector<ALLEGRO_VERTEX> build_mesh();
      AllegroFlare::TileMaps::TileMap<bool> build_tile_mask();
   };
}



