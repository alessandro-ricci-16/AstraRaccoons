#include "../headers/Game.hpp"
#include "../headers/scenes/TestScene.hpp"

Game::Game() {
    managedScenes = {};

    activeScene = 0;
	test = new TestScene();
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
	uniformBlocksInPool = 10;
	texturesInPool = 4;
	setsInPool = 10;
	
	Ar = (float)windowWidth / (float)windowHeight;
}

void Game::onWindowResize(int w, int h) {
	Ar = (float)w / (float)h;
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->updateAspectRatio(Ar);
	}
}

void Game::localInit() {
	TestScene* testScene = new TestScene();
	testScene->proj = this;
	managedScenes.push_back(test);
}

void Game::pipelinesAndDescriptorSetsInit() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->proj = this;
		managedScenes.at(i)->updateAspectRatio(Ar); //Initialize aspect ratio in scene
		managedScenes.at(i)->Instantiate();
	}
}

void Game::pipelinesAndDescriptorSetsCleanup() {
	for (int i = 0; i < managedScenes.size(); i++) {
		managedScenes.at(i)->Destroy();
	}
}

void Game::localCleanup() {	
}

void Game::populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
	managedScenes.at(activeScene)->Draw(commandBuffer, currentImage);
}

void Game::updateUniformBuffer(uint32_t currentImage) {
	// Standard procedure to quit when the ESC key is pressed
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	//Update the active scene
	managedScenes.at(activeScene)->UpdateImpl(currentImage);
}
