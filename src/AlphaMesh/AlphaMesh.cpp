

#include <AlphaMesh/AlphaMesh.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace AlphaMesh
{


AlphaMesh::AlphaMesh()
{
}


AlphaMesh::~AlphaMesh()
{
}


bool AlphaMesh::area_contains_no_pixels(ALLEGRO_BITMAP* bitmap, int x, int y, int w, int h)
{
   const float ALPHA_THRESHOLD = 0.0001f;
   for (int yy=y; yy<y+h; yy++)
   {
      for (int xx=x; xx<x+w; xx++)
      {
         if (al_get_pixel(bitmap, x, y).a >= ALPHA_THRESHOLD) return false;
      }
   }
   return true;
}

AllegroFlare::TileMaps::TileMap<bool> AlphaMesh::process(ALLEGRO_BITMAP* bitmap, int num_rows, int num_columns)
{
   if (!(bitmap))
   {
      std::stringstream error_message;
      error_message << "[AlphaMesh::AlphaMesh::process]: error: guard \"bitmap\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[AlphaMesh::AlphaMesh::process]: error: guard \"bitmap\" not met");
   }
   if (!((num_rows >= 2)))
   {
      std::stringstream error_message;
      error_message << "[AlphaMesh::AlphaMesh::process]: error: guard \"(num_rows >= 2)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[AlphaMesh::AlphaMesh::process]: error: guard \"(num_rows >= 2)\" not met");
   }
   if (!((num_columns >= 2)))
   {
      std::stringstream error_message;
      error_message << "[AlphaMesh::AlphaMesh::process]: error: guard \"(num_columns >= 2)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[AlphaMesh::AlphaMesh::process]: error: guard \"(num_columns >= 2)\" not met");
   }
   AllegroFlare::TileMaps::TileMap<bool> result;
   result.resize(num_columns, num_rows);
   result.initialize();

   int bitmap_width = al_get_bitmap_width(bitmap);
   int bitmap_height = al_get_bitmap_height(bitmap);

   int cell_width_in_pixels = bitmap_width / num_columns;
   int cell_height_in_pixels = bitmap_height / num_rows;

   // TODO: Ensure there are no "remainder" pixels

   // HERE: process the image
   al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
   //const float ALPHA_THRESHOLD = 0.0001f;

   for (int tile_y=0; tile_y<num_rows; tile_y++)
   {
      for (int tile_x=0; tile_x<num_columns; tile_x++)
      {
         int x_pos = tile_x * cell_width_in_pixels;
         int y_pos = tile_y * cell_height_in_pixels;

         bool is_empty = area_contains_no_pixels(bitmap, x_pos, y_pos, cell_width_in_pixels, cell_height_in_pixels);
         //for (int pixel_y=0; pixel_y<cell_height_in_pixels; pixel_y++)
         //{
            //bool empty = true;
            //for (int pixel_x=0; pixel_x<cell_width_in_pixels; pixel_x++)
            //{
               //int x_pos = tile_x * cell_width_in_pixels;
               //int y_pos = tile_y * cell_height_in_pixels;
               //ALLEGRO_COLOR p = al_get_pixel(bitmap, x_pos + pixel_x, y_pos + pixel_y);
               //if (p.a >= ALPHA_THRESHOLD)
               //{
                  //empty = false;
                  //break;
               //}
            //}

         result.set_tile(tile_x, tile_y, !is_empty);
         //}
      }
   }

   al_unlock_bitmap(bitmap);

   return result;
}


} // namespace AlphaMesh


