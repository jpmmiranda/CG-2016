#ifndef SceneReader

#define SceneReader
#include <vector>
#define NO_NODES 21
#define NO_GROUP_NODES 22
#define EMPTY_FILE 23
#define NO_FILE_LOADED 24
#define REPEATED_OPERATIONS 25


using namespace std;


typedef struct point {
	float x;
	float y;
	float z;
}point;

typedef struct model {
	vector<point> vertexs;
	int i;
	
	
}model;



void drawScene();
void prepareScene(char * filename);
void exceptions(int e);
#endif 

