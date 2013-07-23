#include "gtest/gtest.h"
#include "Shapes.h"

/*
 * Cell tests
 */
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

class ShapeTest : public ::testing::Test
{
protected:
   virtual void SetUp() {
      Shape::resetMasterIndex();
   }
};

class BiominoTest : public ShapeTest
{
};

/*
 * Biomino Tests
 */
TEST_F(BiominoTest, defaultConstruction)
{
   Biomino biomino;

   ASSERT_EQ(biomino.getSize(), BIOMINO_SIZE);
   ASSERT_EQ(biomino.getIndex(), -1);
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(0)));
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(1)));
}

TEST_F(BiominoTest, cellConstructor)
{
   Cell cell;
   Biomino biomino(cell, cell);

   ASSERT_EQ(biomino.getSize(), BIOMINO_SIZE);
   ASSERT_EQ(biomino.getIndex(), 0);
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(0)));
   ASSERT_TRUE(testCellUnitialized(biomino.getCell(1)));
}

TEST_F(BiominoTest, multipleConstruction)
{
   Cell cell;
   Biomino biomino1(cell, cell);
   Biomino biomino2(cell, cell);

   ASSERT_EQ(biomino1.getSize(), BIOMINO_SIZE);
   ASSERT_EQ(biomino1.getIndex(), 0);
   ASSERT_EQ(biomino2.getSize(), BIOMINO_SIZE);
   ASSERT_EQ(biomino2.getIndex(), 1);
}

TEST_F(BiominoTest, cellInitialized)
{
   int i;
   Cell cell[BIOMINO_SIZE] = { Cell(0,0), Cell(1,1) };
   Biomino biomino(cell[0], cell[1]);

   ASSERT_EQ(biomino.getSize(), BIOMINO_SIZE);
   ASSERT_EQ(biomino.getIndex(), 0);
   for (i = 0; i < biomino.getSize(); i++)
      ASSERT_TRUE(testCellsEqual(biomino.getCell(i), cell[i]));
}

TEST_F(BiominoTest, defaultDimension)
{
   Biomino biomino;

   ASSERT_EQ(biomino.getIndex(), -1);
   ASSERT_EQ(biomino.getMaxCol(), 0);
   ASSERT_EQ(biomino.getMaxRow(), 0);
}

TEST_F(BiominoTest, assignedDimension)
{
   Biomino biomino(Cell(0,0), Cell(1,1));

   ASSERT_EQ(biomino.getIndex(), 0);
   ASSERT_EQ(biomino.getMaxCol(), 2);
   ASSERT_EQ(biomino.getMaxRow(), 2);
}

TEST_F(BiominoTest, rotateBiomino)
{
   int i;
   Biomino biomino(Cell(0,0), Cell(1,0));
   Cell    testCells[BIOMINO_SIZE] = { Cell(0,0), Cell(0,1) };
   Shape *transformation;

   ASSERT_EQ(biomino.getIndex(), 0);
   ASSERT_EQ(biomino.getMaxCol(), 2);
   ASSERT_EQ(biomino.getMaxRow(), 1);

   transformation = biomino.rotate();
   ASSERT_EQ(transformation->getMaxCol(), 1);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getIndex(), biomino.getIndex());
   ASSERT_EQ(transformation->getSize(), biomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST_F(BiominoTest, reflectBiomino)
{
   int i;
   Biomino biomino(Cell(0,0), Cell(1,1));
   Cell    testCells[BIOMINO_SIZE] = { Cell(1,0), Cell(0,1) };
   Shape *transformation;

   ASSERT_EQ(biomino.getIndex(), 0);

   transformation = biomino.reflect();
   ASSERT_EQ(transformation->getMaxCol(), 2);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getIndex(), biomino.getIndex());
   ASSERT_EQ(transformation->getSize(), biomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

/*
 * Triomino Tests
 */
class TriominoTest : public ShapeTest
{
};

TEST_F(TriominoTest, defaultDimension)
{
   Triomino triomino;

   ASSERT_EQ(triomino.getIndex(), -1);
   ASSERT_EQ(triomino.getMaxCol(), 0);
   ASSERT_EQ(triomino.getMaxRow(), 0);
}

TEST_F(TriominoTest, assignedDimension)
{
   Triomino triomino(Cell(0,0), Cell(1,1), Cell(2,2));
   
   ASSERT_EQ(triomino.getIndex(), 0);
   ASSERT_EQ(triomino.getMaxCol(), 3);
   ASSERT_EQ(triomino.getMaxRow(), 3);
}


TEST_F(TriominoTest, multipleConstruction)
{
   Triomino triominoX(Cell(0,0), Cell(1,1), Cell(2,2));
   Triomino triominoV(Cell(0,0), Cell(1,0), Cell(1,1));
   
   ASSERT_EQ(triominoX.getIndex(), 0);
   ASSERT_EQ(triominoX.getMaxCol(), 3);
   ASSERT_EQ(triominoX.getMaxRow(), 3);

   ASSERT_EQ(triominoV.getIndex(), 1);
   ASSERT_EQ(triominoV.getMaxCol(), 2);
   ASSERT_EQ(triominoV.getMaxRow(), 2);
}

TEST_F(TriominoTest, rotateTriominoI)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(2,0));
   Cell testCells[TRIOMINO_SIZE] = { Cell(0,0), Cell(0,1), Cell(0,2) };
   Shape *transformation;

   ASSERT_EQ(triomino.getIndex(), 0);
   ASSERT_EQ(triomino.getMaxCol(), 3);
   ASSERT_EQ(triomino.getMaxRow(), 1);

   transformation = triomino.rotate();
   ASSERT_EQ(transformation->getIndex(), triomino.getIndex());
   ASSERT_EQ(transformation->getMaxCol(), 1);
   ASSERT_EQ(transformation->getMaxRow(), 3);
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST_F(TriominoTest, reflectTriominoI)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(2,0));
   Cell testCells[TRIOMINO_SIZE] = { Cell(2,0), Cell(1,0), Cell(0,0) };
   Shape *transformation;

   ASSERT_EQ(triomino.getIndex(), 0);
   ASSERT_EQ(triomino.getMaxCol(), 3);
   ASSERT_EQ(triomino.getMaxRow(), 1);

   transformation = triomino.reflect();
   ASSERT_EQ(transformation->getMaxCol(), 3);
   ASSERT_EQ(transformation->getMaxRow(), 1);
   ASSERT_EQ(transformation->getIndex(), triomino.getIndex());
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST_F(TriominoTest, rotateTriominoV)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(0,1));
   Cell testCells[TRIOMINO_SIZE] = { Cell(1,0), Cell(1,1), Cell(0,0) };
   Shape *transformation;

   ASSERT_EQ(triomino.getIndex(), 0);
   ASSERT_EQ(triomino.getMaxCol(), 2);
   ASSERT_EQ(triomino.getMaxRow(), 2);

   transformation = triomino.rotate();
   ASSERT_EQ(transformation->getMaxCol(), 2);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getIndex(), triomino.getIndex());
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

TEST_F(TriominoTest, reflectTriominoV)
{
   int i;
   Triomino triomino(Cell(0,0), Cell(1,0), Cell(1,1));
   Cell testCells[TRIOMINO_SIZE] = { Cell(1,0), Cell(0,0), Cell(0,1) };
   Shape *transformation;

   ASSERT_EQ(triomino.getIndex(), 0);
   ASSERT_EQ(triomino.getMaxCol(), 2);
   ASSERT_EQ(triomino.getMaxRow(), 2);

   transformation = triomino.reflect();
   ASSERT_EQ(transformation->getMaxCol(), 2);
   ASSERT_EQ(transformation->getMaxRow(), 2);
   ASSERT_EQ(transformation->getIndex(), triomino.getIndex());
   ASSERT_EQ(transformation->getSize(), triomino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}

/*
 * Tetromino tests
 */
class TetrominoTest : public ShapeTest
{
};

TEST_F(TetrominoTest, defaultDimension)
{
   Tetromino tetromino;

   ASSERT_EQ(tetromino.getIndex(), -1);
   ASSERT_EQ(tetromino.getMaxCol(), 0);
   ASSERT_EQ(tetromino.getMaxRow(), 0);
}

TEST_F(TetrominoTest, assignedDimension)
{
   Tetromino tetromino(Cell(0,0), Cell(1,1), Cell(2,2), Cell(3,3));

   ASSERT_EQ(tetromino.getIndex(), 0);
   ASSERT_EQ(tetromino.getMaxCol(), 4);
   ASSERT_EQ(tetromino.getMaxRow(), 4);
}

TEST_F(TetrominoTest, rotateTetrominoI)
{
   int i;
   Tetromino tetromino(Cell(0,0), Cell(1,0), Cell(2,0), Cell(3,0));
   Cell testCells[TETROMINO_SIZE] = { Cell(0,0), Cell(0,1), Cell(0,2), Cell(0,3) };
   Shape *transformation;

   ASSERT_EQ(tetromino.getIndex(), 0);
   ASSERT_EQ(tetromino.getMaxCol(), 4);
   ASSERT_EQ(tetromino.getMaxRow(), 1);

   transformation = tetromino.rotate();
   ASSERT_EQ(transformation->getIndex(), tetromino.getIndex());
   ASSERT_EQ(transformation->getMaxCol(), 1);
   ASSERT_EQ(transformation->getMaxRow(), 4);
   ASSERT_EQ(transformation->getSize(), tetromino.getSize());
   for (i = 0; i < transformation->getSize(); i++)
      ASSERT_TRUE(testCellsEqual(transformation->getCell(i), testCells[i]));
}
