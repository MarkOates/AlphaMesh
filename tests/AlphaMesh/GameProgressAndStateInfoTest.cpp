
#include <gtest/gtest.h>

#include <AlphaMesh/GameProgressAndStateInfo.hpp>


TEST(AlphaMesh_GameProgressAndStateInfoTest, can_be_created_without_blowing_up)
{
   AlphaMesh::GameProgressAndStateInfo game_progress_and_state_info;
}


TEST(AlphaMesh_GameProgressAndStateInfoTest, TYPE__has_the_expected_value)
{
   EXPECT_STREQ(
     "AlphaMesh/GameProgressAndStateInfo",
     AlphaMesh::GameProgressAndStateInfo::TYPE
   );
}


TEST(AlphaMesh_GameProgressAndStateInfoTest, type__has_the_expected_value_matching_TYPE)
{
   AlphaMesh::GameProgressAndStateInfo game_progress_and_state_info;
   EXPECT_EQ(AlphaMesh::GameProgressAndStateInfo::TYPE, game_progress_and_state_info.get_type());
}


