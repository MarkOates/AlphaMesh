

#include <AlphaMesh/AlphaMesh.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace AlphaMesh
{


AlphaMesh::AlphaMesh(ALLEGRO_BITMAP* bitmap, int num_rows, int num_columns)
   : bitmap(bitmap)
   , num_rows(num_rows)
   , num_columns(num_columns)
   , width(0)
   , height(0)
   , cell_width(0.0f)
   , cell_height(0.0f)
{
}


AlphaMesh::~AlphaMesh()
{
}


int AlphaMesh::get_width() const
{
   return width;
}


int AlphaMesh::get_height() const
{
   return height;
}


float AlphaMesh::get_cell_width() const
{
   return cell_width;
}


float AlphaMesh::get_cell_height() const
{
   return cell_height;
}


bool AlphaMesh::area_contains_no_pixels(ALLEGRO_BITMAP* bitmap, int x, int y, int w, int h)
{
   const float ALPHA_THRESHOLD = 0.0001f;
   for (int yy=y; yy<=y+h; yy++)
   {
      for (int xx=x; xx<=x+w; xx++)
      {
         if (al_get_pixel(bitmap, xx, yy).a >= ALPHA_THRESHOLD) return false;
      }
   }
   return true;
}

std::vector<ALLEGRO_VERTEX> AlphaMesh::assemble_quad(float x, float y, float x2, float y2)
{
   std::vector<ALLEGRO_VERTEX> result;

   // Create the color once
   ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

   // Bottom-left
   result.push_back(ALLEGRO_VERTEX{x, y2, 0.0f, 0.0f, 1.0f, color});

   // Top-left
   result.push_back(ALLEGRO_VERTEX{x, y, 0.0f, 0.0f, 0.0f, color});

   // Bottom-right
   result.push_back(ALLEGRO_VERTEX{x2, y2, 0.0f, 1.0f, 1.0f, color});

   // Top-left (again, for the second triangle)
   result.push_back(ALLEGRO_VERTEX{x, y, 0.0f, 0.0f, 0.0f, color});

   // Bottom-right (again, for the second triangle)
   result.push_back(ALLEGRO_VERTEX{x2, y2, 0.0f, 1.0f, 1.0f, color});

   // Top-right
   result.push_back(ALLEGRO_VERTEX{x2, y, 0.0f, 1.0f, 0.0f, color});

   return result;
}

std::vector<ALLEGRO_VERTEX> AlphaMesh::build_mesh()
{
   AllegroFlare::TileMaps::TileMap<bool> tile_mask = build_tile_mask();

   std::vector<ALLEGRO_VERTEX> result;

   for (int row=0; row<tile_mask.get_num_rows(); row++)
   {
      for (int column=0; column<tile_mask.get_num_columns(); column++)
      {
         bool is_solid = tile_mask.get_tile(column, row);
         if (is_solid)
         {
            float x1 = column * cell_width;
            float y1 = row * cell_height;
            float x2 = (column+1) * cell_width;
            float y2 = (row+1) * cell_height;

            std::vector<ALLEGRO_VERTEX> quad = assemble_quad(x1, y1, x2, y2);
            //quad = 
            result.insert(result.end(), quad.begin(), quad.end());
         }

         //bool is_empty = area_contains_no_pixels(
            //bitmap,
            //(int)x_pos, // TODO: Convert to float argument
            //(int)y_pos,
            //(int)cell_width+1, // TODO: See if/when +1 is necessary
            //(int)cell_height+1 // TODO: See if/when +1 is necessary
         //);

         //result.set_tile(tile_x, tile_y, !is_empty);
      }
   }

   return result;
}

AllegroFlare::TileMaps::TileMap<bool> AlphaMesh::build_tile_mask()
{
   if (!(bitmap))
   {
      std::stringstream error_message;
      error_message << "[AlphaMesh::AlphaMesh::build_tile_mask]: error: guard \"bitmap\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[AlphaMesh::AlphaMesh::build_tile_mask]: error: guard \"bitmap\" not met");
   }
   if (!((num_rows >= 2)))
   {
      std::stringstream error_message;
      error_message << "[AlphaMesh::AlphaMesh::build_tile_mask]: error: guard \"(num_rows >= 2)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[AlphaMesh::AlphaMesh::build_tile_mask]: error: guard \"(num_rows >= 2)\" not met");
   }
   if (!((num_columns >= 2)))
   {
      std::stringstream error_message;
      error_message << "[AlphaMesh::AlphaMesh::build_tile_mask]: error: guard \"(num_columns >= 2)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[AlphaMesh::AlphaMesh::build_tile_mask]: error: guard \"(num_columns >= 2)\" not met");
   }
   AllegroFlare::TileMaps::TileMap<bool> result;
   result.resize(num_columns, num_rows);
   result.initialize();

   int width = al_get_bitmap_width(bitmap);
   int height = al_get_bitmap_height(bitmap);
   cell_width = (float)width / num_columns;
   cell_height = (float)height / num_rows;

   al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

   for (int tile_y=0; tile_y<num_rows; tile_y++)
   {
      for (int tile_x=0; tile_x<num_columns; tile_x++)
      {
         float x_pos = tile_x * cell_width;
         float y_pos = tile_y * cell_height;

         bool is_empty = area_contains_no_pixels(
            bitmap,
            (int)x_pos, // TODO: Convert to float argument
            (int)y_pos,
            (int)cell_width+1, // TODO: See if/when +1 is necessary
            (int)cell_height+1 // TODO: See if/when +1 is necessary
         );

         result.set_tile(tile_x, tile_y, !is_empty);
      }
   }

   al_unlock_bitmap(bitmap);

   return result;
}


} // namespace AlphaMesh


