#include <vector>
#include "Shapes.h"
#include "Board.h"

class StateSpace
{
public:
	StateSpace() { };
	~StateSpace();

	void setBoard(Board const &_board);
	void addShape(Shape *shape);

	Board &getBoard(void) { return board; }

	int getShapeCount() { return shapes.size(); }
	Shape *getShapeAt(int index);

   void mapAllShapes(void);

   void displayState(void);
   void displayShapes(void);

#if !defined(UNIT_TEST)
protected:
#endif
   int mapShape(int index);
   int *getNewRow(int index);
#if defined(UNIT_TEST)
   int *getStateRow(int index) { return stateRows[index]; }
   int getStateRowCount(void) { return stateRows.size(); }
#endif

private:
	std::vector<int *> stateRows;
	std::vector<Shape *> shapes;
	Board  board;
};
