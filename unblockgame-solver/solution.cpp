#include <iostream>
#include <map>
#include <string>
#include <time.h>

using namespace std;

typedef struct
{
	int x, y;
	int orientation; // 0 - horizontal, 1 - vertical
	int length;
	int isRed;
	char Symbol;
} Stick;

typedef struct
{
	// used letters
	int used[256];
	// array of sticks
	Stick a[100];
	// length of array
	int Count;
	// redstick
	int RedStick;
} GameState;

int n, m;
char a[15][15], currentGameStateMatrix[15][15];
GameState firstState;
bool win = false;
clock_t mytime;
map<string, pair<string, bool>> used;
GameState queue[30000], fin;

string GetStringByState(GameState state)
{
	char a[15][15];
	string result;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			a[i][j] = '.';

	for (int i = 0; i < state.Count; i++)
	{
		int x = state.a[i].x, y = state.a[i].y;
		if (state.a[i].orientation)
		{
			for (int j = 0; j < state.a[i].length; j++)
				a[x + j][y] = state.a[i].Symbol;
		}
		else
		{
			for (int j = 0; j < state.a[i].length; j++)
				a[x][y + j] = state.a[i].Symbol;
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			result += a[i][j];

	return result;
}

bool isWinPosition(GameState state)
{
	return (state.a[state.RedStick].y + state.a[state.RedStick].length == m) ? true : false;
}

void bfs(GameState v)
{
	int ptr1 = 0, ptr2 = 1;
	queue[ptr1] = v;
	used[GetStringByState(v)] = make_pair("-1", true);
	GameState tmpstate;
	mytime = clock();
	while (ptr1 < ptr2)
	{
		//build a matrix of gamestate, check the win-pos
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				currentGameStateMatrix[i][j] = '.';

		for (int i = 0; i < queue[ptr1].Count; i++)
		{
			int x = queue[ptr1].a[i].x, y = queue[ptr1].a[i].y;
			if (queue[ptr1].a[i].orientation)
			{
				for (int j = 0; j < queue[ptr1].a[i].length; j++)
					currentGameStateMatrix[x + j][y] = queue[ptr1].a[i].Symbol;
			}
			else
			{
				for (int j = 0; j < queue[ptr1].a[i].length; j++)
					currentGameStateMatrix[x][y + j] = queue[ptr1].a[i].Symbol;
			}
		}

		if (isWinPosition(queue[ptr1]))
		{
			// final
			fin = queue[ptr1];
			win = true;
			break;
		}

		tmpstate = queue[ptr1];

		// make a turn, add string of gamestate to map
		int off;
		for (int i = 0; i < queue[ptr1].Count; i++)
		{
			if (queue[ptr1].a[i].orientation)
			{ // make 2 turns - up/down
				//up
				off = 0;
				while (queue[ptr1].a[i].x - off >= 0 && currentGameStateMatrix[queue[ptr1].a[i].x - off - 1][queue[ptr1].a[i].y] == '.')
				{
					off++;
				}
				tmpstate.a[i].x -= off;
				if (used[GetStringByState(tmpstate)].second == false)
				{
					used[GetStringByState(tmpstate)] = make_pair(GetStringByState(queue[ptr1]), true);
					queue[ptr2++] = tmpstate;
				}
				tmpstate.a[i].x += off;
				//down
				off = 0;
				while (queue[ptr1].a[i].x + off + queue[ptr1].a[i].length - 1 < n && currentGameStateMatrix[queue[ptr1].a[i].x + off + queue[ptr1].a[i].length][queue[ptr1].a[i].y] == '.')
				{
					off++;
				}
				tmpstate.a[i].x += off;
				if (used[GetStringByState(tmpstate)].second == false)
				{
					used[GetStringByState(tmpstate)] = make_pair(GetStringByState(queue[ptr1]), true);
					queue[ptr2++] = tmpstate;
				}
				tmpstate.a[i].x -= off;
			}
			else
			{ // make 2 turns - left/right
				//left
				off = 0;
				while (queue[ptr1].a[i].y - off >= 0 && currentGameStateMatrix[queue[ptr1].a[i].x][queue[ptr1].a[i].y - off - 1] == '.')
				{
					off++;
				}
				tmpstate.a[i].y -= off;
				if (used[GetStringByState(tmpstate)].second == false)
				{
					used[GetStringByState(tmpstate)] = make_pair(GetStringByState(queue[ptr1]), true);
					queue[ptr2++] = tmpstate;
				}
				tmpstate.a[i].y += off;
				//right
				off = 0;
				while (queue[ptr1].a[i].y + off + queue[ptr1].a[i].length - 1 < n && currentGameStateMatrix[queue[ptr1].a[i].x][queue[ptr1].a[i].y + off + queue[ptr1].a[i].length] == '.')
				{
					off++;
				}
				tmpstate.a[i].y += off;
				if (used[GetStringByState(tmpstate)].second == false)
				{
					used[GetStringByState(tmpstate)] = make_pair(GetStringByState(queue[ptr1]), true);
					queue[ptr2++] = tmpstate;
				}
				tmpstate.a[i].y -= off;
			}
		}

		ptr1++;
	}
	//answer
	mytime = clock() - mytime;
	cout << "time: " << (double)mytime / CLOCKS_PER_SEC << "s" << endl;
	if (win)
	{
		string winning[1000];
		int ind = 0;
		string tmpfin = GetStringByState(fin);
		string tmpst = GetStringByState(v);
		for (string q = tmpfin; q != "-1"; q = used[q].first)
			winning[ind++] = q;
		for (int i = ind - 1; i >= 0; i--)
		{
			cout << "== " << ind - i - 1<< " ==" << endl;
			for (int j = 0; j < winning[i].length() / m; j++)
			{
				cout << winning[i].substr(j * n, m) << endl;
			}
		}
	}
	else
	{
		cout << "No chances";
	}
}

Stick FindStick(int i, int j)
{
	Stick result;
	int cnt = 0;

	result.x = i;
	result.y = j;
	result.Symbol = a[i][j];
	if (a[i][j] == '1') result.isRed = 1;
	else result.isRed = 0;
	if (j != m - 1 && a[i][j + 1] == a[i][j]) result.orientation = 0;
	else result.orientation = 1;
	if (result.orientation)
	{
		while (i + cnt < n && a[i + cnt][j] == a[i][j])
			cnt++;
		result.length = cnt;
	}
	else
	{
		while (j + cnt < m && a[i][j + cnt] == a[i][j])
			cnt++;
		result.length = cnt;
	}
	return result;
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> a[i][j];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (a[i][j] != '.')
			{
				if (firstState.used[a[i][j]] == 0)
				{
					firstState.used[a[i][j]] = 1;
					firstState.a[firstState.Count++] = FindStick(i, j);
				}
				if (a[i][j] == '1')
					firstState.RedStick = firstState.Count - 1;
			}
		}
	}
	bfs(firstState);

	return 0;
}