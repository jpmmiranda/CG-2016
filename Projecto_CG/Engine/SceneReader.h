#ifndef SceneReader

#define SceneReader
#include <vector>
using namespace std;


typedef struct point {
	float x;
	float y;
	float z;
}point;

typedef struct model {
	vector<point> vertexs;
	
	
}model;



void drawScene(char *filename);
void readScene(char * filename);
#endif 

