#include "GameScreen.h"



GameScreen::GameScreen()
{
}


GameScreen::~GameScreen()
{
}

void GameScreen::Initialize(sf::RenderWindow * Window)
{
	window_ = Window;

	// Prepare the world
	InitPhysicalWorld();

	// Init Player and Boxes
	player->init(world, sf::Vector2i(200, 400), playerSize, scale);

	EntityBox* box = new EntityBox();
	box->init(world, sf::Vector2i(100, 100), boxSize, scale);
	boxes.push_back(box);

	EntityBox * box1 = new EntityBox();
	box1->init(world, sf::Vector2i(210, 210), boxSize, scale);
	boxes.push_back(box1);
}

void GameScreen::Update()
{
	// Timestep - DeltaTime
	double newTime = simulationClock_.getElapsedTime().asSeconds();
	double frameTime = newTime - simulationTimeLastUpdated_;
	simulationTimeLastUpdated_ = newTime;

	if (frameTime > 0.25f)
		frameTime = 0.25f; // Avoid "Spiral of death"

	accumulator += frameTime;

	while (accumulator >= dt)
	{
		player->movePlayer();

		// Run Simulation
		world->Step(1 / 60.0f, 8, 3);

		accumulator -= dt;
	}
}

void GameScreen::Render()
{
	for (auto b : boxes)
		b->Render(*window_);

	player->Render(*window_);
}

void GameScreen::InitPhysicalWorld()
{
	world = new b2World(b2Vec2(horizontalGravity, verticalGravity));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
}
