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
   ALLEGRO_BITMAP *bitmap = get_bitmap_bin_ref()["investigator-01.png"];
   AllegroFlare::TileMaps::TileMap<bool> coverage_tile_map = AlphaMesh::AlphaMesh::process(bitmap, 16, 16);

   // Draw the image
   al_draw_bitmap(bitmap, 0, 0, 0);

   // Draw the alpha coverage
   for (int y=0; y<coverage_tile_map.get_num_rows(); y++)
   {
      for (int x=0; x<coverage_tile_map.get_num_columns(); x++)
      {
         bool tile = coverage_tile_map.get_tile(x, y);
         if (tile)
         {
            al_draw_filled_rectangle(x*16, y*16, x*16+16, y*16+16, al_color_html("03a889"));
            //al_draw_filled_rectangle(x*16, y*16, x*16+16, y*16+16, al_color_html("30286c")); // Dark violet
            //al_draw_filled_rectangle(x*16, y*16, x*16+16, y*16+16, al_color_html("233732")); // Dark green
         }
      }
   }

   al_flip_display();
   sleep_for(1);
}


