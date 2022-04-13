#include<iostream>
#include<cstdio>
#include<algorithm>
#include<unordered_map>
#include<queue> 
#include<cmath>
#include<unordered_set>
#include<cstring>
#include<map>
#include<windows.h> 

#define x first
#define y second

using namespace std;
const int N = 5010;
int copy_g[N][N];
int g[N][N];	//邻接矩阵，存边 
typedef pair<int,int> PII;
typedef pair<int,PII> PIII;	
typedef pair<int,PIII> PIIII;
typedef pair<int,char> PIC;
unordered_set<int> queue_yinshe; 
unordered_map<int,PIIII> del;	//删除树，存放出队后的点 
unordered_map<int,PII>	zuobiao;	//存点的坐标 
unordered_map<int,vector<PIII>> bianhua;	//变化 ，第一维是时间，第二维是起点，第二维是终点，第四维是更新后的距离 
priority_queue<PIIII,vector<PIIII>, greater<PIIII>> q;	//优先队列 (第一维实际距离加估价距离，第二维实际距离，第三维点编号，第四维前驱节点编号)
int dist[N];
int bianhua_juzhen[15][80]; 
int pre[N];
const int M = 150;
const int UB = 738;
map<PII , int> jiedao_yinshe;
int oushijuli[N][N];
double jingdu[N] , weidu[N];
 
// 角度转弧度
double rad(double d)
{
    const double PI = 3.1415926535898;
    return d * PI / 180.0;
}
 
// 传入两个经纬度，计算之间的大致直线距离
int CalcDistance(float fLati1, float fLong1, float fLati2, float fLong2)
{
    const float EARTH_RADIUS = 6378.137;
 
    double radLat1 = rad(fLati1);
    double radLat2 = rad(fLati2);
    double a = radLat1 - radLat2;
    double b = rad(fLong1) - rad(fLong2);
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = (int)(s * 10000000) / 10000;
    return s;
}

int cal(int st , int ed)
{
	return abs(oushijuli[st][ed]);
}
int main()
{
	int n ;
	scanf("%d",&n);
	for(int i = 1 ; i <= n ; i++)
	{
		int ID;
		scanf("%d",&ID);
		double tempx , tempy;
		scanf("%lf %lf",&tempx,&tempy);	
		jingdu[i] = tempx;
		weidu[i] = tempy;
	}
	for(int i = 1 ; i <= n ; i++)
	{
		for(int j = 1 ; j <= n ; j++)
		{
			oushijuli[i][j] = (int)CalcDistance(jingdu[i],weidu[i],jingdu[j],weidu[j]);
		}
	}
	//初始化变化矩阵
printf("---------------初始化变化矩阵---------------(输入0，0，0结束)\n");
	for(int i = 1 ; i <= 12 ; i++)
	{
		for(int j = 1 ; j <= 60 ; j++)
		{
			scanf("%d",&bianhua_juzhen[i][j]);
		}
	}
	//初始化街道
printf("---------------初始化街道---------------(输入0，0，0结束)\n"); 
	setbuf(stdin, NULL);//使stdin输入流由默认缓冲区转为无缓冲区
	while(1)
	{
		int a , b , c;
		scanf("%d %d %d",&a,&b,&c);
		jiedao_yinshe[{a,b}] = c;
		jiedao_yinshe[{b,a}] = c;		
		if(a+b+c==0)
		{
			break;
		}
	 } 
	//初始化边
	printf("---------------初始化边---------------(输入0，0，0结束)\n"); 
	for(int i = 1 ; i <= M ; i++)
	{
		for(int j = 1 ; j <= M ; j++)
		{
			g[i][j] = 50000;
		}
	}
	memset(dist,0x3f,sizeof dist);
	while(1)
	{
		int a , b;
		double dis;
		scanf("%d %d %lf",&a,&b,&dis);
		if(a+b+dis==0)
		{
			break;
		}
		g[a][b] = (int)dis;
		g[b][a] = (int)dis;
	}
	for(int i = 1 ; i <= N ; i++)
	{
		g[i][i] = 0;	
	}
	memcpy(copy_g,g,sizeof copy_g);
	//初始化变化
	printf("-----------初始化变换---------------(输入0，0，0，0结束)\n"); 
	while(1)
	{
		int t , a , b , dis;
		for(auto temp:jiedao_yinshe)
		{
			int a = temp.first.first;
			int b = temp.first.second;
			int id = temp.second;
			int pre = bianhua_juzhen[id][1];
			for(int j = 1 ; j <= 60; j++)
			{
				if(bianhua_juzhen[id][j]!=pre)
				{
					if(bianhua_juzhen[id][j]==0)
					bianhua[j].push_back({a,{b,50000}});
					else
					bianhua[j].push_back({a,{b,copy_g[a][b]}});			
				}
				pre = bianhua_juzhen[id][j];
			}	
		}
		break;
	}
	//初始化起点，终点
	printf("-----------初始化起点、终点---------\n");
	int st , ed;
	scanf("%d %d",&st,&ed);
	
	//开始计时
//	double time=0;  
//double counts=0;  
//LARGE_INTEGER nFreq;  
//LARGE_INTEGER nBeginTime;  
//LARGE_INTEGER nEndTime;  
//QueryPerformanceFrequency(&nFreq);  
//QueryPerformanceCounter(&nBeginTime);//开始计时  

	dist[st] = 0;
	q.push({dist[st]+cal(st,ed),{dist[st],{st,-1}}});
	queue_yinshe.insert(st);
	int t = 1;
	while(1)
	{
			t++;
			//查看是否有变化 
			if(bianhua.find(t)!=bianhua.end())
			{
				vector<PIII> temp_bianhua = bianhua[t];
				for(auto cur_node:temp_bianhua)
				{
				int update_st = cur_node.first;//起点 
				int update_ed = cur_node.second.first;//终点 
				int update_dis = cur_node.second.second;
				if(del.find(update_ed)==del.end()&&queue_yinshe.find(update_ed)==queue_yinshe.end())
				{
					//不在删除树和队列中 
					//continue;	
				}
				else if(queue_yinshe.find(update_ed)!=queue_yinshe.end())
				{
					//在队列中 					
					vector<PIIII> temp_v;
					while(q.top().second.second.first!=update_ed)
					{
						temp_v.push_back(q.top());
						q.pop();
					}
					auto temp_item = q.top();
					q.pop();
					if(update_dis == 50000)
					{
						if(temp_item.second.second.second==update_st)
						{
						temp_item.first = 50000;
						temp_item.second.first = 50000;
						}
					}
					else
					{
						if(temp_item.second.second.second==update_st)
						{
						temp_item.first = dist[update_st]+(update_dis) + cal(update_ed,ed);
						temp_item.second.first = dist[update_ed]+(update_dis);	
						}
						else
						{
							if(dist[update_st]+(update_dis) + cal(update_ed,ed)<temp_item.first)
							{
							temp_item.first = dist[update_st]+(update_dis) + cal(update_ed,ed);
							temp_item.second.first = dist[update_ed]+(update_dis);		
							temp_item.second.second.second = update_st;
							pre[temp_item.second.second.first] = update_st;
							}
						}
					} 
					if(temp_item.first>0)
					{
					q.push(temp_item);
					for(auto x:temp_v)
					{
						q.push(x);
					 }
					}
				}
				else
				{
					//在删除树中 										
					auto temp_item = del[update_ed];
					if(update_dis==50000)
					{
						continue;
					}
					if(update_dis != 50000)
					{
						if(dist[update_st]+(update_dis) + cal(update_ed,ed)<temp_item.first)
						{
						vector<int> temp_v;
						int cur_id = temp_item.second.second.first;
						for(auto x: del)
						{
							int cur_id = x.second.second.first;
							if(cur_id!=update_ed&&g[update_ed][cur_id]!=50000)
							{
								temp_v.push_back(cur_id);
							}
						}
						temp_item.first = dist[update_st]+(update_dis) + cal(update_ed,ed);
						if(temp_item.first>0)
						{						
						for(auto x:temp_v)
						{
							del.erase(x);
						}
						del.erase(update_ed);
						temp_item.first = dist[update_st]+(update_dis) + cal(update_ed,ed);
						temp_item.second.first = dist[update_st]+(update_dis);
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
			//	无变化，正常出队松弛。 
//				if(q.size()==0)
//				{
//					q.push({dist[st]+cal(st,ed),{dist[st],{st,-1}}});
//				}
				auto cur_node = q.top();
				q.pop();
				int cur_id = cur_node.second.second.first;
				queue_yinshe.erase(cur_id);		
			//	cout<<cur_id<<endl;	
//				剪枝 
//				int flag_val = cur_node.second.first;
//				if(flag_val>UB)
//				continue;					
				int cur_dist = 	cur_node.second.first;
				del[cur_id] = cur_node;				
				for(int i = 1 ; i <= M ; i++)
				{
					if(g[cur_id][i]!=50000&&i!=cur_id)
					{
				//		if(dist[i]>cur_dist+g[cur_id][i]&&i!=cur_id&&cur_dist+g[cur_id][i]<UB)
						if(dist[i]>cur_dist+g[cur_id][i]&&i!=cur_id)
						{
						if(i==63)
						{
						//	printf("当前节点 = %d     当前节点距离 = %d   边长 = %d 目标节点 = %d\n",cur_id , cur_dist , g[cur_id][i] , i);
							;
						}
							dist[i] = cur_dist+g[cur_id][i];
							pre[i] = cur_id;
							q.push({dist[i]+cal(i,ed),{dist[i],{i,cur_id}}});
							queue_yinshe.insert(i);
						}
					}
				}
				cout<<cur_dist<<"fesfegerhr"<<endl;
				cout<<cur_id<<"fdsfassd"<<endl;
				if(cur_id == ed&&dist[ed]!=50000)
				{
					break;
				}
			} 
	} 
	//停止计时
//	QueryPerformanceCounter(&nEndTime);//停止计时  
//time=(double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;//计算程序执行时间单位为s  
//cout<<"运行时间："<<time*1000<<"ms"<<endl;
	vector<int> temp_v;
	for(int i = 1 ; i <= 150 ; i++)
	{
		cout<<dist[i]<<endl;
	}
	for(int i = ed ; i!=st ; i=pre[i])
	{
		temp_v.push_back(i);
		cout<<i<<"<-";
	}
	temp_v.push_back(st);
	int temp_t = 0;
	double fenzi = 0;
	double fenmu = 0;
	reverse(temp_v.begin(),temp_v.end());
	for(int i = 0 ; i < temp_v.size() ; i++)
	{
		if(i!=0)
		{
			int temp_ID = jiedao_yinshe[{temp_v[i-1],temp_v[i]}];
			if(bianhua_juzhen[temp_t][temp_ID]>=1)
			{
				fenzi+=(double)g[temp_v[i-1]][temp_v[i]];	
			}
				fenmu+=(double)g[temp_v[i-1]][temp_v[i]];
		}
		temp_t++;
	}
	double baifenbi = fenzi/fenmu*100.0;
	printf("百分比 = %.2f\%\n",baifenbi);
	return 0;
} 
