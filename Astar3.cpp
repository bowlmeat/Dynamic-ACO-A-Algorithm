#include <iostream>
#include <cstdio>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <cstring>
#include <map>
#include <windows.h>

#define x first
#define y second

using namespace std;
const int N = 5010;
int copy_g[N][N];
int g[N][N]; // adjacency matrix, store the edge information
typedef pair<int, int> PII;
typedef pair<int, PII> PIII;
typedef pair<int, PIII> PIIII;
typedef pair<int, char> PIC;
unordered_set<int> queue_mapping;
unordered_map<int, PIIII> del;			 // SPT, store vertices poped from the queue
unordered_map<int, PII> coordinates;	 // vertices coordinates
unordered_map<int, vector<PIII>> change; // changes
/* the first dimension is time, the second is starting vertex,
the third is end vertex, the fourth is the updated distance.*/
priority_queue<PIIII, vector<PIIII>, greater<PIIII>> q; // priority queue
/*the first dimension is the estimated distance, the second is the real distance from source to current,
the third is the vertex ID, and the fourth is the predecessor ID.*/

int dist[N];
int change_matrix[15][80];
int pre[N];
const int M = 150;
const int UB = 738;
map<PII, int> streets_mapping;
int Euclidean[N][N];
double longitude[N], latitude[N];

// Angles to radians
double rad(double d)
{
	const double PI = 3.1415926535898;
	return d * PI / 180.0;
}

// Euclidean Distance: Calculate the straight-line distance between two vertices.
int CalcDistance(float fLati1, float fLong1, float fLati2, float fLong2)
{
	const float EARTH_RADIUS = 6378.137;

	double radLat1 = rad(fLati1);
	double radLat2 = rad(fLati2);
	double a = radLat1 - radLat2;
	double b = rad(fLong1) - rad(fLong2);
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
	s = s * EARTH_RADIUS;
	s = (int)(s * 10000000) / 10000;
	return s;
}
// Absolute value of Euclidean distance
int cal(int st, int ed)
{
	return abs(Euclidean[st][ed]);
}

// main function
int main()
{
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
	{
		int ID;
		scanf("%d", &ID);
		double tempx, tempy;
		scanf("%lf %lf", &tempx, &tempy);
		longitude[i] = tempx;
		latitude[i] = tempy;
	}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			Euclidean[i][j] = (int)CalcDistance(longitude[i], latitude[i], longitude[j], latitude[j]);
		}
	}
	// Initialize the state table
	cout << "---------------initialize the state table---------------(Input(0,0,0)to end)\n";
	for (int i = 1; i <= 12; i++)
	{
		for (int j = 1; j <= 60; j++)
		{
			scanf("%d", &change_matrix[i][j]);
		}
	}
	// Initialize the street graph
	cout << "---------------initialize the street graph---------------(Input(0,0,0)to end)\n";
	setbuf(stdin, NULL); // Make stdin input stream to unbuffered
	while (true)
	{
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);
		streets_mapping[{a, b}] = c;
		streets_mapping[{b, a}] = c;
		if (a + b + c == 0)
		{
			break;
		}
	}
	// Initialize the edge information
	cout << "---------------initialize the edge information---------------(Input(0,0,0)to end)\n";
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			g[i][j] = 50000;
		}
	}
	memset(dist, 0x3f, sizeof dist);
	while (true)
	{
		int a, b;
		double dis;
		scanf("%d %d %lf", &a, &b, &dis);
		if (a + b + dis == 0)
		{
			break;
		}
		g[a][b] = (int)dis;
		g[b][a] = (int)dis;
	}
	for (int i = 1; i <= N; i++)
	{
		g[i][i] = 0;
	}
	memcpy(copy_g, g, sizeof copy_g);

	// Initialize the change information
	cout << "-----------initialize the change information---------------(Input(0,0,0)to end)\n";
	while (true)
	{
		int t, a, b, dis;
		for (auto temp : streets_mapping)
		{
			int a = temp.first.first;
			int b = temp.first.second;
			int id = temp.second;
			int pre = change_matrix[id][1];
			for (int j = 1; j <= 60; j++)
			{
				if (change_matrix[id][j] != pre)
				{
					if (change_matrix[id][j] == 0)
						change[j].push_back({a, {b, 50000}});
					else
						change[j].push_back({a, {b, copy_g[a][b]}});
				}
				pre = change_matrix[id][j];
			}
		}
		break;
	}
	// Initialize the source vertex and the destination vertex
	cout << "-----------initialize the source vertex and the destination vertex---------\n";
	int st, ed;
	scanf("%d %d", &st, &ed);

	dist[st] = 0;
	q.push({dist[st] + cal(st, ed), {dist[st], {st, -1}}});
	queue_mapping.insert(st);
	int t = 1;
	while (true)
	{
		t++;
		// check for changes
		if (change.find(t) != change.end())
		{
			vector<PIII> temp_change = change[t];
			for (auto cur_node : temp_change)
			{
				int update_st = cur_node.first;		   // source vertex
				int update_ed = cur_node.second.first; // destination vertex
				int update_dis = cur_node.second.second;
				if (del.find(update_ed) == del.end() && queue_mapping.find(update_ed) == queue_mapping.end())
				{
					// not in queue or SPT
					//continue;
				}
				else if (queue_mapping.find(update_ed) != queue_mapping.end())
				{
					//in queque
					vector<PIIII> temp_v;
					while (q.top().second.second.first != update_ed)
					{
						temp_v.push_back(q.top());
						q.pop();
					}
					auto temp_item = q.top();
					q.pop();
					if (update_dis == 50000)
					{
						if (temp_item.second.second.second == update_st)
						{
							temp_item.first = 50000;
							temp_item.second.first = 50000;
						}
					}
					else
					{
						if (temp_item.second.second.second == update_st)
						{
							temp_item.first = dist[update_st] + (update_dis) + cal(update_ed, ed);
							temp_item.second.first = dist[update_ed] + (update_dis);
						}
						else
						{
							if (dist[update_st] + (update_dis) + cal(update_ed, ed) < temp_item.first)
							{
								temp_item.first = dist[update_st] + (update_dis) + cal(update_ed, ed);
								temp_item.second.first = dist[update_ed] + (update_dis);
								temp_item.second.second.second = update_st;
								pre[temp_item.second.second.first] = update_st;
							}
						}
					}
					if (temp_item.first > 0)
					{
						q.push(temp_item);
						for (auto x : temp_v)
						{
							q.push(x);
						}
					}
				}
				else
				{
					//in SPT
					auto temp_item = del[update_ed];
					if (update_dis == 50000)
					{
						continue;
					}
					if (update_dis != 50000)
					{
						if (dist[update_st] + (update_dis) + cal(update_ed, ed) < temp_item.first)
						{
							vector<int> temp_v;
							int cur_id = temp_item.second.second.first;
							for (auto x : del)
							{
								int cur_id = x.second.second.first;
								if (cur_id != update_ed && g[update_ed][cur_id] != 50000)
								{
									temp_v.push_back(cur_id);
								}
							}
							temp_item.first = dist[update_st] + (update_dis) + cal(update_ed, ed);
							if (temp_item.first > 0)
							{
								for (auto x : temp_v)
								{
									del.erase(x);
								}
								del.erase(update_ed);
								temp_item.first = dist[update_st] + (update_dis) + cal(update_ed, ed);
								temp_item.second.first = dist[update_st] + (update_dis);
								temp_item.second.second.second = update_st;
								q.push(temp_item);
							}
						}
					}
				}
				g[update_st][update_ed] = g[update_ed][update_st] = update_dis;
			}
		}
		else
		{
			auto cur_node = q.top();
			q.pop();
			int cur_id = cur_node.second.second.first;
			queue_mapping.erase(cur_id);

			int cur_dist = cur_node.second.first;
			del[cur_id] = cur_node;
			for (int i = 1; i <= M; i++)
			{
				if (g[cur_id][i] != 50000 && i != cur_id)
				{
					if (dist[i] > cur_dist + g[cur_id][i] && i != cur_id)
					{
						if (i == 63)
						{
							//	cout << "current vertex ID = %d; the dist value  = %d;  the edge length = %d; the target vertex = %d\n",cur_id , cur_dist , g[cur_id][i] , i;
							;
						}
						dist[i] = cur_dist + g[cur_id][i];
						pre[i] = cur_id;
						q.push({dist[i] + cal(i, ed), {dist[i], {i, cur_id}}});
						queue_mapping.insert(i);
					}
				}
			}
			cout << cur_dist << "fesfegerhr" << endl;
			cout << cur_id << "fdsfassd" << endl;
			if (cur_id == ed && dist[ed] != 50000)
			{
				break;
			}
		}
	}

	vector<int> temp_v;
	for (int i = 1; i <= 150; i++)
	{
		cout << dist[i] << endl;
	}
	for (int i = ed; i != st; i = pre[i])
	{
		temp_v.push_back(i);
		cout << i << "<-";
	}
	temp_v.push_back(st);
	int temp_t = 0;
	double item = 0;
	double sum = 0;
	reverse(temp_v.begin(), temp_v.end());
	for (int i = 0; i < temp_v.size(); i++)
	{
		if (i != 0)
		{
			int temp_ID = streets_mapping[{temp_v[i - 1], temp_v[i]}];
			if (change_matrix[temp_t][temp_ID] >= 1)
			{
				item += (double)g[temp_v[i - 1]][temp_v[i]];
			}
			sum += (double)g[temp_v[i - 1]][temp_v[i]];
		}
		temp_t++;
	}
	double per = item / sum * 100.0;
	printf("percentage = %.2f\%\n", per);
	return 0;
}
