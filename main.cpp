#include <iostream>
#include <map>
#include <stack>
#include <vector>

class Cell
{
public:
	Cell()
	{
	}

	Cell(bool value)
		: m_Value{value}
		, m_PredictionValue{value}
	{
	}

	bool getValue() const
	{
		return m_Value;
	}

	void setValue(bool value)
	{
		m_Value = value;
	}

	void setPrediction(bool newValue)
	{
		m_PredictionValue = newValue;
	}

	void update()
	{
		setValue(m_PredictionValue);
	}

private:
	bool m_Value = false;
	bool m_PredictionValue = false;
};

class Board
{
public:
	Board(const int width, const int height)
		: m_Width{width}
		, m_Height{height}
	{
		m_Cells.resize(getHeight());
		for (int h = 0; h < getHeight(); h++)
		{
			m_Cells[h].resize(getWidth());
		}
	}

	int getWidth() const
	{
		return m_Width;
	}

	int getHeight() const
	{
		return m_Height;
	}

	Cell& getCell(int width, int height)
	{
		return m_Cells[loopHeight(height)][loopWidth(width)];
	}

	void update()
	{
		for (auto height = 0; height < getHeight(); ++height)
		{
			for (auto width = 0; width < getWidth(); ++width)
			{
				getCell(width, height).setPrediction(makeNewCellState(width, height));
			}
		}

		for (auto height = 0; height < getHeight(); ++height)
		{
			for (auto width = 0; width < getWidth(); ++width)
			{
				getCell(width, height).update();
			}
		}
	}

	int getAliveNeightboursCount(int width, int height)
	{
		long alive = 0;

		for (auto w = width - 1; w <= width + 1; ++w)
		{
			for (auto h = height - 1; h <= height + 1; ++h)
			{
				if (w == width && h == height)
					continue;

				alive += getCell(w, h).getValue();
			}
		}

		return alive;
	}

	void clearRules()
	{
		m_Rules.clear();
	}

	void addRule(bool currentState, long aliveCount, bool newState)
	{
		m_Rules[{currentState, aliveCount}] = newState;
	}

	bool getNewCellState(bool currentState, long aliveCount)
	{
		auto it = m_Rules.find({currentState, aliveCount});
		if (it != m_Rules.end())
			return it->second;

		return false;
	}

	bool makeNewCellState(int width, int height)
	{
		return getNewCellState(getCell(width, height).getValue(), getAliveNeightboursCount(width, height));
	}

	int loopWidth(int width)
	{
		return (getWidth() + width) % getWidth();
	}

	int loopHeight(int height)
	{
		return (getHeight() + height) % getHeight();
	}

private:
	int m_Width;
	int m_Height;

	std::map<std::pair<bool, long>, bool> m_Rules;

	std::vector<std::vector<Cell>> m_Cells;
};

class Game
{
public:
	Game(int width, int height)
	{
		m_BoardHistory.push({width, height});
	}

	Board& getBoard()
	{
		return m_BoardHistory.top();
	}

	void popBoard()
	{
		if (m_BoardHistory.size() > 1)
		{
			m_BoardHistory.pop();
		}
	}

	void updateBoard()
	{
		Board newBoard = getBoard();
		newBoard.update();
		m_BoardHistory.push(newBoard);
	}

private:
	std::stack<Board> m_BoardHistory;
};

void printBoard(Game& game)
{
	for (auto h = 0; h < game.getBoard().getHeight(); ++h)
	{
		for (auto w = 0; w < game.getBoard().getWidth(); ++w)
		{
			std::cout << game.getBoard().getCell(w, h).getValue();
		}
		std::cout << std::endl;
	}
}

int main()
{
	Game game(5, 5);

	game.getBoard().addRule(false, 3, true);
	game.getBoard().addRule(true, 2, true);
	game.getBoard().addRule(true, 3, true);

	game.getBoard().getCell(0, 1).setValue(true);
	game.getBoard().getCell(1, 2).setValue(true);
	game.getBoard().getCell(2, 0).setValue(true);
	game.getBoard().getCell(2, 1).setValue(true);
	game.getBoard().getCell(2, 2).setValue(true);

	printBoard(game);

	char input = 'a';
	while (input != 'e')
	{
		std::cout << "n-next, p-prev, e-end" << std::endl;

		std::cin >> input;

		if (input == 'n')
		{
			game.updateBoard();
		}
		else if (input == 'p')
		{
			game.popBoard();
		}
		system("cls");
		printBoard(game);
	}

	return 0;
}