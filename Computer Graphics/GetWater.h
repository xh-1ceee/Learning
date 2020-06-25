#include<iostream>
#include<queue>
#include<map>
using namespace std;

struct Node {
	int bucketA;
	int bucketB;
	int step;
};
map<pair<int, int>, pair<int, int> >path;
bool vis[205][205];

void solve()
{
	int volumeA, volumeB;
	int target;
	cout << "Please enter the volume of bucket A and bucket B:" << endl;
	cin >> volumeA >> volumeB;
	cout << "Please enter the target volume you want to get:" << endl;
	cin >> target;
	queue<Node>q;
	Node first;
	first.bucketA = 0;
	first.bucketB = 0;
	first.step = 0;
	q.push(first);
	vis[first.bucketA][first.bucketB] = 1;
	path[make_pair(-1, -1)] = make_pair(0, 0);
	while (!q.empty())
	{
		Node tmp = q.front();
		q.pop();
		if (tmp.bucketA == target || tmp.bucketB == target)
		{
			cout << "The total step is:" << tmp.step << endl;
			cout << tmp.bucketA << "," << tmp.bucketB << "<-";
			pair<int, int> n(tmp.bucketA, tmp.bucketB);
			while (n.first != 0 && n.second != 0)
			{
				for (std::map<pair<int, int>, pair<int, int> >::iterator it = path.begin(); it != path.end(); it++)
				{
					if (it->second == n)
					{
						cout << it->first.first << "," << it->first.second << "<-";
						n = it->first;
					}

				}
			}
			cout << 0 << "," << 0 << endl;
			break;
		}
		Node ne;
		for (int i = 1; i <= 6; i++)
		{
			if (i == 1)
			{
				//給雛A
				ne.bucketA = volumeA;
				ne.bucketB = tmp.bucketB;
				ne.step = tmp.step + 1;
			}
			else if (i == 2)
			{
				//給雛B
				ne.bucketA = tmp.bucketA;
				ne.bucketB = volumeB;
				ne.step = tmp.step + 1;
			}
			else if (i == 3 && tmp.bucketA > 0)
			{
				//給諾A
				ne.bucketA = 0;
				ne.bucketB = tmp.bucketB;
				ne.step = tmp.step + 1;
			}
			else if (i == 4 && tmp.bucketB > 0)
			{
				//給諾B
				ne.bucketB = 0;
				ne.bucketA = tmp.bucketA;
				ne.step = tmp.step + 1;
			}
			else if (i == 5)
			{
				//參A給輛B
				ne.bucketB = (tmp.bucketB + tmp.bucketA > volumeB) ? volumeB : tmp.bucketB + tmp.bucketA;
				ne.bucketA = (volumeB - tmp.bucketB > tmp.bucketA) ? 0 : tmp.bucketA - (volumeB - tmp.bucketB);
				ne.step = tmp.step + 1;
			}
			else if (i == 6)
			{
				//參B給輛A
				ne.bucketA = (tmp.bucketB + tmp.bucketA > volumeA) ? volumeA : tmp.bucketA + tmp.bucketB;
				ne.bucketB = (volumeA - tmp.bucketA > tmp.bucketB) ? 0 : tmp.bucketB - (volumeA - tmp.bucketA);
				ne.step = tmp.step + 1;
			}
			if (vis[ne.bucketA][ne.bucketB] == 0)
			{
				q.push(ne);
				path[make_pair(tmp.bucketA, tmp.bucketB)] = make_pair(ne.bucketA, ne.bucketB);
				//if(tmp.bucketA == 0 && tmp.bucketB == 0)	printf("qaq: %d %d\n",ne.bucketA,ne.bucketB);
				vis[ne.bucketA][ne.bucketB] = 1;
			}

		}
	}


}
