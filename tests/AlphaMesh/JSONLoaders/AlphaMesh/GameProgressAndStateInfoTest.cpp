
#include <gtest/gtest.h>

#include <AlphaMesh/JSONLoaders/AlphaMesh/GameProgressAndStateInfo.hpp>
#include <AlphaMesh/Testing/Comparison/AlphaMesh/GameProgressAndStateInfo.hpp>


TEST(AlphaMesh_JSONLoaders_AlphaMesh_GameProgressAndStateInfoTest,
   to_json__returns_the_object_as_json_with_the_expected_values)
{
   AlphaMesh::GameProgressAndStateInfo game_progress_and_state_info;
   game_progress_and_state_info.set_player_inventory_items({ "fruit", "veggie", "fork" });

   nlohmann::json j = game_progress_and_state_info;

   std::string expected_values =
R"({
  "player_inventory_items": [
    "fruit",
    "veggie",
    "fork"
  ]
})";

   std::string actual_values = j.dump(2);
   EXPECT_EQ(expected_values, actual_values);
}


TEST(AlphaMesh_JSONLoaders_AlphaMesh_GameProgressAndStateInfoTest,
   from_json__loads_json_data_into_the_object)
{
   AlphaMesh::GameProgressAndStateInfo game_progress_and_state_info;

   std::string json =
R"({
  "player_inventory_items": [
    "fruit",
    "veggie",
    "fork"
  ]
})";

   nlohmann::json parsed_json = nlohmann::json::parse(json);
   parsed_json.get_to(game_progress_and_state_info);

   AlphaMesh::GameProgressAndStateInfo expected;
   expected.set_player_inventory_items({ "fruit", "veggie", "fork" });

   EXPECT_EQ(expected, game_progress_and_state_info);
}


