#pragma once


#include <AlphaMesh/GameProgressAndStateInfo.hpp>
#include <ostream>


namespace AlphaMesh
{
   bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   bool operator!=(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os);
}


