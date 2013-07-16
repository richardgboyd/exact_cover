/*******************************************************************************

   Shapes file that provides a way to define and manipulate shapes used in
   building state space for exact cover problem

   Author:  Richard Boyd
   email:   richardgboyd@gmail.com
   Date:    2013.07.15

*******************************************************************************/
#ifndef SHAPES_HEADER
#define SHAPES_HEADER

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

class Cell
{
public:
   Cell() { col = -1; row = -1; }
   Cell(int _col, int _row) { col = _col; row = _row; }
   Cell(Cell const &cell);

   int getCol(void) const { return col; }
   int getRow(void) const { return row; }

   void setPosition(int _col, int _row);
   void setPosition(Cell const &cell);
   void normalize(int colAdjust, int rowAdjust);

   Cell &transform(void);

   void display(bool emitNewline = false);

protected:
   bool assignedCol(void) { return col != -1; }
   bool assignedRow(void) { return row != -1; }

private:
   int col;
   int row;
};


class Shape
{
public:
  Shape(int _size) { 
      size = _size; 
      maxCols = 0;
      maxRows = 0;
      cells = new Cell[size]; 
   }
   virtual ~Shape() { delete[] cells; }

   int getSize(void) { return size; }

   Cell &getCell(int index); 
   void setCell(Cell const &cell, int index);

   virtual Shape *rotate(void) = 0;
   virtual Shape *reflect(void) = 0;

   void display(bool emitNewline = false);
#if !defined(UNIT_TEST)
protected:
#endif
   int getMaxCol(void) { return maxCols; }
   int getMaxRow(void) { return maxRows; }

   void normalize(int normalizeCol, int normalizeRow);

   void rotateTo(Shape *shape);
   void reflectTo(Shape *shape);

   virtual int getColRotateTransform(int index) = 0;
   virtual int getRowRotateTransform(int index) = 0;

   virtual int getColReflectTransform(int index) = 0;
   //virtual int getRowReflectTransform(void);

private:
   bool inRange(int index);

private:
   // Remove default constructor
   Shape();

private:
   int  size;
   int  maxCols;
   int  maxRows;
   Cell *cells;
};


class Biomino : public Shape
{
#define BIOMINO_SIZE (2)

public:
   Biomino():Shape(BIOMINO_SIZE) { };
   Biomino(
         Cell const &loc0, 
         Cell const &loc1) : Shape(BIOMINO_SIZE) 
   {
      setCell(loc0, 0);
      setCell(loc1, 1);
   } 
   virtual ~Biomino() { };

   virtual Shape *rotate(void);
   virtual Shape *reflect(void);

protected:
   static int colRotateTransform[BIOMINO_SIZE * BIOMINO_SIZE];
   static int rowRotateTransform[BIOMINO_SIZE * BIOMINO_SIZE];

   static int colReflectTransform[BIOMINO_SIZE * BIOMINO_SIZE];
   static int rowReflectTransform[BIOMINO_SIZE * BIOMINO_SIZE];

protected:
   virtual int getColRotateTransform(int index) 
      { return colRotateTransform[index]; }
   virtual int getRowRotateTransform(int index) 
      { return rowRotateTransform[index]; }

   virtual int getColReflectTransform(int index) 
      { return colReflectTransform[index]; }
   //virtual int getRowReflectTransform(void) { return rowReflectTransform; }
};

class Triomino : public Shape
{
#define TRIOMINO_SIZE (3)

public:
   Triomino():Shape(TRIOMINO_SIZE) { };
   Triomino(
         Cell const &loc0, 
         Cell const &loc1, 
         Cell const &loc2) : Shape(TRIOMINO_SIZE) 
   {
      setCell(loc0, 0);
      setCell(loc1, 1);
      setCell(loc2, 2);
   }

   virtual Shape *rotate(void);
   virtual Shape *reflect(void);

protected:
   static int colRotateTransform[TRIOMINO_SIZE * TRIOMINO_SIZE];
   static int rowRotateTransform[TRIOMINO_SIZE * TRIOMINO_SIZE];

   static int colReflectTransform[TRIOMINO_SIZE * TRIOMINO_SIZE];
   //static int rowReflectTransform[TRIOMINO_SIZE * TRIOMINO_SIZE];

protected:
   virtual int getColRotateTransform(int index) 
      { return colRotateTransform[index]; }
   virtual int getRowRotateTransform(int index) 
      { return rowRotateTransform[index]; }

   virtual int getColReflectTransform(int index) 
      { return colReflectTransform[index]; }
   //virtual int getRowReflectTransform(void) { return rowReflectTransform; }
};

class Quadomino : public Shape
{
#define QUADOMINO_SIZE (4)

public:
   Quadomino():Shape(QUADOMINO_SIZE) { };
   Quadomino(
         Cell const &loc0, 
         Cell const &loc1, 
         Cell const &loc2,
         Cell const &loc3) : Shape(QUADOMINO_SIZE) 
   {
      setCell(loc0, 0);
      setCell(loc1, 1);
      setCell(loc2, 2);
      setCell(loc3, 3);
   }

   virtual Shape *rotate(void);
   virtual Shape *reflect(void);
};


class Pentomino : public Shape
{
#define PENTOMINO_SIZE (5)

public:
   Pentomino():Shape(PENTOMINO_SIZE) { };
   Pentomino(
         Cell const &loc0, 
         Cell const &loc1, 
         Cell const &loc2,
         Cell const &loc3,
         Cell const &loc4) : Shape(PENTOMINO_SIZE) 
   {
      setCell(loc0, 0);
      setCell(loc1, 1);
      setCell(loc2, 2);
      setCell(loc3, 3);
      setCell(loc4, 4);
   }

   virtual Shape *rotate(void);
   virtual Shape *reflect(void);
};

#endif

