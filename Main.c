/*
	���Ӹ�	 : �׳� ����� �������
	������	 : 1�г� 5�� 9�� ������
	�����帣 : �������
	���ӻ� : ���� �߽��� ��������̴�. ���並 ������ ������ ���۱��� ���� ���(���� �̻�)�� ������,
			   ª�� �Ǻ��� �����ؼ���. ��κ� �ִ� 100 ��Ʈ ������ �����Ǹ�,
			   ���踦 �� ��Ÿ�길 �����ϰ� ������ ���� �� �ִ�.
			   �׷� ������ �����!!! - �� -
	�����Ϸ� : VS2017 �����Ϸ� (conio.h�� ����.)

	���ϱ��� : Main.c	���� �ڵ�
			   sounds	�Ҹ��� ����
					�� sound1 ~ sound8.wav	�������ļֶ�õ� ����
			   songs	�Ǻ�(ä��) ����
					�� ��������.txt ��		������������ ����� �Ǻ��� ä�� ����
			   results	�ְ� ���� ����
					�� ��������_SCORE.txt	������������ ����� �ְ� ���� ����
			   Make_license.py
					�� ���̼��� �̹����� �����ϴ� �ڵ� (system�Լ��� �̿��� argv�� ���� ������)

	����	 :
		1. ���̺귯��
		2.  ��ó����
		3.   ����ü
		4.  ��������
		5.   �Լ���
		6.    �ڵ�
*/

//========== ���̺귯�� ==========//

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib" )

//==========  ��ó����  ==========//

// ȭ�� ũ��
#define WIDTH 42
#define HEIGHT 24

// ��Ʈ ���� ���� �ִ� ���� ==> �������� ��������, ���ϸ� ���� �ɸ��� �ִ�
#define NOTE_NUM 100

// �� ���� ����
#define play_list_size 7

//==========   ����ü   ==========//

// ������ ���� (��, ����)
typedef struct _Sound
{
	int sound;
	int length;
	int key;
} Sound;

// ��Ʈ�� ���� (��ǥ, Ű ��, ��뿩��)
typedef struct _Note
{
	int x;
	int y;
	int key_num;
	int point;
	int state;
} Note;

//==========  ��������  ==========//

// �ڵ� ȿ���� ���̴� �뵵 (d,f,g,h,j,k�� 6�� Ű ���� �ƽ�Ű�� ����)
int key_set[6] = { 'd', 'f', 'g', 'h', 'j', 'k' };

// ���� ����
int score = 0;

// �ִ� 1000�� �� ����
Sound data[1000];
int data_size;

// �� ���
char play_list[play_list_size][100] = {
	"��������",
	"������� �� ��",
	"���䳢",
	"����ȣ��",
	"�ð�",
	"�� ���� �ε��",
	"�ٵ��� ���",
};

// ���� �������� ���� �ε��� ����
int list_index;

//==========   �Լ���   ==========//

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

// �׸��� ����
int main();

//==================== �ڵ� ====================//

// ����
int main() {
	/*
	count	 : �ð� �ɰ��� �뵵�� �̿�
	note_cnt : ������� ��Ʈ�� ������ ��ġ ����
			   (NOTE_NUM���� �������� �����̴�.)
	value	 : ��Ʈ Ű �� ����
	note	 : ��Ʈ�� ������ �迭
	play	 : �ʱ�ȭ�鿡�� ������ ���� ����(1 : ����, 2 : ���, 3 : ����)
	point	 : �ҷ��� �������� ��ġ�� ����
	*/
	int count = 0;
	int note_cnt = 0;
	int value;
	Note note[NOTE_NUM] = { 0,0,0,0 };
	char play;
	int point;

	while(TRUE) {
		// �ܼ� ũ�� �⺻���� ���� (WIDTH, HEIGHT), Ŀ�� �����
		resizeConsole(0, 0);
		clearCursor();
		// �ʱ�ȭ�� ���� (�ƹ� Ű �Է±��� ���)
		// ���� 1, 2, 3 �� ���� ���� �ʾƵ� while������ ���� �����
		play = displayStartScreen();

		system("cls");
		score = 0;
		PlaySound(NULL, 0, 0);

		switch (play) {
		case '1':	// ���� ����
			// ��Ʈ�� �ʱ�ȭ
			note_cnt = 0;
			// data_size : ������ �Ľ� �� ũ�� �ʱ�ȭ
			data_size = 0;
			// ���� ���ùް� play_state�� ������ ������ 1�� ����
			int play_state = displayGameList();
			system("cls");
			// ���� �ִ� ��� switch�� Ż��(�ʱ�ȭ������)
			if (play_state) break;
			// �⺻ ȭ�� ��� (�׵θ��� ����)
			printBound();
			displayScore(score);
			countDown();

			// �����͸� �ҷ��� ��ġ�� ����
			point = 0;
			while (TRUE) {
				// ���ؼ� ��� (�� �� ���� ���� 2ĭ ���� �νĵ�)
				println();

				// count�� ���� �ϸ鼭 �ð��� ���µ�,
				// ���� ����(data[point].length)�� �̿��Ͽ� �󸶳� ���� Ȯ��
				if (count % data[point].length == 0)
				{
					//���� ��Ʈ��
					note_cnt++;

					// ���� ��ġ���� Ű �� �ҷ���
					value = data[point].key;

					// ���� ��ġ ��Ʈ�� ������� ������ ��ġ�� �����ϰ�,
					// ��Ʈ ���� �Լ��� Ű ���� �Բ� �����Ͽ� ����.
					note[note_cnt].point = point++;
					initNote(&note[note_cnt], value);

					// �ð� �ʱ�ȭ
					count = 1;
				}

				// ȭ�� ������Ʈ �� ��Ʈ �ϰ�
				if (count % 3 == 0)
					displayNote(note);

				// ���� ���
				displayScore(score);
				// count ���� �� �ð� ������
				count++;
				Sleep(15);

				// ���� ���� �����͸� �ҷ��� ��ġ�� ���̸�,
				// ���������� HEIGHT * 4 �� ������ ����
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
			// ��� (���� ����ϱ�)
			displayResult();
			break;

		case '2':
			// ��� ����ϱ�
			displayHowto();
			break;

		case '3':
			// ��� �ʱ�ȭ�ϱ�
			resetScore();
			break;

		case '4':
			// ���̼��� �����
			makeLicense();
			break;

		case '5':
			// �����ϱ�
			exit(0);
	
		}
	}
	return 0;
}

// x, y�� Ŀ�� �̵�
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// �ܼ� ũ�� ���� (x, y�� 0 ���� �� WIDTH�� HEIGHT������ ����)
void resizeConsole(int x, int y) {
	system("title \"�׳� ����� �������\"");
	char chTemp[50];
	if (x == 0 && y == 0) sprintf(chTemp, "Mode con cols=%d lines=%d", WIDTH, HEIGHT);
	else sprintf(chTemp, "Mode con cols=%d lines=%d", x, y);
	system(chTemp);
}

// Ŀ�� �����
void clearCursor() {
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

/* �ؽ�Ʈ ���� ���� (16������ 0x(���)(����) �Է�)
0 = ������,			1 = �Ķ���
2 = �ʷϻ�,			3 = ����
4 = ������,			5 = ���ֻ�
6 = �����,			7 = ���
8 = ȸ��,			9 = ���� �Ķ���
a = ���� �ʷϻ�,	b = ���� ����
c = ���� ������,	d = ���� ���ֻ�
e = ���� �����,	f = ���� ���
*/
void ChangeTextColor(int color) {
	char chTemp[20];
	SetConsoleTextAttribute(
		GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// ���� ȭ��
char displayStartScreen() {
	// �µθ� �κ� (0, 0, WIDTH - 2, HEIGHT - 1)�� ���
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
	// �� �ٲ㰡�鼭 ���
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2 - 3);
	ChangeTextColor(7);
	printf("===============");
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2 - 2);
	printf(": �׳� ����� :");
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2 - 1);
	printf(":");
	ChangeTextColor(12);
	printf("  ���� ����  ");
	ChangeTextColor(7);
	printf(":");
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2);
	printf("===============");

	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	printf("1. �� ����");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 3);
	printf("2. �÷��� ���");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 4);
	printf("3. ��� �ʱ�ȭ");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 5);
	printf("4. ���̼��� �߱�");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 6);
	printf("5. ����");

	char ch;
	// �ƹ��ų� ���ۿ� ���� ������ ���
	while (!kbhit());
	// ������ ������ �� �����ϰ� ������ ������ �� ����
	// 1ms���� ���� Ű�� ���� ���ɼ��� ���� ������... �����ϱ� �Ⱦ���. (�ӵ� ���� ����)
	while (kbhit()) ch = getch();
	return ch;
}

// �÷��� ��� ���
void displayHowto() {
	// �۾��� �����Ƿ� â �缳��
	resizeConsole(65, 25);
	// ����� ���� 2���� �迭 (���� * ���ڿ�) ����
	char printing_data[20][100] = {
		"		          �÷��� ���",
		"",
		"		       ���� �� ��������",
		"	    �������� �ش� ��ġ�� Ű�� ������ �˴ϴ�.",
		"",
		"		     �� 6���� Ű�� ������,",
		"    ���ʺ��� d, f, g, h, j, k �� �����Ǿ� �ֽ��ϴ�.",
		"",
		"		   �Ƹ��� �ǵ������� ����Ե�",
		"	   Ű���忡�� Ű�� ���������� ��ġ�Ǿ� �ְ�,",
		"	   ���� ������ ���� ���� ����� �����մϴ�.",
		"",
		"      Ű�� ������ Ÿ�̹��� ���ؼ�(---)�� �������",
		"		        �����ø� �˴ϴ�...",
		"",
		"		   ���̵��� ����� ���� �ֽ��ϴ�.",
		"",
		"",
		"�߰��� �� ���� ����ϰ� ������ ���� ��ȣ ������ ��ҵ˴ϴ�.",
		"                       ������ : �ƹ�Ű"
	};
	// ������ ���� �� �پ� �������� ���
	for (int i = 0; i < 20; i++) {
		gotoxy(3, i + 3);
		printf("%s", printing_data[i]);
	}
	
	// ���� ȭ���� �Լ��� ������ ������ ���� ���������� �ʴ´�.
	while (!kbhit());
	while (kbhit()) getch();
}

// �÷��� ������ ���� ���
int displayGameList() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			// ���ڷ� ���
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
	// ���� ����Ʈ ���
	ChangeTextColor(7);
	for (int cnt = 0, y = 2; cnt < play_list_size; y += 2, cnt++) {
		gotoxy(3, y);
		printf("%d. %s", cnt + 1, play_list[cnt]);
	}

	char ch;
	while (!kbhit());	// �ƹ��ų� ���ۿ� ���� ������ ���
	while (kbhit()) ch = getch();	// ���� ���� ������ �� ch�� ����

	// ���� �÷��� ����Ʈ�� �ε������� ū �� �Է½�
	// ������ ���Ƿ� �ʱ�ȭ������ ���ư�
	if (ch - '1' + 1 > play_list_size)
		return 1;

	// �Է� �ε��� ��ȣ�� ������ �ҷ�����, �ε��� ������ �����Ѵ�.
	list_index = ch - '1';
	char chTemp[100];
	sprintf(chTemp, "songs\\%s.txt", play_list[ch - '1']);
	ParseData(chTemp);
	return 0;
}

// �׵θ� ��� (WIDTH�� HEIGHT�� �������� ����)
void printBound() {
	// �ٸ� �Լ������� �׳� ����������,
	// �� �Լ��� main ������ ���� �ۼ��ϸ� �ڵ尡 �ʹ� ������� ���´�.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			ChangeTextColor(48);
			if (y < 2) for (int x = 0; x < WIDTH; x++) printf(" ");
			else if (x == 0 || y == 2 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(85);
				printf("��");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
}

// ��� ���� ���� Ȯ��
int check() {
	// ȭ���� �����,
	system("cls");
	// �׵θ��� ����Ѵ�.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
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
	printf("python�� 3�����Դϴ�");

	gotoxy(WIDTH / 2 - 6, HEIGHT / 2 - 3);
	printf("�� ��  �� ��");
	ChangeTextColor(6);
	gotoxy(WIDTH / 2 - 13, HEIGHT / 2 - 2);
	printf("�Ʒ� ������ �����Ǿ�������");
	gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 1);
	printf("�ݵ�� Ȯ���� �ּ���");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 15, HEIGHT / 2  + 1);
	printf("PATH�� python�� �߰� �Ǿ��°�?");
	gotoxy(WIDTH / 2 - 17, HEIGHT / 2 + 2);
	printf("python�� �Ʒ� ���̺귯���� �ִ°�?");

	ChangeTextColor(6);
	gotoxy(WIDTH / 2 - 12, HEIGHT / 2 + 3);
	printf("Pillow     opencv-python");

	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 + 7);
	printf("��� ���� : ����");

	// ���ͽ� ��� ���� (����Ű �ƽ�Ű�ڵ� 13)
	while (!kbhit());
	if (getch() != 13) return 1;
	
	return 0;

}

// ���̼��� �߱� ��� ����
int HowToGet() {
	// ȭ���� �����,
	system("cls");
	// �׵θ��� ����Ѵ�.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
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
	printf("�� ��  �� ��");
	gotoxy(WIDTH / 2 - 9, HEIGHT / 2 - 2);
	ChangeTextColor(3);
	printf("�̸�");
	ChangeTextColor(7);
	printf("�� �Է��ϼ���.");
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 - 1);
	ChangeTextColor(3);
	printf("����");
	ChangeTextColor(7);
	printf("�� ��������.");
	gotoxy(WIDTH / 2 - 16, HEIGHT / 2);
	ChangeTextColor(7);
	printf("���Ӱ� �Բ� �ִ� ");
	ChangeTextColor(3);
	printf("\"result\"");
	ChangeTextColor(7);
	printf(" Ȯ��!");
	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 + 1);
	printf("���Դϴ�");

	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 + 7);
	printf("��� ���� : ����");

	// ���ͽ� ��� ���� (����Ű �ƽ�Ű�ڵ� 13)
	while (!kbhit());
	if (getch() != 13) return 1;

	return 0;

}

// �̸� �Է�â
void get_name(char *name) {
	// ȭ���� �����,
	system("cls");
	// �׵θ��� ����Ѵ�.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}

	// �̸��� �Է¹޴´�.
	ChangeTextColor(4);
	gotoxy(WIDTH / 2 - 11, HEIGHT / 2 - 1);
	printf("(�ѿ�Ű ������ ������!)");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 7, HEIGHT / 2);
	printf("�̸� : ");
	scanf("%s", name);
}

// ���â
void WaitPage() {
	// ȭ���� �����,
	system("cls");
	// �׵θ��� ����Ѵ�.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
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
	printf("������ �Ϸ�� ������");
	gotoxy(WIDTH / 2 - 9, HEIGHT / 2 + 1);
	printf("��� ��ٷ� �ּ���");
}

// ���̼��� (�̹���)�߱��ϴ� �Լ� - Python�� Pillow�� opencv2�̿�
void makeLicense() {
	if (check() || HowToGet()) return;

	// chTemp	 : ���ϸ� ����
	// score_sum : ��� ����
	char chTemp[100];
	int score_sum = 0;

	int tmp;
	for (int i = 0; i < play_list_size; i++) {
		// ���ϸ� ���� (list_index�� ���� �����͸� �ҷ��� �� �����ȴ�.)
		sprintf(chTemp, "results\\%s_SCORE.txt", play_list[i]);

		// �б���� ������ ��� ���� ����
		FILE *INP = fopen(chTemp, "r");
		fscanf(INP, "%d", &tmp);
		score_sum += tmp;
		fclose(INP);
	}

	char score_data[100];
	char name[20];

	get_name(name);
	WaitPage();
	// Python.h ��뿡 �����ؼ� system�Լ��� ���̽��� ������״�. (python3�� Pillow, opencv2 �ʼ�)
	/*
		#����#
		python3 ��ġ ���
			https://www.python.org/ ���� �ٿ�ް� �����ؼ� PATH�߰� ������ ��ġ
		Pillow, opencv2 ��ġ ��� (easy_install�� �����մϴ�. (pip�� ����� ������ pip�� �� �Ⱦ��ؿ�))
			easy_install Pillow
			easy_install opencv-python
	*/
	sprintf(score_data, "python Make_license.py \"�̸� : %s\" \"�� ��Ʈ�� : %d\"", name, score_sum);
	system(score_data);
}

// ī��Ʈ�ٿ� �ϴ� �Լ�
void countDown() {
	ChangeTextColor(7);
	// ����(�ƽ�Ű��Ʈ) ����
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

	// 1�ʸ��� 3, 2, 1 �� ���
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 5; j++) {
			gotoxy(WIDTH / 2 - 4, HEIGHT / 2 - 2 + j);
			printf("%s", printing_data[i][j]);
		}
		Sleep(1000);
	}
}

// ��Ʈ ���� (val���� 0 ~ 6)
void initNote(Note *note, int val) {
	note->key_num = key_set[val];	// �Է� Ȯ�ο�
	note->x = val * 6  + 3;	// ��Ʈ x ����
	note->y = 3;	// ��Ʈ y ����
	note->state = 1;	// ��ȿ�ϸ� 1 �ƴϸ� 0 (���⼭�� �����ϴ� �����̹Ƿ� 1)
}

// �� ������ �ҷ�����
void ParseData(char *fname) {
	// ���� ������� ���� fname�� �ҷ��´�.
	// (���� ��� ��������.txt�� �ҷ��´�.)
	FILE *file = fopen(fname, "r");
	// ���� ���� ���� : �� ���� (�� ����)(�� ����) (Ű��ȣ)�� ����
	// �ҷ��ͼ� data�� �����ϰ�, data_size�� ������ ũ��� ����ȴ�.
	while (fscanf(file, "%1d%d %1d",
		&data[data_size].sound, &data[data_size].length, &data[data_size].key) != EOF)
	{
		data[data_size].length++;
		data_size++;
	}
	fclose(file);
}

// �Ҹ����� (��:0 ~ ���� ��:7)
void soundBeep(Sound sound) {
	// PlaySound �Լ��� �̿��� �Ҹ��� ����Ѵ�.
	sndPlaySound(NULL, SND_PURGE);
	switch (sound.sound)
	{
	// �� �� ���
	case 0:
		sndPlaySound(TEXT("sounds\\sound1.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// �� �� ���
	case 1:
		sndPlaySound(TEXT("sounds\\sound2.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// �� �� ���
	case 2:
		sndPlaySound(TEXT("sounds\\sound3.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// �� �� ���
	case 3:
		sndPlaySound(TEXT("sounds\\sound4.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// �� �� ���
	case 4:
		sndPlaySound(TEXT("sounds\\sound5.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// �� �� ���
	case 5:
		sndPlaySound(TEXT("sounds\\sound6.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// �� �� ���
	case 6:
		sndPlaySound(TEXT("sounds\\sound7.wav"), SND_FILENAME | SND_ASYNC);
		break;
	// ������ �� ���
	case 7:
		sndPlaySound(TEXT("sounds\\sound8.wav"), SND_FILENAME | SND_ASYNC);
		break;
	}
}

// ��Ʈ �׸��� �� ��ĭ �Ʒ���
void displayNote(Note *note) {
	// ��Ʈ ������ŭ�� ���� Ȯ���Ѵ�.
	for (int i = 0; i < NOTE_NUM; i++)
	{
		// �ӵ��� ���� ���� ��ȿ�Ѱ��� �̸� Ȯ��
		if (note[i].state == 1)
		{
			// ó��(����)��ġ�� �ƴϸ� ���� ���� �����.
			if (note[i].y > 3) {
				gotoxy(note[i].x, note[i].y - 1);
				ChangeTextColor(0);
				printf("      ");
			}
			// ��Ʈ ��ġ�� �̵��Ͽ� ����Ѵ�.
			gotoxy(note[i].x, note[i].y);
			ChangeTextColor(136);
			printf("����");
			// ��Ʈ�� ��ġ�� ��ĭ �Ʒ��� �����Ѵ�.
			note[i].y++;

			// ��Ʈ�� ���̰� �ν� ������ ���� �Ҹ��� ����Ѵ�.
			if (note[i].y == HEIGHT - 6) soundBeep(data[note[i].point]);

			// ���̰� HEIGHT - 4���� ũ�� �ν��ϱ� �����Ѵ�.
			if (note[i].y > HEIGHT - 4)
			{
				int ch;
				// Ű�� �����ٸ� ������ ������ ���� �����Ѵ�.
				if (kbhit()) {
					while (kbhit())
						ch = getch();
					// ���� ���� Ű�� �Է������� ������ ������Ű��,
					// ��Ʈ�� ���� �� ��ȿ���� �ʰ� �Ѵ�.
					if (ch == note[i].key_num)
					{
						score++;
						note[i].state = 0;
						gotoxy(note[i].x, note[i].y - 1);
						ChangeTextColor(0);
						printf("      ");
					}
				}
				// ���ؼ� �Ʒ����� ���޽� �����
				// ��Ʈ�� ���� �� ��ȿ���� �ʰ� �Ѵ�.
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
				// ���̿� �������� �ʾҴٸ�, �̸� ������ �ʵ��� �׻� ���۸� �����.
				if (kbhit()) {
					while (kbhit())
						getch();
				}
			}
		}
	}
}

// �ְ����� �ҷ����� ���� �ڽ��� �ְ����̸� ����
int GET_HighScore() {
	// chTemp	  : ���ϸ� ����
	// HIGH_score : �ְ��� ����
	char chTemp[100];
	int HIGH_score;

	// ���ϸ� ���� (list_index�� ���� �����͸� �ҷ��� �� �����ȴ�.)
	sprintf(chTemp, "results\\%s_SCORE.txt", play_list[list_index]);

	// �б���� ������ ��� HIGH_score�� ����
	FILE *INP = fopen(chTemp, "r");
	fscanf(INP, "%d", &HIGH_score);
	fclose(INP);

	// ���� HIGH_score�� score���� ũ�� ���� ���� �����.
	if (score > HIGH_score) {
		HIGH_score = score;
		FILE * OUTP = fopen(chTemp, "w");
		fprintf(OUTP, "%d", HIGH_score);
		fclose(OUTP);
	}

	// �������� �ְ��� ����
	return HIGH_score;
}

// ���� ���
void displayScore(int s) {
	// �� �ٿ��� 3/4 �������� ����Ѵ�.
	gotoxy(WIDTH / 4 * 3, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	ChangeTextColor(55);
	printf("Hit : %4d", s);
}

// ���ؼ� ���
void println() {
	// ���ؼ�(------)�� ����Ѵ�.
	ChangeTextColor(7);
	gotoxy(2, HEIGHT - 4);
	for (int i = 2; i < WIDTH - 2; i++)
	{
		printf("-");
	}
	// (ģ���� �ǰ����� ����� ��) ������ �� Ű�� ����Ѵ�.
	gotoxy(2, HEIGHT - 3);
	printf("   D  |  F  |  G  |  H  |  J  |  K");
}

// ��� ���
void displayResult() {
	// ȭ���� �����,
	system("cls");
	// �׵θ��� ����Ѵ�.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x += 2) {
			if (x == 0 || y == 0 || x == WIDTH - 2 || y == HEIGHT - 1)
			{
				ChangeTextColor(170);
				printf("��");
			}
			else
			{
				ChangeTextColor(0);
				printf("  ");
			}
		}
	}
	
	// �ְ����� ����Ѵ�.
	ChangeTextColor(2);
	gotoxy(WIDTH / 2 - 4, HEIGHT / 2 - 3);
	printf("�ְ���");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 - 2);
	printf("HighScore : %4d", GET_HighScore());

	// �ڽ��� ����� ����Ѵ�.
	ChangeTextColor(5);
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2);
	printf("����� ������...");
	ChangeTextColor(7);
	gotoxy(WIDTH / 2 - 6, HEIGHT / 2 + 1);
	printf("Score : %4d", score);

	Sleep(500);

	// �̸� ���۸� ���� �� Ű�� ���������� ��ٸ��� ������ ���۸� ��� �����.
	if (kbhit())
		while (kbhit())
			getch();
	while (!kbhit());
	while (kbhit())
		getch();
}

// ��� �ʱ�ȭ
void resetScore() {
	char chTemp[100];
	// �� �̸� �ϳ��� �ҷ����鼭 �ش� ���� ������ 0���� ����
	for (int index = 0; index < play_list_size; index++) {
		sprintf(chTemp, "results\\%s_SCORE.txt", play_list[index]);
		FILE * OUTP = fopen(chTemp, "w");
		fprintf(OUTP, "0");
		fclose(OUTP);
	}
}
