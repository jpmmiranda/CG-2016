#include "tinyxml2.h"
#include "SceneReader.h"
#include <vector>
#include <GL/glut.h>
#include <valarray>

using namespace tinyxml2; 
using namespace std;

tinyxml2::XMLDocument xmlDoc;

vector<model> allModels;
vector<int> allModels2;
int *TriangleBuffer;
bool FileLoaded = false;

static model readTriangles(const char * filename) {
	XMLDocument xmlDoc2;
	model model;
	xmlDoc2.LoadFile(filename);
	XMLNode * first = xmlDoc2.FirstChild();
	XMLElement * element = first->FirstChildElement("Triangle");

	while (element != NULL) {
		element = element->FirstChildElement("Vertex");
		while (element != NULL) {
			float x = atoi(element->Attribute("X"));
			float y = atoi(element->Attribute("Y"));
			float z = atoi(element->Attribute("Z"));
			point p; p.x = x; p.y = y; p.z = z;
			model.vertexs.push_back(p);
			element = element->NextSiblingElement("Vertex");
		}
		element = element->NextSiblingElement("Triangle");
	}

	return model;
}


void readScene(char * filename) {
	int n = 0;
	XMLError error = xmlDoc.LoadFile(filename);

	if (error != XML_SUCCESS) {
		printf("Erro. %s \n", xmlDoc.ErrorName());
	}
	else printf("Ficheiro %s carregado com sucesso",filename);
	
	FileLoaded = true;
	XMLNode *orig = xmlDoc.FirstChild();
	orig = orig->FirstChildElement("Scene");
	XMLElement * modelo = orig->FirstChildElement("model");
	vector<model> modelos;

	while (modelo) {
		model Model;
		const char * name;
		name = modelo->Attribute("file");
		if (name) {
			Model = readTriangles(name);
			allModels2.push_back(n++);
		}
		
		modelos.push_back(Model);
		modelo = orig->NextSiblingElement("model");

	}
	
	allModels = modelos;

}



 void drawScene(char *filename) {
	for (int i = 0; i < allModels2.size();i++)
	glDrawArrays(GL_TRIANGLES, 0, allModels[allModels2[i]].vertexs.size()/3);




}
