#pragma once


#include <string>


namespace AlphaMesh
{
   class Rectangle
   {
   private:

   protected:


   public:
      int x1;
      int y1;
      int x2;
      int y2;
      Rectangle(int x1=0, int y1=0, int x2=0, int y2=0);
      ~Rectangle();

      std::string run();
   };
}



