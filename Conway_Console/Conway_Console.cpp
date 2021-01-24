// Conway_Console.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <stdio.h>
constexpr short conX = 315;//地图宽度
constexpr short conY = 165;//地图高度
constexpr int ndelay = 1;
constexpr wchar_t cellAlive = L'●';
constexpr wchar_t cellDead = L'○';

HANDLE hConsole = nullptr;
int iBuffer = 0;
wchar_t kBuffer[2][conY][conX] = { 0 };

inline DWORD printXY(const wchar_t* str, const DWORD sLen, const short& nX, const short& nY)
{
	static DWORD len;
	SetConsoleCursorPosition(hConsole, { nX,nY });
	WriteConsoleW(hConsole, str, sLen, &len, NULL);
	//WriteConsoleOutputCharacterW(hConsole, str, sLen, { nX,nY }, &len);
	return len;
}
inline auto& getMap()
{
	return kBuffer[iBuffer];
}
inline auto& getMapAlt()
{
	return kBuffer[1 - iBuffer];
}
inline const wchar_t& getCell(const short& x, const short& y)
{
	if (x < 0 || y < 0 || x >= conX || y >= conY)
		return cellDead;
	return getMap()[y][x];
}
inline const wchar_t& getCellAlt(const short& x, const short& y)
{
	if (x < 0 || y < 0 || x >= conX || y >= conY)
		return cellDead;
	return getMapAlt()[y][x];
}
inline void setCell(const short& x, const short& y, const bool& cell)
{
	auto& mcell = getMap()[y][x];
	if (!(x < 0 || y < 0 || x >= conX || y >= conY))
		mcell = (cell ? cellAlive : cellDead);
}
inline void setCellAlt(const short& x, const short& y, const bool& cell)
{
	auto& mcell = getMapAlt()[y][x];
	if (!(x < 0 || y < 0 || x >= conX || y >= conY))
		mcell = (cell ? cellAlive : cellDead);
}
inline const bool isCellAlive(const short& x, const short& y)
{
	return ((getCell(x, y) == cellAlive) ? true : false);
}
inline const short countLivesAround(const short& x, const short& y)
{
	return
		static_cast<short>(isCellAlive(x - 1, y - 1)) +
		static_cast<short>(isCellAlive(x - 1, y)) +
		static_cast<short>(isCellAlive(x - 1, y + 1)) +
		static_cast<short>(isCellAlive(x, y - 1)) +
		static_cast<short>(isCellAlive(x, y + 1)) +
		static_cast<short>(isCellAlive(x + 1, y - 1)) +
		static_cast<short>(isCellAlive(x + 1, y)) +
		static_cast<short>(isCellAlive(x + 1, y + 1));
}
inline void initMap()
{
	for (auto& i : getMap())
		for (auto& j : i)
			j = (((rand() % 100) > 50) ? cellDead : cellAlive);
	for (auto& i : getMapAlt())
		for (auto& j : i)
			j = cellDead;
}
inline const bool drawMap()
{
	short livesAround = 0;
	bool isChanged = false;
	for (short y = 0; y < conY; y++)
	{
		for (short x = 0; x < conX; x++)
		{
			livesAround = countLivesAround(x, y);
			if (isCellAlive(x, y))
			{
					setCellAlt(x, y, true);
				if (livesAround < 2 || livesAround > 3)
				{
					setCellAlt(x, y, false);
					isChanged = true;
				}
			}
			else
			{
				setCellAlt(x, y, false);
				if (livesAround == 3)
				{
					setCellAlt(x, y, true);
					isChanged = true;
				}
			}
		}
	}
	return isChanged;
}
inline void printMap()
{
	static DWORD len;
	auto line = getMap()[0];
	for (short y = 0; y < conY; y++)
	{
		line = getMap()[y];
		printXY(line, conX, 0, y);
	}
}
inline void toggleMap()
{
	iBuffer = 1 - iBuffer;
}
int main()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	initMap();
	printMap();
	getchar();
	while (1)
	{
		printMap();
		if (!drawMap())break;
		toggleMap();
		Sleep(ndelay);
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
