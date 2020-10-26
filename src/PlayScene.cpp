#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include <iostream>
using namespace std;

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);


	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	
	m_pBall->getTransform()->rotation = calcAngle();

	direction = glm::vec2(m_pRamp->getRun(), m_pRamp->getYPos());

	m_pBall->getRigidBody()->acceleration = Util::normalize(direction);

	//totalAccel = 9.8 * sin( calcAngle());

	//m_pBall->getRigidBody()->acceleration = glm::vec2(totalAccel * cos(calcAngle()),totalAccel * sin(calcAngle()));

	//m_pBall->update();

	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{

	TextureManager::Instance()->load("../Assets/textures/Background.png", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	m_pRamp = new Plane();
	addChild(m_pRamp);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	//Ball
	m_pBall = new Target();
	addChild(m_pBall);

	

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	m_pActivateButton = new Button("../Assets/textures/activateButton.png", "activateButton", ACTIVATE_BUTTON);
	
	m_pBall->throwPosition = glm::vec2(150, 100);
	m_pBall->throwSpeed = glm::vec2(0, 0);
	m_pActivateButton->getTransform()->position = glm::vec2(400.0f, 500.0f);
	m_pActivateButton->addEventListener(CLICK, [&]()-> void
	{
		
		m_pActivateButton->setActive(false);
		m_pBall->doThrow();
	});

	m_pActivateButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pActivateButton->setAlpha(128);
	});

	m_pActivateButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pActivateButton->setAlpha(255);
	});

	addChild(m_pActivateButton);

	/* Instructions Label */
	//m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	//m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 100.0f);

	//addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("Activate")) {
		m_pBall->doThrow();
	}

	if (ImGui::Button("Reset")) {
		m_pBall->m_reset();
	}
	ImGui::Separator();

	static bool isGravityEnabled = false;
	if (ImGui::Checkbox("Gravity off", &isGravityEnabled)) {
		m_pBall->isGravityEnabled = isGravityEnabled;
	}

	ImGui::Separator();

	static int xRise = 155;
	if (ImGui::SliderInt("Rise", &xRise, 10, m_pRamp->yPosition - 50)) {
		m_pRamp->rise = xRise;
		m_pBall->throwPosition.y = xRise - 15;
	}

	static int xRun = 155;
	if (ImGui::SliderInt("Run", &xRun, m_pRamp->xPosition + 50, 750 )) {
		m_pRamp->run = xRun;

	}

	static int xPosition = 155;
	if (ImGui::SliderInt("X Position", &xPosition, 10, m_pRamp->run - 50)) {
		m_pRamp->xPosition = xPosition;
		m_pBall->throwPosition.x = xPosition + (m_pBall->getWidth()/2);

	}

	static int yPosition = 155;
	if (ImGui::SliderInt("Y Position", &yPosition, m_pRamp->rise + 50, 550)) {
		m_pRamp->yPosition = yPosition;

	}

	ImGui::Separator();

	static int Mmass = 155;
	if (ImGui::SliderInt("Mass", &Mmass, 0, 500)) {
		m_pBall->getRigidBody()->mass = Mmass;

	}



	/*static int xPosEnemy = 640;
	if (ImGui::SliderInt("Plane's X Position", &xPosEnemy, 450, 800)) {
		m_pPlane->getTransform()->position.x = xPosEnemy;
	}*/

	/*static float velocity[2] = { 40 , 60 };
	if (ImGui::SliderFloat2("Throw Speed", velocity, 0, 100)) {
		m_pBall->throwSpeed = glm::vec2(velocity[0], -velocity[1]);
	}*/

	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

float PlayScene::calcAngle() 
{
	int sideLength = m_pRamp->getRun() - m_pRamp->getXPos();
	int sideWidth = m_pRamp->getYPos() - m_pRamp->getRise();
	int sideHype = (sideLength * sideLength) + (sideWidth * sideWidth);
	// float angle = Util::angle(glm::vec2(m_pRamp->getXPos(), m_pRamp->getRise()), glm::vec2(m_pRamp->getRun(), m_pRamp->getYPos()));
	return acos(((sideWidth * sideWidth) + sideHype - (sideLength * sideLength))/(2*sideWidth*(sqrt(sideHype)))) * -57.29578f;
}
