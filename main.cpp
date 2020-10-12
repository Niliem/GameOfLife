#include <iostream>
#include <vector>

class Cell
{
public:
	Cell()
	{
	}

	Cell(bool value)
		: m_Value{value}
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

private:
	bool m_Value = false;
};

class Board
{
public:
	Board(const int width, const int height)
		: m_Width{width}
		, m_Height{height}
	{
		m_Cells.resize(getHeight());
		for (int i = 0; i < getHeight(); i++)
		{
			m_Cells[i].resize(getWidth());
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
		std::vector<std::vector<Cell>> temp;

		temp.resize(getHeight());
		for (auto height = 0; height < getHeight(); ++height)
		{
			temp[height].resize(getWidth());
		}

		for (auto height = 0; height < getHeight(); ++height)
		{
			for (auto width = 0; width < getWidth(); ++width)
			{
				temp[height][width] = makeCellState(width, height);
			}
		}

		m_Cells = temp;
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

	bool makeCellState(int width, int height)
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

int main()
{
	Board board(5, 5);

	board.getCell(1, 2).setValue(true);
	board.getCell(2, 2).setValue(true);
	board.getCell(3, 2).setValue(true);

	for (auto h = 0; h < board.getHeight(); ++h)
	{
		for (auto w = 0; w < board.getWidth(); ++w)
		{
			std::cout << board.getCell(w, h).getValue();
		}
		std::cout << std::endl;
	}

	board.update();
	std::cout << std::endl;

	for (auto h = 0; h < board.getHeight(); ++h)
	{
		for (auto w = 0; w < board.getWidth(); ++w)
		{
			std::cout << board.getCell(w, h).getValue();
		}
		std::cout << std::endl;
	}

	board.update();
	std::cout << std::endl;

	for (auto h = 0; h < board.getHeight(); ++h)
	{
		for (auto w = 0; w < board.getWidth(); ++w)
		{
			std::cout << board.getCell(w, h).getValue();
		}
		std::cout << std::endl;
	}

	return 0;
}