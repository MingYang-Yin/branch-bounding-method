#include <iostream>
#include <vector>
#include <math.h>
#include "queueDef.h"
#define NodeNum 4
using namespace std;
int cost = 10000; //储存代价
int graphCost[100][100]; //储存代价矩阵
int graphCostSort[100][2]; //储存每个结点最小的两个Cost
bool visit[100] = { 0 }; //记录结点是否被访问过
int SUM = 0; //储存结点个数
bool flag = true; //记录用户输入是否为正整数
bool dfs_visited[100];
int lowerBound = 0; //记录下界
int upBound = 0; //记录上界
int check[100];

//检测用户的输入是否为数字
int examine(char a[]) {
	flag = true;
	int i = 0;
	int result = 0;
	for (; a[i] != 0; i++) {
		if (a[i] < 48 || a[i]>57) {
			cout << "请输入一个正整数！" << endl;
			flag = false;
			break;
		}
	}
	if (flag) {
		for (int j = 0; a[j] != 0; j++) {//将用户输入的数字字符转化为数
			result = result + (a[j] - 48) * pow(10, i - j - 1);
		}
	}
	return result;
}

//初始化排序数组
void initCostSort() {
	for (int i = 0; i < SUM; i++) {
		graphCostSort[i][0] = 10000;
		graphCostSort[i][1] = 10000;
	}
}

//获取用户输入
void getUserInput() {
	//用户输入代价矩阵
	char tempL[20]; //暂时储存用户的输入
	while(1) {
		cout << "请输入结点个数（小于100，大于2）：";
		cin >> tempL;
		SUM = examine(tempL);
		if (!flag || SUM >= 100 || SUM <= 2) {
			if(SUM >= 100)
				cout << "节点数需小于100，请重新输入" << "\n";
			if (SUM <= 2 && flag)
				cout << "节点数需大于2，请重新输入" << "\n";
		}
		else
			break;
	}
	initCostSort();
	cout << "请输入代价矩阵：\n";
	for (int i = 0; i < SUM; i++) {
		for (int j = 0; j < SUM; j++) {
			if (j == i) {
				graphCost[i][j] = 9999;
			}
			else {
				cout << "请输入结点" << i + 1 << "与结点" << j + 1 << "之间的代价：";
				cin >> tempL;
				graphCost[i][j] = examine(tempL); //检验用户输入
				if (!flag) {
					j--;
				}
				else { //对每个结点，找到用户输入的最小的两个代价
					int tempCost = 0;
					if (graphCost[i][j] < graphCostSort[i][0]) {
						tempCost = graphCostSort[i][0];
						graphCostSort[i][0] = graphCost[i][j];
						if (tempCost < graphCostSort[i][1]) {
							graphCostSort[i][1] = tempCost;
						}
					}
					else if (graphCost[i][j] < graphCostSort[i][1]) {
						graphCostSort[i][1] = graphCost[i][j];
					}
				}
			}
		}
		cout << endl;
	}
	cout << "\n-----------------------------------------------------\n";
}

//初始化check数组
void initCheck() {
	for (int i = 0; i < SUM; i++) {
		check[i] = 0;
	}
}

//计算结点e的lb值
int getlb(vector<int> temp1) {
	int record[100][2]; //记录已经确定的路径
	vector<int> temp = temp1;
	double sum = 0;
	initCheck();
	for (int i = 0; i < SUM; i++) {	//初始化record数组
		record[i][0] = 0;
		record[i][1] = 0;
	}
	for (int i = 0; i < temp.size()-1; i++) { //对每个已确定的路径，在record数组记录它们，并求和
		sum += graphCost[temp[i]][temp[i+1]];
		sum += graphCost[temp[i+1]][temp[i]];
		record[temp[i]][1] = graphCost[temp[i]][temp[i + 1]];
		record[temp[i + 1]][0] = graphCost[temp[i]][temp[i + 1]];
		check[temp[i]]++;
		check[temp[i + 1]]++;
	}
	for (int j = 0; j < SUM; j++) { //遍历所有结点
		if (check[j] == 0) { //若该结点没有确定的边，则用最小的两个代价求和
			sum += graphCostSort[j][0];
			sum += graphCostSort[j][1];
		}
		else if (check[j] == 1) { //若该结点有一个确定的边，则除此边之外，找最小的代价求和
			int find = record[j][0] + record[j][1];
			if (find == graphCostSort[j][0] || find == graphCostSort[j][1]) {
				if (find == graphCostSort[j][0])
					sum += graphCostSort[j][1];
				if (find == graphCostSort[j][1])
					sum += graphCostSort[j][0];
			}
			else
				sum += graphCostSort[j][0];
		}
	}
	double ret = sum / 2;
	return ceil(ret);
}

//定义贪心算法,计算上界
int dfs(int u, int k, int l) {
	int minlen = 999;
	int p;
	if (k == SUM)
	{
		//最后最后一个节点和开始节点的距离
		return l + graphCost[u-1][1-1];
	}
	for (int i = 1; i <= SUM; i++)
	{
		if (!dfs_visited[i] && graphCost[u-1][i-1] < minlen)
		{
			minlen = graphCost[u-1][i-1];
			p = i;
		}
	}
	//标记当前行中最小节点为访问过
	dfs_visited[p] = true;
	return dfs(p, k + 1, l + minlen);
}

//计算下界
void getLowerBound() {
	int sum = 0;
	for (int i = 0; i < SUM; i++) {
		sum += graphCostSort[i][0];
		sum += graphCostSort[i][1];
	}
	lowerBound = sum / 2;
}

//计算上界
void getUpBound() {
	dfs_visited[1] = true;
	upBound = dfs(1, 1, 0);
}

//算法主体部分
operatewaitNode solve(waitLine *PT) { 
	while (!PT->isEmpty()) {
		operatewaitNode WaitNode = PT->leaveNode(); //从PT表中取出结点进行扩展
		if (WaitNode->sequence.size() == SUM) { //若该结点为叶子结点，则算法运行完毕
			return WaitNode;
		}
		int LB = 0;
		vector<int> tempSeq = WaitNode->sequence;
		for (int k = 0; k < SUM; k++) { //遍历WaitNode的所有孩子结点
			int nCount = count(tempSeq.begin(), tempSeq.end(), k);
			if (nCount > 0) {
				continue;
			}
			else {
				tempSeq.push_back(k);
				LB = getlb(tempSeq);
				if (LB >= lowerBound && LB <= upBound) { //LB符合规定，将该结点放入PT表
					operatewaitNode newWaitNode1 = new waitNode;
					newWaitNode1->lb = LB;
					newWaitNode1->sequence = tempSeq;
					newWaitNode1->next = NULL;
					PT->addNode(newWaitNode1);
					tempSeq.erase(tempSeq.end()-1);
				}
				else
					tempSeq.erase(tempSeq.end() - 1);
			}
		}
	}
}

//初始化PT表，放入出发城市
void initPTTable(waitLine *PT, int i) {
	operatewaitNode newWaitNode1 = new waitNode;
	newWaitNode1->lb = lowerBound;
	newWaitNode1->next = NULL;
	newWaitNode1->sequence.push_back(i);
	PT->addNode(newWaitNode1);
}

int main() {
	cout << "********************获取用户输入********************\n\n";
	getUserInput();
	cout << "**********************输出结果**********************\n";
	cout << "注：以结点1作为起始结点\n\n";
	getLowerBound();
	getUpBound();
	waitLine *ptTable = new waitLine;
	operatewaitNode MinNode = new waitNode;
	initPTTable(ptTable, 0);
	MinNode = solve(ptTable);
	cout << "TSP问题的最小代价：" << MinNode->lb << endl;
	cout << "其中一条走过的路径为：";
	for (int ele : MinNode->sequence) {
		cout << ele+1 << " ";
	}
	cout << "1" << endl;
	system("pause");
}