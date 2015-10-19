#include <iostream>
#include <string>
#include <limits>

const int INFINITY = std::numeric_limits<int>::max();

class GameState {
public:
	int queenRow, queenColumn, kingRow, kingColumn, currentColor;

	GameState(int newKingRow, int newKingColumn, int newQueenRow, int newQueenColumn, int newCurrentColor) {
		queenRow = newQueenRow;
		queenColumn = newQueenColumn;
		kingRow = newKingRow;
		kingColumn = newKingColumn;
		currentColor = newCurrentColor;
	}

	void attackKing() {
		queenRow = kingRow;
		queenColumn = kingColumn;
	}

	void moveKing(int currentRow, int currentColumn) {
		kingRow = currentRow;
		kingColumn = currentColumn;
	}
};

void InputColumn(int &columnValue) {
	char inputPosition;
	std::cin >> inputPosition;
	columnValue = inputPosition - 'a';
}

void InputRow(int &rowValue) {
	char inputPosition;
	std::cin >> inputPosition;
	rowValue = inputPosition - '1';
}

bool CheckQueenMove(const GameState &firstState, GameState secondState) {
	if ((firstState.kingRow != secondState.kingRow) || (firstState.kingColumn != secondState.kingColumn)) {
		return false;
	}

	if ((firstState.queenColumn == secondState.queenColumn) && (firstState.queenRow == secondState.queenRow)) {
		return false;
	}
	
	for (int string = firstState.queenRow; string >= 0; --string) {
		if ((string == 2) && (firstState.queenColumn == 2)) {
			break;
		}
		if ((string == secondState.queenRow) && (firstState.queenColumn == secondState.queenColumn)) {
			return true;
		}
	}

	for (int string = firstState.queenRow; string < 8; ++string) {
		if ((string == 2) && (firstState.queenColumn == 2)) {
			break;
		}
		if ((string == secondState.queenRow) && (firstState.queenColumn == secondState.queenColumn)) {
			return true;
		}
	}

	for (int column = firstState.queenColumn; column >= 0; --column) {
		if ((column == 2) && (firstState.queenRow == 2)) {
			break;
		}
		if ((firstState.queenRow == secondState.queenRow) && (column == secondState.queenColumn)) {
			return true;
		}
	}

	for (int column = firstState.queenColumn; column < 8; ++column) {
		if ((column == 2) && (firstState.queenRow == 2)) {
			break;
		}
		if ((firstState.queenRow == secondState.queenRow) && (column == secondState.queenColumn)) {
			return true;
		}
	}

	for (int string = firstState.queenRow, column = firstState.queenColumn; string >= 0 && column >= 0; --string, --column) {
		if ((column == 2) && (string == 2)) {
			break;
		}
		if ((string == secondState.queenRow) && (column == secondState.queenColumn)) {
			return true;
		}
	}

	for (int string = firstState.queenRow, column = firstState.queenColumn; string >= 0 && column < 8; --string, ++column) {
		if ((column == 2) && (string == 2)) {
			break;
		}
		if ((string == secondState.queenRow) && (column == secondState.queenColumn)) {
			return true;
		}
	}

	for (int string = firstState.queenRow, column = firstState.queenColumn; string < 8 && column >= 0; ++string, --column) {
		if ((column == 2) && (string == 2)) {
			break;
		}
		if ((string == secondState.queenRow) && (column == secondState.queenColumn)) {
			return true;
		}
	}

	for (int string = firstState.queenRow, column = firstState.queenColumn; string < 8 && column < 8; ++string, ++column) {
		if ((column == 2) && (string == 2)) {
			break;
		}
		if ((string == secondState.queenRow) && (column == secondState.queenColumn)) {
			return true;
		}
	}

	return false;
}

bool CheckKingMove(const GameState &firstState, const GameState& secondState) {
	if ((firstState.queenRow != secondState.queenRow) || (firstState.queenColumn != secondState.queenColumn)) {
		return false;
	}

	if ((firstState.kingColumn == secondState.kingColumn) && (firstState.kingRow == secondState.kingRow)) {
		return false;
	}

	if ((abs(firstState.kingColumn - secondState.kingColumn) < 2) && (abs(firstState.kingRow - secondState.kingRow) < 2)) {
		return true;
	}

	return false;
}

bool KingIsAttacked(const GameState &currentState) {
	if ((abs(currentState.kingRow - 2) < 2) && (abs(currentState.kingColumn - 2) < 2)) {
		return true;
	}

	GameState destinationState = currentState;
	destinationState.attackKing();
	
	if (CheckQueenMove(currentState, destinationState)) {
		return true;
	} else {
		return false;
	}

}

bool QueenIsAttacked(const GameState &currentState) {
	if ((abs(currentState.queenRow - 2) < 2) && (abs(currentState.queenColumn - 2) < 2)) {
		return false;
	}

	if ((abs(currentState.queenRow - currentState.kingRow) < 2) && (abs(currentState.queenColumn - currentState.kingColumn) < 2)) {
		return true;
	}

	return false;
}

bool CheckStateValidity(const GameState &currentState) {
	if ((abs(currentState.kingRow - 2) < 2) && (abs(currentState.kingColumn - 2) < 2)) {
		return false;
	}
	
	if ((currentState.queenRow == 2) && (currentState.queenColumn == 2)) {
		return false;
	}

	if ((currentState.kingRow == currentState.queenRow) && (currentState.kingColumn == currentState.queenColumn)) {
		return false;
	}
	
	if ((currentState.currentColor == 0) && (KingIsAttacked(currentState))) {
		return false;
	}

	if ((currentState.currentColor == 1) && (QueenIsAttacked(currentState))) {
		return false;
	}
	
	return true;
}

bool CheckPositionValidity(int currentRow, int currentColumn) {
	if ((currentRow >= 0) && (currentRow < 8) && (currentColumn >= 0) && (currentColumn < 8)) { 
		return true;
	} else {
		return false;
	}
}

bool CheckKingMovable(const GameState &currentState) {
	for (int currentRow = currentState.kingRow - 1; currentRow <= currentState.kingRow + 1; ++currentRow) {
		for (int currentColumn = currentState.kingColumn - 1; currentColumn <= currentState.kingColumn + 1; ++currentColumn) {
			if (CheckPositionValidity(currentRow, currentColumn)) { 
				GameState destinationState = currentState;
				destinationState.moveKing(currentRow, currentColumn);

				if ((CheckKingMove(currentState, destinationState)) && (!(KingIsAttacked(destinationState)))) {
					return true;
				}
			}
		}
	}
	return false;
}

int CheckMat(const GameState &currentState) { 
	if (CheckKingMovable(currentState)) {
		return false;
	}

	if (KingIsAttacked(currentState)) {
		return true;
	} else {
		return false;
	}
}

int CheckStalemat(const GameState &currentState) {
	if (currentState.currentColor == 0) {
		return false;
	}

	if (CheckKingMovable(currentState)) {
		return false;
	}

	if (!(KingIsAttacked(currentState))) {
		return true;
	} else {
		return false;
	}
}

bool CheckGameValidity(const GameState &startGameState) {
	if ((!(CheckStateValidity(startGameState))) || (CheckStalemat(startGameState))) {
		return false;
	}
	return true;
}

int CountAnswer(const GameState &startGameState) {
	int states[8][8][8][8][2];
	int matDistance = 0;

	for (int kingRow = 0; kingRow < 8; ++kingRow) {
		for (int kingColumn = 0; kingColumn < 8; ++kingColumn) {
			for (int queenRow = 0; queenRow < 8; ++queenRow) {
				for (int queenColumn = 0; queenColumn < 8; ++queenColumn) {					
					states[kingRow][kingColumn][queenRow][queenColumn][0] = INFINITY;
					states[kingRow][kingColumn][queenRow][queenColumn][1] = INFINITY;
					
					GameState currentState(kingRow, kingColumn, queenRow, queenColumn, 1);

					if ((CheckStateValidity(currentState)) && (CheckMat(currentState))) {
						states[kingRow][kingColumn][queenRow][queenColumn][1] = 0;
					}					
				}
			}
		}
	}

	while (states[startGameState.kingRow][startGameState.kingColumn][startGameState.queenRow][startGameState.queenColumn][0] == INFINITY) {		
		for (int kingRow = 0; kingRow < 8; ++kingRow) {
			for (int kingColumn = 0; kingColumn < 8; ++kingColumn) {
				for (int queenRow = 0; queenRow < 8; ++queenRow) {
					for (int queenColumn = 0; queenColumn < 8; ++queenColumn) {						
						GameState currentState(kingRow, kingColumn, queenRow, queenColumn, 0);
						if ((!(CheckStateValidity(currentState))) || (states[kingRow][kingColumn][queenRow][queenColumn][0] <= matDistance)) {
							continue;
						}

						bool breakFlag = false;
						for (int newQueenRow = 0; newQueenRow < 8; ++newQueenRow) { 
							for (int newQueenColumn = 0; newQueenColumn < 8; ++newQueenColumn) {
								GameState newState(kingRow, kingColumn, newQueenRow, newQueenColumn, 1);
								if ((CheckStateValidity(newState)) && (CheckQueenMove(currentState, newState))) {
									if (states[kingRow][kingColumn][newQueenRow][newQueenColumn][1] == matDistance) {
										states[kingRow][kingColumn][queenRow][queenColumn][0] = matDistance + 1;
										breakFlag = true;
										break;
									}
								}
							}
							if (breakFlag) {
								break;
							}
						}
						
					}
				}
			}
		}	
		
		++matDistance;

		for (int kingRow = 0; kingRow < 8; ++kingRow) {
			for (int kingColumn = 0; kingColumn < 8; ++kingColumn) {
				for (int queenRow = 0; queenRow < 8; ++queenRow) {
					for (int queenColumn = 0; queenColumn < 8; ++queenColumn) {							
						GameState currentState(kingRow, kingColumn, queenRow, queenColumn, 1);
						if ((!(CheckStateValidity(currentState))) || ((states[kingRow][kingColumn][queenRow][queenColumn][1] <= matDistance) || (CheckStalemat(currentState)))) {
							continue;
						}

						bool escapeFlag = false;
						bool breakFlag = false;
						for (int newKingRow = std::max(0, kingRow - 1); newKingRow < std::min(8, kingRow + 2); ++newKingRow) {
							for (int newKingColumn = std::max(0, kingColumn - 1); newKingColumn < std::min(8, kingColumn + 2); ++newKingColumn) {
								GameState newState(newKingRow, newKingColumn, queenRow, queenColumn, 0);
								if ((CheckStateValidity(newState)) && (CheckKingMove(currentState, newState)) && (states[newKingRow][newKingColumn][queenRow][queenColumn][0] == INFINITY)) {
									escapeFlag = true;
									breakFlag = true;
									break;
								}
							}
							if (breakFlag) {
								break;
							}
						}

						if (!(escapeFlag)) {
							states[kingRow][kingColumn][queenRow][queenColumn][1] = matDistance + 1;
						}
					}
				}
			}
		}	

		++matDistance;
	}

	return states[startGameState.kingRow][startGameState.kingColumn][startGameState.queenRow][startGameState.queenColumn][0];
}

int main() {
	int inputtedQueenRow, inputtedQueenColumn, inputtedKingRow, inputtedKingColumn;
	InputColumn(inputtedQueenColumn);
	InputRow(inputtedQueenRow);
	InputColumn(inputtedKingColumn);
	InputRow(inputtedKingRow);

	GameState startGameState(inputtedKingRow, inputtedKingColumn, inputtedQueenRow, inputtedQueenColumn, 0);
	
	if (!(CheckGameValidity(startGameState))) {
		std::cout << "IMPOSSIBLE";
		return 0;
	}

	std::cout << CountAnswer(startGameState); 

	return 0;
}