#include "queueDef.h"
void waitLine::addNode(operatewaitNode e) { //�����������ӵȴ���㣬����lb��С�����˳������
	operatewaitNode temp = front;
	bool putAtLast = false;
	if (num == 0) { //������Ϊ�գ�ֱ�Ӽ���
		e->next = temp->next;
		temp->next = e;
	}
	else { //�������ʵ�λ�ò����½��
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

//��������ȡ�����
operatewaitNode waitLine::leaveNode() {
	operatewaitNode temp = front;
	temp = front->next;
	front->next = temp->next;
	num--;
	return temp;
}

//�ж������Ƿ�Ϊ��
bool waitLine::isEmpty() {
	if (num == 0)
		return true;
	else
		return false;
}