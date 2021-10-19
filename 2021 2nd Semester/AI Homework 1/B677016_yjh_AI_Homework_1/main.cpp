#include <iostream>
#include <random>
#include <Windows.h>

using namespace std;

struct Tom
{
	enum STATE {
		REST,	//휴식
		TRACK,	//추적
		ATTACK,	//공격
		RUN,	//도주
		FAINT	//기절

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
		REST,	//휴식
		ACTIVE,	//식량 조달
		TRAP_READY, //함정 대기
		FAINT	//기절

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
		REST,	//휴식
		ATTACK_READY, //공격 대기
		TRACK,	//추적
		ATTACK	//공격

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
	//휴식 상태
	if (tom_.state == tom_.REST) {
		if (jerry_.state == jerry_.REST) {

			cout << "톰은 자신의 자리에서 편하게 쉬고 있어요." << endl;
		}
		else {
			tom_.trackCounter = RandomNumber(2, 5);
			tom_.state = tom_.TRACK;

			cout << "톰이 집안을 헤집고 다니는 제리를 잡으러 자신의 자리에서 일어났네요." << endl;
		}
	}

	//추적 상태
	else if (tom_.state == tom_.TRACK) {
		if (tom_.trackCounter > 0) {
			tom_.trackCounter--;
			cout << "톰이 맹렬한 기세로 제리를 잡기위해 돌아다니고있어요!" << endl;
			cout << "앞으로 " << tom_.trackCounter << "발자국만 더..." << endl;
		}
		else if (jerry_.state == jerry_.REST) {
			tom_.state = tom_.REST;
			cout << "톰의 착각이었나봐요. 톰은 허망한 표정으로 다시 쉬러 가고있어요." << endl;
		}
		else {
			tom_.state = tom_.ATTACK;
			cout << "톰이 제리를 붙잡았나봐요!" << endl;
		}
	}

	//공격 상태
	else if (tom_.state == tom_.ATTACK) {
		if (jerry_.state == jerry_.FAINT) {
			tom_.state = tom_.REST;
			cout << "톰이 제리에게 한방 먹였네요!" << endl;
		}
		else if (spike_.state == spike_.TRACK){
			tom_.state = tom_.RUN;
			cout << "저런.. 톰이 때린것은 스파이크의 엉덩이였네요.. 톰이 도망가기 시작했어요!" << endl;
		}
		else {
			cout << "톰이 힘껏 발차기를 했어요!" << endl;
		}
	}

	//도주 상태
	else if (tom_.state == tom_.RUN) {
		if (spike_.state == spike_.TRACK) {
			cout << "톰이 스파이크에게서 도망치고 있어요!" << endl;
		}
		else {
			tom_.faintCounter = 5;
			tom_.state = tom_.FAINT;
			cout << "톰이 결국 스파이크에게 한방 맞았네요.." << endl;
		}
	}

	//기절 상태
	else if (tom_.state == tom_.FAINT) {
		if (tom_.faintCounter > 0) {
			tom_.faintCounter--;
			cout << "톰이 정신을 차리고 있어요." << endl;
			cout << "정신이 돌아오려면 " << tom_.faintCounter << "턴 정도 더 걸리겠네요." << endl;
		}
		else {
			tom_.state = tom_.REST;
			cout << "톰이 드디어 정신을 차렸네요." << endl;
		}
	}

	//예외
	else {
		cout << "State Error" << endl;
		return;
	}
}

void JerryStateUpdate(Tom tom_, Jerry& jerry_, Spike spike_) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
	//휴식 상태
	if (jerry_.state == jerry_.REST) {
		if (jerry_.foodCounter > 0) {
			jerry_.foodCounter--;
			cout << "제리는 집에서 맛있는 음식을 먹으면서 쉬고있어요." << endl;
			cout << "제리는 현재 " << jerry_.foodCounter << "개의 음식이 남아있어요." << endl;
		}
		else {
			jerry_.activeCounter = RandomNumber(1, 4);
			jerry_.foodCounter = jerry_.activeCounter;
			jerry_.state = jerry_.ACTIVE;
			cout << "제리가 음식을 찾으러 집밖으로 나왔어요." << endl;
		}
	}

	//식량 조달 상태
	else if (jerry_.state == jerry_.ACTIVE) {
		if (jerry_.activeCounter > 0) {
			jerry_.activeCounter--;
			cout << "제리가 톰을 피해다니면서 음식을 챙기고 있어요!" << endl;
			cout << "제리는 추가로 " << jerry_.activeCounter << "개의 음식을 가져가고 싶어해요." << endl;
		}
		else if (tom_.state == tom_.TRACK) {
			jerry_.state = jerry_.TRAP_READY;
			cout << "제리가 함정을 준비하고 톰을 대놓고 약올리고 있네요." << endl;
		}
		else if (tom_.state == tom_.ATTACK) {
			jerry_.foodCounter = 0;
			jerry_.faintCounter = 5;
			jerry_.state = jerry_.FAINT;
			cout << "제리가 톰에게 한방 맞았네요.." << endl;
		}
		else {
			jerry_.state = jerry_.REST;
			cout << "제리는 평화롭게 집으로 복귀했어요." << endl;
		}
	}

	//함정 대기 상태
	else if (jerry_.state == jerry_.TRAP_READY) {
		if (tom_.state == tom_.ATTACK) {
			jerry_.state = jerry_.REST;
			cout << "제리는 자신의 함정에 빠진 톰을보고 웃으며 집으로 복귀했어요." << endl;
		}
		else {
			cout << "제리가 스파이크쪽으로 톰을 유인하고 있어요." << endl;
		}
	}

	//기절 상태
	else if (jerry_.state == jerry_.FAINT) {
		if (jerry_.faintCounter > 0) {
			jerry_.faintCounter--;
			cout << "제리가 정신을 차리고 있어요." << endl;
			cout << "제리는 " << jerry_.faintCounter << "턴 정도 쉬어야 할것 같아요." << endl;
		}
		else {
			jerry_.state = jerry_.REST;
			cout << "제리가 정신을 차렸네요." << endl;
		}
	}

	//예외
	else {
		cout << "State Error" << endl;
		return;
	}
}

void SpikeStateUpdate(Tom tom_, Jerry jerry_, Spike& spike_) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	//휴식 상태
	if (spike_.state == spike_.REST) {
		if (jerry_.state == jerry_.TRAP_READY) {
			spike_.state = spike_.ATTACK_READY;
			cout << "스파이크의 엉덩이에 제리가 숨었어요." << endl;
		}
		else {
			cout << "스파이크는 자신의 집에서 자고있어요." << endl;
		}
	}

	//공격 준비 상태
	else if (spike_.state == spike_.ATTACK_READY) {
		if (tom_.state == tom_.ATTACK) {
			spike_.trackCounter = RandomNumber(1, 5);
			spike_.state = spike_.TRACK;
			cout << "스파이크가 톰에게 발차기를 맞았어요!!" << endl;
		}
		else {
			cout << "스파이크는 엉덩이를 내놓고 편하게 자고있어요." << endl;
		}
	}
	
	//추격 상태
	else if (spike_.state == spike_.TRACK) {
		if (spike_.trackCounter > 0) {
			spike_.trackCounter--;
			cout << "스파이크가 톰을 맹렬히 쫓고 있어요!" << endl;
			cout << "이제 " << spike_.trackCounter << "걸음 남았어요!!" << endl;
		}
		else {
			spike_.state = spike_.ATTACK;
			cout << "스파이크가 톰을 붙잡았네요." << endl;
		}
	}

	//공격 상태
	else if (spike_.state == spike_.ATTACK) {
		if (tom_.state == tom_.FAINT) {
			spike_.state = spike_.REST;
			cout << "스파이크가 톰에게 한방 먹여주고 쉬러 가네요." << endl;
		}
		else {
			cout << "스파이크가 톰에게 한방 먹일 준비를 하네요." << endl;
		}
	}

	//예외
	else {
		cout << "State Error" << endl;
		return;
	}
}