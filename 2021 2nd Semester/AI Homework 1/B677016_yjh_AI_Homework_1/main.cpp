#include <iostream>
#include <random>
#include <Windows.h>

using namespace std;

struct Tom
{
	enum STATE {
		REST,	//�޽�
		TRACK,	//����
		ATTACK,	//����
		RUN,	//����
		FAINT	//����

	};

	int state;
	int trackCounter;
	int faintCounter;

	Tom() {
		state = REST;
		trackCounter = 0;
		faintCounter = 0;
	}
	~Tom() {}
};

struct Jerry
{
	enum STATE {
		REST,	//�޽�
		ACTIVE,	//�ķ� ����
		TRAP_READY, //���� ���
		FAINT	//����

	};

	int state;
	int foodCounter;
	int faintCounter;
	int activeCounter;

	Jerry() {
		state = REST;
		foodCounter = 5;
		faintCounter = 0;
		activeCounter = 0;
	}
	~Jerry() {};
};

struct Spike
{
	enum STATE {
		REST,	//�޽�
		ATTACK_READY, //���� ���
		TRACK,	//����
		ATTACK	//����

	};

	int state;
	int trackCounter;

	Spike() {
		state = REST;
		trackCounter = 0;
	};
	~Spike() {};
};

void TomStateUpdate(Tom&, Jerry, Spike);
void JerryStateUpdate(Tom, Jerry&, Spike);
void SpikeStateUpdate(Tom, Jerry, Spike&);
int RandomNumber(int, int);

int main() {
	srand(time(NULL));

	Tom tom;
	Jerry jerry;
	Spike spike;

	for (int i = 0; i < 30; i++)
	{
		TomStateUpdate(tom, jerry, spike);
		JerryStateUpdate(tom, jerry, spike);
		SpikeStateUpdate(tom, jerry, spike);
		cout << endl;
		Sleep(1600);
	}

	return 0;
}

int RandomNumber(int min, int max) {
	return rand() % (max - min) + min;
}

void TomStateUpdate(Tom& tom_, Jerry jerry_, Spike spike_) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	//�޽� ����
	if (tom_.state == tom_.REST) {
		if (jerry_.state == jerry_.REST) {

			cout << "���� �ڽ��� �ڸ����� ���ϰ� ���� �־��." << endl;
		}
		else {
			tom_.trackCounter = RandomNumber(2, 5);
			tom_.state = tom_.TRACK;

			cout << "���� ������ ������ �ٴϴ� ������ ������ �ڽ��� �ڸ����� �Ͼ�׿�." << endl;
		}
	}

	//���� ����
	else if (tom_.state == tom_.TRACK) {
		if (tom_.trackCounter > 0) {
			tom_.trackCounter--;
			cout << "���� �ͷ��� �⼼�� ������ ������� ���ƴٴϰ��־��!" << endl;
			cout << "������ " << tom_.trackCounter << "���ڱ��� ��..." << endl;
		}
		else if (jerry_.state == jerry_.REST) {
			tom_.state = tom_.REST;
			cout << "���� �����̾�������. ���� ����� ǥ������ �ٽ� ���� �����־��." << endl;
		}
		else {
			tom_.state = tom_.ATTACK;
			cout << "���� ������ ����ҳ�����!" << endl;
		}
	}

	//���� ����
	else if (tom_.state == tom_.ATTACK) {
		if (jerry_.state == jerry_.FAINT) {
			tom_.state = tom_.REST;
			cout << "���� �������� �ѹ� �Կ��׿�!" << endl;
		}
		else if (spike_.state == spike_.TRACK){
			tom_.state = tom_.RUN;
			cout << "����.. ���� �������� ������ũ�� �����̿��׿�.. ���� �������� �����߾��!" << endl;
		}
		else {
			cout << "���� ���� �����⸦ �߾��!" << endl;
		}
	}

	//���� ����
	else if (tom_.state == tom_.RUN) {
		if (spike_.state == spike_.TRACK) {
			cout << "���� ������ũ���Լ� ����ġ�� �־��!" << endl;
		}
		else {
			tom_.faintCounter = 5;
			tom_.state = tom_.FAINT;
			cout << "���� �ᱹ ������ũ���� �ѹ� �¾ҳ׿�.." << endl;
		}
	}

	//���� ����
	else if (tom_.state == tom_.FAINT) {
		if (tom_.faintCounter > 0) {
			tom_.faintCounter--;
			cout << "���� ������ ������ �־��." << endl;
			cout << "������ ���ƿ����� " << tom_.faintCounter << "�� ���� �� �ɸ��ڳ׿�." << endl;
		}
		else {
			tom_.state = tom_.REST;
			cout << "���� ���� ������ ���ȳ׿�." << endl;
		}
	}

	//����
	else {
		cout << "State Error" << endl;
		return;
	}
}

void JerryStateUpdate(Tom tom_, Jerry& jerry_, Spike spike_) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
	//�޽� ����
	if (jerry_.state == jerry_.REST) {
		if (jerry_.foodCounter > 0) {
			jerry_.foodCounter--;
			cout << "������ ������ ���ִ� ������ �����鼭 �����־��." << endl;
			cout << "������ ���� " << jerry_.foodCounter << "���� ������ �����־��." << endl;
		}
		else {
			jerry_.activeCounter = RandomNumber(1, 4);
			jerry_.foodCounter = jerry_.activeCounter;
			jerry_.state = jerry_.ACTIVE;
			cout << "������ ������ ã���� �������� ���Ծ��." << endl;
		}
	}

	//�ķ� ���� ����
	else if (jerry_.state == jerry_.ACTIVE) {
		if (jerry_.activeCounter > 0) {
			jerry_.activeCounter--;
			cout << "������ ���� ���شٴϸ鼭 ������ ì��� �־��!" << endl;
			cout << "������ �߰��� " << jerry_.activeCounter << "���� ������ �������� �;��ؿ�." << endl;
		}
		else if (tom_.state == tom_.TRACK) {
			jerry_.state = jerry_.TRAP_READY;
			cout << "������ ������ �غ��ϰ� ���� ����� ��ø��� �ֳ׿�." << endl;
		}
		else if (tom_.state == tom_.ATTACK) {
			jerry_.foodCounter = 0;
			jerry_.faintCounter = 5;
			jerry_.state = jerry_.FAINT;
			cout << "������ �迡�� �ѹ� �¾ҳ׿�.." << endl;
		}
		else {
			jerry_.state = jerry_.REST;
			cout << "������ ��ȭ�Ӱ� ������ �����߾��." << endl;
		}
	}

	//���� ��� ����
	else if (jerry_.state == jerry_.TRAP_READY) {
		if (tom_.state == tom_.ATTACK) {
			jerry_.state = jerry_.REST;
			cout << "������ �ڽ��� ������ ���� �������� ������ ������ �����߾��." << endl;
		}
		else {
			cout << "������ ������ũ������ ���� �����ϰ� �־��." << endl;
		}
	}

	//���� ����
	else if (jerry_.state == jerry_.FAINT) {
		if (jerry_.faintCounter > 0) {
			jerry_.faintCounter--;
			cout << "������ ������ ������ �־��." << endl;
			cout << "������ " << jerry_.faintCounter << "�� ���� ����� �Ұ� ���ƿ�." << endl;
		}
		else {
			jerry_.state = jerry_.REST;
			cout << "������ ������ ���ȳ׿�." << endl;
		}
	}

	//����
	else {
		cout << "State Error" << endl;
		return;
	}
}

void SpikeStateUpdate(Tom tom_, Jerry jerry_, Spike& spike_) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	//�޽� ����
	if (spike_.state == spike_.REST) {
		if (jerry_.state == jerry_.TRAP_READY) {
			spike_.state = spike_.ATTACK_READY;
			cout << "������ũ�� �����̿� ������ �������." << endl;
		}
		else {
			cout << "������ũ�� �ڽ��� ������ �ڰ��־��." << endl;
		}
	}

	//���� �غ� ����
	else if (spike_.state == spike_.ATTACK_READY) {
		if (tom_.state == tom_.ATTACK) {
			spike_.trackCounter = RandomNumber(1, 5);
			spike_.state = spike_.TRACK;
			cout << "������ũ�� �迡�� �����⸦ �¾Ҿ��!!" << endl;
		}
		else {
			cout << "������ũ�� �����̸� ������ ���ϰ� �ڰ��־��." << endl;
		}
	}
	
	//�߰� ����
	else if (spike_.state == spike_.TRACK) {
		if (spike_.trackCounter > 0) {
			spike_.trackCounter--;
			cout << "������ũ�� ���� �ͷ��� �Ѱ� �־��!" << endl;
			cout << "���� " << spike_.trackCounter << "���� ���Ҿ��!!" << endl;
		}
		else {
			spike_.state = spike_.ATTACK;
			cout << "������ũ�� ���� ����ҳ׿�." << endl;
		}
	}

	//���� ����
	else if (spike_.state == spike_.ATTACK) {
		if (tom_.state == tom_.FAINT) {
			spike_.state = spike_.REST;
			cout << "������ũ�� �迡�� �ѹ� �Կ��ְ� ���� ���׿�." << endl;
		}
		else {
			cout << "������ũ�� �迡�� �ѹ� ���� �غ� �ϳ׿�." << endl;
		}
	}

	//����
	else {
		cout << "State Error" << endl;
		return;
	}
}