#include "queueDef.h"
void waitLine::addNode(operatewaitNode e) { //�����������ӵȴ���㣨����ub�Ӵ�С���У�
	operatewaitNode temp = front;
	bool putAtLast = false;
	if (num == 0) { //���������еĵ�һ����㣬��ֱ�����
		e->next = temp->next;
		temp->next = e;
	}
	else { //������ӵ���ȷλ��
		for (int i = 0; i < num + 1; i++) {
			if (temp->next == NULL) { //����������β
				putAtLast = true;
				break;
			}
			if (e->ub >= (temp->next)->ub) { //����
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
operatewaitNode waitLine::leaveNode() { //�������м��ٽ��
	operatewaitNode temp = front;
	temp = front->next;
	front->next = temp->next;
	num--;
	return temp;
}

bool waitLine::isEmpty() { //�ж������Ƿ�Ϊ��
	if (num == 0)
		return true;
	else
		return false;
}