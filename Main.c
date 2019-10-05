/*
	게임명	 : 그냥 평범한 리듬게임
	제작자	 : 1학년 5반 9번 나영연
	게임장르 : 리듬게임
	게임상세 : 동요 중심의 리듬게임이다. 동요를 선택한 이유는 저작권이 없는 경우(작자 미상)가 많으며,
			   짧고 악보가 간단해서다. 대부분 최대 100 노트 정도로 구현되며,
			   음계를 한 옥타브만 구현하고도 게임을 만들 수 있다.
			   그럼 동심의 세계로!!! - 끝 -
	컴파일러 : VS2017 컴파일러 (conio.h가 없다.)

	파일구성 : Main.c	메인 코드
			   sounds	소리들 저장
					ㄴ sound1 ~ sound8.wav	도레미파솔라시도 저장
			   songs	악보(채보) 저장
					ㄴ 곰세마리.txt 등		곰세마리같은 곡들의 악보와 채보 저장
			   results	최고 점수 저장
					ㄴ 곰세마리_SCORE.txt	곰세마리같은 곡들의 최고 점수 저장
			   Make_license.py
					ㄴ 라이센스 이미지를 생성하는 코드 (system함수를 이용해 argv로 값을 전달함)

	목차	 :
		1. 라이브러리
		2.  전처리기
		3.   구조체
		4.  전역변수
		5.   함수명
		6.    코드
*/

//========== 라이브러리 ==========//

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib" )

//==========  전처리기  ==========//

// 화면 크기
#define WIDTH 42
#define HEIGHT 24

// 노트 동시 생성 최대 갯수 ==> 많아지면 편하지만, 심하면 렉이 걸릴수 있다
#define NOTE_NUM 100

// 곡 개수 저장
#define play_list_size 7

//==========   구조체   ==========//

// 실행음 정보 (음, 길이)
typedef struct _Sound
{
	int sound;
	int length;
	int key;
} Sound;

// 노트별 상태 (좌표, 키 값, 사용여부)
typedef struct _Note
{
	int x;
	int y;
	int key_num;
	int point;
	int state;
} Note;

//==========  전역변수  ==========//

// 코드 효율성 높이는 용도 (d,f,g,h,j,k의 6개 키 값을 아스키로 저장)
int key_set[6] = { 'd', 'f', 'g', 'h', 'j', 'k' };

// 점수 저장
int score = 0;

// 최대 1000개 음 저장
Sound data[1000];
int data_size;

// 곡 목록
char play_list[play_list_size][100] = {
	"곰세마리",
	"사과같은 내 얼굴",
	"산토끼",
	"산중호걸",
	"시계",
	"열 꼬마 인디언",
	"바둑이 방울",
};

// 현재 진행중인 곡의 인덱스 저장
int list_index;

//==========   함수명   ==========//

void gotoxy(int x, int y);
void resizeConsole(int x, int y);
void clearCursor();
void ChangeTextColor(int color);
char displayStartScreen();
void displayHowto();
int displayGameList();
void printBound();
int check();
void get_name(char *name);
void WaitPage();
void makeLicense();
void countDown();
void ParseData(char *file);
void soundBeep(Sound sound);
void initNote(Note *note, int val);
void displayNote(Note *note);
void displayScore(int s);
void println();
void displayResult();
void resetScore();

// 그리고 메인
int main();

//==================== 코드 ====================//

// 메인
int main() {
	/*
	count	 : 시간 쪼개는 용도로 이용
	note_cnt : 사용중인 노트의 마지막 위치 저장
			   (NOTE_NUM개로 돌려쓰기 위함이다.)
	value	 : 노트 키 값 저장
	note	 : 노트를 저장할 배열
	play	 : 초기화면에서 선택한 값을 저장(1 : 시작, 2 : 방법, 3 : 종료)
	point	 : 불러올 데이터의 위치를 저장
	*/
	int count = 0;
	int note_cnt = 0;
	int value;
	Note note[NOTE_NUM] = { 0,0,0,0 };
	char play;
	int point;

	while(TRUE) {
		// 콘솔 크기 기본으로 설정 (WIDTH, HEIGHT), 커서 지우기
		resizeConsole(0, 0);
		clearCursor();
		// 초기화면 생성 (아무 키 입력까지 대기)
		// 만약 1, 2, 3 중 선택 하지 않아도 while구문에 의해 재실행
		play = displayStartScreen();

		system("cls");
		score = 0;
		PlaySound(NULL, 0, 0);

		switch (play) {
		case '1':	// 게임 실행
			// 노트들 초기화
			note_cnt = 0;
			// data_size : 데이터 파싱 전 크기 초기화
			data_size = 0;
			// 곡을 선택받고 play_state에 오류가 있으면 1을 저장
			int play_state = displayGameList();
			system("cls");
			// 오류 있는 경우 switch문 탈출(초기화면으로)
			if (play_state) break;
			// 기본 화면 출력 (테두리와 점수)
			printBound();
			displayScore(score);
			countDown();

			// 데이터를 불러올 위치를 저장
			point = 0;
			while (TRUE) {
				// 기준선 출력 (이 선 기준 위로 2칸 까지 인식됨)
				println();

				// count가 증가 하면서 시간을 끄는데,
				// 음의 길이(data[point].length)를 이용하여 얼마나 끌지 확인
				if (count % data[point].length == 0)
				{
					//다음 노트로
					note_cnt++;

					// 현재 위치에서 키 값 불러옴
					value = data[point].key;

					// 현재 위치 노트에 사용중인 데이터 위치를 저장하고,
					// 노트 생성 함수에 키 값과 함께 전달하여 생성.
					note[note_cnt].point = point++;
					initNote(&note[note_cnt], value);

					// 시간 초기화
					count = 1;
				}

				// 화면 업데이트 및 노트 하강
				if (count % 3 == 0)
					displayNote(note);

				// 점수 출력
				displayScore(score);
				// count 증가 및 시간 딜레이
				count++;
				Sleep(15);

				// 만약 현재 데이터를 불러올 위치가 끝이면,
				// 마지막으로 HEIGHT * 4 번 반족후 종료
				if (point == data_size) {
					for (int i = 0; i < HEIGHT * 4; i++) {
						println();
						if (count % 3 == 0)
							displayNote(note);
						displayScore(score);
						count++;
						Sleep(15);
					}
					break;
				}
			}
			// 결과 (점수 출력하기)
			displayResult();
			break;

		case '2':
			// 방법 출력하기
			displayHowto();
			break;

		case '3':
			// 기록 초기화하기
			resetScore();
			break;

		case '4':
			// 라이센스 만들기
			makeLicense();
			break;

		case '5':
			// 종료하기
			exit(0);
	
		}
	}
	return 0;
}

// x, y로 커서 이동
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 콘솔 크기 지정 (x, y에 0 넣을 시 WIDTH와 HEIGHT값으로 변경)
void resizeConsole(int x, int y) {
	system("title \"그냥 평범한 리듬게임\"");
	char chTemp[50];
	if (x == 0 && y == 0) sprintf(chTemp, "Mode con cols=%d lines=%d", WIDTH, HEIGHT);
	else sprintf(chTemp, "Mode con cols=%d lines=%d", x, y);
	system(chTemp);
}

// 커서 지우기
void clearCursor() {
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

/* 텍스트 색깔 지정 (16진수로 0x(배경)(글자) 입력)
0 = 검정색,			1 = 파란색
2 = 초록색,			3 = 옥색
4 = 빨간색,			5 = 자주색
6 = 노란색,			7 = 흰색
8 = 회색,			9 = 연한 파란색
a = 연한 초록색,	b = 연한 옥색
c = 연한 빨간색,	d = 연한 자주색
e = 연한 노란색,	f = 진한 흰색
*/
void ChangeTextColor(int color) {
	char chTemp[20];
	SetConsoleTextAttribute(
		GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 시작 화면
char displayStartScreen() {
	// 태두리 부분 (0, 0, WIDTH - 2, HEIGHT - 1)줄 출력
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
	// 색 바꿔가면서 출력
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2 - 3);
	ChangeTextColor(7);
	printf("===============");
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2 - 2);
	printf(": 그냥 평범한 :");
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2 - 1);
	printf(":");
	ChangeTextColor(12);
	printf("  리듬 게임  ");
	ChangeTextColor(7);
	printf(":");
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2);
	printf("===============");

	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	printf("1. 곡 선택");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 3);
	printf("2. 플레이 방법");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 4);
	printf("3. 기록 초기화");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 5);
	printf("4. 라이센스 발급");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 6);
	printf("5. 종료");

	char ch;
	// 아무거나 버퍼에 생길 때까지 대기
	while (!kbhit());
	// 여러개 눌리면 다 무시하고 버퍼의 마지막 값 리턴
	// 1ms내에 여러 키를 누를 가능성은 극히 적지만... 무시하긴 싫었다. (속도 영향 적음)
	while (kbhit()) ch = getch();
	return ch;
}

// 플레이 방법 출력
void displayHowto() {
	// 글씨가 많으므로 창 재설정
	resizeConsole(65, 25);
	// 출력할 문구 2차원 배열 (갯수 * 문자열) 저장
	char printing_data[20][100] = {
		"		          플레이 방법",
		"",
		"		       ■■■ 이 떨어질때",
		"	    떨어지는 해당 위치의 키를 누르면 됩니다.",
		"",
		"		     총 6개의 키가 있으며,",
		"    왼쪽부터 d, f, g, h, j, k 로 구성되어 있습니다.",
		"",
		"		   아마도 의도했지만 놀랍게도",
		"	   키보드에서 키가 연속적으로 배치되어 있고,",
		"	   자판 운지상 검지 중지 사용이 용이합니다.",
		"",
		"      키를 누르는 타이밍은 기준선(---)에 닿았을때",
		"		        누르시면 됩니다...",
		"",
		"		   난이도가 어려울 수도 있습니다.",
		"",
		"",
		"추가로 곡 선택 취소하고 싶을때 없는 번호 누르면 취소됩니다.",
		"                       나가기 : 아무키"
	};
	// 저장한 문구 한 줄씩 내려가며 출력
	for (int i = 0; i < 20; i++) {
		gotoxy(3, i + 3);
		printf("%s", printing_data[i]);
	}
	
	// 시작 화면의 함수와 같으나 마지막 값을 리턴하지는 않는다.
	while (!kbhit());
	while (kbhit()) getch();
}

// 플레이 가능한 게임 출력
int displayGameList() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			// ㅁ자로 출력
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
	// 음악 리스트 출력
	ChangeTextColor(7);
	for (int cnt = 0, y = 2; cnt < play_list_size; y += 2, cnt++) {
		gotoxy(3, y);
		printf("%d. %s", cnt + 1, play_list[cnt]);
	}

	char ch;
	while (!kbhit());	// 아무거나 버퍼에 생길 때까지 대기
	while (kbhit()) ch = getch();	// 버퍼 비우고 마지막 값 ch에 저장

	// 만약 플레이 리스트의 인덱스보다 큰 수 입력시
	// 에러가 나므로 초기화면으로 돌아감
	if (ch - '1' + 1 > play_list_size)
		return 1;

	// 입력 인덱스 번호의 음악을 불러오고, 인덱스 정보를 저장한다.
	list_index = ch - '1';
	char chTemp[100];
	sprintf(chTemp, "songs\\%s.txt", play_list[ch - '1']);
	ParseData(chTemp);
	return 0;
}

// 테두리 출력 (WIDTH와 HEIGHT값 변경으로 적용)
void printBound() {
	// 다른 함수에서는 그냥 구현했지만,
	// 이 함수도 main 내에서 직접 작성하면 코드가 너무 길어져서 빼냈다.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			ChangeTextColor(48);
			if (y < 2) for (int x = 0; x < WIDTH; x++) printf(" ");
			else if (x == 0 || y == 2 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(85);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
}

// 사용 가능 여부 확인
int check() {
	// 화면을 지우고,
	system("cls");
	// 테두리를 출력한다.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}

	ChangeTextColor(4);
	gotoxy(4, 3);
	printf("python은 3버전입니다");

	gotoxy(WIDTH / 2 - 6, HEIGHT / 2 - 3);
	printf("주 의  사 항");
	ChangeTextColor(6);
	gotoxy(WIDTH / 2 - 13, HEIGHT / 2 - 2);
	printf("아래 조건이 만족되었는지를");
	gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 1);
	printf("반드시 확인해 주세요");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 15, HEIGHT / 2  + 1);
	printf("PATH에 python이 추가 되었는가?");
	gotoxy(WIDTH / 2 - 17, HEIGHT / 2 + 2);
	printf("python에 아래 라이브러리가 있는가?");

	ChangeTextColor(6);
	gotoxy(WIDTH / 2 - 12, HEIGHT / 2 + 3);
	printf("Pillow     opencv-python");

	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 + 7);
	printf("계속 진행 : 엔터");

	// 엔터시 계속 진행 (엔터키 아스키코드 13)
	while (!kbhit());
	if (getch() != 13) return 1;
	
	return 0;

}

// 라이센스 발급 방법 설명
int HowToGet() {
	// 화면을 지우고,
	system("cls");
	// 테두리를 출력한다.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}

	ChangeTextColor(6);
	gotoxy(WIDTH / 2 - 6, HEIGHT / 2 - 4);
	printf("하 는  방 법");
	gotoxy(WIDTH / 2 - 9, HEIGHT / 2 - 2);
	ChangeTextColor(3);
	printf("이름");
	ChangeTextColor(7);
	printf("을 입력하세요.");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 - 1);
	ChangeTextColor(3);
	printf("사진");
	ChangeTextColor(7);
	printf("을 찍히세요.");
	gotoxy(WIDTH / 2 - 16, HEIGHT / 2);
	ChangeTextColor(7);
	printf("게임과 함께 있는 ");
	ChangeTextColor(3);
	printf("\"result\"");
	ChangeTextColor(7);
	printf(" 확인!");
	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 + 1);
	printf("끝입니다");

	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 + 7);
	printf("계속 진행 : 엔터");

	// 엔터시 계속 진행 (엔터키 아스키코드 13)
	while (!kbhit());
	if (getch() != 13) return 1;

	return 0;

}

// 이름 입력창
void get_name(char *name) {
	// 화면을 지우고,
	system("cls");
	// 테두리를 출력한다.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}

	// 이름을 입력받는다.
	ChangeTextColor(4);
	gotoxy(WIDTH / 2 - 11, HEIGHT / 2 - 1);
	printf("(한영키 누르지 마세요!)");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2);
	printf("이름 : ");
	scanf("%s", name);
}

// 대기창
void WaitPage() {
	// 화면을 지우고,
	system("cls");
	// 테두리를 출력한다.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}

	ChangeTextColor(6);
	gotoxy(WIDTH / 2 - 10, HEIGHT / 2);
	printf("저장이 완료될 때까지");
	gotoxy(WIDTH / 2 - 9, HEIGHT / 2 + 1);
	printf("잠시 기다려 주세요");
}

// 라이센스 (이미지)발급하는 함수 - Python의 Pillow와 opencv2이용
void makeLicense() {
	if (check() || HowToGet()) return;

	// chTemp	 : 파일명 저장
	// score_sum : 기록 누적
	char chTemp[100];
	int score_sum = 0;

	int tmp;
	for (int i = 0; i < play_list_size; i++) {
		// 파일명 저장 (list_index는 음악 데이터를 불러올 때 설정된다.)
		sprintf(chTemp, "results\\%s_SCORE.txt", play_list[i]);

		// 읽기모드로 파일을 열어서 총점 누적
		FILE *INP = fopen(chTemp, "r");
		fscanf(INP, "%d", &tmp);
		score_sum += tmp;
		fclose(INP);
	}

	char score_data[100];
	char name[20];

	get_name(name);
	WaitPage();
	// Python.h 사용에 실패해서 system함수로 파이썬을 실행시켰다. (python3과 Pillow, opencv2 필수)
	/*
		#참고#
		python3 설치 방법
			https://www.python.org/ 에서 다운받고 실행해서 PATH추가 누르고 설치
		Pillow, opencv2 설치 방법 (easy_install을 지향합니다. (pip가 기능이 많지만 pip가 절 싫어해요))
			easy_install Pillow
			easy_install opencv-python
	*/
	sprintf(score_data, "python Make_license.py \"이름 : %s\" \"총 히트수 : %d\"", name, score_sum);
	system(score_data);
}

// 카운트다운 하는 함수
void countDown() {
	ChangeTextColor(7);
	// 숫자(아스키아트) 저장
	char printing_data[4][5][13] = {
		{
			"            ",
			"            ",
			"            ",
			"            ",
			"            ",
		},
		{
			"   _____    ",
			"  /_   ||   ",
			"   |   ||   ",
			"   |   ||   ",
			"   |___||   ",
		},
		{
			" ________   ",
			"|_____  ||  ",
			"  / ____//  ",
			" /       || ",
			"/________|| ",
		},
		{
			" ________   ",
			"|_____  ||  ",
			" __(__  <<  ",
			"|        || ",
			"|________|| ",
		}
	};

	// 1초마다 3, 2, 1 을 출력
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 5; j++) {
			gotoxy(WIDTH / 2 - 4, HEIGHT / 2 - 2 + j);
			printf("%s", printing_data[i][j]);
		}
		Sleep(1000);
	}
}

// 노트 생성 (val값은 0 ~ 6)
void initNote(Note *note, int val) {
	note->key_num = key_set[val];	// 입력 확인용
	note->x = val * 6  + 3;	// 노트 x 지정
	note->y = 3;	// 노트 y 지정
	note->state = 1;	// 유효하면 1 아니면 0 (여기서는 생성하는 과정이므로 1)
}

// 곡 데이터 불러오기
void ParseData(char *fname) {
	// 파일 입출력을 통해 fname을 불러온다.
	// (예를 들어 곰세마리.txt를 불러온다.)
	FILE *file = fopen(fname, "r");
	// 파일 저장 형식 : 줄 별로 (음 높이)(음 길이) (키번호)로 구분
	// 불러와서 data에 저장하고, data_size는 데이터 크기로 저장된다.
	while (fscanf(file, "%1d%d %1d",
		&data[data_size].sound, &data[data_size].length, &data[data_size].key) != EOF)
	{
		data[data_size].length++;
		data_size++;
	}
	fclose(file);
}

// 소리내기 (도:0 ~ 높은 도:7)
void soundBeep(Sound sound) {
	// PlaySound 함수를 이용해 소리를 출력한다.
	sndPlaySound(NULL, SND_PURGE);
	switch (sound.sound)
	{
	// 도 음 재생
	case 0:
		sndPlaySound(TEXT("sounds\\sound1.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 레 음 재생
	case 1:
		sndPlaySound(TEXT("sounds\\sound2.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 미 음 재생
	case 2:
		sndPlaySound(TEXT("sounds\\sound3.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 파 음 재생
	case 3:
		sndPlaySound(TEXT("sounds\\sound4.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 솔 음 재생
	case 4:
		sndPlaySound(TEXT("sounds\\sound5.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 라 음 재생
	case 5:
		sndPlaySound(TEXT("sounds\\sound6.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 시 음 재생
	case 6:
		sndPlaySound(TEXT("sounds\\sound7.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// 높은도 음 재생
	case 7:
		sndPlaySound(TEXT("sounds\\sound8.wav"), SND_FILENAME | SND_ASYNC);
		break;
	}
}

// 노트 그리기 및 한칸 아래로
void displayNote(Note *note) {
	// 노트 갯수만큼을 전부 확인한다.
	for (int i = 0; i < NOTE_NUM; i++)
	{
		// 속도를 위해 값이 유효한가를 미리 확인
		if (note[i].state == 1)
		{
			// 처음(생성)위치가 아니면 위의 흔적 지운다.
			if (note[i].y > 3) {
				gotoxy(note[i].x, note[i].y - 1);
				ChangeTextColor(0);
				printf("      ");
			}
			// 노트 위치로 이동하여 출력한다.
			gotoxy(note[i].x, note[i].y);
			ChangeTextColor(136);
			printf("■■■");
			// 노트의 위치를 한칸 아래로 저장한다.
			note[i].y++;

			// 노트의 높이가 인식 범위에 들어가면 소리를 재생한다.
			if (note[i].y == HEIGHT - 6) soundBeep(data[note[i].point]);

			// 높이가 HEIGHT - 4보다 크면 인식하기 시작한다.
			if (note[i].y > HEIGHT - 4)
			{
				int ch;
				// 키를 눌렀다면 버퍼의 마지막 값을 저장한다.
				if (kbhit()) {
					while (kbhit())
						ch = getch();
					// 만약 옳은 키를 입력했으면 점수를 증가시키고,
					// 노트를 지운 후 유효하지 않게 한다.
					if (ch == note[i].key_num)
					{
						score++;
						note[i].state = 0;
						gotoxy(note[i].x, note[i].y - 1);
						ChangeTextColor(0);
						printf("      ");
					}
				}
				// 기준선 아래까지 도달시 지우고
				// 노트를 지운 후 유효하지 않게 한다.
				if (note[i].y > HEIGHT - 2)
				{
					note[i].state = 0;
					gotoxy(note[i].x, note[i].y - 1);
					ChangeTextColor(0);
					printf("      ");
				}
			}
			else
			{	
				// 높이에 도달하지 않았다면, 미리 누르지 않도록 항상 버퍼를 지운다.
				if (kbhit()) {
					while (kbhit())
						getch();
				}
			}
		}
	}
}

// 최고기록을 불러오고 만약 자신이 최고기록이면 저장
int GET_HighScore() {
	// chTemp	  : 파일명 저장
	// HIGH_score : 최고기록 저장
	char chTemp[100];
	int HIGH_score;

	// 파일명 저장 (list_index는 음악 데이터를 불러올 때 설정된다.)
	sprintf(chTemp, "results\\%s_SCORE.txt", play_list[list_index]);

	// 읽기모드로 파일을 열어서 HIGH_score에 저장
	FILE *INP = fopen(chTemp, "r");
	fscanf(INP, "%d", &HIGH_score);
	fclose(INP);

	// 만약 HIGH_score가 score보다 크면 값을 덮어 씌운다.
	if (score > HIGH_score) {
		HIGH_score = score;
		FILE * OUTP = fopen(chTemp, "w");
		fprintf(OUTP, "%d", HIGH_score);
		fclose(OUTP);
	}

	// 최종적인 최고기록 리턴
	return HIGH_score;
}

// 점수 출력
void displayScore(int s) {
	// 위 줄에서 3/4 지점에서 출력한다.
	gotoxy(WIDTH / 4 * 3, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	ChangeTextColor(55);
	printf("Hit : %4d", s);
}

// 기준선 출력
void println() {
	// 기준선(------)을 출력한다.
	ChangeTextColor(7);
	gotoxy(2, HEIGHT - 4);
	for (int i = 2; i < WIDTH - 2; i++)
	{
		printf("-");
	}
	// (친구의 의견으로 만들게 됨) 눌러야 할 키를 출력한다.
	gotoxy(2, HEIGHT - 3);
	printf("   D  |  F  |  G  |  H  |  J  |  K");
}

// 결과 출력
void displayResult() {
	// 화면을 지우고,
	system("cls");
	// 테두리를 출력한다.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("■");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
	
	// 최고기록을 출력한다.
	ChangeTextColor(2);
	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 - 3);
	printf("최고기록");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 - 2);
	printf("HighScore : %4d", GET_HighScore());

	// 자신의 기록을 출력한다.
	ChangeTextColor(5);
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2);
	printf("당신의 점수는...");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 6, HEIGHT / 2 + 1);
	printf("Score : %4d", score);

	Sleep(500);

	// 미리 버퍼를 지운 뒤 키를 누를때까지 기다리고 눌리면 버퍼를 모두 지운다.
	if (kbhit())
		while (kbhit())
			getch();
	while (!kbhit());
	while (kbhit())
		getch();
}

// 기록 초기화
void resetScore() {
	char chTemp[100];
	// 곡 이름 하나씩 불러오면서 해당 점수 파일을 0으로 저장
	for (int index = 0; index < play_list_size; index++) {
		sprintf(chTemp, "results\\%s_SCORE.txt", play_list[index]);
		FILE * OUTP = fopen(chTemp, "w");
		fprintf(OUTP, "0");
		fclose(OUTP);
	}
}
