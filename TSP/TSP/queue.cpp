#include "queueDef.h"
void waitLine::addNode(operatewaitNode e) { //在链表中增加等待结点，按照lb从小到大的顺序排列
	operatewaitNode temp = front;
	bool putAtLast = false;
	if (num == 0) { //若链表为空，直接加入
		e->next = temp->next;
		temp->next = e;
	}
	else { //在链表适当位置插入新结点
		for (int i = 0; i < num + 1; i++) {
			if (temp->next == NULL) { 
				putAtLast = true;
				break;
			}
			if (e->lb <= (temp->next)->lb) {
				e->next = temp->next;
				temp->next = e;
				break;
			}
			temp = temp->next;
		}
		if (putAtLast) {
			temp->next = e;
		}
	}
	num++;
}

//从链表中取出结点
operatewaitNode waitLine::leaveNode() {
	operatewaitNode temp = front;
	temp = front->next;
	front->next = temp->next;
	num--;
	return temp;
}

//判断链表是否为空
bool waitLine::isEmpty() {
	if (num == 0)
		return true;
	else
		return false;
}