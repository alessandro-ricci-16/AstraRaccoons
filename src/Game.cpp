#include "../headers/Game.hpp"
#include "../headers/scenes/TestScene.hpp"
#include "../headers/engine/base/Time.hpp"

Game::Game() {
	managedScenes = {};

	activeScene = 0;
}

// Here you set the main application parameters
void Game::setWindowParameters() {
	// window size, titile and initial background
	windowWidth = 800;
	windowHeight = 600;
	windowTitle = "AstroRaccoons";
	windowResizable = GLFW_TRUE;
	initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };

	// Descriptor pool sizes
	uniformBlocksInPool = 10;
	texturesInPool = 10;
	setsInPool = 10;

	Ar = (float)windowWidth / (float)windowHeight;
}

void Game::onWindowResize(int w, int h) {
	if (w & h) {
		Ar = (float)w / (float)h;
	}
}

void Game::localInit() {
	TestScene* testScene = new TestScene(&Ar);
	testScene->proj = this;
	testScene->Instantiate();
	managedScenes.push_back(testScene);
}

void Game::pipelinesAndDescriptorSetsInit() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->proj = this;
		managedScenes.at(i)->CompileObjects();
	}
}

void Game::pipelinesAndDescriptorSetsCleanup() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->CleanupImpl();
	}
}

void Game::localCleanup() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->DestroyImpl();
		delete managedScenes.at(i);
	}
	managedScenes = {};
}

void Game::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
	managedScenes.at(activeScene)->Draw(commandBuffer, currentImage);
}

void Game::updateUniformBuffer(uint32_t currentImage) {
	Time::computeDeltaT();

	// Standard procedure to quit when the ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	//Update the active scene
	managedScenes.at(activeScene)->UpdateImpl(currentImage);
}

void Game::recreateVulkanSwapChain() {
	framebufferResized = true;
}
