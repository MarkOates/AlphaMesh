

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


int AlphaMesh::get_num_rows() const
{
   return num_rows;
}


int AlphaMesh::get_num_columns() const
{
   return num_columns;
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
   result.push_back(ALLEGRO_VERTEX{x, y2, 0.0f, x, y2, color});

   // Top-left
   result.push_back(ALLEGRO_VERTEX{x, y, 0.0f, x, y, color});

   // Bottom-right
   result.push_back(ALLEGRO_VERTEX{x2, y2, 0.0f, x2, y2, color});

   // Top-left (again, for the second triangle)
   result.push_back(ALLEGRO_VERTEX{x, y, 0.0f, x, y, color});

   // Bottom-right (again, for the second triangle)
   result.push_back(ALLEGRO_VERTEX{x2, y2, 0.0f, x2, y2, color});

   // Top-right
   result.push_back(ALLEGRO_VERTEX{x2, y, 0.0f, x2, y, color});

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
      }
   }

   return result;
}

std::vector<ALLEGRO_VERTEX> AlphaMesh::build_mesh__run_length_encoding_by_rows()
{
   AllegroFlare::TileMaps::TileMap<bool> tile_mask = build_tile_mask();

   std::vector<ALLEGRO_VERTEX> result;

   float rect_x1 = 0;
   float rect_y1 = 0;
   float rect_x2 = 0;
   float rect_y2 = 0;
   //bool close_rectangle = false;
   bool state_assembling_rectangle = false;

   for (int row=0; row<tile_mask.get_num_rows(); row++)
   {
      state_assembling_rectangle = false;
      //close_rectangle = false;

      for (int column=0; column<tile_mask.get_num_columns(); column++)
      {
         bool at_last_column = (column == tile_mask.get_num_columns() - 1);
         bool is_solid = tile_mask.get_tile(column, row);
         bool close_rectangle = false;
         //bool state_assembling_rectangle = false;

         if (is_solid)
         {
            if (!state_assembling_rectangle)
            {
               float x1 = column * cell_width;
               float y1 = row * cell_height;
               rect_x1 = x1;
               rect_y1 = y1;
               state_assembling_rectangle = true;
            }
            else
            {
               float x2 = (column+1) * cell_width;
               float y2 = (row+1) * cell_height;
               rect_x2 = x2;
               rect_y2 = y2;
            }
         }
         else
         {
            if (state_assembling_rectangle)
            {
               close_rectangle = true;
            }
            else
            {
               // Nothing to do, consecutive empty tiles
            }
         }

         if (at_last_column) close_rectangle = true;

         if (close_rectangle)
         {
            //std::cout << "HERE" << std::endl;
            std::vector<ALLEGRO_VERTEX> quad = assemble_quad(rect_x1, rect_y1, rect_x2, rect_y2);
            std::cout << "HERE: quad: " << quad[0].x << ", " << quad[0].y << std::endl;
            result.insert(result.end(), quad.begin(), quad.end());
            state_assembling_rectangle = false;
         }
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


