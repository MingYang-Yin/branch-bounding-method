#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
using namespace std;
typedef struct waitNode {
	vector<int> sequence; //储存扩展结点到根结点的路径
	int lb; //评估函数的值
	struct waitNode *next;
}waitNode, *operatewaitNode;

class waitLine {
private:
	operatewaitNode front; //储存链表头结点
	operatewaitNode rear; //储存链表尾结点
	int num = 0; //储存链表中元素个数
public:
	waitLine() {
		front = new waitNode;
		front->next = 0;
		rear = front;
	} //无参构造函数
	void addNode(operatewaitNode e);//在链表中增加等待结点
	operatewaitNode leaveNode();//在链表中减少结点
	bool isEmpty();//检验链表是否为空。若为空，返回true；若不为空，返回false。
};

