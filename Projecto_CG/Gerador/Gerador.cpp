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


static void drawConeinXML(float raio, float altura, float fatias, float camadas, char*filename) {
	using namespace tinyxml2;
	XMLNode * orig = xmlDoc.NewElement("Cone");
	xmlDoc.InsertEndChild(orig);

	point p1, p2, p3, p4;
	float h = altura / 2;

	for (float i = 0; i < 2 * M_PI; i += 2 * M_PI / fatias) {

		//base inferior
		p1.x = 0;
		p1.y = -h;
		p1.z = 0;

		p2.x = raio*sin(i);
		p2.y = -h;
		p2.z = raio *cos(i);

		p3.x = raio*sin(i + 2 * M_PI / fatias);
		p3.y = -h;

		p3.z = raio *cos(i + 2 * M_PI / fatias);

		writeTriangleinXML(orig, p1, p3, p2);

	}


	float r = raio; // raio atual (utlizado na construcao das camadas 
	float r1 = raio / camadas; // valor a retirar ao raio sempre que avancamos uma camada
	float l1 = -h;
	float l2;
	for (float l = (altura / camadas), l2 = -h+l; l2 < h; l2 += l) {
		for (float i = 0; i < 2 * M_PI; i += 2 * M_PI / fatias) {


			//lados

			p1.x = (r - r1)*sin(i);
			p1.y = l2;
			p1.z = (r - r1)*cos(i);

			p2.x = (r - r1)*sin(i + 2 * M_PI / fatias);
			p2.y = l2;
			p2.z = (r - r1)*cos(i + 2 * M_PI / fatias);

			p3.x = r * sin(i);
			p3.y = l1;
			p3.z = r * cos(i);

			p4.x = r * sin(i + 2 * M_PI / fatias);
			p4.y = l1;
			p4.z = r * cos(i + 2 * M_PI / fatias);

			writeTriangleinXML(orig, p3, p4, p2);
			writeTriangleinXML(orig, p2, p1, p3);
		}

		l1 = l2;
		r -= r1;

	}

	for (float i = 0; i < 2 * M_PI; i += 2 * M_PI / fatias) {
		//ultima camada
		p1.x = 0;
		p1.y = h;
		p1.z = 0;

		p2.x = r1 * sin(i);
		p2.y = h - altura / camadas;
		p2.z = r1 * cos(i);

		p3.x = r1 *sin(i + 2 * M_PI / fatias);
		p3.y = h - altura/ camadas;
		p3.z = r1 * cos(i + 2 * M_PI / fatias);

		writeTriangleinXML(orig, p1, p2, p3);

	}

		

	xmlDoc.SaveFile(filename);
}


static void drawSphereinXML(float raio, int fatias,int camadas,char*filename){
	using namespace tinyxml2;
	XMLNode * orig = xmlDoc.NewElement("Sphere");
	xmlDoc.InsertEndChild(orig);

	int theta, phi;
	point p1, p2, p3, p4;

	for (phi = 0; phi < fatias; phi++) {
		float phi1 = ((float)phi / fatias) * 2 * M_PI;
		float phi2 = ((float)(phi + 1) / fatias) * 2 * M_PI;

		for (theta = 0; theta < camadas; theta++) {
			float theta1 = ((float)(theta) / camadas)*M_PI;
			float theta2 = ((float)(theta + 1) / camadas)*M_PI;

			
			p1.x = (float)raio*sin(theta1)*sin(phi1);
			p1.y = (float)raio*cos(theta1);
			p1.z = (float)raio*sin(theta1)*cos(phi1);

			p2.x = (float)raio*sin(theta1)*sin(phi2);
			p2.y = (float)raio*cos(theta1);
			p2.z = (float)raio*sin(theta1)*cos(phi2);

			p3.x = (float)raio*sin(theta2)*sin(phi2);
			p3.y = (float)raio*cos(theta2);
			p3.z = (float)raio*sin(theta2)*cos(phi2);

			p4.x = (float)raio*sin(theta2)*sin(phi1);
			p4.y = (float)raio*cos(theta2);
			p4.z = (float)raio*sin(theta2)*cos(phi1);



			if (theta == 0) {
				writeTriangleinXML(orig, p1, p3, p4);
			}
			else if (theta + 1 == camadas) {
				writeTriangleinXML(orig, p3, p1, p2);
			}
			else {
				writeTriangleinXML(orig, p1, p2, p4);
				writeTriangleinXML(orig, p2, p3, p4);
			}
		}
	}
	xmlDoc.SaveFile(filename);
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
		
		else if (strcmp(argv[1], "sphere") == 0) {
			if (argc == 6) {
				drawSphereinXML((float)atoi(argv[2]), (float)atoi(argv[3]), (float)atoi(argv[4]), argv[5]);
				printf("\nFoi guardado no ficheiro %s uma esfera com raio %f, %f fatias e  %f camadas.\n", argv[5], (float)atoi(argv[2]), (float)atoi(argv[3]), (float)atoi(argv[4]));
			}
			else  printf("\nErro na quantidade/tipo de argumentos.\n");

		}
		else if (strcmp(argv[1], "cone") == 0) {
			if (argc == 7) {
				drawConeinXML((float)atoi(argv[2]), (float)atoi(argv[3]), (float)atoi(argv[4]), (float)atoi(argv[5]), argv[6]);
				printf("\nFoi guardado no ficheiro %s um cone com raio %f de base inferior de altura %f, com %f fatias e  %f camadas.\n", argv[6], (float)atoi(argv[2]), (float)atoi(argv[3]), (float)atoi(argv[4]), (float)atoi(argv[5]));
			}
			else printf("\nErro na quantidade/tipo de argumentos.\n");
		}
	}
	else printf("\n Quantidade de argumentos invalida.\n");

}
