#include<ctime>
#include<forward_list>

#include"header/resource.hpp"
#include"header/game.hpp"


//自机
player reimu(MapWidth >> 1, MapHeight * 0.8);
//自机弹幕
std::forward_list<playerBullet>playerBulletList;
//敌机
std::forward_list<enemyBullet>enemyBulletList;
//敌机弹幕
std::forward_list<enemy>enemyList;

//更新列表内对象，并删除非活跃对象
template<typename T>
void listUpdate(std::forward_list<T> &lst){
	static_assert(T::getState);
	static_assert(T::update);
	for (auto i = lst.before_begin();i._M_next() != lst.end();){
		if (!(*i._M_next()).getState())lst.erase_after(i);
		else (*++i).update();
	}
}

void drawBackground();
void drawGUI();
void enemyAppear();
void playerShot();
//计算数据、更新画面
void runGame();

void Setup(){
	initWindow("", DEFAULT, DEFAULT, 640, 480);
	initConsole();

	loadResource();

	srand(time(nullptr));

	//win32计时器误差, 15ms=60+fps
	startTimer(0, 15, runGame);
}

void runGame(){
	//更新自机
	reimu.update();

	//更新自机弹幕
	listUpdate(playerBulletList);
	if (ishold(0x5a))playerShot();

	//更新敌机
	listUpdate(enemyList);
	enemyAppear();

	//更新敌机弹幕
	listUpdate(enemyBulletList);


	beginPaint();

	//重绘
	drawBackground();
	drawGUI();

	for (auto i : playerBulletList)i.draw();
	reimu.draw();
	for (auto i : enemyList)i.draw();
	for (auto i : enemyBulletList)i.draw();

	endPaint();
}

void enemyAppear(){
	//TODO 重新设计关卡
	static int framecount = 0;
	switch (++framecount){
		case 60:;case 65:;case 70:;case 75:;case 80:;
			enemyList.emplace_front((MapWidth >> 2) + rand() % 41 - 20, 0, (MapWidth >> 2) + rand() % 41 - 20, MapHeight >> 1, 120);
			break;
		case 120:;case 125:;case 130:;case 135:;case 140:;
			enemyList.emplace_front(MapWidth - (MapWidth >> 2) + rand() % 41 - 20, 0, MapWidth - (MapWidth >> 2) + rand() % 41 - 20, MapHeight >> 1, 120);
			break;
	}
}

void playerShot(){
	static int CD = 5;
	if (++CD == 6){
		CD = 0;
		playerBulletList.emplace_front(reimu.getX(), reimu.getY() - 10, 0, -15);
	}
}

void drawBackground(){
	//TODO 更换图片
	setBrushColor(COLOR::White);
	rectangle(0, 0, MapWidth, MapHeight);
}

void drawGUI(){
	//TODO 显示得分/生命/...
	setPenColor(COLOR::Black);
	setPenWidth(2);
	line(MapWidth, 0, MapWidth, WinHeight);
	setBrushColor(COLOR::White);
	rectangle(MapWidth, 0, WinWidth - MapWidth, WinHeight);
}
