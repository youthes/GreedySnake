#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<time.h>

using namespace sf;

//定义宏，蛇的最大长度定义为100
#define SNAKE_LEN_MAX 100

const int WIDTH_BLOCK_COUNT = 20;
const int HEIGHT_BLOCK_COUNT = 12;
const int BlockSize = 64;
int GameWidth = BlockSize * WIDTH_BLOCK_COUNT;
int GameHeight = BlockSize * HEIGHT_BLOCK_COUNT;

//方向，枚举类型
enum {
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
};
int dir;

//结构，第几行第几列，蛇的身体，身体每一个部位的坐标
struct Position {
	int x;
	int y;
};

struct Position snake[SNAKE_LEN_MAX];	//数组，蛇的最大长度
int num = 4;	//蛇的最开始长度为4
struct Position fruit = { WIDTH_BLOCK_COUNT / 2,HEIGHT_BLOCK_COUNT / 2 };	//毒苹果最开始在屏幕中间

Sound eat;	//吃的声音

//蛇跑的函数
void snakeGo() {
	//除蛇头snake[0]之外，都向前移动一个位置，玩家控制蛇头，后面的每一块位置都移动到其前一块的位置
	for (int i = num - 1; i > 0; i--) {	//0是蛇头，num-1是蛇尾
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	//更新蛇头位置
	if (dir == DIR_DOWN) snake[0].y += 1;
	if (dir == DIR_UP) snake[0].y -= 1;
	if (dir == DIR_LEFT) snake[0].x -= 1;
	if (dir == DIR_RIGHT) snake[0].x += 1;

	//处理撞墙
	if (snake[0].x >= WIDTH_BLOCK_COUNT) snake[0].x = 0;
	if (snake[0].x < 0) snake[0].x = WIDTH_BLOCK_COUNT;
	if (snake[0].y > HEIGHT_BLOCK_COUNT) snake[0].y = 0;
	if (snake[0].y < 0) snake[0].y = HEIGHT_BLOCK_COUNT;

	//吃苹果处理
	if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
		num++;

		//生成新水果,水果的随机坐标.需要进一步优化，避免苹果出现在蛇的身体上
		fruit.x = rand() % WIDTH_BLOCK_COUNT;	//0-19
		fruit.y = rand() % HEIGHT_BLOCK_COUNT;	//0-11

		//蛇的身体要加一个单位
		snake[num - 1].x = snake[num - 2].x;
		snake[num - 1].y = snake[num - 2].y;

		//播放一个吃苹果的音效
		eat.play();
	}

	//蛇头碰到自己的身体！
	//处理：把剩余部分截断
	for (int i = 1; i < num; i++) {
		if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
			num = i;
			break;
		}
	}


}

int main(void) {
	srand(time(0));	//先设置种子

	//创建游戏窗口
	RenderWindow window(VideoMode(GameWidth, GameHeight), "Rock_Snack");

	float delay = 0.2;	//蛇0.2s动一次
	Clock clock;	//计时器
	float timer = 0;

	//加载纹理W
	Texture t1, t2;
	t1.loadFromFile("images/apple.png");
	t2.loadFromFile("images/snake.png");

	Sprite spriteApple(t1);
	Sprite spriteSnake(t2);

	SoundBuffer tmp;	//声音缓存
	tmp.loadFromFile("images/13358.wav");	//加载声音缓存
	eat.setBuffer(tmp);	//设置声音

	while (window.isOpen()) {
		float tmp = clock.getElapsedTime().asSeconds();	//从上次启动到现在有多少秒
		timer += tmp;
		clock.restart();
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
		}

		//方向处理
		if (Keyboard::isKeyPressed(Keyboard::Left)) dir = DIR_LEFT;
		if (Keyboard::isKeyPressed(Keyboard::Right)) dir = DIR_RIGHT;
		if (Keyboard::isKeyPressed(Keyboard::Up)) dir = DIR_UP;
		if (Keyboard::isKeyPressed(Keyboard::Down)) dir = DIR_DOWN;

		//如果时间到了，就让蛇动起来
		if (timer > delay) {
			snakeGo();
			timer = 0;
		}

		//画面的渲染
		window.clear(Color::White);	//用白色清空画面

		//画蛇
		for (int i = 0; i < num; i++) {
			spriteSnake.setPosition(snake[i].x * BlockSize, snake[i].y * BlockSize);
			window.draw(spriteSnake);
		}

		//画毒苹果
		spriteApple.setPosition(fruit.x * BlockSize, fruit.y * BlockSize);
		window.draw(spriteApple);

		window.display();
	}


	return 0;

}