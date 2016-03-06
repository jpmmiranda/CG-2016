#include "tinyxml2.h"
#include "SceneReader.h"
#include <vector>
#include <GL/glut.h>
#include <valarray>

using namespace tinyxml2; 
using namespace std;

tinyxml2::XMLDocument xmlDoc;

vector<model> allModels;

bool FileLoaded = false;



static void readVertexs(XMLElement*element, model *model) {
	element = element->FirstChildElement("Vertex");
	while (element != NULL) {
		point p;
		element->QueryFloatAttribute("X", &p.x);
		element->QueryFloatAttribute("Y", &p.y);
		element->QueryFloatAttribute("Z", &p.z);

		model->vertexs.push_back(p);
		element = element->NextSiblingElement("Vertex");
	}
}




static model readTriangles(const char * filename) {
	using namespace tinyxml2;
	XMLDocument xmlDoc;
	model model;
	xmlDoc.LoadFile(filename);
	XMLNode * first = xmlDoc.FirstChild();
	XMLElement * element = first->FirstChildElement("Triangle");

	while (element != NULL) {		
		readVertexs(element, &model);
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

	
	XMLElement * modelo = orig->FirstChildElement("model");
	vector<model> modelos;

	while (modelo) {
		model Model;
		const char * name;
		name = modelo->Attribute("file");
		if (name) {
			
			Model = readTriangles(name);
			
		}
		printf("\n\n %s \n\n", name);
		modelos.push_back(Model);
		modelo = modelo->NextSiblingElement("model");

	}
	
	allModels = modelos;

}



 void drawScene(char *filename) {

	 typedef vector<model>::iterator it_type;
	 int color = 0;
	 for (it_type iterator = allModels.begin(); iterator != allModels.end(); ++iterator) {

		 glBegin(GL_TRIANGLES);
		 if(color%2==0) glColor3f(0, 0, 1);
		 else glColor3f(1, 1, 1);
		 for (int i = 0; i < iterator->vertexs.size(); i++) {
			 
			
			 glVertex3f(iterator->vertexs[i].x, iterator->vertexs[i].y, iterator->vertexs[i].z);
		 }
		 color++;
		 glEnd();
		 
	 }
		
				


}
