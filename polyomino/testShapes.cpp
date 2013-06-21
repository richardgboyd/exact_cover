#include "gtest/gtest.h"
#include "Shapes.h"

bool testCellUnitialized(Cell &cell)
{
   if ((cell.getCol() == -1) && (cell.getRow() == -1))
   {
      return true;
   }

   return false;
}

bool testCellsEqual(Cell &first, Cell &second)
{
   if (  (first.getCol() == second.getCol())
      && (first.getRow() == second.getRow()) )
   {
      return true;
   }

   first.display(true);
   second.display(true);
   return false;
}

TEST(Biomino, defaultConstruction)
{
   Biomino biomino;

   ASSERT_EQ(biomino.getSize(), BIOMINO_SIZE);
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(0)));
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(1)));
}

TEST(Biomino, cellConstructor)
{
   Cell cell;
   Biomino biomino(cell, cell);

   ASSERT_EQ(biomino.getSize(), BIOMINO_SIZE);
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(0)));
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(1)));
}

TEST(Biomino, cellInitialized)
{
   int i;
   Cell cell[BIOMINO_SIZE] = { Cell(0,0), Cell(1,1) };
   Biomino biomino(cell[0], cell[1]);

   ASSERT_EQ(biomino.getSize(), BIOMINO_SIZE);
   for (i = 0; i < biomino.getSize(); i++)
      ASSERT_TRUE(testCellsEqual(biomino.getCell(i), cell[i]));
}

TEST(Biomino, defaultDimension)
{
   Biomino biomino;

   ASSERT_EQ(biomino.getMaxCol(), 0);
   ASSERT_EQ(biomino.getMaxRow(), 0);
}

TEST(Triomino, defaultDimension)
{
   Triomino triomino;

   ASSERT_EQ(triomino.getMaxCol(), 0);
   ASSERT_EQ(triomino.getMaxRow(), 0);
}

TEST(Biomino, assignedDimension)
{
   Biomino biomino(Cell(0,0), Cell(1,1));

   ASSERT_EQ(biomino.getMaxCol(), 2);
   ASSERT_EQ(biomino.getMaxRow(), 2);
}

TEST(Triomino, assignedDimension)
{
   Triomino triomino(Cell(0,0), Cell(1,1), Cell(2,2));
   
   ASSERT_EQ(triomino.getMaxCol(), 3);
   ASSERT_EQ(triomino.getMaxRow(), 3);
}

TEST(Biomino, rotateBiomino)
{
   int i;
   Biomino biomino(Cell(0,0), Cell(1,0));
   Cell    testCells[BIOMINO_SIZE] = { Cell(0,0), Cell(0,1) };
   Shape *transformation;

   ASSERT_EQ(biomino.getMaxCol(), 2);
   ASSERT_EQ(biomino.getMaxRow(), 1);

   transformation = biomino.rotate();
   ASSERT_EQ(transformation->getMaxCol(), 1);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getSize(), biomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST(Biomino, reflectBiomino)
{
   int i;
   Biomino biomino(Cell(0,0), Cell(1,1));
   Cell    testCells[BIOMINO_SIZE] = { Cell(1,0), Cell(0,1) };
   Shape *transformation;

   transformation = biomino.reflect();
   ASSERT_EQ(transformation->getMaxCol(), 2);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getSize(), biomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST(Triomino, rotateTriominoI)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(2,0));
   Cell testCells[TRIOMINO_SIZE] = { Cell(0,0), Cell(0,1), Cell(0,2) };
   Shape *transformation;

   ASSERT_EQ(triomino.getMaxCol(), 3);
   ASSERT_EQ(triomino.getMaxRow(), 1);

   transformation = triomino.rotate();
   ASSERT_EQ(transformation->getMaxCol(), 1);
   ASSERT_EQ(transformation->getMaxRow(), 3);
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST(Triomino, reflectTriominoI)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(2,0));
   Cell testCells[TRIOMINO_SIZE] = { Cell(2,0), Cell(1,0), Cell(0,0) };
   Shape *transformation;

   ASSERT_EQ(triomino.getMaxCol(), 3);
   ASSERT_EQ(triomino.getMaxRow(), 1);

   transformation = triomino.reflect();
   ASSERT_EQ(transformation->getMaxCol(), 3);
   ASSERT_EQ(transformation->getMaxRow(), 1);
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST(Triomino, rotateTriominoV)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(0,1));
   Cell testCells[TRIOMINO_SIZE] = { Cell(1,0), Cell(1,1), Cell(0,0) };
   Shape *transformation;

   ASSERT_EQ(triomino.getMaxCol(), 2);
   ASSERT_EQ(triomino.getMaxRow(), 2);

   transformation = triomino.rotate();
   transformation->display(true);
   ASSERT_EQ(transformation->getMaxCol(), 2);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST(Triomino, reflectTriominoV)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(1,1));
   Cell testCells[TRIOMINO_SIZE] = { Cell(1,0), Cell(0,0), Cell(0,1) };
   Shape *transformation;

   ASSERT_EQ(triomino.getMaxCol(), 2);
   ASSERT_EQ(triomino.getMaxRow(), 2);

   transformation = triomino.reflect();
   ASSERT_EQ(transformation->getMaxCol(), 2);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

