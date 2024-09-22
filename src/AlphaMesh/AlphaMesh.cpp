

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

   // HERE: process the image
   result.set_tile(4, 6, true);
   result.set_tile(0, 0, true);
   result.set_tile(15, 15, true);

   return result;
}


} // namespace AlphaMesh


