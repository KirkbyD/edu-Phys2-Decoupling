#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <memory>

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DEBUG_NEW
#else
#define DBG_NEW
#endif

#include "cFileManager.hpp"
#include "cAudioManager.hpp"
#include <iostream>
#include <algorithm>
using namespace std;


#pragma region("SINGLETON")
cFileManager cFileManager::stonFileMngr;
cFileManager* cFileManager::GetFileManager() { return &(cFileManager::stonFileMngr); }
cFileManager::cFileManager() { std::cout << "File Manager Created" << std::endl; }
#pragma endregion

std::string cFileManager::GetModelFile() { return _model_File; }
std::string cFileManager::GetMeshFile() { return _mesh_File; }
std::string cFileManager::GetTextureFile() { return _texture_File; }
std::string cFileManager::GetSkyboxTextureFile() { return _skybox_texture_File; }
std::string cFileManager::GetLightsFile() { return _lights_File; }
std::string cFileManager::GetShadersFile() { return _shaders_File; }
std::string cFileManager::GetAudioFile() { return _audio_File; }
std::string cFileManager::GetComplexObjFile() { return _complexObj_File; }

void cFileManager::LoadData(string filename) {
	if (!_data.empty())
		_data.clear();
	_input.open(_filePath + filename + _fileExt);

	if (!_input.is_open()) {
		errmngr.DisplayError("cFileManager", __func__, "invalidfile", "Unable to open specified file.");
		return;
	}

	while (getline(_input, _tempData)) {
		_data.push_back(_tempData);
	}
	_input.close();
}

bool cFileManager::LoadPhysicsLibraryStruct(sPhysicsLibs* PhysicsDLLs) {
	rapidjson::Document document = cJSONUtility::open_document("./" + _filePath + _physics_library_path);
	cJSONUtility::deserialize(PhysicsDLLs, &document);
	return true;
}

void cFileManager::LoadModelsOntoGPU(cModelLoader* pModelLoader, cVAOManager* pVAOManager, const GLuint& shaderProgID, string filename, map<string, cMesh*>& mpMesh) {
	vector<string> lhs, rhs;
	string configNode;
	LoadData(filename);

	for (size_t i = 0; i < _data.size(); ++i) {
		if (!_data[i].empty() && _data[i].front() == '[' && _data[i].back() == ']')
			configNode = _data[i];

		if (configNode == "[MESHES]") {
			if (_data[i] != configNode) {
				if (_data[i] != "") {
					lhs.push_back(_data[i].substr(0, _data[i].find('=')));
					rhs.push_back(_data[i].substr(_data[i].find('=') + 1, _data[i].back()));
				}
			}
		}
	}

	for (size_t i = 0; i < rhs.size(); i += 2) {
		g_vec_pMeshObjects.push_back(new cMesh());
		mpMesh[rhs[i + 1]] = g_vec_pMeshObjects[g_vec_pMeshObjects.size() - 1];

		if (!pModelLoader->LoadPlyModel(rhs[i], *g_vec_pMeshObjects[g_vec_pMeshObjects.size() - 1]))
			cout << "Didn't find the file" << endl;
		// Report Error
		sModelDrawInfo drawInfo;
		if (!pVAOManager->LoadModelIntoVAO(rhs[i + 1], *g_vec_pMeshObjects[g_vec_pMeshObjects.size() - 1], drawInfo, shaderProgID))
			cout << "Couldn't load mesh file onto the GPU" << endl;
	}
}

void cFileManager::LoadTexturesOntoGPU(cBasicTextureManager* pTextureManager, string filename, map<std::string, GLuint>& mpTex) {
	vector<string> lhs, rhs;
	string configNode;
	LoadData(filename);
	pTextureManager->SetBasePath(_texture_path);

	for (size_t i = 0; i < _data.size(); ++i) {
		if (!_data[i].empty() && _data[i].front() == '[' && _data[i].back() == ']')
			configNode = _data[i];

		if (configNode == "[TEXTURES]") {
			if (_data[i] != configNode) {
				if (_data[i] != "") {
					lhs.push_back(_data[i].substr(0, _data[i].find('=')));
					rhs.push_back(_data[i].substr(_data[i].find('=') + 1, _data[i].back()));
				}
			}
		}
	}

	for (size_t i = 0; i < rhs.size(); i += 2) {
		pTextureManager->Create2DTextureFromBMPFile(rhs[i], true);
		mpTex[rhs[i + 1]] = pTextureManager->getTextureIDFromName(rhs[i]);
	}
}

void cFileManager::LoadSkyboxTexturesOntoGPU(cBasicTextureManager* pTextureManager, string filename, map<std::string, GLuint>& mpTex) {
	vector<string> lhs, rhs;
	string configNode;
	LoadData(filename);
	pTextureManager->SetBasePath(_skybox_texture_path);

	for (size_t i = 0; i < _data.size(); ++i) {
		if (!_data[i].empty() && _data[i].front() == '[' && _data[i].back() == ']')
			configNode = _data[i];

		if (configNode == "[SKYBOXTEXTURES]") {
			if (_data[i] != configNode) {
				if (_data[i] != "") {
					lhs.push_back(_data[i].substr(0, _data[i].find('=')));
					rhs.push_back(_data[i].substr(_data[i].find('=') + 1, _data[i].back()));
				}
			}
		}
	}

	for (size_t j = 0; j < rhs.size(); j += 7) {
		std::string errorString;
		pTextureManager->CreateCubeTextureFromBMPFiles(rhs[j + 6],
			rhs[j], rhs[j + 1], rhs[j + 2], rhs[j + 3], rhs[j + 4], rhs[j + 5], true, errorString);
		mpTex[rhs[j + 6]] = pTextureManager->getTextureIDFromName(rhs[j + 6]);
	}
}

void cFileManager::LoadShaders(std::string filename, std::map<std::string, cShaderManager::cShader>& mpVertexShader, std::map<std::string, cShaderManager::cShader>& mpFragmentShader) {
	LoadData(filename);

	for (size_t i = 0; i < _data.size(); ++i) {
		if (!_data[i].empty() && _data[i].front() == '[' && _data[i].back() == ']')
			_config_node = _data[i];

		if (_config_node == "[SHADERS]") {
			if (_data[i] != _config_node) {
				if (_data[i] != "") {
					_vlhs.push_back(_data[i].substr(0, _data[i].find('=')));
					_vrhs.push_back(_data[i].substr(_data[i].find('=') + 1, _data[i].back()));
				}
			}
		}
	}

	for (size_t i = 0; i < _vrhs.size(); i += 2) {
		if (_vlhs[i] == "vertName") {
			cShaderManager::cShader shader;
			shader.fileName = _vrhs[i + 1];
			mpVertexShader[_vrhs[i]] = shader;
		}
		else if (_vlhs[i] == "fragName") {
			cShaderManager::cShader shader;
			shader.fileName = _vrhs[i + 1];
			mpFragmentShader[_vrhs[i]] = shader;
		}
		else {
			errmngr.DisplayError("cFileManager", __func__, "unknownshader", "Shader type in file is unknown");
		}
	}
}

void cFileManager::BuildObjects(std::string filename)
{
	LoadData(filename);

	std::string _config_node;
	iGameObject* theObject = NULL;

	for (size_t i = 0; i < _data.size(); ++i) {
		if (!_data[i].empty() && _data[i].front() == '[' && _data[i].back() == ']')
			_config_node = _data[i];

		if (_data[i] != _config_node) {
			if (_data[i] != "") {
				_lhs = _data[i].substr(0, _data[i].find('='));
				_rhs = _data[i].substr(_data[i].find('=') + 1, _data[i].size());

				if (_config_node == "[LIGHTS]") {
					if (_lhs == "name") {
						theObject = fact_game_obj.CreateGameObject(_config_node);
						mpLight[_rhs] = g_vec_pLightObjects[g_vec_pLightObjects.size() - 1];
						theObject->friendlyName = _rhs;
					}
					else if (_lhs == "position") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->position = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
					else if (_lhs == "diffuse") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->diffuse = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
					else if (_lhs == "specular") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->specular = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
					else if (_lhs == "atten") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->atten = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
					else if (_lhs == "direction") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->direction = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
					else if (_lhs == "param1") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->param1 = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
					else if (_lhs == "param2") {
						ConvertStringToFloatArray(_rhs);
						((cLightObject*)theObject)->param2 = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}
				}

				if (_config_node == "[MODELS]") {
					// load the possible configurations into a map with an enum, this way the if / if else / else block can be converted into a switch statement.
					if (_lhs == "meshName") {
						theObject = fact_game_obj.CreateGameObject(_config_node);
						((cModelObject*)theObject)->meshName = _rhs;
					}

					else if (_lhs == "friendlyName") {
						mpModel[_rhs] = g_vec_pGameObjects[g_vec_pGameObjects.size() - 1];
						((cModelObject*)theObject)->friendlyName = _rhs;
					}

					else if (_lhs == "positionXYZ") {
						ConvertStringToFloatArray(_rhs);
						((cModelObject*)theObject)->positionXYZ = glm::vec3(vf[0], vf[1], vf[2]);
						ResetConversionVars();
					}

					else if (_lhs == "rotationXYZ") {
						ConvertStringToFloatArray(_rhs);
						((cModelObject*)theObject)->setOrientation(glm::vec3(vf[0], vf[1], vf[2]));
						ResetConversionVars();
					}

					else if (_lhs == "scale") {
						ConvertStringToFloat(_rhs);
						((cModelObject*)theObject)->scale = f;
						ResetConversionVars();
					}

					else if (_lhs == "objectColourRGBA") {
						ConvertStringToFloatArray(_rhs);
						((cModelObject*)theObject)->objectColourRGBA = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}

					else if (_lhs == "texture") {
						((cModelObject*)theObject)->v_textureNames.push_back(_rhs);
						ResetConversionVars();
					}

					else if (_lhs == "texureBlendRatio") {
						ConvertStringToFloat(_rhs);
						((cModelObject*)theObject)->v_texureBlendRatio.push_back(f);
						ResetConversionVars();
					}

					else if (_lhs == "alphaTransparency") {
						ConvertStringToFloat(_rhs);
						((cModelObject*)theObject)->alphaTransparency = f;
						ResetConversionVars();
					}

					else if (_lhs == "discardTexture") {
						((cModelObject*)theObject)->discardTextureName = _rhs;
						ResetConversionVars();
					}

					else if (_lhs == "debugColour") {
						ConvertStringToFloatArray(_rhs);
						((cModelObject*)theObject)->debugColour = glm::vec4(vf[0], vf[1], vf[2], vf[3]);
						ResetConversionVars();
					}

					// DEFAULT = FALSE
					else if (_lhs == "isWireframe") {
						if (_rhs == "true")
							((cModelObject*)theObject)->isWireframe = true;
					}

					// PHYSICS STUFF BELOW HERE
					else if (_lhs == "velocity") {
						ConvertStringToFloatArray(_rhs);
						((cModelObject*)theObject)->velocity = glm::vec3(vf[0], vf[1], vf[2]);
						ResetConversionVars();
					}

					else if (_lhs == "accel") {
						ConvertStringToFloatArray(_rhs);
						((cModelObject*)theObject)->accel = glm::vec3(vf[0], vf[1], vf[2]);
						ResetConversionVars();
					}

					else if (_lhs == "physicsShapeType")
						((cModelObject*)theObject)->physicsShapeType = DetermineShapeType(_rhs, g_mp_String_to_ShapeType);

					else if (_lhs == "inverseMass") {
						ConvertStringToFloat(_rhs);
						((cModelObject*)theObject)->inverseMass = f;
						ResetConversionVars();
					}
				}

				if (_config_node == "[AUDIO]") {
					cAudioManager* pAudioManager = cAudioManager::GetAudioManager();

					//first node step - mandatory
					if (_lhs == "friendlyName") {
						theObject = fact_game_obj.CreateGameObject(_config_node);
						theObject->friendlyName = _rhs;
						mpAudio[_rhs] = (cAudioObject*)theObject;
					}
					else if (_lhs == "path") {
						std::string filepath = "data/audio/" + _rhs;
						((cAudioObject*)theObject)->SetPath(filepath);
					}
					else if (_lhs == "isbgm") {
						if (_rhs == "true") {
							((cAudioObject*)theObject)->SetIsBGM(true);
							((cAudioObject*)theObject)->CreateSound(true);
						}
						else {
							((cAudioObject*)theObject)->SetIsBGM(false);
							((cAudioObject*)theObject)->Create3DSound(false);
						}
					}
					else if (_lhs == "startpaused") {
						if (_rhs == "false")
							((cAudioObject*)theObject)->SetIsPaused(false);
					}
					else if (_lhs == "channel") {
						ConvertStringToInt(_rhs);
						((cAudioObject*)theObject)->SetChannel(n);
						ResetConversionVars();
					}

					else if (_lhs == "positionXYZ") {
						ConvertStringToFloatArray(_rhs);
						((cAudioObject*)theObject)->SetPosition(glm::vec3(vf[0], vf[1], vf[2]));
						ResetConversionVars();
					}

					else if (_lhs == "channelgroup") {
						((cAudioObject*)theObject)->SetChannelGroup(_rhs);
					}
				}

				if (_config_node == "[COMPLEX]") {
					//do shit
					if (_lhs == "friendlyName") {
						theObject = fact_game_obj.CreateGameObject(_config_node);
						((cComplexObject*)theObject)->friendlyName = _rhs;
					}

					else if (_lhs == "light") {
						((cComplexObject*)theObject)->AddLight(_rhs);
					}

					else if (_lhs == "model") {
						((cComplexObject*)theObject)->AddModel(_rhs);
					}

					else if (_lhs == "scale") {
						ConvertStringToFloat(_rhs);
						((cComplexObject*)theObject)->SetScale(f);
						ResetConversionVars();
					}

					else if (_lhs == "position") {
						ConvertStringToFloatArray(_rhs);
						//((cComplexObject*)theObject)->SetPosition(glm::vec3(vf[0], vf[1], vf[2]));
						ResetConversionVars();
					}

					// PHYSICS STUFF BELOW HERE
					/*else if (_lhs == "velocity") {
						ConvertStringToFloatArray(_rhs);
						((cComplexObject*)theObject)->SetVelocity(glm::vec3(vf[0], vf[1], vf[2]));
						ResetConversionVars();
					}*/

					/*else if (_lhs == "accel") {
						ConvertStringToFloatArray(_rhs);
						((cComplexObject*)theObject)->SetAcceleration(glm::vec3(vf[0], vf[1], vf[2]));
						ResetConversionVars();
					}*/

					else if (_lhs == "inverseMass") {
						ConvertStringToFloat(_rhs);
						//((cComplexObject*)theObject)->SetInverseMass(f);
						ResetConversionVars();
					}

				/*	else if (_lhs == "collpoint") {
						ConvertStringToFloatArray(_rhs);
						((cComplexObject*)theObject)->AddCollisionPoint(glm::vec3(vf[0], vf[1], vf[2]));
						ResetConversionVars();
					}*/
				}
			}
		}
	}
}



void cFileManager::BuildEntities(cPhysicsManager* pPhysicsManager) {
	rapidjson::Document document = cJSONUtility::open_document("./" + _filePath + _entity_path);
	cJSONUtility::deserialize(&EntitySerializer, &document);

	for (size_t i = 0; i < EntitySerializer.Entities.size(); i++) {
		iGameObject* objInterface = fact_game_obj.CreateGameObject("[COMPLEX]");
		cComplexObject* Entity = (cComplexObject*)objInterface;
		Entity->friendlyName = EntitySerializer.Entities[i].Name;
		for (size_t j = 0; j < EntitySerializer.Entities[i].PhysicsComponents.size(); j++) {
			sPhysicsComponent data = EntitySerializer.Entities[i].PhysicsComponents[j];
			nPhysics::iPhysicsComponent* PhysicsComponent;
			switch (data.shape) {
			case nPhysics::eComponentType::ball:
				PhysicsComponent = pPhysicsManager->FactoryCreateBall(data.position, data.velocity, data.radius, data.mass);
				Entity->AddPhysicsComponent(PhysicsComponent);
				pPhysicsManager->WorldAddRigidBody(PhysicsComponent);
				break;
			case nPhysics::eComponentType::plane:
				PhysicsComponent = pPhysicsManager->FactoryCreatePlane(data.position, data.normal);
				Entity->AddPhysicsComponent(PhysicsComponent);
				pPhysicsManager->WorldAddRigidBody(PhysicsComponent);
				break;
			default:
				break;
			}
		}
		for (size_t j = 0; j < EntitySerializer.Entities[i].GraphicsComponents.size(); j++) {
			iGameObject* objInterface = fact_game_obj.CreateGameObject("[MODELS]");
			cModelObject* GraphicsComponent = (cModelObject*)objInterface;
			GraphicsComponent->meshName = EntitySerializer.Entities[i].GraphicsComponents[j].mesh;
			GraphicsComponent->friendlyName = EntitySerializer.Entities[i].GraphicsComponents[j].name;
			GraphicsComponent->v_textureNames = { EntitySerializer.Entities[i].GraphicsComponents[j].texture };
			GraphicsComponent->v_texureBlendRatio = { 1.0f };
			GraphicsComponent->scale = EntitySerializer.Entities[i].GraphicsComponents[j].scale;
			Entity->AddModel(GraphicsComponent);
		}
	}

}

bool cFileManager::SaveData() {
	FormatModelData();
	_output.open((_filePath + _model_File + "_savetest" + _fileExt));
	_output << _data_stream.str();
	_data_stream.str(std::string());
	_output.close();

	FormatLightData();
	_output.open((_filePath + _lights_File + "_savetest" + _fileExt));
	_output << _data_stream.str();
	_data_stream.str(std::string());
	_output.close();

	FormatComplexModelData();
	_output.open((_filePath + _complexObj_File + "_savetest" + _fileExt));
	_output << _data_stream.str();
	_data_stream.str(std::string());
	_output.close();

	return true;
}

bool cFileManager::SaveData(std::string modelsFilename, std::string lightsFilename, std::string complexFilename) {
	FormatModelData();
	_output.open((_filePath + _graphics_path + modelsFilename + _fileExt));
	_output << _data_stream.str();
	_data_stream.str(std::string());
	_output.close();

	FormatLightData();
	_output.open((_filePath + _lights_path + lightsFilename + _fileExt));
	_output << _data_stream.str();
	_data_stream.str(std::string());
	_output.close();

	FormatComplexModelData();
	_output.open((_filePath + _complex_object_path + complexFilename + _fileExt));
	_output << _data_stream.str();
	_data_stream.str(std::string());
	_output.close();

	return true;
}

void cFileManager::FormatModelData() {
	_data_stream << "[MODELS]" << "\n";

	for (size_t i = 0; i < g_vec_pGameObjects.size(); ++i) {
		_data_stream << "meshName" << "=" << g_vec_pGameObjects[i]->meshName << "\n";
		_data_stream << "friendlyName" << "=" << g_vec_pGameObjects[i]->friendlyName << "\n";

		_data_stream << "positionXYZ" << "=";
		_data_stream << g_vec_pGameObjects[i]->positionXYZ.x << ((CheckDecimal(g_vec_pGameObjects[i]->positionXYZ.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->positionXYZ.y << ((CheckDecimal(g_vec_pGameObjects[i]->positionXYZ.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->positionXYZ.z << ((CheckDecimal(g_vec_pGameObjects[i]->positionXYZ.z)) ? "f\n" : ".0f\n");

		_data_stream << "rotationXYZ" << "=";
		_data_stream << g_vec_pGameObjects[i]->getEulerAngle().x << ((CheckDecimal(g_vec_pGameObjects[i]->getEulerAngle().x)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->getEulerAngle().y << ((CheckDecimal(g_vec_pGameObjects[i]->getEulerAngle().y)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->getEulerAngle().z << ((CheckDecimal(g_vec_pGameObjects[i]->getEulerAngle().z)) ? "f\n" : ".0f\n");

		_data_stream << "scale" << "=" << g_vec_pGameObjects[i]->scale << ((CheckDecimal(g_vec_pGameObjects[i]->scale)) ? "f\n" : ".0f\n");

		_data_stream << "objectColourRGBA" << "=";
		_data_stream << g_vec_pGameObjects[i]->objectColourRGBA.r << ((CheckDecimal(g_vec_pGameObjects[i]->objectColourRGBA.r)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->objectColourRGBA.g << ((CheckDecimal(g_vec_pGameObjects[i]->objectColourRGBA.g)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->objectColourRGBA.b << ((CheckDecimal(g_vec_pGameObjects[i]->objectColourRGBA.b)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->objectColourRGBA.a << ((CheckDecimal(g_vec_pGameObjects[i]->objectColourRGBA.a)) ? "f\n" : ".0f\n");

		_data_stream << "debugColour" << "=";
		_data_stream << g_vec_pGameObjects[i]->debugColour.r << ((CheckDecimal(g_vec_pGameObjects[i]->debugColour.r)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->debugColour.g << ((CheckDecimal(g_vec_pGameObjects[i]->debugColour.g)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->debugColour.b << ((CheckDecimal(g_vec_pGameObjects[i]->debugColour.b)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->debugColour.a << ((CheckDecimal(g_vec_pGameObjects[i]->debugColour.a)) ? "f\n" : ".0f\n");

		_data_stream << "isWireframe" << "=" << (g_vec_pGameObjects[i]->isWireframe == 0 ? "false" : "true") << "\n";
		_data_stream << "isVisible" << "=" << (g_vec_pGameObjects[i]->isVisible == 0 ? "false" : "true") << "\n";

		_data_stream << "velocity" << "=";
		_data_stream << g_vec_pGameObjects[i]->velocity.x << ((CheckDecimal(g_vec_pGameObjects[i]->velocity.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->velocity.y << ((CheckDecimal(g_vec_pGameObjects[i]->velocity.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->velocity.z << ((CheckDecimal(g_vec_pGameObjects[i]->velocity.z)) ? "f\n" : ".0f\n");

		_data_stream << "accel" << "=";
		_data_stream << g_vec_pGameObjects[i]->accel.x << ((CheckDecimal(g_vec_pGameObjects[i]->accel.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->accel.y << ((CheckDecimal(g_vec_pGameObjects[i]->accel.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pGameObjects[i]->accel.z << ((CheckDecimal(g_vec_pGameObjects[i]->accel.z)) ? "f\n" : ".0f\n");

		_data_stream << "physicsShapeType" << "=" << GetShapeTypeString(g_vec_pGameObjects[i]->physicsShapeType) << "\n";

		if (i != g_vec_pGameObjects.size() - 1) {
			_data_stream << "inverseMass" << "=" << g_vec_pGameObjects[i]->inverseMass << ((CheckDecimal(g_vec_pGameObjects[i]->inverseMass)) ? "f\n" : ".0f\n");
			_data_stream << "\n";
		}
		else
			_data_stream << "inverseMass" << "=" << g_vec_pGameObjects[i]->inverseMass << ((CheckDecimal(g_vec_pGameObjects[i]->inverseMass)) ? "f" : ".0f");
	}
}

void cFileManager::FormatLightData() {
	_data_stream << "[LIGHTS]" << "\n";

	for (size_t i = 0; i < g_vec_pLightObjects.size(); ++i) {
		_data_stream << "name" << "=" << g_vec_pLightObjects[i]->friendlyName << "\n";

		_data_stream << "position" << "=";
		_data_stream << g_vec_pLightObjects[i]->position.x << ((CheckDecimal(g_vec_pLightObjects[i]->position.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->position.y << ((CheckDecimal(g_vec_pLightObjects[i]->position.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->position.z << ((CheckDecimal(g_vec_pLightObjects[i]->position.z)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->position.w << ((CheckDecimal(g_vec_pLightObjects[i]->position.w)) ? "f\n" : ".0f\n");

		_data_stream << "diffuse" << "=";
		_data_stream << g_vec_pLightObjects[i]->diffuse.r << ((CheckDecimal(g_vec_pLightObjects[i]->diffuse.r)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->diffuse.g << ((CheckDecimal(g_vec_pLightObjects[i]->diffuse.g)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->diffuse.b << ((CheckDecimal(g_vec_pLightObjects[i]->diffuse.b)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->diffuse.a << ((CheckDecimal(g_vec_pLightObjects[i]->diffuse.a)) ? "f\n" : ".0f\n");

		_data_stream << "specular" << "=";
		_data_stream << g_vec_pLightObjects[i]->specular.r << ((CheckDecimal(g_vec_pLightObjects[i]->specular.r)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->specular.g << ((CheckDecimal(g_vec_pLightObjects[i]->specular.g)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->specular.b << ((CheckDecimal(g_vec_pLightObjects[i]->specular.b)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->specular.a << ((CheckDecimal(g_vec_pLightObjects[i]->specular.a)) ? "f\n" : ".0f\n");

		_data_stream << "atten" << "=";
		_data_stream << g_vec_pLightObjects[i]->atten.x << ((CheckDecimal(g_vec_pLightObjects[i]->atten.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->atten.y << ((CheckDecimal(g_vec_pLightObjects[i]->atten.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->atten.z << ((CheckDecimal(g_vec_pLightObjects[i]->atten.z)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->atten.w << ((CheckDecimal(g_vec_pLightObjects[i]->atten.w)) ? "f\n" : ".0f\n");

		_data_stream << "direction" << "=";
		_data_stream << g_vec_pLightObjects[i]->direction.x << ((CheckDecimal(g_vec_pLightObjects[i]->direction.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->direction.y << ((CheckDecimal(g_vec_pLightObjects[i]->direction.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->direction.z << ((CheckDecimal(g_vec_pLightObjects[i]->direction.z)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->direction.w << ((CheckDecimal(g_vec_pLightObjects[i]->direction.w)) ? "f\n" : ".0f\n");

		_data_stream << "param1" << "=";
		_data_stream << g_vec_pLightObjects[i]->param1.x << ((CheckDecimal(g_vec_pLightObjects[i]->param1.x)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->param1.y << ((CheckDecimal(g_vec_pLightObjects[i]->param1.y)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->param1.z << ((CheckDecimal(g_vec_pLightObjects[i]->param1.z)) ? "f " : ".0f ");
		_data_stream << g_vec_pLightObjects[i]->param1.w << ((CheckDecimal(g_vec_pLightObjects[i]->param1.w)) ? "f\n" : ".0f\n");

		if (i != g_vec_pLightObjects.size() - 1) {
			_data_stream << "param2" << "=";
			_data_stream << g_vec_pLightObjects[i]->param2.x << ((CheckDecimal(g_vec_pLightObjects[i]->param2.x)) ? "f " : ".0f ");
			_data_stream << g_vec_pLightObjects[i]->param2.y << ((CheckDecimal(g_vec_pLightObjects[i]->param2.y)) ? "f " : ".0f ");
			_data_stream << g_vec_pLightObjects[i]->param2.z << ((CheckDecimal(g_vec_pLightObjects[i]->param2.z)) ? "f " : ".0f ");
			_data_stream << g_vec_pLightObjects[i]->param2.w << ((CheckDecimal(g_vec_pLightObjects[i]->param2.w)) ? "f\n" : ".0f\n");
			_data_stream << "\n";
		}
		else {
			_data_stream << "param2" << "=";
			_data_stream << g_vec_pLightObjects[i]->param2.x << ((CheckDecimal(g_vec_pLightObjects[i]->param2.x)) ? "f " : ".0f ");
			_data_stream << g_vec_pLightObjects[i]->param2.y << ((CheckDecimal(g_vec_pLightObjects[i]->param2.y)) ? "f " : ".0f ");
			_data_stream << g_vec_pLightObjects[i]->param2.z << ((CheckDecimal(g_vec_pLightObjects[i]->param2.z)) ? "f " : ".0f ");
			_data_stream << g_vec_pLightObjects[i]->param2.w << ((CheckDecimal(g_vec_pLightObjects[i]->param2.w)) ? "f" : ".0f");
		}
	}
}

void cFileManager::FormatComplexModelData() {
	_data_stream << "[COMPLEX]" << "\n";

	for (size_t i = 0; i < g_vec_pComplexObjects.size(); ++i) {
		_data_stream << "friendlyName" << "=" << g_vec_pComplexObjects[i]->friendlyName << "\n";

		std::vector<cLightObject*> vpLightObjects = g_vec_pComplexObjects[i]->GetLights();
		for (cLightObject* pclo : vpLightObjects)
			_data_stream << "light" << "=" << pclo->friendlyName << "\n";

		std::vector<cModelObject*> vpModelObjects = g_vec_pComplexObjects[i]->GetModels();
		for (cModelObject* pcmo : vpModelObjects)
			_data_stream << "model" << "=" << pcmo->friendlyName << "\n";

		_data_stream << "scale" << "=" << ((CheckDecimal(g_vec_pComplexObjects[i]->GetScale())) ? "f\n" : ".0f\n");
		_data_stream << "position" << "=";
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->`ion().x)) ? "f " : ".0f ");
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetPosition().y)) ? "f " : ".0f ");
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetPosition().z)) ? "f\n" : ".0f\n");

		_data_stream << "velocity" << "=";
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetVelocity().x)) ? "f " : ".0f ");
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetVelocity().y)) ? "f " : ".0f ");
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetVelocity().z)) ? "f\n" : ".0f\n");

		_data_stream << "accel" << "=";
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetAcceleration().x)) ? "f " : ".0f ");
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetAcceleration().y)) ? "f " : ".0f ");
		//_data_stream << ((CheckDecimal(g_vec_pComplexObjects[i]->GetAcceleration().z)) ? "f\n" : ".0f\n");

		if (i != g_vec_pComplexObjects.size() - 1) {
			//_data_stream << "inverseMass" << "=" << ((CheckDecimal(g_vec_pComplexObjects[i]->GetInverseMass())) ? "f\n" : ".0f\n");
			_data_stream << "\n";
		}
		//else
			//_data_stream << "inverseMass" << "=" << ((CheckDecimal(g_vec_pComplexObjects[i]->GetInverseMass())) ? "f" : ".0f");
	}
}

void cFileManager::ConvertStringToFloatArray(std::string cs) {
	stringstream ss;
	float tf;
	ss << cs;
	while (!ss.eof()) {
		for (float fl; ss >> fl; )
			vf.push_back(fl);
		if (!(ss >> tf)) {
			ss.clear();
			string discard;
			ss >> discard;
		}
	}
}

void cFileManager::ConvertStringToFloat(std::string cs) {
	stringstream ss;
	ss << cs;
	ss >> f;
}

void cFileManager::ConvertStringToInt(std::string cs) {
	stringstream ss;
	ss << cs;
	ss >> n;
}

void cFileManager::ResetConversionVars() {
	vf.clear();
	f = 0.0f;
	n = 0;
}

bool cFileManager::CheckDecimal(float num) {
	std::stringstream ss;
	ss << num;
	if (ss.str().find('.') != std::string::npos)
		return true;
	return false;
}

// IF NOT FOUND RETURNS 0 ( NULL )
eShapeTypes cFileManager::DetermineShapeType(std::string type, std::map<std::string, eShapeTypes>& mpShapeTypes) { return mpShapeTypes[type]; }

std::string cFileManager::GetShapeTypeString(eShapeTypes shapeType) {
	std::string shapeString;
	switch (shapeType) {
	case AABB:
		// WHY???
		break;
	case SPHERE:
		shapeString = "SPHERE";
		break;
	case CUBE:
		shapeString = "CUBE";
		break;
	case PLANE:
		shapeString = "PLANE";
		break;
	case MESH:
		shapeString = "MESH";
		break;
	case UNKNOWN:
		shapeString = "UNKNOWN";
		break;
	default:
		// call error routine should never hit default case
		break;
	}
	return shapeString;
}

void cFileManager::LoadLuaScripts() {
	LoadData(_script_File);

	std::string tempName, tempFile, tempScript;
	size_t scriptSize, erasedElements;

	for (size_t i = 0; i < _data.size(); ++i) {
		if (!_data[i].empty() && _data[i].front() == '[' && _data[i].back() == ']')
			_config_node = _data[i];

		if (_data[i] != _config_node) {
			if (_data[i] != "") {
				_lhs = _data[i].substr(0, _data[i].find('='));
				_rhs = _data[i].substr(_data[i].find('=') + 1, _data[i].size());

				if (_config_node == "[SCRIPTS]") {
					if (_lhs == "name")
						tempName = _rhs;
					if (_lhs == "path") {
						tempFile = _rhs;
						_input.open(tempFile);

						if (!_input.is_open()) {
							errmngr.DisplayError("cFileManager", __func__, "invalidfile", "Unable to open specified file.");
							return;
						}

						std::stringstream ss;
						ss << _input.rdbuf();
						tempScript = ss.str();
						_input.close();

						scriptSize = tempScript.length();
						erasedElements = std::count(tempScript.begin(), tempScript.end(), '\n');
						std::remove(tempScript.begin(), tempScript.end(), '\n');
						tempScript.resize((scriptSize - erasedElements));

						scriptSize = tempScript.length();
						erasedElements = std::count(tempScript.begin(), tempScript.end(), '\t');
						std::remove(tempScript.begin(), tempScript.end(), '\t');
						tempScript.resize((scriptSize - erasedElements));

						std::replace(tempScript.begin(), tempScript.end(), ';', '\n');

						ss.str(std::string());
						mpScripts[tempName] = tempScript;
					}
				}
			}
			else {
				tempName = "";
				tempFile = "";
				tempScript = "";
				scriptSize = 0;
				erasedElements = 0;
			}
		}
	}
}

bool cFileManager::sEntitySerializer::deserialize(const rapidjson::Document* document) {
	const auto& doc = *document;
	if (doc["Entities"].IsArray()) {
		int numEntities = doc["Entities"].Size();
		if (numEntities == 0) return false;
		Entities.resize(numEntities);
		for (size_t i = 0; i < numEntities; i++) {
			if (doc["Entities"][i]["Name"].IsString()) {
				Entities[i].Name = doc["Entities"][i]["Name"].GetString();
			}
			//Physics Component Loop
			if (doc["Entities"][i].HasMember("Physics") && doc["Entities"][i]["Physics"].IsArray()) {
				int numPhysics = doc["Entities"][i]["Physics"].Size();
				Entities[i].PhysicsComponents.resize(numPhysics);
				for (size_t j = 0; j < numPhysics; j++) {
					if (doc["Entities"][i]["Physics"][j].HasMember("Shape") && doc["Entities"][i]["Physics"][j]["Shape"].IsString()) {
						std::string shape = doc["Entities"][i]["Physics"][j]["Shape"].GetString();
						if (shape == "Ball") Entities[i].PhysicsComponents[j].shape = nPhysics::eComponentType::ball;
						if (shape == "Plane") Entities[i].PhysicsComponents[j].shape = nPhysics::eComponentType::plane;
					}
					if (doc["Entities"][i]["Physics"][j].HasMember("Position") && doc["Entities"][i]["Physics"][j]["Position"].IsString()) {
						std::string in = doc["Entities"][i]["Physics"][j]["Position"].GetString();
						stonFileMngr.ConvertStringToFloatArray(in);
						Entities[i].PhysicsComponents[j].position = glm::vec3(stonFileMngr.vf[0], stonFileMngr.vf[1], stonFileMngr.vf[2]);
						stonFileMngr.ResetConversionVars();
					}
					if (doc["Entities"][i]["Physics"][j].HasMember("Normal") && doc["Entities"][i]["Physics"][j]["Normal"].IsString()) {
						std::string in = doc["Entities"][i]["Physics"][j]["Normal"].GetString();
						stonFileMngr.ConvertStringToFloatArray(in);
						Entities[i].PhysicsComponents[j].normal = glm::vec3(stonFileMngr.vf[0], stonFileMngr.vf[1], stonFileMngr.vf[2]);
						stonFileMngr.ResetConversionVars();
					}
					if (doc["Entities"][i]["Physics"][j].HasMember("Velocity") && doc["Entities"][i]["Physics"][j]["Velocity"].IsString()) {
						std::string in = doc["Entities"][i]["Physics"][j]["Velocity"].GetString();
						stonFileMngr.ConvertStringToFloatArray(in);
						Entities[i].PhysicsComponents[j].velocity = glm::vec3(stonFileMngr.vf[0], stonFileMngr.vf[1], stonFileMngr.vf[2]);
						stonFileMngr.ResetConversionVars();
					}
					if (doc["Entities"][i]["Physics"][j].HasMember("Radius") && doc["Entities"][i]["Physics"][j]["Radius"].IsString()) {
						std::string in = doc["Entities"][i]["Physics"][j]["Radius"].GetString();
						stonFileMngr.ConvertStringToFloat(in);
						Entities[i].PhysicsComponents[j].radius = stonFileMngr.f;
						stonFileMngr.ResetConversionVars();
					}
					if (doc["Entities"][i]["Physics"][j].HasMember("Mass") && doc["Entities"][i]["Physics"][j]["Mass"].IsString()) {
						std::string in = doc["Entities"][i]["Physics"][j]["Mass"].GetString();
						stonFileMngr.ConvertStringToFloat(in);
						Entities[i].PhysicsComponents[j].mass = stonFileMngr.f;
						stonFileMngr.ResetConversionVars();
					}
				}
			}// Physics
			// Graphics
			if (doc["Entities"][i].HasMember("Graphics") && doc["Entities"][i]["Graphics"].IsArray()) {
				int numGraphics = doc["Entities"][i]["Graphics"].Size();
				Entities[i].GraphicsComponents.resize(numGraphics);
				for (size_t j = 0; j < numGraphics; j++) {
					if (doc["Entities"][i]["Graphics"][j]["Name"].IsString()) {
						Entities[i].GraphicsComponents[j].name = doc["Entities"][i]["Graphics"][j]["Name"].GetString();
					}
					if (doc["Entities"][i]["Graphics"][j]["Mesh"].IsString()) {
						Entities[i].GraphicsComponents[j].mesh = doc["Entities"][i]["Graphics"][j]["Mesh"].GetString();
					}
					if (doc["Entities"][i]["Graphics"][j]["Texture"].IsString()) {
						Entities[i].GraphicsComponents[j].texture = doc["Entities"][i]["Graphics"][j]["Texture"].GetString();
					}
					if (doc["Entities"][i]["Graphics"][j]["Scale"].IsString()) {
						std::string in = doc["Entities"][i]["Graphics"][j]["Scale"].GetString();
						stonFileMngr.ConvertStringToFloat(in);
						Entities[i].GraphicsComponents[j].scale = stonFileMngr.f;
						stonFileMngr.ResetConversionVars();
					}
				}
			}// Graphics
		}
	}
	return true;
}