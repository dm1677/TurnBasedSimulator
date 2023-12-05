#pragma once
#include <cstdint>

namespace TurnBasedSimulator
{
	struct BitBoard
	{
		enum class Preset //TODO: Better name
		{
			Empty,
			Full
		};

		BitBoard(Preset preset = Preset::Empty)
		{
			switch (preset)
			{
			case Preset::Empty:
				m_Board[0] = 0;
				m_Board[1] = 0;
				m_Board[2] = 0;
				m_Board[3] = 0;
				break;
			case Preset::Full:
				m_Board[0] = 0xFFFFFFFFFFFFFFF0;
				m_Board[1] = 0xFFFFFFFFFFFFFFF0;
				m_Board[2] = 0xFFFFFFFFFFFFFFF0;
				m_Board[3] = 0xFFFFFFFFFFF80000;
				break;
			}

		}

		BitBoard(std::uint64_t board[4])
		{
			m_Board[0] = board[0];
			m_Board[1] = board[1];
			m_Board[2] = board[2];
			m_Board[3] = board[3];
		}

		BitBoard(std::uint64_t board1, std::uint64_t board2, std::uint64_t board3, std::uint64_t board4)
		{
			m_Board[0] = board1;
			m_Board[1] = board2;
			m_Board[2] = board3;
			m_Board[3] = board4;
		}

		bool IsSet(unsigned char x, unsigned char y) const
		{
			int pos = 63 - x - (15 * (y % 4));
			uint64_t mask = 1;
			mask <<= pos;
			return (m_Board[y / 4] & mask) == mask;
		}

		bool IsEmpty() const
		{
			return (m_Board[0] == 0
				&& m_Board[1] == 0
				&& m_Board[2] == 0
				&& m_Board[3] == 0);
		}

		BitBoard operator|(const BitBoard& other) const
		{
			uint64_t board[4];
			board[0] = m_Board[0] | other.m_Board[0];
			board[1] = m_Board[1] | other.m_Board[1];
			board[2] = m_Board[2] | other.m_Board[2];
			board[3] = m_Board[3] | other.m_Board[3];
			return BitBoard(board);
		}

		BitBoard operator&(const BitBoard& other) const
		{
			uint64_t board[4];
			board[0] = m_Board[0] & other.m_Board[0];
			board[1] = m_Board[1] & other.m_Board[1];
			board[2] = m_Board[2] & other.m_Board[2];
			board[3] = m_Board[3] & other.m_Board[3];
			return BitBoard(board);
		}

		BitBoard operator~() const
		{
			return BitBoard(
				~m_Board[0],
				~m_Board[1],
				~m_Board[2],
				~m_Board[3]
			);
		}
	private:
		std::uint64_t m_Board[4];
	};
}
/*
000000000000000
000000000000000
000000000000000
000000000000000
0000
000000000000000
000000000000000
000000000000000
000000000000000
0000
000000000000000
000000000000000
000000000000000
000000000000000
0000
000000000000000
000000000000000
000000000000000
000000000000000 //full row of extra bits
0000

y -> y//4th index
x -> left shift 1 by:  4 + (14  % x) + ((3-(y//4))*15)

14, 3 -> 4
13, 3 -> 5
...
0, 3 -> 18
14, 2 -> 19

63 - x - 15y
*/