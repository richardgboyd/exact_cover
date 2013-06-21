#include "Shapes.h"

class Board
{
public:
   Board() { maxCols = 0; maxRows = 0; }
   Board(int _maxCols, int _maxRows, int *_boardMap) {
      shapeMap(_maxCols, _maxRows, _boardMap);
   }

   void shapeMap(int cols, int rows, int *_boardMap);

   int getCell(int index);
   bool isCellMapped(int col, int row);
   int getIndex(int col, int row);
   int getIndex(int cellNumber);
   int getMaxCols() const { return maxCols; }
   int getMaxRows() const { return maxRows; }

   bool mapShapeAt(int col, int row, Shape &shape, int *state);

#if !defined(UNIT_TEST)
protected:
#endif
   bool inRange(int index);
   bool canShapeMap(int col, int row, Shape &shape);

private:
   int maxCols;
   int maxRows;
   int *boardMap;
   int *boardIndex;
};
