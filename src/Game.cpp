#include "../headers/Game.hpp"
#include "../headers/scenes/MainScene.hpp"
#include <headers/scenes/IntroScene.hpp>
#include <headers/scenes/TestScene.hpp>
#include <headers/scenes/GameOverScene.hpp>
#include "../headers/engine/base/Inputs.hpp"
#include "../headers/engine/base/Time.hpp"
#include "../headers/engine/base/Random.hpp"

Game::Game() {
	managedScenes = {};
	activeScene = 0;
	Random::initialize();
	fullscreen = false;
}

// Here you set the main application parameters
void Game::setWindowParameters() {
	// window size, titile and initial background
	windowWidth = 800;
	windowHeight = 600;
	oldWindowWidth = windowWidth;
	oldWindowHeight = windowHeight;
	windowTitle = "AstraRaccoons";
	windowResizable = GLFW_TRUE;
	initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };

	// Descriptor pool sizes
	uniformBlocksInPool = 50;
	texturesInPool = 50;
	setsInPool = 50;

	Ar = (float)windowWidth / (float)windowHeight;
}

void Game::onWindowResize(int w, int h) {
	if (w && h) {
		windowWidth = w;
		windowHeight = h;
		Ar = (float)w / (float)h;
		Time::setPause(false);
		for (Scene* scene : managedScenes) {
			scene->windowResize();
		}
		std::cout << "Window resized\n";	}
	else { // window minimized
		Time::setPause(true);
	}
}

void Game::localInit() {
#ifdef DEBUG_LIGHTS
	TestScene* testScene = new TestScene(&Ar);
	testScene->proj = this;
	testScene->Instantiate();

	managedScenes.push_back(testScene);
#else
	IntroScene* introScene = new IntroScene(&Ar);
	introScene->proj = this;
	introScene->Instantiate();

	MainScene* mainScene = new MainScene(&Ar);
	mainScene->proj = this;
	mainScene->Instantiate();

	GameOverScene* gameOverScene = new GameOverScene(&Ar);
	gameOverScene->proj = this;
	gameOverScene->Instantiate();

	managedScenes.push_back(introScene);
	managedScenes.push_back(mainScene);
	managedScenes.push_back(gameOverScene);
#endif
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes[i]->applyObjectModifications();
	}
}

void Game::pipelinesAndDescriptorSetsInit() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->proj = this;
		managedScenes.at(i)->CompileObjects(false);
	}
}

void Game::pipelinesAndDescriptorSetsCleanup() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->CleanupImpl();
	}
	GraphicsPipeline::cleanupGUBOs();
}

void Game::localCleanup() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->DestroyImpl();
		delete managedScenes.at(i);
	}
	managedScenes = {};
	GraphicsPipeline::destroyGUBOs();
}

void Game::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
	managedScenes.at(activeScene)->Draw(commandBuffer, currentImage);
}

void Game::updateUniformBuffer(uint32_t currentImage) {
	Time::computeDeltaT(); // update delta time also if game paused to avoid having huge delT when resuming
	if (!Time::isPaused()) { // only if game is not paused
		// Standard procedure to quit when the ESC key is pressed
		if (Inputs::isKeyPressed(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		} // check if toogle to fullscreen on key released
		else if (Inputs::isKeyPressed(GLFW_KEY_ENTER)) {
			fullScreenClicked = true;
		}
		else if (fullScreenClicked) {
			fullScreenClicked = false;
			toggleFullscreen();
		}
		//Update the active scene
		managedScenes.at(activeScene)->UpdateImpl(currentImage);
		performSceneSwitchIfRequested();
	}
}

void Game::recreateVulkanSwapChain(bool commandBufferOnly) {
	bool recreateFullSwapchain = !commandBufferOnly;
	int newUniformBlocksCount = 0;
	int newTexturesCount = 2;
	for (int i = 0; i < managedScenes.size(); i++) {
		newUniformBlocksCount += managedScenes.at(i)->totalUniformsCount();
		newTexturesCount += managedScenes.at(i)->totalTextureCount();
	}
	if (commandBufferOnly) {
		if (uniformBlocksInPool < newUniformBlocksCount || texturesInPool < newTexturesCount) {
			recreateFullSwapchain = true;
		}
		else {
			for (int i = 0; i < managedScenes.size(); i++) {
				managedScenes.at(i)->proj = this;
				managedScenes.at(i)->CompileObjects(true);
			}
			refillCommandBuffers();
		}
	}
	if (recreateFullSwapchain) {
		RebuildPipeline();
		//Resize pools
		uniformBlocksInPool = newUniformBlocksCount * 10;
		texturesInPool = newTexturesCount * 5;
		setsInPool = uniformBlocksInPool;
	}
}

bool Game::requestSwitchToScene(int sceneID) {
	if (sceneID >= 0 && sceneID < managedScenes.size()) {
		nextSceneToSwitch = sceneID;
		sceneSwitchRequested = true;
	}
	else {
		sceneSwitchRequested = false;
		std::cout << "WARNING: Attempting to switch to a non-existing scene. This attempt will be ignored. (requested scene: " << sceneID << ")\n";
	}
	return sceneSwitchRequested;
}

void Game::performSceneSwitchIfRequested() {
	if (nextSceneToSwitch >= 0 && nextSceneToSwitch < managedScenes.size() && sceneSwitchRequested) {
		activeScene = nextSceneToSwitch;
		sceneSwitchRequested = false;
		managedScenes[activeScene]->WillAppearImpl();
		recreateVulkanSwapChain(true);
	}
	else if (sceneSwitchRequested) {
		std::cout << "WARNING: Attempting to switch to a non-existing scene. This attempt will be ignored.\n";
	}
}

void Game::toggleFullscreen() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int screenWidth = mode->width;
    int screenHeight = mode->height;

    if (fullscreen) {
        std::cout << "Exit fullscreen\n";
        int xPos = (screenWidth - oldWindowWidth) / 2;
        int yPos = (screenHeight - oldWindowHeight) / 2;
        glfwSetWindowMonitor(window, nullptr, xPos, yPos, oldWindowWidth, oldWindowHeight, GLFW_DONT_CARE);
    }
    else {
        std::cout << "Enter fullscreen\n";
        glfwGetWindowSize(window, &oldWindowWidth, &oldWindowHeight);
        glfwSetWindowMonitor(window, monitor, 0, 0, screenWidth, screenHeight, GLFW_DONT_CARE);
    }
    fullscreen = !fullscreen;
}

uint32_t Game::getWidth() {
	return windowWidth;
}

uint32_t Game::getHeight() {
	return windowHeight;
}
