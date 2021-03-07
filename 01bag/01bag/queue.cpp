#include "queueDef.h"
void waitLine::addNode(operatewaitNode e) { //在链表中增加等待结点（按照ub从大到小排列）
	operatewaitNode temp = front;
	bool putAtLast = false;
	if (num == 0) { //若是链表中的第一个结点，则直接添加
		e->next = temp->next;
		temp->next = e;
	}
	else { //否则，添加到正确位置
		for (int i = 0; i < num + 1; i++) {
			if (temp->next == NULL) { //添加在链表结尾
				putAtLast = true;
				break;
			}
			if (e->ub >= (temp->next)->ub) { //插入
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
operatewaitNode waitLine::leaveNode() { //在链表中减少结点
	operatewaitNode temp = front;
	temp = front->next;
	front->next = temp->next;
	num--;
	return temp;
}

bool waitLine::isEmpty() { //判断链表是否为空
	if (num == 0)
		return true;
	else
		return false;
}