#include <iostream>
#include "queueDef.h"
using namespace std;
using namespace std;
struct goods {
	int weight;
	int value;
	float val_wei;
};
struct goods goods[100]; //����һ������100����Ʒ�Ľṹ������
int SUM; //������Ʒ����
int bagContain; //���汳����װ������
int totalWeight = 0; //���汳�������е���Ʒ����
int totalValue = 0; //���汳�������е���Ʒ��ֵ
int MaxValue = 0; //����Ŀǰ��������װ��Ʒ������ֵ
bool flag = false; //��¼�û������Ƿ�Ϊ��������false��ʾ��Ϊ��true��ʾΪ��
int sortedValwei[100][2];
int lowerBound = 0; //��¼�½�
int upBound = 0; //��¼�Ͻ�

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
		for (int j = 0; a[j] != 0; j++) { //���û�����������ַ�ת��Ϊ��
			result = result + (a[j] - 48) * pow(10, i - j - 1);
		}
		flag = true;
	}
	return result;
}


//��ȡ�û�����
void getUserInput() {
	char tempL[20]; //��ʱ�����û�����
	while (flag == false) {
		cout << "�����뱳�������ɵ�����:";
		cin >> tempL;
		bagContain = examine(tempL);
	}
	flag = false;
	while (flag == false || SUM>=100) {
		cout << "��������Ʒ������С��100��:";
		cin >> tempL;
		SUM = examine(tempL);
		if (SUM >= 100) {
			cout << "��Ʒ����ӦС��100!" << endl;
		}
	}
	flag = false;
	cout << "\n";
	for (int i = 0; i < SUM; i++) {
		while (flag == false) {
			cout << "�������" << i + 1 << "����Ʒ������:";
			cin >> tempL;
			goods[i].weight = examine(tempL);
		}
		flag = false;
		while (flag == false) {
			cout << "�������" << i + 1 << "����Ʒ�ļ�ֵ:";
			cin >> tempL;
			goods[i].value = examine(tempL);
		}
		flag = false;
		goods[i].val_wei = goods[i].value / goods[i].weight;
		cout << "\n";
	}
}

//����Ʒ���ռ�ֵ�������ı�ֵ����
void initSort() {
	for (int i = 0; i < SUM; i++) {
		sortedValwei[i][1] = goods[i].val_wei;
		sortedValwei[i][0] = i;
	}
	for (int i = 0; i < SUM - 1; i++) {
		for (int j = 0; j < SUM - i - 1; j++)
		{
			if (sortedValwei[j][1] < sortedValwei[j + 1][1]) //�Ӵ�С����
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

//Ѱ�����½�
void findBound() {
	lowerBound = goods[sortedValwei[0][0]].value;
	upBound = bagContain * goods[sortedValwei[0][0]].val_wei;
}

//�޽纯��������ub
int pruning(int v, int i, int w) {
	return v + (bagContain - w)*(goods[i].val_wei);
}

//valueָ��ǰ��������Ʒ�ļ�ֵ��weightָ��ǰ�����е�����
//��֧�޽編 �㷨���岿��
operatewaitNode solve(waitLine *PT) {
	while (!PT->isEmpty()) {
		operatewaitNode WaitNode = PT->leaveNode(); //��PT����ȡ�����
		if (WaitNode->sequence.size() == SUM) {
			return WaitNode;
		}
		int indexOrd = WaitNode->indexOrder; //ָ��ǰ����������е�λ��
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
		//�鿴�����Ƿ����Ҫ��
		if (curWeight + goods[curIndex].weight <= bagContain) { //��������Ҫ��
			if (UB1 >= lowerBound) { //�����½�Ҫ��
				operatewaitNode newWaitNode = new waitNode; //�½���㣬���ò���
				newWaitNode->ub = UB1;
				newWaitNode->indexOrder = indexOrd + 1;
				newWaitNode->sequence = WaitNode->sequence;
				newWaitNode->sequence.push_back(1); //��������
				newWaitNode->weight = curWeight + goods[curIndex].weight;
				newWaitNode->value = curValue + goods[curIndex].value;
				newWaitNode->next = NULL;
				PT->addNode(newWaitNode); //���½�����PT��
			}
		}

		if (indexOrd + 1 >= SUM) { 
			UB2 = curValue;
		}
		else
			UB2 = pruning(curValue, nextIndex, curWeight);
		
		//�鿴�Һ����Ƿ����Ҫ��
		if (UB2 >= lowerBound) { //�����½�Ҫ��
			operatewaitNode newWaitNode1 = new waitNode;  //�½���㣬���ò���
			newWaitNode1->ub = UB2;
			newWaitNode1->indexOrder = indexOrd + 1;
			newWaitNode1->sequence = WaitNode->sequence;
			newWaitNode1->sequence.push_back(0); //��������
			newWaitNode1->weight = curWeight;
			newWaitNode1->value = curValue;
			newWaitNode1->next = NULL;
			PT->addNode(newWaitNode1); //���½�����PT��
		}
	}
}

//����ʼ�ڵ㣨�����ڵ㣩����PT��
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
	cout << "********************��ȡ�û�����********************\n\n";
	getUserInput(); //��ȡ�û�����
	initSort(); //����
	findBound(); 
	cout << "**********************������**********************\n";
	waitLine *line = new waitLine;
	operatewaitNode MaxNode = new waitNode;
	initQueue(line); //�����ڵ����PT��
	MaxNode = solve(line);
	cout << "װ����Ʒ��ϵ�����ֵΪ��" << MaxNode->value << "��\n";
	cout << "�ﵽ�ü�ֵ�����Ϊ��";
	for (int ele : MaxNode->sequence) { //��ӡ�������
		cout << ele<< " ";
	}
	cout << "\n";
	system("pause");
	return 0;
}