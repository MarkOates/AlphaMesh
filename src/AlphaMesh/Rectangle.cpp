

#include <AlphaMesh/Rectangle.hpp>




namespace AlphaMesh
{


Rectangle::Rectangle(int x1, int y1, int x2, int y2)
   : x1(x1)
   , y1(y1)
   , x2(x2)
   , y2(y2)
{
}


Rectangle::~Rectangle()
{
}


std::string Rectangle::run()
{
   return "Hello World!";
}


} // namespace AlphaMesh


