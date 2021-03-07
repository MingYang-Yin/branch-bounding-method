#include <iostream>
#include <vector>
#include <math.h>
#include "queueDef.h"
#define NodeNum 4
using namespace std;
int cost = 10000; //�������
int graphCost[100][100]; //������۾���
int graphCostSort[100][2]; //����ÿ�������С������Cost
bool visit[100] = { 0 }; //��¼����Ƿ񱻷��ʹ�
int SUM = 0; //���������
bool flag = true; //��¼�û������Ƿ�Ϊ������
bool dfs_visited[100];
int lowerBound = 0; //��¼�½�
int upBound = 0; //��¼�Ͻ�
int check[100];

//����û��������Ƿ�Ϊ����
int examine(char a[]) {
	flag = true;
	int i = 0;
	int result = 0;
	for (; a[i] != 0; i++) {
		if (a[i] < 48 || a[i]>57) {
			cout << "������һ����������" << endl;
			flag = false;
			break;
		}
	}
	if (flag) {
		for (int j = 0; a[j] != 0; j++) {//���û�����������ַ�ת��Ϊ��
			result = result + (a[j] - 48) * pow(10, i - j - 1);
		}
	}
	return result;
}

//��ʼ����������
void initCostSort() {
	for (int i = 0; i < SUM; i++) {
		graphCostSort[i][0] = 10000;
		graphCostSort[i][1] = 10000;
	}
}

//��ȡ�û�����
void getUserInput() {
	//�û�������۾���
	char tempL[20]; //��ʱ�����û�������
	while(1) {
		cout << "�������������С��100������2����";
		cin >> tempL;
		SUM = examine(tempL);
		if (!flag || SUM >= 100 || SUM <= 2) {
			if(SUM >= 100)
				cout << "�ڵ�����С��100������������" << "\n";
			if (SUM <= 2 && flag)
				cout << "�ڵ��������2������������" << "\n";
		}
		else
			break;
	}
	initCostSort();
	cout << "��������۾���\n";
	for (int i = 0; i < SUM; i++) {
		for (int j = 0; j < SUM; j++) {
			if (j == i) {
				graphCost[i][j] = 9999;
			}
			else {
				cout << "��������" << i + 1 << "����" << j + 1 << "֮��Ĵ��ۣ�";
				cin >> tempL;
				graphCost[i][j] = examine(tempL); //�����û�����
				if (!flag) {
					j--;
				}
				else { //��ÿ����㣬�ҵ��û��������С����������
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

//��ʼ��check����
void initCheck() {
	for (int i = 0; i < SUM; i++) {
		check[i] = 0;
	}
}

//������e��lbֵ
int getlb(vector<int> temp1) {
	int record[100][2]; //��¼�Ѿ�ȷ����·��
	vector<int> temp = temp1;
	double sum = 0;
	initCheck();
	for (int i = 0; i < SUM; i++) {	//��ʼ��record����
		record[i][0] = 0;
		record[i][1] = 0;
	}
	for (int i = 0; i < temp.size()-1; i++) { //��ÿ����ȷ����·������record�����¼���ǣ������
		sum += graphCost[temp[i]][temp[i+1]];
		sum += graphCost[temp[i+1]][temp[i]];
		record[temp[i]][1] = graphCost[temp[i]][temp[i + 1]];
		record[temp[i + 1]][0] = graphCost[temp[i]][temp[i + 1]];
		check[temp[i]]++;
		check[temp[i + 1]]++;
	}
	for (int j = 0; j < SUM; j++) { //�������н��
		if (check[j] == 0) { //���ý��û��ȷ���ıߣ�������С�������������
			sum += graphCostSort[j][0];
			sum += graphCostSort[j][1];
		}
		else if (check[j] == 1) { //���ý����һ��ȷ���ıߣ�����˱�֮�⣬����С�Ĵ������
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

//����̰���㷨,�����Ͻ�
int dfs(int u, int k, int l) {
	int minlen = 999;
	int p;
	if (k == SUM)
	{
		//������һ���ڵ�Ϳ�ʼ�ڵ�ľ���
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
	//��ǵ�ǰ������С�ڵ�Ϊ���ʹ�
	dfs_visited[p] = true;
	return dfs(p, k + 1, l + minlen);
}

//�����½�
void getLowerBound() {
	int sum = 0;
	for (int i = 0; i < SUM; i++) {
		sum += graphCostSort[i][0];
		sum += graphCostSort[i][1];
	}
	lowerBound = sum / 2;
}

//�����Ͻ�
void getUpBound() {
	dfs_visited[1] = true;
	upBound = dfs(1, 1, 0);
}

//�㷨���岿��
operatewaitNode solve(waitLine *PT) { 
	while (!PT->isEmpty()) {
		operatewaitNode WaitNode = PT->leaveNode(); //��PT����ȡ����������չ
		if (WaitNode->sequence.size() == SUM) { //���ý��ΪҶ�ӽ�㣬���㷨�������
			return WaitNode;
		}
		int LB = 0;
		vector<int> tempSeq = WaitNode->sequence;
		for (int k = 0; k < SUM; k++) { //����WaitNode�����к��ӽ��
			int nCount = count(tempSeq.begin(), tempSeq.end(), k);
			if (nCount > 0) {
				continue;
			}
			else {
				tempSeq.push_back(k);
				LB = getlb(tempSeq);
				if (LB >= lowerBound && LB <= upBound) { //LB���Ϲ涨�����ý�����PT��
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

//��ʼ��PT�������������
void initPTTable(waitLine *PT, int i) {
	operatewaitNode newWaitNode1 = new waitNode;
	newWaitNode1->lb = lowerBound;
	newWaitNode1->next = NULL;
	newWaitNode1->sequence.push_back(i);
	PT->addNode(newWaitNode1);
}

int main() {
	cout << "********************��ȡ�û�����********************\n\n";
	getUserInput();
	cout << "**********************������**********************\n";
	cout << "ע���Խ��1��Ϊ��ʼ���\n\n";
	getLowerBound();
	getUpBound();
	waitLine *ptTable = new waitLine;
	operatewaitNode MinNode = new waitNode;
	initPTTable(ptTable, 0);
	MinNode = solve(ptTable);
	cout << "TSP�������С���ۣ�" << MinNode->lb << endl;
	cout << "����һ���߹���·��Ϊ��";
	for (int ele : MinNode->sequence) {
		cout << ele+1 << " ";
	}
	cout << "1" << endl;
	system("pause");
}