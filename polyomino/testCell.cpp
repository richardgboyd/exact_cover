#include "gtest/gtest.h"
#include "Shapes.h"

TEST(Cell, defaultConstructor)
{
   Cell cell;
   
   ASSERT_EQ(cell.getCol(), -1);
   ASSERT_EQ(cell.getRow(), -1);
}

TEST(Cell, parameterConstructor)
{
   Cell cell(1,2);

   ASSERT_EQ(cell.getCol(), 1);
   ASSERT_EQ(cell.getRow(), 2);
}

TEST(Cell, copyConstructor)
{
   Cell cell(1,2);
   Cell cellCopy(cell);

   ASSERT_EQ(cell.getCol(), 1);
   ASSERT_EQ(cell.getRow(), 2);
}

TEST(Cell, setPositionByValue)
{
   Cell cell;

   cell.setPosition(1,2);

   ASSERT_EQ(cell.getCol(), 1);
   ASSERT_EQ(cell.getRow(), 2);
}

TEST(Cell, setPositionByCopy)
{
   Cell cell(1,2);
   Cell cellCopy;

   cellCopy.setPosition(cell);

   ASSERT_EQ(cell.getCol(), 1);
   ASSERT_EQ(cell.getRow(), 2);
}

