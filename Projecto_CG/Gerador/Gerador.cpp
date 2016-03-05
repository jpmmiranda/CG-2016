#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include "tinyxml2.h"
#include "Ponto.h"

tinyxml2::XMLDocument xmlDoc;


static void writeVertexinXMLasPoint(tinyxml2::XMLNode * orig, point p) {
	using namespace tinyxml2;
	
	XMLElement *element = xmlDoc.NewElement("Vertex");
	element->SetAttribute("X", p.x);
	element->SetAttribute("Y", p.y);
	element->SetAttribute("Z", p.z);
	orig->InsertEndChild(element);
}

static void writeTriangleinXML(tinyxml2::XMLNode * orig, point p1,point p2,point p3) {
	using namespace tinyxml2;
	XMLNode *triangle = xmlDoc.NewElement("Triangle");
	writeVertexinXMLasPoint(triangle, p1);
	writeVertexinXMLasPoint(triangle, p2);
	writeVertexinXMLasPoint(triangle, p3);
	orig->InsertEndChild(triangle);
}

static void drawPlaneinXML(float comprimento, float largura, char *filename) {
	using namespace tinyxml2;
	XMLNode * orig = xmlDoc.NewElement("Plane");
	xmlDoc.InsertEndChild(orig);

	point p1, p2, p3, p4;

	p1.x = -comprimento / 2;
	p1.y = 0;
	p1.z = largura / 2;

	p2.x = comprimento / 2;
	p2.y = 0;
	p2.z = largura / 2;


	p3.x = comprimento / 2;
	p3.y = 0;
	p3.z = -largura / 2;

	p4.x = -comprimento / 2;
	p4.y = 0;
	p4.z = -largura / 2;

	writeTriangleinXML(orig, p1, p2, p3);
	writeTriangleinXML(orig, p3, p4, p1);
	xmlDoc.SaveFile(filename);
		
}

static void drawBoxinXML(float comprimento, float largura, float altura, char * filename) {
	using namespace tinyxml2;
	XMLNode * orig = xmlDoc.NewElement("Box");
	xmlDoc.InsertEndChild(orig);

	point p1, p2, p3, p4, p5, p6, p7, p8;

	p1.x = -comprimento / 2;
	p1.y = altura / 2;
	p1.z = largura / 2;

	p2.x = comprimento / 2;
	p2.y = altura / 2;
	p2.z = largura / 2;

	p3.x = -comprimento / 2;
	p3.y = -altura / 2;
	p3.z = largura / 2;
	
	p4.x = comprimento / 2;
	p4.y = -altura / 2;
	p4.z = largura / 2;

	p5.x = -comprimento / 2;
	p5.y = altura / 2;
	p5.z = -largura / 2;

	p6.x = comprimento / 2;
	p6.y = altura / 2;
	p6.z = -largura / 2;

	p7.x = -comprimento / 2;
	p7.y = -altura / 2;
	p7.z = -largura / 2;

	p8.x = comprimento / 2;
	p8.y = -altura / 2;
	p8.z = -largura / 2;

	//face da frente
	writeTriangleinXML(orig, p3, p4, p2);
	writeTriangleinXML(orig, p2, p1, p3);


	//face de baixo
	writeTriangleinXML(orig, p7, p8, p4);
	writeTriangleinXML(orig, p4, p3, p7);

	//face de tras
	writeTriangleinXML(orig, p8, p7, p5);
	writeTriangleinXML(orig, p5, p6, p8);

	//face da direita
	writeTriangleinXML(orig, p4, p8, p6);
	writeTriangleinXML(orig, p6, p2, p4);

	//face da esquerda
	writeTriangleinXML(orig, p7, p3, p1);
	writeTriangleinXML(orig, p1, p5, p7);

	//face de cima
	writeTriangleinXML(orig, p1, p2, p6);
	writeTriangleinXML(orig, p6, p5, p1);

	xmlDoc.SaveFile(filename);

}


static void drawConeinXML(float raio, float altura, float slices, float stacks, char*filename) {
	using namespace tinyxml2;
	XMLNode * orig = xmlDoc.NewElement("Cone");
	xmlDoc.InsertEndChild(orig);

	point p1, p2, p3, p4;


}


int main(int argc, char *argv[]) {
	if (argc > 1) {
		if (strcmp(argv[1], "plane") == 0) {
			if(argc==5){
				drawPlaneinXML((float)atoi(argv[2]), (float)atoi(argv[3]), argv[4]);
				printf("\nFoi guardado no ficheiro %s um plano XZ com comprimento %f e largura %f.\n", argv[4], (float)atoi(argv[2]), (float)atoi(argv[3]));

			}
			else printf("\nErro na quantidade/tipo de argumentos.\n");
				
		}
		else if (strcmp(argv[1], "box") == 0) {
			if (argc == 6) {
				drawBoxinXML((float)atoi(argv[2]), (float)atoi(argv[3]), (float)atoi(argv[4]), argv[5]);
				printf("\nFoi guardado no ficheiro %s uma caixa com comprimento %f, largura %f e altura %f.\n", argv[5], (float)atoi(argv[2]), (float)atoi(argv[3]), (float)atoi(argv[4]));
			}
			else printf("\nErro na quantidade/tipo de argumentos.\n");

		}
	}
	else printf("\n Quantidade de argumentos invalida.\n");

}
