#include "../headers/Game.hpp"

Game::Game() {
    managedScenes = {};
}

// Here you set the main application parameters
void Game::setWindowParameters() {
	// window size, titile and initial background
	windowWidth = 800;
	windowHeight = 600;
	windowTitle = "AstroRaccoons";
   	windowResizable = GLFW_TRUE;
	initialBackgroundColor = {0.0f, 0.005f, 0.01f, 1.0f};
	
	// Descriptor pool sizes
	uniformBlocksInPool = 4;
	texturesInPool = 4;
	setsInPool = 4;
	
	Ar = (float)windowWidth / (float)windowHeight;
}

void Game::onWindowResize(int w, int h) {
	Ar = (float)w / (float)h;
}

void Game::localInit() {
}

void Game::pipelinesAndDescriptorSetsInit() {
}

void Game::pipelinesAndDescriptorSetsCleanup() {
}

void Game::localCleanup() {	
}

void Game::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
}

void Game::updateUniformBuffer(uint32_t currentImage) {
	// Standard procedure to quit when the ESC key is pressed
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}