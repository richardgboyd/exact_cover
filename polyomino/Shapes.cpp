#include <stdio.h>
#include "Shapes.h"

Cell::Cell(Cell const &cell)
{
   setPosition(cell);
}

void Cell::setPosition(int _col, int _row) 
{ 
   if (!assignedCol()) col = _col; 
   if (!assignedRow()) row = _row; 
}

void Cell::setPosition(Cell const &cell)
{
   setPosition(cell.col, cell.row);
}

void Cell::normalize(int colAdjust, int rowAdjust)
{
   if (assignedCol() && assignedRow())
   {
      col = col - colAdjust;
      row = row - rowAdjust;
   }
}

void Cell::display(bool emitNewline)
{
   printf("Cell(%d, %d)%c", getCol(), getRow(), emitNewline ? '\n':'\0');
}

Cell &Cell::transform(void)
{
   return *this;
}

int Shape::masterIndex = 0;

void Shape::display(bool emitNewline)
{
   int i;
   printf("Shape(");

   for (i = 0; i < getSize(); i++)
   {
      getCell(i).display(false);
   }
   printf(")%c", emitNewline ? '\n':'\0');
}

bool Shape::inRange(int index)
{
   return ((index > -1) && (index < getSize()));
}

Cell &Shape::getCell(int index) 
{ 
   static Cell dummyCell;

   if (inRange(index))
      return cells[index]; 
   else
      return dummyCell;
}

void Shape::setCell(Cell const &cell, int index)
{
   if (inRange(index))
   {
      if (cell.getCol() + 1 > maxCols)
         maxCols = cell.getCol() + 1;

      if (cell.getRow() + 1 > maxRows) 
         maxRows = cell.getRow() + 1;

      cells[index].setPosition(cell);
   }
}

void Shape::normalize(int normalizeCol, int normalizeRow)
{
   int index;

   if (normalizeCol > 0 || normalizeRow > 0)
   {
      for (index = 0; index < getSize(); index++)
         getCell(index).normalize(normalizeCol, normalizeRow);

      maxCols = maxCols - normalizeCol;
      maxRows = maxRows - normalizeRow;
   }
}

void Shape::rotateTo(Shape *newShape)
{
   int index, matrixIndex;
   int col, row;
   int normalizeCol, normalizeRow;
   
   // A 1X2 shape becomes a 2x1 shape after rotation
   // So after rotation the cells will report off by one row
   // We need to normalize the end result
   normalizeCol = getSize();// - getMaxRow();
   normalizeRow = getSize();// - getMaxCol();

   for (index = 0; index < getSize(); index++)
   {
      matrixIndex = getCell(index).getCol() 
                  + getCell(index).getRow() * getSize();

      col =   getCell(index).getCol() 
            + getColRotateTransform(matrixIndex);
      if (col < normalizeCol)
         normalizeCol = col;

      row =   getCell(index).getRow()
            + getRowRotateTransform(matrixIndex);
      if (row < normalizeRow)
         normalizeRow = row;

      newShape->setCell(Cell(col, row), index);
   }

   newShape->setIndex(getIndex());
   newShape->normalize(normalizeCol, normalizeRow);
}

void Shape::reflectTo(Shape *newShape)
{
   int index, matrixIndex;
   int col, normalizeCol;
   
   // A 1X2 shape becomes a 2x1 shape after rotation
   // So after rotation the cells will report off by one row
   // We need to normalize the end result
   normalizeCol = getSize(); // - getMaxCol();

   for (index = 0; index < getSize(); index++)
   {
      matrixIndex = getCell(index).getCol() 
                  + getCell(index).getRow() * getSize();

      col =   getCell(index).getCol() 
            + getColReflectTransform(matrixIndex);
            //- normalizeCol;

      if (col < normalizeCol) 
         normalizeCol = col;

      newShape->setCell(Cell(col, getCell(index).getRow()), index);
   }

   newShape->setIndex(getIndex());
   newShape->normalize(normalizeCol, 0);
}

/*******************************************************************************
   Biomino rotate and transform operations
*******************************************************************************/
int Biomino::colRotateTransform[]  = { 1,  0,  0, -1 };
int Biomino::rowRotateTransform[]  = { 0,  1, -1,  0 };
Shape *Biomino::rotate(void)
{
   Biomino *biomino = new Biomino;
   rotateTo(biomino);
   return biomino;
}

int Biomino::colReflectTransform[] = {  1, -1,  1, -1 };
int Biomino::rowReflectTransform[] = {  1,  1, -1, -1 };
Shape *Biomino::reflect(void)
{
   Biomino *biomino = new Biomino;
   reflectTo(biomino);
   return biomino;
}

/*******************************************************************************
   Teriomino rotate and transform operations
*******************************************************************************/
int Triomino::colRotateTransform[] = {  
      2,  1,  0, 
      1,  0, -1,
      0, -1, -2 
      };

int Triomino::rowRotateTransform[] = { 
      0,  1,  2,
     -1,  0,  1,
      0, -1, -2 
      };

Shape *Triomino::rotate(void)
{
   Triomino *triomino = new Triomino;
   rotateTo(triomino);
   return triomino;
}

int Triomino::colReflectTransform[] = {  
      2,  0, -2,
      2,  0, -2,
      2,  0, -2 
      };

Shape *Triomino::reflect(void)
{
   Triomino *triomino = new Triomino;
   reflectTo(triomino);
   return triomino;
}


/*******************************************************************************
   Tetromino rotate and transform operations
*******************************************************************************/
int Tetromino::colRotateTransform[] = {  
      3,  2,  1,  0, 
      2,  1,  0, -1,
     -1,  0, -1, -2,
      0, -1, -2, -3
      };

int Tetromino::rowRotateTransform[] = { 
      0,  1,  2,  3,
     -1,  0,  1,  2,
     -2, -1,  0,  1,
     -3, -2, -1,  0
      };

Shape *Tetromino::rotate(void)
{
   Tetromino *tetromino = new Tetromino;
   rotateTo(tetromino);
   return tetromino;
}

int Tetromino::colReflectTransform[] = {  
      2,  1, -1, -2,
      2,  1, -1, -2,
      2,  1, -1, -2,
      2,  1, -1, -2 
};

Shape *Tetromino::reflect(void)
{
   Tetromino *tetromino = new Tetromino;
   reflectTo(tetromino);
   return tetromino;
}

/*******************************************************************************
   Pentomino rotate and transform operations
*******************************************************************************/
int Pentomino::colRotateTransform[] = {  
      4,  3,  2,  1,  0, 
      3,  2,  1,  0, -1,
      2,  1,  0, -1, -2,
      1,  0, -1, -2, -3,
      0, -1, -2, -3, -4
      };

int Pentomino::rowRotateTransform[] = { 
      0,  1,  2,  3,  4,
     -1,  0,  1,  2,  3,
     -2, -1,  0,  1,  2,
     -3, -2, -1,  0,  1,
     -4, -3, -2, -1,  0
      };

Shape *Pentomino::rotate(void)
{
   Pentomino *tentomino = new Pentomino;
   rotateTo(tentomino);
   return tentomino;
}

int Pentomino::colReflectTransform[] = {  
      4,  2,  0, -2, -4,
      4,  2,  0, -2, -4,
      4,  2,  0, -2, -4,
      4,  2,  0, -2, -4,
      4,  2,  0, -2, -4 
};

Shape *Pentomino::reflect(void)
{
   Pentomino *tentomino = new Pentomino;
   reflectTo(tentomino);
   return tentomino;
}


