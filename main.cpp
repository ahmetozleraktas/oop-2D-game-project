#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <math.h>
using namespace std;

const float pi = 3.1415927f;

// Coordinate class
class Coord {
public:
	float x;
	float y;

public:
	// default constructor
	Coord() {
		x = 0;
		y = 0;
	}

	// non-default constructor
	Coord(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

// Object base class
class Object {
private:
	sf::RenderWindow* window;
	sf::Texture texture;
	sf::Sprite sprite;
	Coord pos;

public:
	// initializes the object
	void init(sf::RenderWindow* window, string texturePath, Coord pos) {
		this->window = window;

		// load the object texture
		if (!texturePath.empty()) {
			texture.loadFromFile(texturePath);
			texture.setSmooth(true);
			sprite.setTexture(texture);
		}

		if (sprite.getTexture()) {
			// define the center point for the object
			float x = sprite.getTexture()->getSize().x * 0.5f;
			float y = sprite.getTexture()->getSize().y * 0.5f;

			// for sandbags and barrels, the center is higher
			if (texturePath == "bags.png") y = sprite.getTexture()->getSize().y * 0.4f;
			if (texturePath == "barrel.png") y = sprite.getTexture()->getSize().y * 0.3f;

			sprite.setOrigin(x, y);
		}

		this->pos = pos;
	}

	// changes the sprite's texture
	void setTexture(const sf::Texture& texture) {
		sprite.setTexture(texture);
	}

	// returns the sprite's texture
	const sf::Texture* getTexture() {
		return sprite.getTexture();
	}

	// changes the object's position
	void setPosition(float x, float y) {
		pos.x = x;
		pos.y = y;
	}

	// returns the object's position
	Coord getPosition() {
		return pos;
	}

	// moves the object by a given offset
	void move(float offsetX, float offsetY) {
		pos.x += offsetX;
		pos.y += offsetY;
	}

	// changes the object's angle
	void setAngle(float radians) {
		// convert radians to degrees
		float degrees = radians / pi * 180;
		sprite.setRotation(degrees);
	}

	// draws the object's sprite
	void paint() {
		sprite.setPosition(pos.x, pos.y);
		window->draw(sprite);
	}

	// checks whether object collides with another object
	bool collideObject(Object& other) {
		// distance-based check
		float dx = pos.x - other.pos.x;
		float dy = pos.y - other.pos.y;
		float distance = sqrt(dx * dx + dy * dy);
		return distance < 30;
	}

	// checks whether the object is inside the window
	bool insideWindow(float border) {
		if (pos.x < border || pos.x > 1024 - border)
			return false;
		if (pos.y < border || pos.y > 768 - border)
			return false;
		return true;
	}
};

// Sandbag class inherits from base Object class
class Sandbag : public Object {
};

// Barrel class inherits from base Object class
class Barrel : public Object {
private:
	bool isVisible;

public:
	// constructor for the Barrel class
	Barrel() {
		isVisible = true;
	}

	// returns isVisible attribute of barrel
	bool getVisible() {
		return isVisible;
	}

	// hides or shows the barrel
	void setVisible(bool visible) {
		isVisible = visible;
	}
};

// Player class inherits from base Object class
class Player : public Object {
private:
	sf::Texture textures[14];
	int score;
	int bulletState; // state of the player when the bullet was fired

public:
	enum WalkDirection { Left, Up, Right, Down };

	// initializes the player
	void init(sf::RenderWindow* window, Coord pos) {

		// load textures
		textures[0].loadFromFile("soldier0.png");
		textures[1].loadFromFile("soldier1.png");
		textures[2].loadFromFile("soldier2.png");
		textures[3].loadFromFile("soldier3.png");
		textures[4].loadFromFile("soldier4.png");
		textures[5].loadFromFile("soldier5.png");
		textures[6].loadFromFile("soldier6.png");
		textures[7].loadFromFile("soldier7.png");
		textures[8].loadFromFile("soldier8.png");
		textures[9].loadFromFile("soldier9.png");
		textures[10].loadFromFile("soldier10.png");
		textures[11].loadFromFile("soldier11.png");
		textures[12].loadFromFile("soldier12.png");
		textures[13].loadFromFile("soldier13.png");
		setTexture(textures[0]);

		// initialize base Object class
		Object::init(window, string(), pos);

		score = 0;
		bulletState = 1;
	}

	// checks whether player collides with one of the other objects
	bool checkCollision(Barrel* barrels, Sandbag* sandbags, int nb, int ns) {

		// collide the player with sandbags
		for (int i = 0; i < ns; i++)
			if (collideObject(sandbags[i]))
				return true;

		// collide the player with visible barrels
		for (int i = 0; i < nb; i++)
			if (barrels[i].getVisible() && collideObject(barrels[i]))
				return true;

		return false;
	}

	// sets the current score of the player
	void setScore(int score) {
		this->score = score;
	}

	// increments score of the player
	void incrementScore() {
		score++;
	}

	// returns the current score of the player
	int getScore() {
		return score;
	}

	// returns the state of the player when the bullet was fired
	int getBulletState() {
		return bulletState;
	}

	// returns a random number between a and b
	float random(float a, float b) {
		float r = (rand() % 1000) / 1000.0f;
		return a + (b - a) * r;
	}

	// respawns the object at random location
	void respawn() {
		const float border = 50;
		float x = random(border, 1024 - border);
		float y = random(border, 768 - border);
		setPosition(x, y);
	}

	// soldier walk function for 4 directions. Last texture checking in for loop, next state decided according to that.
	void walk(float speed, WalkDirection dir) {
		if (dir == Up) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
						setTexture(textures[8]);
						break;
					}if (i == 1) {
						setTexture(textures[0]);
						break;
					}if (i == 2) {
						setTexture(textures[1]);
						break;
					}if (i == 3) {
						setTexture(textures[2]);
						break;
					}if (i == 4) {
						setTexture(textures[5]);
						break;
					}if (i == 5) {
						setTexture(textures[6]);
						break;
					}if (i == 6) {
						setTexture(textures[0]);
						break;
					}if (i == 7) {
						setTexture(textures[8]);
						break;
					}if (i == 8) {
						setTexture(textures[7]);
						break;
					}if (i == 9) {
						setTexture(textures[2]);
						break;
					}if (i == 10) {
						setTexture(textures[1]);
						break;
					}if (i == 11) {
						setTexture(textures[5]);
						break;
					}if (i == 12) {
						setTexture(textures[7]);
						break;
					}if (i == 13) {
						setTexture(textures[12]);
						break;
					}
				}
			}

			move(0, -speed);

		}
		else if (dir == Down) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
						setTexture(textures[1]);
						break;
					}if (i == 1) {
						setTexture(textures[2]);
						break;
					}if (i == 2) {
						setTexture(textures[3]);
						break;
					}if (i == 3) {
						setTexture(textures[11]);
						break;
					}if (i == 4) {
						setTexture(textures[3]);
						break;
					}if (i == 5) {
						setTexture(textures[4]);
						break;
					}if (i == 6) {
						setTexture(textures[5]);
						break;
					}if (i == 7) {
						setTexture(textures[6]);
						break;
					}if (i == 8) {
						setTexture(textures[7]);
						break;
					}if (i == 9) {
						setTexture(textures[3]);
						break;
					}if (i == 10) {
						setTexture(textures[9]);
						break;
					}if (i == 11) {
						setTexture(textures[3]);
						break;
					}if (i == 12) {
						setTexture(textures[13]);
						break;
					}if (i == 13) {
						setTexture(textures[5]);
						break;
					}
				}
			}
			move(0, speed);
		}
		else if (dir == Left) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
						setTexture(textures[7]);
						break;
					}if (i == 1) {
						setTexture(textures[0]);
						break;
					}if (i == 2) {
						setTexture(textures[1]);
						break;
					}if (i == 3) {
						setTexture(textures[4]);
						break;
					}if (i == 4) {
						setTexture(textures[5]);
						break;
					}if (i == 5) {
						setTexture(textures[6]);
						break;
					}if (i == 6) {
						setTexture(textures[13]);
						break;
					}if (i == 7) {
						setTexture(textures[6]);
						break;
					}if (i == 8) {
						setTexture(textures[7]);
						break;
					}if (i == 9) {
						setTexture(textures[10]);
						break;
					}if (i == 10) {
						setTexture(textures[7]);
						break;
					}if (i == 11) {
						setTexture(textures[7]);
						break;
					}if (i == 12) {
						setTexture(textures[13]);
						break;
					}if (i == 13) {
						setTexture(textures[12]);
						break;
					}
				}
			}
			move(-speed, 0);
		}
		else if (dir == Right) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
						setTexture(textures[1]);
						break;
					}if (i == 1) {
						setTexture(textures[2]);
						break;
					}if (i == 2) {
						setTexture(textures[10]);
						break;
					}if (i == 3) {
						setTexture(textures[2]);
						break;
					}if (i == 4) {
						setTexture(textures[3]);
						break;
					}if (i == 5) {
						setTexture(textures[6]);
						break;
					}if (i == 6) {
						setTexture(textures[7]);
						break;
					}if (i == 7) {
						setTexture(textures[8]);
						break;
					}if (i == 8) {
						setTexture(textures[0]);
						break;
					}if (i == 9) {
						setTexture(textures[2]);
						break;
					}if (i == 10) {
						setTexture(textures[2]);
						break;
					}if (i == 11) {
						setTexture(textures[4]);
						break;
					}if (i == 12) {
						setTexture(textures[6]);
						break;
					}if (i == 13) {
						setTexture(textures[2]);
						break;
					}
				}
			}
			move(speed, 0);
		}

		bulletState = 0;
		if (getTexture() == &textures[0] || getTexture() == &textures[1] || getTexture() == &textures[7] || getTexture() == &textures[8])
			bulletState = 1;
		if (getTexture() == &textures[6] || getTexture() == &textures[12] || getTexture() == &textures[13])
			bulletState = 2;
		if (getTexture() == &textures[3] || getTexture() == &textures[4] || getTexture() == &textures[5] || getTexture() == &textures[11])
			bulletState = 3;
	}

	// checks which key released
	void checkKeyrelease(WalkDirection dir) {
		if (dir == Up) {
			// we can make this code for only soldier 7 and 8. Because soldier7 and soldier8 are available while soldier walks upwards.
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
						break;
					}if (i == 1) {
					}if (i == 2) {
					}if (i == 3) {
					}if (i == 4) {
					}if (i == 5) {
					}if (i == 6) {
					}if (i == 7) {
						setTexture(textures[0]);
					}if (i == 8) {
						setTexture(textures[0]);
					}if (i == 9) {
					}if (i == 10) {
					}if (i == 11) {
					}if (i == 12) {
					}if (i == 13) {
					}
				}
			}
		}
		if (dir == Down) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
					}
					if (i == 1) {
					}
					if (i == 2) {
					}
					if (i == 3) {
						setTexture(textures[4]);
					}
					if (i == 4) {
						break;
					}
					if (i == 5) {
					}
					if (i == 6) {
					}
					if (i == 7) {
					}
					if (i == 8) {
					}
					if (i == 9) {
					}
					if (i == 10) {
					}
					if (i == 11) {
						setTexture(textures[4]);
					}
					if (i == 12) {
					}
					if (i == 13) {
					}
				}
			}
		}
		if (dir == Left) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
					}
					if (i == 1) {
					}
					if (i == 2) {
					}
					if (i == 3) {
					}
					if (i == 4) {
					}
					if (i == 5) {
					}
					if (i == 6) {
						break;
					}
					if (i == 7) {
					}
					if (i == 8) {
					}
					if (i == 9) {
					}
					if (i == 10) {
					}
					if (i == 11) {
					}
					if (i == 12) {
						setTexture(textures[6]);
					}
					if (i == 13) {
						setTexture(textures[6]);
					}
				}
			}
		}
		if (dir == Right) {
			for (int i = 0; i < 14; i++) {
				if (getTexture() == &textures[i]) {
					if (i == 0) {
					}
					if (i == 1) {
					}
					if (i == 2) {
						break;
					}
					if (i == 3) {
					}
					if (i == 4) {
					}
					if (i == 5) {
					}
					if (i == 6) {
					}
					if (i == 7) {
					}
					if (i == 8) {
					}
					if (i == 9) {
						setTexture(textures[2]);
					}
					if (i == 10) {
						setTexture(textures[2]);
					}
					if (i == 11) {
					}
					if (i == 12) {
					}
					if (i == 13) {
					}
				}
			}
		}
	}
};

// Bullet class inherits from base Object class
class Bullet : public Object {
private:
	float speed;
	float angle;
	Bullet* next;

public:
	// moves the bullet in its travel direction (angle)
	void move() {
		float offsetX = +speed * cos(angle);
		float offsetY = -speed * sin(angle);
		Object::move(offsetX, offsetY);
	}

	// sets the pointer to next bullet in a linked list
	void setNext(Bullet* next) {
		this->next = next;
	}

	// returns the pointer to next bullet in a linked list
	Bullet* getNext() {
		return next;
	}

	// sets the bullet speed
	void setSpeed(float speed) {
		this->speed = speed;
	}

	// sets the bullet angle
	void setAngle(float radians) {
		this->angle = radians;
		Object::setAngle(radians + pi / 2);
	}
};

// Bullet list class
class BulletList {
private:
	sf::RenderWindow* window;
	Bullet* list;

public:
	// constructor for the BulletList class
	BulletList(sf::RenderWindow* window) {
		this->window = window;
		list = nullptr;
	}

	// destructor for the BulletList class
	~BulletList() {
		// delete all bullets in the linked list
		while (list) {
			list = remove(nullptr, list);
		}
	}

	// adds a new bullet to the list
	void add(Coord pos, int state) {

		// create a new bullet
		Bullet* bullet = new Bullet;
		bullet->init(window, "bullet.png", pos);
		bullet->setSpeed(40);
		bullet->setAngle(state * pi / 2);

		// set the new bullet at the start of the list
		bullet->setNext(list);
		list = bullet;
	}

	// removes the bullet from the list
	Bullet* remove(Bullet* prev, Bullet* bullet) {
		// next bullet in the list after the one being deleted
		Bullet* next = bullet->getNext();

		// update the next pointer of the previous bullet in the list (or list pointer if deleting the first bullet)
		if (prev)
			prev->setNext(next);
		else list = next;

		// delete the bullet and return the pointer to the next one in the list
		delete bullet;
		return next;
	}

	// moves every bullet in the list
	void update() {
		// move each bullet
		for (Bullet* bullet = list; bullet; bullet = bullet->getNext()) {
			bullet->move();
		}
	}

	// checks whether a bullet in the list collided with other objects or with the edge of the screen
	void checkCollision(Player* players, Barrel* barrels, Sandbag* sandbags, int np, int nb, int ns) {

		// collide bullets with the edge of the screen
		Bullet* prev = nullptr;
		for (Bullet* bullet = list; bullet; ) {
			if (!bullet->insideWindow(0)) {
				// delete the bullet
				bullet = remove(prev, bullet);
			}
			else {
				// go to the next bullet
				prev = bullet;
				bullet = bullet->getNext();
			}
		}

		// collide bullets with players
		for (int i = 0; i < np; i++) {
			Bullet* prev = nullptr;
			for (Bullet* bullet = list; bullet; ) {
				if (bullet->collideObject(players[i])) {
					// delete the bullet, respawn the player, and increment the opposite score
					bullet = remove(prev, bullet);
					players[i].respawn();
					players[1 - i].incrementScore();
				}
				else {
					// go to the next bullet
					prev = bullet;
					bullet = bullet->getNext();
				}
			}
		}

		// collide bullets with sandbags
		for (int i = 0; i < ns; i++) {
			Bullet* prev = nullptr;
			for (Bullet* bullet = list; bullet; ) {
				if (bullet->collideObject(sandbags[i])) {
					// delete the bullet
					bullet = remove(prev, bullet);
				}
				else {
					// go to the next bullet
					prev = bullet;
					bullet = bullet->getNext();
				}
			}
		}

		// collide bullets with visible barrels
		for (int i = 0; i < nb; i++) {
			// ignore hidden barrels
			if (!barrels[i].getVisible())
				continue;

			Bullet* prev = nullptr;
			for (Bullet* bullet = list; bullet; ) {
				if (bullet->collideObject(barrels[i])) {
					// delete the bullet and hide the barrel
					bullet = remove(prev, bullet);
					barrels[i].setVisible(false);
				}
				else {
					// go to the next bullet
					prev = bullet;
					bullet = bullet->getNext();
				}
			}
		}
	}

	void paint() {
		// paint each bullet in the list
		for (Bullet* bullet = list; bullet; bullet = bullet->getNext()) {
			bullet->paint();
		}
	}
};

class Game {
private:
	float speed;
	int numBarrels;
	int numSandbags;
	int numPlayers;
	int width;
	int height;
	sf::RenderWindow* window;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	Barrel* barrels;
	Sandbag* sandbags;
	Player* players;
	sf::Keyboard::Key* stickyKeys; // current sticky keys for each player
	BulletList* bullets;
	sf::Text text;
	sf::Font font;

public:
	// constructor for the Game class
	Game(float speed, int w, int h, int nb, int ns, int np) {
		this->speed = speed;
		numBarrels = nb;
		numSandbags = ns;
		numPlayers = np;
		width = w;
		height = h;

		// create window
		window = new sf::RenderWindow;
		window->create(sf::VideoMode(width, height), "My game");

		// load background image and enable repeating
		bgTexture.loadFromFile("grass.png");
		bgTexture.setSmooth(true);
		bgTexture.setRepeated(true);
		bgSprite.setTexture(bgTexture);
		bgSprite.setTextureRect(sf::IntRect(0, 0, w, h));

		// create game objects
		barrels = new Barrel[nb];
		sandbags = new Sandbag[ns];
		players = new Player[np];
		stickyKeys = new sf::Keyboard::Key[np];
		bullets = new BulletList(window);

		// initialize game objects
		players[0].init(window, Coord(440, 650));
		players[1].init(window, Coord(200, 250));
		stickyKeys[0] = sf::Keyboard::Unknown;
		stickyKeys[1] = sf::Keyboard::Unknown;
		barrels[0].init(window, "barrel.png", Coord(950, 200));
		barrels[1].init(window, "barrel.png", Coord(545, 400));
		barrels[2].init(window, "barrel.png", Coord(800, 322));
		barrels[3].init(window, "barrel.png", Coord(435, 500));
		barrels[4].init(window, "barrel.png", Coord(100, 100));
		sandbags[0].init(window, "bags.png", Coord(747, 140));
		sandbags[1].init(window, "bags.png", Coord(268, 50));
		sandbags[2].init(window, "bags.png", Coord(375, 110));
		sandbags[3].init(window, "bags.png", Coord(60, 680));
		sandbags[4].init(window, "bags.png", Coord(60, 460));

		// load font
		font.loadFromFile("font.ttf");
		text.setFont(font);
	}

	// destructor for the Game class
	~Game()
	{
		// delete pointers for prevent memory leaks
		delete window;
		delete[] barrels;
		delete[] sandbags;
		delete[] players;
		delete[] stickyKeys;
		delete bullets;
	}

	// draws game background
	void drawBackground() {
		window->draw(bgSprite);
	};

	// draws all objects and updates screen
	void update() {
		window->display();
	}

	// returns true if the window is still open
	bool isOpen() {
		return window->isOpen();
	}

	// returns true if the game is over
	bool gameOver() {
		// when a player reaches 10 total shots, game is over
		for (int i = 0; i < numPlayers; i++)
			if (players[i].getScore() >= 10)
				return true;
		return false;
	}

	// processes game events
	void processEvents() {
		sf::Event event;
		sf::Color color;

		// all events checking in here
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close(); // option for closing the game
			}

			// for walking, pressed keys should stick until released
			if (!gameOver())
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
					switch (event.key.code) {
					case sf::Keyboard::Left:
					case sf::Keyboard::Up:
					case sf::Keyboard::Right:
					case sf::Keyboard::Down:
						if (event.type == sf::Event::KeyPressed)
							stickyKeys[0] = event.key.code;         // stick the key for player 1
						else stickyKeys[0] = sf::Keyboard::Unknown; // unstick the key for player 1
						break;

					case sf::Keyboard::A:
					case sf::Keyboard::W:
					case sf::Keyboard::D:
					case sf::Keyboard::S:
						if (event.type == sf::Event::KeyPressed)
							stickyKeys[1] = event.key.code;         // stick the key for player 2
						else stickyKeys[1] = sf::Keyboard::Unknown; // unstick the key for player 2
						break;
					}

			// other pressed keys
			if (event.type == sf::Event::KeyPressed)
				switch (event.key.code) {
				case sf::Keyboard::Enter:
					// fire bullet by player 1
					if (!gameOver())
						bullets->add(players[0].getPosition(), players[0].getBulletState());
					break;

				case sf::Keyboard::Space:
					// fire bullet by player 2
					if (!gameOver())
						bullets->add(players[1].getPosition(), players[1].getBulletState());
					break;

				case sf::Keyboard::Y:
					// restart the game
					if (gameOver()) {
						for (int i = 0; i < numBarrels; i++)
							barrels[i].setVisible(true);
						for (int i = 0; i < numPlayers; i++)
							players[i].setScore(0);
					}
					break;

				case sf::Keyboard::N:
					// exit the game
					if (gameOver())
						window->close();
					break;
				}
		}

		// walk function for player 1
		Coord prevPos = players[0].getPosition();
		if (stickyKeys[0] == sf::Keyboard::Left)
			players[0].walk(speed, Player::Left);
		if (stickyKeys[0] == sf::Keyboard::Up)
			players[0].walk(speed, Player::Up);
		if (stickyKeys[0] == sf::Keyboard::Right)
			players[0].walk(speed, Player::Right);
		if (stickyKeys[0] == sf::Keyboard::Down)
			players[0].walk(speed, Player::Down);

		// on collision with the edge of the screen, restore the previous position of player 1
		if (!players[0].insideWindow(50))
			players[0].setPosition(prevPos.x, prevPos.y);

		// on collision with sandbags or barrels, restore the previous position of player 1
		if (players[0].checkCollision(barrels, sandbags, numBarrels, numSandbags))
			players[0].setPosition(prevPos.x, prevPos.y);

		// walk function for player 2
		prevPos = players[1].getPosition();
		if (stickyKeys[1] == sf::Keyboard::A)
			players[1].walk(speed, Player::Left);
		if (stickyKeys[1] == sf::Keyboard::W)
			players[1].walk(speed, Player::Up);
		if (stickyKeys[1] == sf::Keyboard::D)
			players[1].walk(speed, Player::Right);
		if (stickyKeys[1] == sf::Keyboard::S)
			players[1].walk(speed, Player::Down);

		// on collision with the edge of the screen, restore the previous position of player 2
		if (!players[1].insideWindow(50))
			players[1].setPosition(prevPos.x, prevPos.y);

		// on collision with sandbags or barrels, restore the previous position of player 2
		if (players[1].checkCollision(barrels, sandbags, numBarrels, numSandbags))
			players[1].setPosition(prevPos.x, prevPos.y);

		// move every bullet in the list
		bullets->update();

		// collisions of bullets with other objects
		bullets->checkCollision(players, barrels, sandbags, numPlayers, numBarrels, numSandbags);

		// draw grass background
		window->clear(color.Black);
		drawBackground();

		// draw game objects
		for (int i = 0; i < numBarrels; i++)
			if (barrels[i].getVisible())
				barrels[i].paint();
		for (int i = 0; i < numSandbags; i++)
			sandbags[i].paint();
		for (int i = 0; i < numPlayers; i++)
			players[i].paint();
		bullets->paint();

		if (!gameOver()) {
			// display the scoreboard at the bottom of the screen at the center
			ostringstream stream;
			stream << "Player  1: " << players[0].getScore() << endl;
			stream << "Player 2: " << players[1].getScore();
			text.setString(stream.str());
			text.setPosition(width * 0.4f, height * 0.9f);
			window->draw(text);
		}
		else {
			// display the winning message
			ostringstream stream;
			stream << "Player ";
			stream << (players[0].getScore() > players[1].getScore() ? 1 : 2);
			stream << " wins, start over? (Y/N)";
			text.setString(stream.str());
			text.setPosition(width * 0.2f, height * 0.9f);
			window->draw(text);
		}
	}
};

int main()
{
	Game game_obj(10, 1024, 768, 5, 5, 2);

	// game loop
	while (game_obj.isOpen()) {
		// process game events
		game_obj.processEvents();

		// draw all objects and update screen
		game_obj.update();

		// sleep code limits the speed
		sf::sleep(sf::seconds(0.1f));
	}

	return 0;
}