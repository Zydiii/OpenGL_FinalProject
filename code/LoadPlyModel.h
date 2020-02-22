#pragma once
#ifndef LOADPLYMODEL_H_
#define LOADPLYMODEL_H_

#include <vector>
#include <glad/glad.h>
using namespace std;

struct Vector3
{
	float x, y, z;
};

struct Point
{
	Vector3 vertex;
	Vector3 normal;
};

struct Face
{
	unsigned vertex1, vertex2, vertex3;
};

class PlyModel
{
public:
	PlyModel(char* filePath, bool multithread)
	{
		if (!multithread)
			Init(filePath);
		else
			InitMultiThread(filePath);
	}
	void Init(char *filePath);
	void Draw();
	void afterMultiInit();
	bool needInit();
	void InitMultiThread(char* filePath);

private:
	int vertexCount;
	int faceCount;
	vector<Vector3> vertex;
	vector<Point> vertexes;
	vector<Face> indexes;
	GLuint VBO, VAO, EBO;
	float* vertexData;
	bool inited = false;

};

#endif // !LOADPLYMODEL_H_
