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
int g[N][N];	//�ڽӾ��󣬴�� 
typedef pair<int,int> PII;
typedef pair<int,PII> PIII;	
typedef pair<int,PIII> PIIII;
typedef pair<int,char> PIC;
unordered_set<int> queue_yinshe; 
unordered_map<int,PIIII> del;	//ɾ��������ų��Ӻ�ĵ� 
unordered_map<int,PII>	zuobiao;	//�������� 
unordered_map<int,vector<PIII>> bianhua;	//�仯 ����һά��ʱ�䣬�ڶ�ά����㣬�ڶ�ά���յ㣬����ά�Ǹ��º�ľ��� 
priority_queue<PIIII,vector<PIIII>, greater<PIIII>> q;	//���ȶ��� (��һάʵ�ʾ���ӹ��۾��룬�ڶ�άʵ�ʾ��룬����ά���ţ�����άǰ���ڵ���)
int dist[N];
int bianhua_juzhen[15][80]; 
int pre[N];
const int M = 150;
const int UB = 738;
map<PII , int> jiedao_yinshe;
int oushijuli[N][N];
double jingdu[N] , weidu[N];
 
// �Ƕ�ת����
double rad(double d)
{
    const double PI = 3.1415926535898;
    return d * PI / 180.0;
}
 
// ����������γ�ȣ�����֮��Ĵ���ֱ�߾���
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
	//��ʼ���仯����
printf("---------------��ʼ���仯����---------------(����0��0��0����)\n");
	for(int i = 1 ; i <= 12 ; i++)
	{
		for(int j = 1 ; j <= 60 ; j++)
		{
			scanf("%d",&bianhua_juzhen[i][j]);
		}
	}
	//��ʼ���ֵ�
printf("---------------��ʼ���ֵ�---------------(����0��0��0����)\n"); 
	setbuf(stdin, NULL);//ʹstdin��������Ĭ�ϻ�����תΪ�޻�����
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
	//��ʼ����
	printf("---------------��ʼ����---------------(����0��0��0����)\n"); 
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
	//��ʼ���仯
	printf("-----------��ʼ���任---------------(����0��0��0��0����)\n"); 
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
	//��ʼ����㣬�յ�
	printf("-----------��ʼ����㡢�յ�---------\n");
	int st , ed;
	scanf("%d %d",&st,&ed);
	
	//��ʼ��ʱ
//	double time=0;  
//double counts=0;  
//LARGE_INTEGER nFreq;  
//LARGE_INTEGER nBeginTime;  
//LARGE_INTEGER nEndTime;  
//QueryPerformanceFrequency(&nFreq);  
//QueryPerformanceCounter(&nBeginTime);//��ʼ��ʱ  

	dist[st] = 0;
	q.push({dist[st]+cal(st,ed),{dist[st],{st,-1}}});
	queue_yinshe.insert(st);
	int t = 1;
	while(1)
	{
			t++;
			//�鿴�Ƿ��б仯 
			if(bianhua.find(t)!=bianhua.end())
			{
				vector<PIII> temp_bianhua = bianhua[t];
				for(auto cur_node:temp_bianhua)
				{
				int update_st = cur_node.first;//��� 
				int update_ed = cur_node.second.first;//�յ� 
				int update_dis = cur_node.second.second;
				if(del.find(update_ed)==del.end()&&queue_yinshe.find(update_ed)==queue_yinshe.end())
				{
					//����ɾ�����Ͷ����� 
					//continue;	
				}
				else if(queue_yinshe.find(update_ed)!=queue_yinshe.end())
				{
					//�ڶ����� 					
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
					//��ɾ������ 										
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
			//	�ޱ仯�����������ɳڡ� 
//				if(q.size()==0)
//				{
//					q.push({dist[st]+cal(st,ed),{dist[st],{st,-1}}});
//				}
				auto cur_node = q.top();
				q.pop();
				int cur_id = cur_node.second.second.first;
				queue_yinshe.erase(cur_id);		
			//	cout<<cur_id<<endl;	
//				��֦ 
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
						//	printf("��ǰ�ڵ� = %d     ��ǰ�ڵ���� = %d   �߳� = %d Ŀ��ڵ� = %d\n",cur_id , cur_dist , g[cur_id][i] , i);
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
	//ֹͣ��ʱ
//	QueryPerformanceCounter(&nEndTime);//ֹͣ��ʱ  
//time=(double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;//�������ִ��ʱ�䵥λΪs  
//cout<<"����ʱ�䣺"<<time*1000<<"ms"<<endl;
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
	printf("�ٷֱ� = %.2f\%\n",baifenbi);
	return 0;
} 
