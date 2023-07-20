#include "../headers/Game.hpp"
#include "../headers/scenes/MainScene.hpp"
#include <headers/scenes/IntroScene.hpp>
#include <headers/scenes/TestScene.hpp>
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
	uniformBlocksInPool = 50;
	texturesInPool = 50;
	setsInPool = 50;

	Ar = (float)windowWidth / (float)windowHeight;
}

void Game::onWindowResize(int w, int h) {
	if (w & h) {
		Ar = (float)w / (float)h;
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

	managedScenes.push_back(introScene);
	managedScenes.push_back(mainScene);
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
	Time::computeDeltaT();

	// Standard procedure to quit when the ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	//Update the active scene
	managedScenes.at(activeScene)->UpdateImpl(currentImage);
}

void Game::recreateVulkanSwapChain(bool commandBufferOnly) {
	bool recreateFullSwapchain = !commandBufferOnly;
	int newUniformBlocksCount = 0;
	int newTexturesCount = 2;
	for (int i = 0; i < managedScenes.size(); i++) {
		newUniformBlocksCount += managedScenes.at(i)->totalUniformsCount();
		newTexturesCount += managedScenes.at(i)->totalTextureCount();
	}
	newUniformBlocksCount *= swapChainImages.size();
	newTexturesCount  *= swapChainImages.size();
	if (commandBufferOnly) {
		if (uniformBlocksInPool < newUniformBlocksCount || texturesInPool < newTexturesCount) {
			recreateFullSwapchain = true;
		} else {
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

void Game::switchToScene(int sceneID) {
	if (sceneID >= 0 && sceneID < managedScenes.size()) {
		activeScene = sceneID;
		recreateVulkanSwapChain();
	} else {
		std::cout << "WARNING: Attempting to switch to a non-existing scene. This attempt will be ignored.\n";
	}
}