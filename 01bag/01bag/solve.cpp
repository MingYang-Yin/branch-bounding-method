#include <iostream>
#include "queueDef.h"
using namespace std;
using namespace std;
struct goods {
	int weight;
	int value;
	float val_wei;
};
struct goods goods[100]; //建立一个储存100件物品的结构体数组
int SUM; //储存物品总数
int bagContain; //储存背包能装的重量
int totalWeight = 0; //储存背包中现有的物品重量
int totalValue = 0; //储存背包中现有的物品价值
int MaxValue = 0; //储存目前背包中所装物品的最大价值
bool flag = false; //记录用户输入是否为正整数（false表示不为，true表示为）
int sortedValwei[100][2];
int lowerBound = 0; //记录下界
int upBound = 0; //记录上界

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
		for (int j = 0; a[j] != 0; j++) { //将用户输入的数字字符转化为数
			result = result + (a[j] - 48) * pow(10, i - j - 1);
		}
		flag = true;
	}
	return result;
}


//获取用户输入
void getUserInput() {
	char tempL[20]; //暂时储存用户输入
	while (flag == false) {
		cout << "请输入背包能容纳的重量:";
		cin >> tempL;
		bagContain = examine(tempL);
	}
	flag = false;
	while (flag == false || SUM>=100) {
		cout << "请输入物品个数（小于100）:";
		cin >> tempL;
		SUM = examine(tempL);
		if (SUM >= 100) {
			cout << "物品个数应小于100!" << endl;
		}
	}
	flag = false;
	cout << "\n";
	for (int i = 0; i < SUM; i++) {
		while (flag == false) {
			cout << "请输入第" << i + 1 << "个物品的重量:";
			cin >> tempL;
			goods[i].weight = examine(tempL);
		}
		flag = false;
		while (flag == false) {
			cout << "请输入第" << i + 1 << "个物品的价值:";
			cin >> tempL;
			goods[i].value = examine(tempL);
		}
		flag = false;
		goods[i].val_wei = goods[i].value / goods[i].weight;
		cout << "\n";
	}
}

//将物品按照价值与重量的比值排序
void initSort() {
	for (int i = 0; i < SUM; i++) {
		sortedValwei[i][1] = goods[i].val_wei;
		sortedValwei[i][0] = i;
	}
	for (int i = 0; i < SUM - 1; i++) {
		for (int j = 0; j < SUM - i - 1; j++)
		{
			if (sortedValwei[j][1] < sortedValwei[j + 1][1]) //从大到小排序
			{
				int temp1, temp2;
				temp1 = sortedValwei[j][0];
				temp2 = sortedValwei[j][1];
				sortedValwei[j][1] = sortedValwei[j + 1][1];
				sortedValwei[j][0] = sortedValwei[j + 1][0];
				sortedValwei[j + 1][0] = temp1;
				sortedValwei[j + 1][1] = temp2;
			}
		}
	}
}

//寻找上下界
void findBound() {
	lowerBound = goods[sortedValwei[0][0]].value;
	upBound = bagContain * goods[sortedValwei[0][0]].val_wei;
}

//限界函数，返回ub
int pruning(int v, int i, int w) {
	return v + (bagContain - w)*(goods[i].val_wei);
}

//value指当前背包中物品的价值，weight指当前背包中的重量
//分支限界法 算法主体部分
operatewaitNode solve(waitLine *PT) {
	while (!PT->isEmpty()) {
		operatewaitNode WaitNode = PT->leaveNode(); //从PT表中取出结点
		if (WaitNode->sequence.size() == SUM) {
			return WaitNode;
		}
		int indexOrd = WaitNode->indexOrder; //指当前结点在排序中的位置
		int curIndex = sortedValwei[indexOrd][0];
		int nextIndex = sortedValwei[indexOrd + 1][0];
		int curValue = WaitNode->value;
		int curWeight = WaitNode->weight;

		int UB1 = 0;
		int UB2 = 0;

		if (indexOrd+1 >= SUM) {
			UB1 = curValue;
		}
		else
			UB1 = pruning(curValue + goods[curIndex].value, nextIndex, curWeight + goods[curIndex].weight);
		//查看左孩子是否符合要求
		if (curWeight + goods[curIndex].weight <= bagContain) { //符合容量要求
			if (UB1 >= lowerBound) { //符合下界要求
				operatewaitNode newWaitNode = new waitNode; //新建结点，设置参数
				newWaitNode->ub = UB1;
				newWaitNode->indexOrder = indexOrd + 1;
				newWaitNode->sequence = WaitNode->sequence;
				newWaitNode->sequence.push_back(1); //更新序列
				newWaitNode->weight = curWeight + goods[curIndex].weight;
				newWaitNode->value = curValue + goods[curIndex].value;
				newWaitNode->next = NULL;
				PT->addNode(newWaitNode); //将新结点放入PT表
			}
		}

		if (indexOrd + 1 >= SUM) { 
			UB2 = curValue;
		}
		else
			UB2 = pruning(curValue, nextIndex, curWeight);
		
		//查看右孩子是否符合要求
		if (UB2 >= lowerBound) { //符合下界要求
			operatewaitNode newWaitNode1 = new waitNode;  //新建结点，设置参数
			newWaitNode1->ub = UB2;
			newWaitNode1->indexOrder = indexOrd + 1;
			newWaitNode1->sequence = WaitNode->sequence;
			newWaitNode1->sequence.push_back(0); //更新序列
			newWaitNode1->weight = curWeight;
			newWaitNode1->value = curValue;
			newWaitNode1->next = NULL;
			PT->addNode(newWaitNode1); //将新结点放入PT表
		}
	}
}

//将起始节点（即根节点）放入PT表
void initQueue(waitLine *PT) {
	operatewaitNode firstNode1 = new waitNode;
	firstNode1->ub = upBound;
	firstNode1->indexOrder = 0;
	firstNode1->weight = 0;
	firstNode1->value = 0;
	firstNode1->next = NULL;
	PT->addNode(firstNode1);
}

int main() {
	cout << "********************获取用户输入********************\n\n";
	getUserInput(); //获取用户输入
	initSort(); //排序
	findBound(); 
	cout << "**********************输出结果**********************\n";
	waitLine *line = new waitLine;
	operatewaitNode MaxNode = new waitNode;
	initQueue(line); //将根节点放入PT表
	MaxNode = solve(line);
	cout << "装入物品组合的最大价值为：" << MaxNode->value << "；\n";
	cout << "达到该价值的组合为：";
	for (int ele : MaxNode->sequence) { //打印结果序列
		cout << ele<< " ";
	}
	cout << "\n";
	system("pause");
	return 0;
}