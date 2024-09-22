#include <gtest/gtest.h>

#include <AllegroFlare/Testing/ErrorAssertions.hpp>
#include <AllegroFlare/Testing/WithAllegroRenderingFixture.hpp>
#include <AlphaMesh/AlphaMesh.hpp>
#include <allegro5/allegro_primitives.h> // for al_is_primitives_addon_initialized();
#include <allegro5/allegro_color.h> // for al_color_html();


class AlphaMesh_AlphaMeshTest : public ::testing::Test {};
class AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture
   : public AllegroFlare::Testing::WithAllegroRenderingFixture
{};



TEST_F(AlphaMesh_AlphaMeshTest, can_be_created_without_blowing_up)
{
   AlphaMesh::AlphaMesh alpha_mesh;
}


TEST_F(AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture,
   CAPTURE__process__will_produce_a_tile_map_of_alpha_coverage_area)
{
   int num_rows = 32;
   int num_columns = 16;
   ALLEGRO_BITMAP *bitmap = get_bitmap_bin_ref()["investigator-01.png"];
   AllegroFlare::TileMaps::TileMap<bool> coverage_tile_map =
      AlphaMesh::AlphaMesh::process(bitmap, num_rows, num_columns);

   int bitmap_width = al_get_bitmap_width(bitmap);
   int bitmap_height = al_get_bitmap_height(bitmap);

   ALLEGRO_TRANSFORM camera_transform;
   al_identity_transform(&camera_transform);
   al_translate_transform(&camera_transform,  -bitmap_width/2, -bitmap_height/2);
   al_scale_transform(&camera_transform,  0.5, 0.5);
   al_translate_transform(&camera_transform,  1920/2, 1080/2);
   al_use_transform(&camera_transform);

   // Draw the image
   al_draw_bitmap(bitmap, 0, 0, 0);

   // Draw crosshairs for the bitmap edges
   draw_crosshair(0, 0);
   draw_crosshair(bitmap_width, 0);
   draw_crosshair(bitmap_width, bitmap_height);
   draw_crosshair(0, bitmap_height);

   // Draw the alpha coverage
   float cell_width_in_pixels = (float)bitmap_width / num_columns;
   float cell_height_in_pixels = (float)bitmap_height / num_rows;
   ALLEGRO_COLOR hit = al_color_html("03a889");
   float op = 0.4;
   hit.r *= op;
   hit.g *= op;
   hit.b *= op;
   hit.a *= op;
   for (int y=0; y<coverage_tile_map.get_num_rows(); y++)
   {
      for (int x=0; x<coverage_tile_map.get_num_columns(); x++)
      {
         bool tile = coverage_tile_map.get_tile(x, y);
         if (tile)
         {
            al_draw_filled_rectangle(
               x*cell_width_in_pixels,
               y*cell_height_in_pixels,
               (x+1)*cell_width_in_pixels,
               (y+1)*cell_height_in_pixels,
               hit
            );
            //al_draw_filled_rectangle(x*16, y*16, x*16+16, y*16+16, al_color_html("30286c")); // Dark violet
            //al_draw_filled_rectangle(x*16, y*16, x*16+16, y*16+16, al_color_html("233732")); // Dark green
         }

         al_draw_rectangle(
            x*cell_width_in_pixels,
            y*cell_height_in_pixels,
            (x+1)*cell_width_in_pixels,
            (y+1)*cell_height_in_pixels,
            hit,
            1.0f
         );
      }
   }

   al_flip_display();
   sleep_for(1);
}


