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
   CAPTURE__build_tile_mask__will_produce_a_tile_map_of_alpha_coverage_area)
{
   int num_rows = 64;
   int num_columns = 32;
   ALLEGRO_BITMAP *bitmap = get_bitmap_bin_ref()["investigator-01.png"];
   AlphaMesh::AlphaMesh alpha_mesh(bitmap, num_rows, num_columns);

   AllegroFlare::TileMaps::TileMap<bool> coverage_tile_map = alpha_mesh.build_tile_mask();

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
   float cell_width_in_pixels = alpha_mesh.get_cell_width();
   float cell_height_in_pixels = alpha_mesh.get_cell_height();
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

   auto mesh = alpha_mesh.build_mesh();
   al_draw_prim(&mesh[0], nullptr, bitmap, 0, mesh.size(), ALLEGRO_PRIM_TRIANGLE_LIST);

   ALLEGRO_FONT *font = get_any_font();
   al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());

   al_flip_display();
   //sleep_for(1);
}


TEST_F(AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture,
   CAPTURE__build_mesh__run_length_encoding_by_rows__will_produce_an_optimized_mesh)
{
   int num_rows = 32;
   int num_columns = 16;
   ALLEGRO_BITMAP *bitmap = get_bitmap_bin_ref()["investigator-01.png"];
   AlphaMesh::AlphaMesh alpha_mesh(bitmap, num_rows, num_columns);


   int bitmap_width = al_get_bitmap_width(bitmap);
   int bitmap_height = al_get_bitmap_height(bitmap);

   ALLEGRO_TRANSFORM camera_transform;
   al_identity_transform(&camera_transform);
   al_translate_transform(&camera_transform,  -bitmap_width/2, -bitmap_height/2);
   al_scale_transform(&camera_transform,  0.5, 0.5);
   al_translate_transform(&camera_transform,  1920/2, 1080/2);
   al_use_transform(&camera_transform);

   // Draw the image
   //al_draw_bitmap(bitmap, 0, 0, 0);

   // Draw crosshairs for the bitmap edges
   //draw_crosshair(0, 0);
   //draw_crosshair(bitmap_width, 0);
   //draw_crosshair(bitmap_width, bitmap_height);
   //draw_crosshair(0, bitmap_height);

   // Draw the alpha coverage (for easier debuging)
   AllegroFlare::TileMaps::TileMap<bool> coverage_tile_map = alpha_mesh.build_tile_mask();
   float cell_width_in_pixels = alpha_mesh.get_cell_width();
   float cell_height_in_pixels = alpha_mesh.get_cell_height();
   ALLEGRO_COLOR hit = al_color_html("03a889");
   float op = 0.4;
   hit.r *= op;
   hit.g *= op;
   hit.b *= op;
   hit.a *= op;
   ///*
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
   //*/


   ALLEGRO_TRANSFORM subject_position_transform;

   // Draw the mesh (optimized by rows)
   {
      al_copy_transform(&subject_position_transform, &camera_transform);
      al_translate_transform(&subject_position_transform, -500, 0);
      al_use_transform(&subject_position_transform);

      auto mesh = alpha_mesh.build_mesh__run_length_encoding_by_rows();
      al_draw_prim(&mesh[0], nullptr, bitmap, 0, mesh.size(), ALLEGRO_PRIM_TRIANGLE_LIST);

      ALLEGRO_FONT *font = get_any_font();
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 36, 0, "rows: %d", alpha_mesh.get_num_rows());
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 62, 0, "columns: %d", alpha_mesh.get_num_columns());
      //al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());

      // Draw the lines in the mesh
      ALLEGRO_COLOR orange = al_color_name("orange");
      for (int vertex_i=2; vertex_i<mesh.size(); vertex_i+=3)
      {
         al_draw_line(mesh[vertex_i-2].x, mesh[vertex_i-2].y, mesh[vertex_i-1].x, mesh[vertex_i-1].y, orange, 2.0);
         al_draw_line(mesh[vertex_i-1].x, mesh[vertex_i-1].y, mesh[vertex_i-0].x, mesh[vertex_i-0].y, orange, 2.0);
         al_draw_line(mesh[vertex_i-0].x, mesh[vertex_i-0].y, mesh[vertex_i-2].x, mesh[vertex_i-2].y, orange, 2.0);
      }
   }

   // Draw the mesh (optimized by columns)
   {
      al_copy_transform(&subject_position_transform, &camera_transform);
      al_translate_transform(&subject_position_transform, 500, 0);
      al_use_transform(&subject_position_transform);

      auto mesh = alpha_mesh.build_mesh__run_length_encoding_by_columns();
      al_draw_prim(&mesh[0], nullptr, bitmap, 0, mesh.size(), ALLEGRO_PRIM_TRIANGLE_LIST);

      ALLEGRO_FONT *font = get_any_font();
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 36, 0, "rows: %d", alpha_mesh.get_num_rows());
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 62, 0, "columns: %d", alpha_mesh.get_num_columns());
      //al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());

      // Draw the lines in the mesh
      ALLEGRO_COLOR orange = al_color_name("orange");
      for (int vertex_i=2; vertex_i<mesh.size(); vertex_i+=3)
      {
         al_draw_line(mesh[vertex_i-2].x, mesh[vertex_i-2].y, mesh[vertex_i-1].x, mesh[vertex_i-1].y, orange, 2.0);
         al_draw_line(mesh[vertex_i-1].x, mesh[vertex_i-1].y, mesh[vertex_i-0].x, mesh[vertex_i-0].y, orange, 2.0);
         al_draw_line(mesh[vertex_i-0].x, mesh[vertex_i-0].y, mesh[vertex_i-2].x, mesh[vertex_i-2].y, orange, 2.0);
      }
   }

   al_flip_display();
   //sleep_for(1);
}


TEST_F(AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture,
   FOCUS__all_cells_directly_below_are_solid__will_return_true_when_cells_below_are_solid)
{
   AllegroFlare::TileMaps::TileMap<bool> tile_mask;
   tile_mask.resize_with_fill(5, 3, false);
   tile_mask.initialize();
   tile_mask.fill_with_data(std::vector<std::vector<bool>>{
      { 0, 1, 1, 1, 0 },
      { 0, 1, 1, 1, 1 },
      { 0, 1, 1, 0, 0 },
   });
   EXPECT_EQ(true, AlphaMesh::AlphaMesh::all_cells_directly_below_are_solid(0, 1, 0, 3, &tile_mask));
}


TEST_F(AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture,
   FOCUS__all_cells_directly_below_are_solid__will_return_false_when_cells_below_are_solid)
{
   AllegroFlare::TileMaps::TileMap<bool> tile_mask;
   tile_mask.resize_with_fill(5, 3, false);
   tile_mask.initialize();
   tile_mask.fill_with_data(std::vector<std::vector<bool>>{
      { 0, 1, 1, 1, 0 },
      { 0, 1, 1, 1, 1 },
      { 0, 1, 1, 0, 0 },
   });
   EXPECT_EQ(false, AlphaMesh::AlphaMesh::all_cells_directly_below_are_solid(1, 1, 1, 3, &tile_mask));
}


TEST_F(AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture,
   FOCUS__all_cells_directly_below_are_solid__will_return_false_when_provided_cells_are_on_the_last_row)
{
   AllegroFlare::TileMaps::TileMap<bool> tile_mask;
   tile_mask.resize_with_fill(5, 3, false);
   tile_mask.initialize();
   tile_mask.fill_with_data(std::vector<std::vector<bool>>{
      { 0, 1, 1, 1, 0 },
      { 0, 1, 1, 1, 1 },
      { 0, 1, 1, 0, 0 },
   });
   EXPECT_EQ(false, AlphaMesh::AlphaMesh::all_cells_directly_below_are_solid(1, 1, 2, 3, &tile_mask));
}



TEST_F(AlphaMesh_AlphaMeshTestWithAllegroRenderingFixture,
   CAPTURE__build_mesh__collapse_columns_rows___will_produce_an_optimized_mesh)
{
   int num_rows = 32;
   int num_columns = 16;
   ALLEGRO_BITMAP *bitmap = get_bitmap_bin_ref()["investigator-01.png"];
   AlphaMesh::AlphaMesh alpha_mesh(bitmap, num_rows, num_columns);


   int bitmap_width = al_get_bitmap_width(bitmap);
   int bitmap_height = al_get_bitmap_height(bitmap);

   ALLEGRO_TRANSFORM camera_transform;
   al_identity_transform(&camera_transform);
   al_translate_transform(&camera_transform,  -bitmap_width/2, -bitmap_height/2);
   al_scale_transform(&camera_transform,  0.5, 0.5);
   al_translate_transform(&camera_transform,  1920/2, 1080/2);
   al_use_transform(&camera_transform);

   // Draw the image
   //al_draw_bitmap(bitmap, 0, 0, 0);

   // Draw crosshairs for the bitmap edges
   //draw_crosshair(0, 0);
   //draw_crosshair(bitmap_width, 0);
   //draw_crosshair(bitmap_width, bitmap_height);
   //draw_crosshair(0, bitmap_height);

   // Draw the alpha coverage (for easier debuging)
   AllegroFlare::TileMaps::TileMap<bool> coverage_tile_map = alpha_mesh.build_tile_mask();
   float cell_width_in_pixels = alpha_mesh.get_cell_width();
   float cell_height_in_pixels = alpha_mesh.get_cell_height();
   ALLEGRO_COLOR hit = al_color_html("03a889");
   float op = 0.4;
   hit.r *= op;
   hit.g *= op;
   hit.b *= op;
   hit.a *= op;
   ///*
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
   //*/


   //ALLEGRO_TRANSFORM subject_position_transform;

   // Draw the mesh (optimized by rows)
   {
      //al_copy_transform(&subject_position_transform, &camera_transform);
      //al_translate_transform(&subject_position_transform, -500, 0);
      //al_use_transform(&subject_position_transform);

      auto mesh = alpha_mesh.build_mesh__collapse_columns_rows();
      al_draw_prim(&mesh[0], nullptr, bitmap, 0, mesh.size(), ALLEGRO_PRIM_TRIANGLE_LIST);

      ALLEGRO_FONT *font = get_any_font();
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 36, 0, "rows: %d", alpha_mesh.get_num_rows());
      al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 62, 0, "columns: %d", alpha_mesh.get_num_columns());
      //al_draw_textf(font, ALLEGRO_COLOR{1, 1, 1, 1}, 18, 12, 0, "vertices: %d", mesh.size());

      // Draw the lines in the mesh
      ALLEGRO_COLOR orange = al_color_name("orange");
      for (int vertex_i=2; vertex_i<mesh.size(); vertex_i+=3)
      {
         al_draw_line(mesh[vertex_i-2].x, mesh[vertex_i-2].y, mesh[vertex_i-1].x, mesh[vertex_i-1].y, orange, 2.0);
         al_draw_line(mesh[vertex_i-1].x, mesh[vertex_i-1].y, mesh[vertex_i-0].x, mesh[vertex_i-0].y, orange, 2.0);
         al_draw_line(mesh[vertex_i-0].x, mesh[vertex_i-0].y, mesh[vertex_i-2].x, mesh[vertex_i-2].y, orange, 2.0);
      }
   }

   al_flip_display();
   sleep_for(1);
}





