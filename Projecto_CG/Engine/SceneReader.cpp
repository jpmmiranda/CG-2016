#include "tinyxml2.h"
#include "SceneReader.h"
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

#include <valarray>

#pragma comment(lib,"glew32.lib")


using namespace std;
using namespace tinyxml2;

//variavel onde fica gravado o documento que abrimos
tinyxml2::XMLDocument xmlDoc;

//variavel que nos diz se um ficheiro ja se encontra carregado
bool loaded = false;
int toDraw = 0;

vector<model> allModels;
GLuint * vbuffer;


void exceptions(int e) {
	if (e == NO_NODES) printf("O ficheiro xml não contem nodos!\n");
	if (e == NO_GROUP_NODES) printf("O ficheiro xml não contem nodos de group!\n");
	if (e == EMPTY_FILE) printf("Ficheiro vazio.\n");
	if (e == NO_FILE_LOADED) printf("Erro: a aplicacao tentou desenhar o ficheiro sem o ter carregado anteriormente.\n");
	if (e == REPEATED_OPERATIONS)printf("Erro: so pode ter uma operacao geometrica de cada tipo num unico nodo group!\n");

}


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



void readScene(tinyxml2::XMLNode * orig, vector<model> *modelos) {

	//caso ao invocar a funcao recursivamente nao exista mais nenhum grupo este sai.
	if (orig == NULL) return;

	XMLElement * modelo;

	//obter o Nodo models
	XMLElement * modelo_aux = orig->FirstChildElement("models");
	

	if (modelo_aux) {

		modelo = modelo_aux->FirstChildElement("model");
		//ciclio que le e guarda os modelos numa variavel global
		while (modelo) {
			model Model;
			const char * name;
			name = modelo->Attribute("file");
			if (name) {

				Model = readTriangles(name);
				Model.i = modelos->size();
				modelos->push_back(Model);
				
			}
			
			modelo = modelo->NextSiblingElement("model");

		}
		
	}

	readScene(orig->FirstChildElement("group"), modelos);
	readScene(orig->NextSiblingElement("group"), modelos);
}


void prepareScene(char * filename) {
	vector<model> modelos;
	//carregar o ficheiro e verificar se este foi carregado com sucesso
	XMLError error = xmlDoc.LoadFile(filename);
	if (error != XML_SUCCESS) {
		throw EMPTY_FILE;
	}
	printf("Ficheiro %s carregado com sucesso\n", filename);
	loaded = true;
		//obter o nodo inicial scene e verificar se este esta presente 
		XMLNode *orig = xmlDoc.FirstChild();
		if (orig == NULL) throw NO_NODES;

			//obter o primeiro grupo e verificar se este se encontra presente
			orig = orig->FirstChildElement("group");
			if (orig == NULL)throw NO_GROUP_NODES;

				
				readScene(orig, &modelos);
				allModels = modelos;

				
				//parte do codigo que associa a cada modelo um buffer para mais tarde ser utilizado para os desenhar.
				vbuffer = new GLuint[allModels.size()];
				
				glGenBuffers(allModels.size(), vbuffer);
				typedef vector<model>::iterator it_type;
				int i = 0;
				for (it_type iterator = allModels.begin(); iterator != allModels.end(); iterator++, i++) {
					glBindBuffer(GL_ARRAY_BUFFER, vbuffer[i]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->vertexs.size()*3, &iterator->vertexs[0], GL_STATIC_DRAW);
				}
				
		
				
}



void drawModel(model Model) {
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer[Model.i]);
	
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, Model.vertexs.size() *3 );
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
}






//funcao que processao as transformacoes geometricas para depois se desenhar
void preparetoDraw(tinyxml2::XMLNode*orig) {
	unsigned int i = toDraw;

	//variaveis que nos dizem se ja fizemos alguma destas operacoes. Sao utilizadas para não deixar fazer duas operacoes do mesmo tipo uma vez que por grupo so podemos fazer uma operacao de cada tipo
	bool translate = false;
	bool scale = false;
	bool rotate = false;

	//caso nao existam mais Nodos filho damos pop a matrix para as que as operacoes geometricas que tinhamos carregado nao afectem os proximos grupos.
	if (orig == NULL) {
		glPopMatrix();
		return;
	}
	

	glPushMatrix();
	

	XMLElement *Op = orig->FirstChildElement();
	XMLElement * modelo;
	//obter o Nodo models
	XMLElement * modelo_aux = orig->FirstChildElement("models");
	

	
	if (modelo_aux) {

		modelo = modelo_aux->FirstChildElement("model");
		//ciclio que le e guarda os modelos numa variavel global
		while (modelo) {
			model Model;
			const char * name;
			name = modelo->Attribute("file");
			if (name) {
				toDraw++;
			}
			modelo = modelo->NextSiblingElement("model");
		}
	}
	
	
	while (Op) {
		

		if (strcmp("translate", Op->Name()) == 0) {
		
			if (translate)throw REPEATED_OPERATIONS;
			float x=0, y=0, z=0;
			Op->QueryFloatAttribute("X", &x);
			Op->QueryFloatAttribute("Y", &y);
			Op->QueryFloatAttribute("Z", &z);
			glTranslatef(x, y, z);
			translate = true;
		}

		else if (strcmp("scale", Op->Name()) == 0) {
			if (scale)throw REPEATED_OPERATIONS;
			float x = 0, y = 0, z = 0;
			Op->QueryFloatAttribute("X", &x);
			Op->QueryFloatAttribute("Y", &y);
			Op->QueryFloatAttribute("Z", &z);
			glScalef(x, y, z);
			scale = true;
		}

		else if (strcmp("rotate", Op->Name()) == 0) {
			if (scale)throw REPEATED_OPERATIONS;
			float x = 0, y = 0, z = 0, angle = 0;
			Op->QueryFloatAttribute("angle", &angle);
			Op->QueryFloatAttribute("X", &x);
			Op->QueryFloatAttribute("Y", &y);
			Op->QueryFloatAttribute("Z", &z);
			glRotatef(angle,x, y, z);
			rotate = true;
		}
				
		Op = Op->NextSiblingElement();
	}
	
	for (i; i < toDraw; i++) {
		
		drawModel(allModels[i]);
	}
	
	preparetoDraw(orig->FirstChildElement("group"));
	preparetoDraw(orig->NextSiblingElement("group"));
	
}


void drawScene(){
	toDraw = 0;
	//primeiro verificamos se o ficheiro ja se econtra carregado
	if (!loaded) throw NO_FILE_LOADED;
	
	//caso se encontre carregamos o primeiro nodo group  do ficheiro e enviamos-lo para a funcao preparetoDraw
	XMLNode *orig = xmlDoc.FirstChildElement();
	
	preparetoDraw(orig->FirstChildElement("group"));
	
}
