
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Scene03::Scene03() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Scene03::~Scene03() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void Scene03::start() {
	Game* game = Game::getInstance();
	Resources* resources = game->resources;

	// camera
	GameObject* camera = new GameObject();
	camera->staticObject = false;
	camera->addComponent<Camera>();
	
	// player
	GameObject* player = new GameObject();
	player->staticObject = false;
	Transform* playerTransform = player->addComponent<Transform>();
	playerTransform->modelScale = glm::vec3(GLOBAL_SCALE);
	playerTransform->position = glm::vec3(0, 0, 0);
	PlayerController* playerController = player->addComponent<PlayerController>();
	playerController->camera = camera;
	CameraController* cameraController = player->addComponent<CameraController>();
	cameraController->camera = camera;
	MeshRenderer* playerMeshRenderer = player->addComponent<MeshRenderer>();
	playerMeshRenderer->model = game->resources->getModel("player");
	playerMeshRenderer->materials.push_back(game->resources->getMaterial("player"));
	playerMeshRenderer->castShadow = true;
	this->addGameObject("player", player);
	game->shadowMapping->target = player;
	
	// camera target
	GameObject* cameraTarget = new GameObject();
	Transform* cameraTargetTransform = cameraTarget->addComponent<Transform>();
	Offset* cameraTargetOffset = cameraTarget->addComponent<Offset>();
	cameraTargetOffset->parent = playerTransform;
	cameraTargetOffset->offsetPosition = glm::vec3(0, 1, 0);
	this->addGameObject("cameraTarget", cameraTarget);
	
	// camera
	Transform* cameraTransform = camera->addComponent<Transform>();
	cameraTransform->position = glm::vec3(0.0,1.0,4.0);
	camera->getComponent<Camera>()->target = cameraTargetTransform;
	camera->addComponent<FrustumCulling>();
	this->mainCamera = camera;
	this->addGameObject("mainCamera", camera);
	
	// light
	GameObject* light[4];
	for (int i = 0; i < 4; i++) {
		light[i] = new GameObject();
		light[i]->staticObject = false;
		Transform* lightTransform = light[i]->addComponent<Transform>();
		lightTransform->modelScale = glm::vec3(5 * GLOBAL_SCALE);
		light[i]->addComponent<PointLight>();
		// for test
		MeshRenderer* lightMeshRenderer = light[i]->addComponent<MeshRenderer>();
		lightMeshRenderer->model = game->resources->getModel("sphere");
		lightMeshRenderer->materials.push_back(game->resources->getMaterial("simple"));
		this->addGameObject(("light" + std::to_string(i)).c_str(), light[i]);
	}
	light[0]->getComponent<Transform>()->position = glm::vec3( 3.0f,  2,  3.0f);
	light[1]->getComponent<Transform>()->position = glm::vec3( 3.0f,  2, -3.0f);
	light[2]->getComponent<Transform>()->position = glm::vec3(-3.0f,  2,  3.0f);
	light[3]->getComponent<Transform>()->position = glm::vec3(-3.0f,  2, -3.0f);
	light[0]->getComponent<PointLight>()->color = glm::vec3(20.0f, 20.0f, 20.0f);
	light[1]->getComponent<PointLight>()->color = glm::vec3(20.0f, 0.0f, 0.0f);
	light[2]->getComponent<PointLight>()->color = glm::vec3(0.0f, 20.0f, 0.0f);
	light[3]->getComponent<PointLight>()->color = glm::vec3(0.0f, 0.0f, 20.0f);
	
	// fire
	// resources
	resources->loadTexture("fx_fire_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_D.TGA");
	resources->createMaterial("fx_fire", resources->getShader("fx_uv_animation"));
	resources->getMaterial("fx_fire")->addTexture("albedoMap", resources->getTexture("fx_fire_D"));
	resources->loadTexture("fx_fire_smoke_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_smoke_D.TGA");
	resources->createMaterial("fx_fire_smoke", resources->getShader("fx_image"));
	resources->getMaterial("fx_fire_smoke")->addTexture("albedoMap", resources->getTexture("fx_fire_smoke_D"));
	resources->loadTexture("fx_fire_drop_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_drop_D.TGA");
	resources->createMaterial("fx_fire_drop", resources->getShader("fx_image"));
	resources->getMaterial("fx_fire_drop")->addTexture("albedoMap", resources->getTexture("fx_fire_drop_D"));
	resources->loadTexture("fx_fire_spark_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_spark_D.TGA");
	resources->createMaterial("fx_fire_spark", resources->getShader("fx_image"));
	resources->getMaterial("fx_fire_spark")->addTexture("albedoMap", resources->getTexture("fx_fire_spark_D"));
	
	// create
	FxTorchFire* fxTorchFire = FxTorchFire::getInstance();
	glm::mat4 fireTransformation = glm::translate(glm::vec3(0,1,1));
	fxTorchFire->createPrefab("fire", fireTransformation, this);
}

