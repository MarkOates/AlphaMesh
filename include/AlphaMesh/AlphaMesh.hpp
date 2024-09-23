#pragma once


#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <AlphaMesh/Rectangle.hpp>
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

      int get_num_rows() const;
      int get_num_columns() const;
      int get_width() const;
      int get_height() const;
      float get_cell_width() const;
      float get_cell_height() const;
      static bool area_contains_no_pixels(ALLEGRO_BITMAP* bitmap=nullptr, int x=0, int y=0, int w=0, int h=0);
      static bool all_cells_directly_below_are_solid(int rect_row1=0, int rect_column1=0, int rect_row2=0, int rect_column2=0, AllegroFlare::TileMaps::TileMap<bool>* tile_mask=nullptr);
      static bool all_cells_directly_right_are_solid(int rect_row1=0, int rect_column1=0, int rect_row2=0, int rect_column2=0, AllegroFlare::TileMaps::TileMap<bool>* tile_mask=nullptr);
      static std::vector<ALLEGRO_VERTEX> assemble_quad(float x=0, float y=0, float x2=0, float y2=0);
      std::vector<ALLEGRO_VERTEX> build_mesh();
      std::vector<ALLEGRO_VERTEX> build_mesh__run_length_encoding_by_rows();
      std::vector<ALLEGRO_VERTEX> build_mesh__run_length_encoding_by_columns();
      std::vector<ALLEGRO_VERTEX> build_mesh__collapse_columns_rows();
      std::vector<ALLEGRO_VERTEX> build_mesh__collapse_rows_columns();
      std::vector<::AlphaMesh::Rectangle> find_rectangles();
      AllegroFlare::TileMaps::TileMap<bool> build_tile_mask();
   };
}



