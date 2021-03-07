#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
using namespace std;
typedef struct waitNode {
	vector<int> sequence; //������չ��㵽������·��
	int ub; //����������ֵ
	int value; 
	int weight;
	int indexOrder; //�����е�λ��
	struct waitNode *next;
}waitNode, *operatewaitNode;

class waitLine {
private:
	operatewaitNode front; //��������ͷ���
	operatewaitNode rear; //��������β���
	int num = 0; //����������Ԫ�ظ���
public:
	waitLine() {
		front = new waitNode;
		front->next = 0;
		rear = front;
	} //�޲ι��캯��
	void addNode(operatewaitNode e); //�����������ӵȴ����
	operatewaitNode leaveNode(); //�������м��ٽ��
	bool isEmpty(); //���������Ƿ�Ϊ�ա���Ϊ�գ�����true������Ϊ�գ�����false��
};
