#include <iostream>
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

		alive += getCell(width - 1, height - 1).getValue();
		alive += getCell(width, height - 1).getValue();
		alive += getCell(width + 1, height - 1).getValue();
		alive += getCell(width - 1, height).getValue();
		alive += getCell(width + 1, height).getValue();
		alive += getCell(width - 1, height + 1).getValue();
		alive += getCell(width, height + 1).getValue();
		alive += getCell(width + 1, height + 1).getValue();

		return alive;
	}

	bool makeNewCellState(int width, int height)
	{
		auto alive = getAliveNeightboursCount(width, height);

		if (!getCell(width, height).getValue() && alive == 3)
		{
			return true;
		}
		else if (getCell(width, height).getValue() && (alive == 3 || alive == 2))
		{
			return true;
		}
		return false;
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