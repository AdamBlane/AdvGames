#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdexcept>
#include <list>
#include <cmath>
#include <fstream>
 

using namespace sf;

class PowerUp 
{
	public:
		Sprite sprite;
		int effect;
};

// High score persistence
std::ofstream highScoreFileW;
std::ifstream highScoreFileR;

// Track window size
Vector2u res;

// Tracks which screen is currently active
int curScreen = 0;

// START SCREEN sprites and textures for bg and buttons
Sprite startBg, startButton, settingsButton, exitButton;
Texture startBgTex, startBtnTex, startBtnActiveTex, settingsBtnTex, settingsBtnActiveTex, exitBtnTex, exitBtnActiveTex;
std::list<Sprite> startSprites;
Text highScoreStartText;
// tracks currently highlighted menu item
int currentlyActive;
// close window on exit btn select
bool downPressed = false, upPressed = false, closeWin = false;

// SETTINGS SCREEN 
Sprite settingsBg, backBtn;
Texture settingsBgTex, backActiveTex;
std::list<Sprite> settingsSprites;
bool fullscreen = false;

// PAUSE SCREEN 
Sprite pauseBg, resumeBtn, endgameBtn;
Texture pauseBgTex, resumeBtnTex, resumeBtnActiveTex, endgameBtnTex, endgameBtnActiveTex;
std::list<Sprite> pauseSprites;
bool paused, enterPressed = false;

// GAME SCREEN 
Sprite gameBg, player;
Texture gameBgTex, playerTex, greenShotTex, powerupSpeed, powerupSpeedActive, powerupRepair, powerupRepairActive,
powerupShield, powerupShieldActive, playerShielded;
Texture enemy1Tex, enemy2Tex, enemy3Tex;
std::list<Sprite> bulletSprites;
std::list<Sprite> slowEnemySprites;
std::list<Sprite> fastEnemySprites;
std::list<Sprite> fasterEnemySprites;
std::list<PowerUp> powerUps;
Text  highScoreTxt;
int killCounter, kill10, threshold, playerLivesCount, enemyTick, highScoreCount;
float gameTimeTotal, playerSpeed;
bool playerShieldActive, controllerConnected, beamPlaying = false, pPressed = false;
SoundBuffer explosionBuffer;
Sound explosionSound;
Music bgMusic;

int controller = 0;

// Loads from file all textures for all screens, assigns to sprites
void LoadTextures()
{
	// Start screen
	if (!startBgTex.loadFromFile("res/img/background1.png"))
		throw std::invalid_argument("Error loading start button tex");
	if (!startBtnTex.loadFromFile("res/img/Startbutton.png"))
		throw std::invalid_argument("Error loading start button tex");
	if (!startBtnActiveTex.loadFromFile("res/img/StartbuttonActive.png"))
		throw std::invalid_argument("Error loading start btn active tex");
	if (!settingsBtnTex.loadFromFile("res/img/settings.png"))
		throw std::invalid_argument("Error loading menu btn tex");
	if (!settingsBtnActiveTex.loadFromFile("res/img/settingsActive.png"))
		throw std::invalid_argument("Error loading menu btn active tex");
	if (!exitBtnTex.loadFromFile("res/img/Exit.png"))
		throw std::invalid_argument("Error loading exit btn tex");
	if (!exitBtnActiveTex.loadFromFile("res/img/ExitActive.png"))
		throw std::invalid_argument("Error loading exit btn active tex");

	// Assign textures to sprites
	startBg.setTexture(startBgTex);
	startButton.setTexture(startBtnActiveTex);
	settingsButton.setTexture(settingsBtnTex);
	exitButton.setTexture(exitBtnTex);
	// Set positions
	startBg.setScale(res.x / startBg.getLocalBounds().width, res.y / startBg.getLocalBounds().height);
	startButton.setPosition(res.x / 2 - startButton.getLocalBounds().width / 2, 5);
	settingsButton.setPosition(res.x / 2 - settingsButton.getGlobalBounds().width / 2, res.y - exitButton.getLocalBounds().height * 2);
	exitButton.setPosition(res.x / 2 - exitButton.getLocalBounds().width / 2, res.y - exitButton.getLocalBounds().height / 1.15);
	// Add to list to be rendered
	startSprites.push_back(startBg);
	startSprites.push_back(startButton);
	startSprites.push_back(settingsButton);
	startSprites.push_back(exitButton);

	currentlyActive = 0;

	// SETTINGS SCREEN 
	if (!settingsBgTex.loadFromFile("res/img/background1.png"))
		throw std::invalid_argument("Error loading settings screen bg tex");
	if (!backActiveTex.loadFromFile("res/img/backActive.png"))
		throw std::invalid_argument("Error loading back active tex");

	//// Assign textures to sprites
	settingsBg.setTexture(settingsBgTex);
	settingsBg.setScale(res.x / startBg.getLocalBounds().width, res.y / startBg.getLocalBounds().height);
	backBtn.setTexture(backActiveTex);
	backBtn.setPosition(res.x - exitButton.getLocalBounds().width, res.y - exitButton.getLocalBounds().height / 1.15);
	settingsSprites.push_back(backBtn);

	//// PAUSE SCREEN
	if (!pauseBgTex.loadFromFile("res/img/background1.png"))
		throw std::invalid_argument("Error loading pause screen bg tex");
	if (!resumeBtnTex.loadFromFile("res/img/resume.png"))
		throw std::invalid_argument("Error loading resume tex");
	if (!resumeBtnActiveTex.loadFromFile("res/img/resumeActive.png"))
		throw std::invalid_argument("Error loading resume active tex");
	if (!endgameBtnTex.loadFromFile("res/img/end.png"))
		throw std::invalid_argument("Error loading endgame btn tex");
	if (!endgameBtnActiveTex.loadFromFile("res/img/endActive.png"))
		throw std::invalid_argument("Error loading end active tex");


	//// Assign textures to sprites
	pauseBg.setTexture(pauseBgTex);
	pauseBg.setScale(res.x / startBg.getLocalBounds().width, res.y / startBg.getLocalBounds().height);
	resumeBtn.setTexture(resumeBtnActiveTex);
	endgameBtn.setTexture(endgameBtnTex);
	// Set positions
	resumeBtn.setPosition(res.x / 2 - startButton.getLocalBounds().width / 2, res.y / 4);
	endgameBtn.setPosition(res.x / 2 - exitButton.getLocalBounds().width / 2, res.y - exitButton.getLocalBounds().height );
	//// Add to list
	pauseSprites.push_back(resumeBtn);
	pauseSprites.push_back(endgameBtn);
	//// GAME SCREEN
	if (!gameBgTex.loadFromFile("res/img/gameBg.png"))
		throw std::invalid_argument("Error loading game bg tex");
	if (!playerTex.loadFromFile("res/img/player.png"))
		throw std::invalid_argument("Error loading player tex");
	if (!greenShotTex.loadFromFile("res/img/greenShot.png"))
		throw std::invalid_argument("Error loading greenShot tex");
	if (!enemy1Tex.loadFromFile("res/img/A3.png"))
		throw std::invalid_argument("Error loading A3 tex");
	if (!enemy2Tex.loadFromFile("res/img/F10.png"))
		throw std::invalid_argument("Error loading F10 tex");
	if (!enemy3Tex.loadFromFile("res/img/C5.png"))
		throw std::invalid_argument("Error loading C5 tex");
	if (!powerupSpeed.loadFromFile("res/img/speedup.png"))
		throw std::invalid_argument("Error loading speedup tex");
	if (!powerupSpeedActive.loadFromFile("res/img/speedupActive.png"))
		throw std::invalid_argument("Error loading speed up active tex");


	// Assign textures to sprites
	gameBg.setTexture(gameBgTex);
	gameBg.setScale(res.x / startBg.getLocalBounds().width, res.y / startBg.getLocalBounds().height);
	player.setTexture(playerTex);
	// Player starting position
	player.move(res.x / 2 - player.getLocalBounds().width / 2, res.y - player.getLocalBounds().height * 2);

	//// AUDIO
	if (!explosionBuffer.loadFromFile("res/img/0477.wav"))
		throw std::invalid_argument("Error loading screem wav");
	//// Set to sounds
	explosionSound.setBuffer(explosionBuffer);
	explosionSound.setVolume(70);
}

// Reset counters
void StartGame()
{
	// Reset counters
	enemyTick = 0;
	kill10 = 0;
	gameTimeTotal = 0;
	threshold = 1000;
	playerLivesCount = 3;
	playerSpeed = 500.0f;
	playerShieldActive = false;
	paused = false;
	// Load high score
	highScoreFileR.open("res/highScore.txt");
	highScoreFileR >> highScoreCount;
	highScoreFileR.close();
	highScoreTxt.setString("High score: " + std::to_string(highScoreCount));
}

// Save high scores, clear lists
void EndGame()
{
	// Clear lists
	bulletSprites.clear();
	slowEnemySprites.clear();
	fastEnemySprites.clear();
	fasterEnemySprites.clear();
	powerUps.clear();
	// Save score
	if (killCounter > highScoreCount)
	{
		// Open file for writing
		highScoreFileW.open("res/highScore.txt");
		highScoreFileW << std::to_string(killCounter);
		highScoreFileW.close();
	}

	// For use in start screen
	currentlyActive = 0;
}

// Start screen - resets sprite textures to show currently active button
void SetTextures(int index)
{
	// Reset menu button textures based on curScreen
	switch (curScreen)
	{
		// START SCREEN
	case 0:
	{
		// Reset all button textures
		startButton.setTexture(startBtnTex);
		settingsButton.setTexture(settingsBtnTex);
		exitButton.setTexture(exitBtnTex);
		// Set current to active
		switch (index)
		{
		case 0: // Start button selected
			startButton.setTexture(startBtnActiveTex);
			break;
		case 1: // Menu button selected
			settingsButton.setTexture(settingsBtnActiveTex);
			break;
		case 2: // Exit button selected
			exitButton.setTexture(exitBtnActiveTex);
			break;
		default: break;
		}

		// Clear start sprites collection
		startSprites.clear();
		// Repopulate
		startSprites.push_back(startBg);
		startSprites.push_back(startButton);
		startSprites.push_back(settingsButton);
		startSprites.push_back(exitButton);

		break;
	}

	// PAUSE SCREEN
	case 3:
	{
		// Reset all button textures
		resumeBtn.setTexture(resumeBtnTex);
		endgameBtn.setTexture(endgameBtnTex);
		switch (index)
		{
			// Resume game
		case 0:
			resumeBtn.setTexture(resumeBtnActiveTex);
			break;
			// End game - show start screen
		case 1:
			endgameBtn.setTexture(endgameBtnActiveTex);
			break;
		default: break;
		}

		// Clear start sprites collection
		pauseSprites.clear();
		// Repopulate
		pauseSprites.push_back(resumeBtn);
		pauseSprites.push_back(endgameBtn);

		break;
	}

	default: break;
	}





}

// Random chance of spawning a powerup, triggered every kill
void PowerupChance()
{
	// 1 in 3 chance of powerup on kill
	int chance = rand() % 20 + 1;
	switch (chance)
	{
		// Speedy 
	case 5:
	{
		// Create a new powerup
		PowerUp powerup;
		// Its sprite
		Sprite powerupSprite;
		// Set speedy effect
		powerup.effect = 1;
		// Set the sprite's texture
		powerupSprite.setTexture(powerupSpeed);
		// Assign to powerup
		powerup.sprite = powerupSprite;
		// Randomise start posision, set 
		int startPos = rand() % res.y + 0;
		float toMove = (float)startPos;
		powerup.sprite.move(toMove, -100.0f);
		// Add to list to be rendered
		powerUps.push_back(powerup);
	}
	break;
	default: break;

	}
}

// Applies effect of powerup, resets effect to default for next frame of collision
void PoweredUp(int effect)
{

	switch (effect)
	{
	case 1:
	{
		// Increase player speed
		playerSpeed += 100.0f;
	}
	default: break;
	}

}

// Squares stuff
int Squared(int num)
{
	return (num * num);
}

// Determines if two sprites are colliding
bool ColCheck(Sprite s1, Sprite s2)
{
	int distance;
	distance = abs(sqrt(Squared(s1.getPosition().x - s2.getPosition().x) + Squared(s1.getPosition().y - s2.getPosition().y)));
	distance -= 76;

	// Collision
	if (distance <= 0)
		return true;

	return false;
}

// Checks for collisions
void Collisions()
{
	// First tidy up player bullets
	for (std::list<Sprite>::iterator it = bulletSprites.begin(); it != bulletSprites.end(); )
	{
		// If bullets have gone off screen
		if (bulletSprites.front().getPosition().y < -10.0f)
		{
			// Remove them
			it = bulletSprites.erase(it);
		}
		else
		{
			it++;
		}
	}


	// Check for player collision with enemy ships
	for (std::list<Sprite>::iterator itE = slowEnemySprites.begin(); itE != slowEnemySprites.end(); )
	{
		bool flag = true;
		// If collision 
		if (ColCheck(player, *itE))
		{
			// Destroy enemy ship
			itE = slowEnemySprites.erase(itE);
			// If shield is active, lose shield
			if (playerShieldActive)
			{
				playerShieldActive = false;
				player.setTexture(playerTex);
			}
			else {
				// Otherwise lose a life
				playerLivesCount--;
			}

		}
		// If no collision
		else
		{
			// Check if off screen
			Sprite temp = *itE;
			if (flag && temp.getPosition().y > res.y)
			{
				playerLivesCount--;
				flag = false;
			}
			// Go to next item
			itE++;
		}

	}
	for (std::list<Sprite>::iterator itE = fastEnemySprites.begin(); itE != fastEnemySprites.end(); )
	{
		bool flag = true;
		// If collision
		if (ColCheck(player, *itE))
		{
			// Destroy enemy ship
			itE = fastEnemySprites.erase(itE);
			// If shield is active, lose shield
			if (playerShieldActive)
			{
				playerShieldActive = false;
				player.setTexture(playerTex);
			}
			else {
				// Otherwise lose a life
				playerLivesCount--;
			}
		}
		// If no collision
		else
		{
			// Check if off screen
			Sprite temp = *itE;
			if (flag && temp.getPosition().y > res.y)
			{
				playerLivesCount--;
				flag = false;
			}
			// Go to next item
			itE++;
		}

	}
	for (std::list<Sprite>::iterator itE = fasterEnemySprites.begin(); itE != fasterEnemySprites.end(); )
	{
		bool flag = true;
		// If collision
		if (ColCheck(player, *itE))
		{
			// Destroy enemy ship
			itE = fasterEnemySprites.erase(itE);
			playerLivesCount--;
		}
		// If no collision
		else
		{
			// Check if off screen
			Sprite temp = *itE;
			if (flag && temp.getPosition().y > res.y)
			{
				playerLivesCount--;
				flag = false;
			}
			// Go to next item
			itE++;
		}

	}
	// Check for bullet collisions with enemy ships
	for (std::list<Sprite>::iterator itB = bulletSprites.begin(); itB != bulletSprites.end(); )
	{
		for (std::list<Sprite>::iterator itE = slowEnemySprites.begin(); itE != slowEnemySprites.end(); )
		{
			if (ColCheck(*itB, *itE))
			{
				// Erase bullet and ship
				itB = bulletSprites.erase(itB);
				itE = slowEnemySprites.erase(itE);
				killCounter++;
				// Chance of powerup drop
				PowerupChance();
				// Play explosion sound effect
				explosionSound.play();
			}
			else
				itE++;
		}
		itB++;
	}
	for (std::list<Sprite>::iterator itB = bulletSprites.begin(); itB != bulletSprites.end(); )
	{
		for (std::list<Sprite>::iterator itE = fastEnemySprites.begin(); itE != fastEnemySprites.end(); )
		{
			if (ColCheck(*itB, *itE))
			{
				itB = bulletSprites.erase(itB);
				itE = fastEnemySprites.erase(itE);
				// Update kill count
				killCounter++;
				// Chance of powerup
				PowerupChance();
				// Play explosion sound effect
				explosionSound.play();
			}
			else
				itE++;
		}
		itB++;
	}
	for (std::list<Sprite>::iterator itB = bulletSprites.begin(); itB != bulletSprites.end(); )
	{
		for (std::list<Sprite>::iterator itE = fasterEnemySprites.begin(); itE != fasterEnemySprites.end(); )
		{
			if (ColCheck(*itB, *itE))
			{
				itB = bulletSprites.erase(itB);
				itE = fasterEnemySprites.erase(itE);
				// Update kill count
				killCounter++;
				// Chance of powerup
				PowerupChance();
				// Play explosion sound effect
				explosionSound.play();
			}
			else
				itE++;
		}
		itB++;
	}

	// Check for player collision with powerups
	for (PowerUp & pu : powerUps)
	{

		if (ColCheck(player, pu.sprite))
		{
			// Act on effect
			PoweredUp(pu.effect);

			// Change to active texture
			switch (pu.effect)
			{
			case 1:
				pu.sprite.setTexture(powerupSpeedActive);
				break;
			case 2:
				pu.sprite.setTexture(powerupRepairActive);
				break;
			case 3:
				pu.sprite.setTexture(powerupShieldActive);
				player.setTexture(playerShielded);
				break;
			default: break;
			}

			if (pu.effect != 0)
				// Play powerup pickup sound
			// Reset effect so as not to trigger each frame of collision
			pu.effect = 0;


		}
	}


}


// Listen for input
void Update()
{

	// Clocks
	static sf::Clock deltaClock;
	float dt = deltaClock.restart().asSeconds();

	if (Joystick::isConnected(0))
		controller = 0;
	if (Joystick::isConnected(1))
		controller = 1;
	// Act depending on current screen
	switch (curScreen)
	{
		// START SCREEN
	case 0:
	{
		// Menu nav controls
		if (!downPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Down) || Joystick::getAxisPosition(controller, Joystick::Axis::Y) > 99)
			{

				// Reached end of buttons list?
				if (currentlyActive == 2)
				{
					// Reset to go to top of buttons list
					currentlyActive = 0;
				}
				else
				{
					// Moving down list of buttons
					currentlyActive++;
				}
				// Redraw
				SetTextures(currentlyActive);

				downPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Down))
			downPressed = false;
		if (!upPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Up) || Joystick::getAxisPosition(controller, Joystick::Axis::Y) < -99)
			{
				// Reached top of buttons list?
				if (currentlyActive == 0)
				{
					// Set to max to go to bottom of list
					currentlyActive = 2;
				}
				else
				{
					// Moving up list of buttons
					currentlyActive--;
				}
				// Redraw
				SetTextures(currentlyActive);

				upPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Up))
			upPressed = false;
		// Button logic
		if (!enterPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Return) || Joystick::isButtonPressed(controller, 0))
			{
				switch (currentlyActive)
				{
				case 0:
					// Start the game
					curScreen = 2;
					StartGame();
					break;
				case 1:
					// Show settings screen
					curScreen = 1;
					currentlyActive = 0;
					SetTextures(currentlyActive);
					break;

				case 2:
					// Exit the game
					closeWin = true;
					break;
				default: break;
				}
				enterPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Return))
			enterPressed = false;
		break;
	}

	// SETTINGS SCREEN
	case 1:
	{
		// Button logic
		if (!enterPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Return) || Joystick::isButtonPressed(controller, 0))
			{

				curScreen = 0;
				currentlyActive = 0;
				SetTextures(currentlyActive);
				enterPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Return))
			enterPressed = false;

		break;
	}

	// GAME SCREEN
	case 2:
	{
		// Counts frames
		enemyTick++;
		// Reset if too high
		if (enemyTick > threshold * 2 - 10)
		{
			enemyTick = 0;
		}
		// Pause game
		if (Keyboard::isKeyPressed(Keyboard::P) || Joystick::isButtonPressed(controller, 7))
		{
			if (!paused)
			{
				// Reset currentlyActive counter for new menu screen
				currentlyActive = 0;
				paused = true;
				curScreen = 3;
				SetTextures(currentlyActive);
				upPressed = false;
				downPressed = false;
				enterPressed = false;
			}
			pPressed = true;
		}
		if (!Keyboard::isKeyPressed(Keyboard::P))
			pPressed = false;
		// Track player movement
		Vector2f playerMove;
		// Player movement
		if (Keyboard::isKeyPressed(Keyboard::W) || Joystick::getAxisPosition(controller, Joystick::Axis::Y) < -50) // Up
		{
			if (player.getPosition().y > 0)
				playerMove.y--;
		}
		if (Keyboard::isKeyPressed(Keyboard::S) || Joystick::getAxisPosition(controller, Joystick::Axis::Y) > 50) // Down
		{
			if (player.getPosition().y < res.y - playerTex.getSize().y)
				playerMove.y++;
		}
		if (Keyboard::isKeyPressed(Keyboard::A) || Joystick::getAxisPosition(controller, Joystick::Axis::X) < -50) // Left
		{
			if (player.getPosition().x > 0)
				playerMove.x--;
		}
		if (Keyboard::isKeyPressed(Keyboard::D) || Joystick::getAxisPosition(controller, Joystick::Axis::X) > 50) // Right
		{
			if (player.getPosition().x < res.x - playerTex.getSize().x)
				playerMove.x++;
		}

		// Firing
		if (Keyboard::isKeyPressed(Keyboard::Space) || Joystick::isButtonPressed(controller, 2))
		{
			// Create new sprite for bullet
			Sprite shot;
			// Set appropriate texture 
			shot.setTexture(greenShotTex);
			// Set bullet start position
			shot.move(player.getPosition() + Vector2f(50.0f, -20.0f)); // Adjust bullet offset (starts top left corner of player image)
																	   // Add to list to be moved & rendered
			bulletSprites.push_back(shot);
			if (!beamPlaying)
			{
				// Play sound
				beamPlaying = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Space))
		{
			beamPlaying = false;

		}

		// Difficulty scaling - every 10 kills increase enemy spawn rate
		if (killCounter % 10 == 0 && killCounter > kill10)
		{
			// Decrease wait time between spawns
			if (threshold > 260)
			{
				threshold -= 200;
				// Prevent reoccurence every frame at this kill count
				kill10 += 10;
			}
		}


		// Check for pause before moving/spawning anything
		if (!paused)
		{
			if (enemyTick % threshold == 0)
			{
				// Create a new enemy
				Sprite enemy;
				// Randomise x start pos, set back on y just off screen
				//srand(time(NULL));
				int startPos = rand() % res.y + 0;
				float toMove = (float)startPos;
				enemy.move(toMove, -100.0f);
				// Choose a random enemy type & add to enemy list
				switch (int r = rand() % 3 + 1)
				{
				case 1:
					enemy.setTexture(enemy1Tex);
					slowEnemySprites.push_back(enemy);
					break;
				case 2:
					enemy.setTexture(enemy2Tex);
					fastEnemySprites.push_back(enemy);
					break;
				case 3:
					enemy.setTexture(enemy3Tex);
					fasterEnemySprites.push_back(enemy);
					break;
				default:
					enemy.setTexture(enemy1Tex);
					slowEnemySprites.push_back(enemy);
				}

			}

			// Enemy movement
			for (Sprite & s : slowEnemySprites)
			{
				s.move(Vector2f(0.0f, 1.0f)*50.0f*dt);
			}
			for (Sprite & s : fastEnemySprites)
			{
				s.move(Vector2f(0.0f, 1.0f)*100.0f*dt);
			}
			for (Sprite & s : fasterEnemySprites)
			{
				s.move(Vector2f(0.0f, 1.0f)*150.0f*dt);
			}

			// Make bullets fly
			for (Sprite & s : bulletSprites)
			{
				s.move(Vector2f(0.0f, -20.0f)*300.0f*dt);
			}

			// Move player
			player.move(playerMove*playerSpeed*dt);

			//Move powerups
			for (PowerUp & pu : powerUps)
			{
				pu.sprite.move(Vector2f(0.0f, 1.0f)*300.0f*dt);
			}

			// Check for collisions
			Collisions();

			// Check for player death	
			if (playerLivesCount < 0)
			{
				// Game over, return to start screen
				curScreen = 0;
				// Tidy up game resources
				EndGame();
			}
		}

		break;
	}
	// PAUSE SCREEN
	case 3:
	{
		// Menu nav controls
		if (!downPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Down) || Joystick::getAxisPosition(1, Joystick::Axis::Y) > 99)
			{
				// Reached end of buttons list?
				if (currentlyActive == 1)
				{
					// Reset to go to top of buttons list
					currentlyActive = 0;
				}
				else
				{
					// Moving down list of buttons
					currentlyActive++;
				}
				// Redraw
				SetTextures(currentlyActive);

				downPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Down))
			downPressed = false;
		if (!upPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Up) || Joystick::getAxisPosition(1, Joystick::Axis::Y) < -99)
			{
				// Reached top of buttons list?
				if (currentlyActive == 0)
				{
					// Set to max to go to bottom of list
					currentlyActive = 1;
				}
				else
				{
					// Moving up list of buttons
					currentlyActive--;
				}
				// Redraw
				SetTextures(currentlyActive);

				upPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Up))
			upPressed = false;
		// Button logic
		if (!enterPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Return) || Joystick::isButtonPressed(1, 1))
			{
				switch (currentlyActive)
				{
					// Resume game
				case 0:
					paused = false;
					curScreen = 2;
					break;
					// End game to start screen
				case 1:
					paused = false;
					EndGame();
					curScreen = 0;
					break;
				default: break;
				}
				enterPressed = true;
			}
		}
		if (!Keyboard::isKeyPressed(Keyboard::Return))
			enterPressed = false;
		break;
	}

	default: break;
	}




}

// Draw to screen
void Render(RenderWindow &window)
{
	switch (curScreen)
	{
		// START SCREEN
	case 0:
		window.draw(highScoreStartText);
		// Loop through and draw each sprite in screen collection
		for (Sprite & s : startSprites)
			window.draw(s);
		break;
		// SETTINGS SCREEN
	case 1:
		window.draw(settingsBg);
		for (Sprite & s : settingsSprites)
			window.draw(s);
		break;
		// GAME SCREEN
	case 2:
	{
		// Check for pause (THIS SHOULDNT RUN)
		if (paused)
		{
			//break;
		}
		else
		{
			// Background and player
			window.draw(gameBg);
			window.draw(player);
			// Draw bullets
			for (Sprite & s : bulletSprites)
				window.draw(s);
			// Draw slow enemies
			for (Sprite & s : slowEnemySprites)
				window.draw(s);
			// Draw fast enemies
			for (Sprite & s : fastEnemySprites)
				window.draw(s);
			// Draw faster enemies
			for (Sprite & s : fasterEnemySprites)
				window.draw(s);
			// Draw powerups
			for (PowerUp & pu : powerUps)
				window.draw(pu.sprite);
		}
		break;
	}
	// PAUSE SCREEN
	case 3:
		window.draw(pauseBg);
		for (Sprite & s : pauseSprites)
			window.draw(s);

		break;
	}

}

int main()
{
	res.x = 800;
	res.y = 600;
	// Create window
	RenderWindow window(VideoMode(res.x, res.y), "");
	View view = window.getView();
	// Load textures and setup game sprites
	LoadTextures();
	// Play music
	bgMusic.setVolume(70);
	bgMusic.play();

	// Close window controls
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				EndGame();
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
		{
			EndGame();
			window.close();
		}
		if (closeWin)
			window.close();
		// Toggle fullscreen 
		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::Key::Num1)
			{

				window.create(VideoMode(800.0f, 600.0f), "", sf::Style::Resize | sf::Style::Close);
				window.setView(view);
			}
			if (event.key.code == Keyboard::Key::Num2)
			{
				window.create(VideoMode(1024.0f, 745.0f), "");
				window.setView(view);

			}
			if (event.key.code == Keyboard::Key::Num3)
			{
				window.create(VideoMode(1280.0f, 800.0f),"");
				window.setView(view);
			}

		}

		// Update resolution if changed (not while fullscreen)
		//if(!fullscreen)
		res = window.getSize();

		window.clear();
		Update();
		Render(window);
		window.display();


	}

	return 0;
}