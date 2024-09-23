
#include <gtest/gtest.h>

#include <AlphaMesh/Rectangle.hpp>


TEST(AlphaMesh_RectangleTest, can_be_created_without_blowing_up)
{
   AlphaMesh::Rectangle rectangle;
}


TEST(AlphaMesh_RectangleTest, run__returns_the_expected_response)
{
   AlphaMesh::Rectangle rectangle;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, rectangle.run());
}


