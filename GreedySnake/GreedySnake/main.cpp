#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<time.h>

using namespace sf;

//����꣬�ߵ���󳤶ȶ���Ϊ100
#define SNAKE_LEN_MAX 100

const int WIDTH_BLOCK_COUNT = 20;
const int HEIGHT_BLOCK_COUNT = 12;
const int BlockSize = 64;
int GameWidth = BlockSize * WIDTH_BLOCK_COUNT;
int GameHeight = BlockSize * HEIGHT_BLOCK_COUNT;

//����ö������
enum {
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
};
int dir;

//�ṹ���ڼ��еڼ��У��ߵ����壬����ÿһ����λ������
struct Position {
	int x;
	int y;
};

struct Position snake[SNAKE_LEN_MAX];	//���飬�ߵ���󳤶�
int num = 4;	//�ߵ��ʼ����Ϊ4
struct Position fruit = { WIDTH_BLOCK_COUNT / 2,HEIGHT_BLOCK_COUNT / 2 };	//��ƻ���ʼ����Ļ�м�

Sound eat;	//�Ե�����

//���ܵĺ���
void snakeGo() {
	//����ͷsnake[0]֮�⣬����ǰ�ƶ�һ��λ�ã���ҿ�����ͷ�������ÿһ��λ�ö��ƶ�����ǰһ���λ��
	for (int i = num - 1; i > 0; i--) {	//0����ͷ��num-1����β
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	//������ͷλ��
	if (dir == DIR_DOWN) snake[0].y += 1;
	if (dir == DIR_UP) snake[0].y -= 1;
	if (dir == DIR_LEFT) snake[0].x -= 1;
	if (dir == DIR_RIGHT) snake[0].x += 1;

	//����ײǽ
	if (snake[0].x >= WIDTH_BLOCK_COUNT) snake[0].x = 0;
	if (snake[0].x < 0) snake[0].x = WIDTH_BLOCK_COUNT;
	if (snake[0].y > HEIGHT_BLOCK_COUNT) snake[0].y = 0;
	if (snake[0].y < 0) snake[0].y = HEIGHT_BLOCK_COUNT;

	//��ƻ������
	if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
		num++;

		//������ˮ��,ˮ�����������.��Ҫ��һ���Ż�������ƻ���������ߵ�������
		fruit.x = rand() % WIDTH_BLOCK_COUNT;	//0-19
		fruit.y = rand() % HEIGHT_BLOCK_COUNT;	//0-11

		//�ߵ�����Ҫ��һ����λ
		snake[num - 1].x = snake[num - 2].x;
		snake[num - 1].y = snake[num - 2].y;

		//����һ����ƻ������Ч
		eat.play();
	}

	//��ͷ�����Լ������壡
	//������ʣ�ಿ�ֽض�
	for (int i = 1; i < num; i++) {
		if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
			num = i;
			break;
		}
	}


}

int main(void) {
	srand(time(0));	//����������

	//������Ϸ����
	RenderWindow window(VideoMode(GameWidth, GameHeight), "Rock_Snack");

	float delay = 0.2;	//��0.2s��һ��
	Clock clock;	//��ʱ��
	float timer = 0;

	//��������W
	Texture t1, t2;
	t1.loadFromFile("images/apple.png");
	t2.loadFromFile("images/snake.png");

	Sprite spriteApple(t1);
	Sprite spriteSnake(t2);

	SoundBuffer tmp;	//��������
	tmp.loadFromFile("images/13358.wav");	//������������
	eat.setBuffer(tmp);	//��������

	while (window.isOpen()) {
		float tmp = clock.getElapsedTime().asSeconds();	//���ϴ������������ж�����
		timer += tmp;
		clock.restart();
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
		}

		//������
		if (Keyboard::isKeyPressed(Keyboard::Left)) dir = DIR_LEFT;
		if (Keyboard::isKeyPressed(Keyboard::Right)) dir = DIR_RIGHT;
		if (Keyboard::isKeyPressed(Keyboard::Up)) dir = DIR_UP;
		if (Keyboard::isKeyPressed(Keyboard::Down)) dir = DIR_DOWN;

		//���ʱ�䵽�ˣ������߶�����
		if (timer > delay) {
			snakeGo();
			timer = 0;
		}

		//�������Ⱦ
		window.clear(Color::White);	//�ð�ɫ��ջ���

		//����
		for (int i = 0; i < num; i++) {
			spriteSnake.setPosition(snake[i].x * BlockSize, snake[i].y * BlockSize);
			window.draw(spriteSnake);
		}

		//����ƻ��
		spriteApple.setPosition(fruit.x * BlockSize, fruit.y * BlockSize);
		window.draw(spriteApple);

		window.display();
	}


	return 0;

}