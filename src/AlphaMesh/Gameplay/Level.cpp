

#include <AlphaMesh/Gameplay/Level.hpp>




namespace AlphaMesh
{
namespace Gameplay
{


Level::Level()
   : AllegroFlare::Levels::Base(AlphaMesh::Gameplay::Level::TYPE)
   , title("[unset-title]")
{
}


Level::~Level()
{
}


void Level::set_title(std::string title)
{
   this->title = title;
}


std::string Level::get_title() const
{
   return title;
}




} // namespace Gameplay
} // namespace AlphaMesh


