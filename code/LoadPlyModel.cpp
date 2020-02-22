#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "LoadPlyModel.h"

void normalize(Vector3 &v)
{
	float len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	v.x = v.x / len;
	v.y = v.y / len;
	v.z = v.z / len;
}

Vector3 operator-(Vector3 v1, Vector3 v2)
{
	Vector3 v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

Vector3 cross(Vector3 e1, Vector3 e2)
{
	Vector3 e;
	e.x = e1.y*e2.z - e1.z*e2.y;
	e.y = e1.z*e2.x - e1.x*e2.y;
	e.z = e1.x*e2.y - e1.y*e2.x;
	return e;
}

Vector3 CalculateNormal(Vector3 v1,Vector3 v2,Vector3 v3)
{
	Vector3 normal;
	Vector3 e1 = v2 - v1;
	Vector3 e2 = v3 - v1;
	normal = cross(e1, e2);
	normalize(normal);
	return normal;
}

void PlyModel::Init(char *filePath)
{
	printf("loading %s\n", filePath);
	FILE *file = fopen(filePath, "r");
	if (!file)
	{
		printf("load ply mode failed---%s\n", filePath);
		return;
	}
	else
	{
		char buffer[1000];
		fgets(buffer, 300, file);
		while (strncmp("element vertex",buffer,strlen("element vertex")!=0))
		{
			fgets(buffer, 300, file);
		}
		strcpy(buffer, buffer + strlen("element vertex"));
		sscanf(buffer, "%i", &vertexCount);
		//fseek(file, 0, SEEK_SET);
		fgets(buffer, 300, file);
		while (strncmp("element face",buffer,strlen("element face")!=0))
		{
			fgets(buffer, 300, file);
		}
		strcpy(buffer, buffer + strlen("element face"));
		sscanf(buffer, "%i", &faceCount);
		printf("vertex:%d face:%d\n", vertexCount, faceCount);
		while (strncmp("end_header",buffer,strlen("end_buffer")!=0))
		{
			fgets(buffer, 300, file);
		}
		for (int i=0;i<vertexCount;i++)
		{
			fgets(buffer, 300, file);
			Vector3 vertex_;
			sscanf(buffer, "%f %f %f", &vertex_.x, &vertex_.y, &vertex_.z);
			vertex.push_back(vertex_);
			//printf("%f %f %f\n", vertex_.x, vertex_.y, vertex_.z);
		}
		float *vertexData;
		vertexData = new float[faceCount * 18];
		for (int i=0;i<faceCount;i++)
		{
			fgets(buffer, 300, file);
			if (buffer[0]=='3')
			{
				int v1, v2, v3;
				buffer[0] = ' ';
				sscanf(buffer, "%i %i %i", &v1, &v2, &v3);
				//printf("face %i %i %i\n", v1, v2, v3);
				Point p1, p2, p3;
				p1.vertex = vertex[v1];
				p2.vertex = vertex[v2];
				p3.vertex = vertex[v3];
				Vector3 normal = CalculateNormal(vertex[v1], vertex[v2], vertex[v3]);
				p1.normal = normal;
				p2.normal = normal;
				p3.normal = normal;
				vertexData[i * 18] = p1.vertex.x;
				vertexData[i * 18 + 1] = p1.vertex.y;
				vertexData[i * 18 + 2] = p1.vertex.z;
				vertexData[i * 18 + 3] = p1.normal.x;
				vertexData[i * 18 + 4] = p1.normal.y;
				vertexData[i * 18 + 5] = p1.normal.z;

				vertexData[i * 18 + 6] = p2.vertex.x;
				vertexData[i * 18 + 7] = p2.vertex.y;
				vertexData[i * 18 + 8] = p2.vertex.z;
				vertexData[i * 18 + 9] = p2.normal.x;
				vertexData[i * 18 + 10] = p2.normal.y;
				vertexData[i * 18 + 11] = p2.normal.z;

				vertexData[i * 18 + 12] = p3.vertex.x;
				vertexData[i * 18 + 13] = p3.vertex.y;
				vertexData[i * 18 + 14] = p3.vertex.z;
				vertexData[i * 18 + 15] = p3.normal.x;
				vertexData[i * 18 + 16] = p3.normal.y;
				vertexData[i * 18 + 17] = p3.normal.z;
			}
		}
		fclose(file);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*faceCount * 18, vertexData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		printf("%s loaded!\n\n", filePath);
	}
}

void PlyModel::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, faceCount * 3);
	glBindVertexArray(0);
}

void PlyModel::InitMultiThread(char* filePath)
{
	printf("loading %s\n", filePath);
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		printf("load ply mode failed---%s\n", filePath);
		return;
	}
	else
	{
		char buffer[1000];
		fgets(buffer, 300, file);
		while (strncmp("element vertex", buffer, strlen("element vertex") != 0))
		{
			fgets(buffer, 300, file);
		}
		strcpy(buffer, buffer + strlen("element vertex"));
		sscanf(buffer, "%i", &vertexCount);
		//fseek(file, 0, SEEK_SET);
		fgets(buffer, 300, file);
		while (strncmp("element face", buffer, strlen("element face") != 0))
		{
			fgets(buffer, 300, file);
		}
		strcpy(buffer, buffer + strlen("element face"));
		sscanf(buffer, "%i", &faceCount);
		printf("vertex:%d face:%d\n", vertexCount, faceCount);
		while (strncmp("end_header", buffer, strlen("end_buffer") != 0))
		{
			fgets(buffer, 300, file);
		}
		for (int i = 0; i < vertexCount; i++)
		{
			fgets(buffer, 300, file);
			Vector3 vertex_;
			sscanf(buffer, "%f %f %f", &vertex_.x, &vertex_.y, &vertex_.z);
			vertex.push_back(vertex_);
			//printf("%f %f %f\n", vertex_.x, vertex_.y, vertex_.z);
		}
		vertexData = new float[faceCount * 18];
		for (int i = 0; i < faceCount; i++)
		{
			fgets(buffer, 300, file);
			if (buffer[0] == '3')
			{
				int v1, v2, v3;
				buffer[0] = ' ';
				sscanf(buffer, "%i %i %i", &v1, &v2, &v3);
				//printf("face %i %i %i\n", v1, v2, v3);
				Point p1, p2, p3;
				p1.vertex = vertex[v1];
				p2.vertex = vertex[v2];
				p3.vertex = vertex[v3];
				Vector3 normal = CalculateNormal(vertex[v1], vertex[v2], vertex[v3]);
				p1.normal = normal;
				p2.normal = normal;
				p3.normal = normal;
				vertexData[i * 18] = p1.vertex.x;
				vertexData[i * 18 + 1] = p1.vertex.y;
				vertexData[i * 18 + 2] = p1.vertex.z;
				vertexData[i * 18 + 3] = p1.normal.x;
				vertexData[i * 18 + 4] = p1.normal.y;
				vertexData[i * 18 + 5] = p1.normal.z;

				vertexData[i * 18 + 6] = p2.vertex.x;
				vertexData[i * 18 + 7] = p2.vertex.y;
				vertexData[i * 18 + 8] = p2.vertex.z;
				vertexData[i * 18 + 9] = p2.normal.x;
				vertexData[i * 18 + 10] = p2.normal.y;
				vertexData[i * 18 + 11] = p2.normal.z;

				vertexData[i * 18 + 12] = p3.vertex.x;
				vertexData[i * 18 + 13] = p3.vertex.y;
				vertexData[i * 18 + 14] = p3.vertex.z;
				vertexData[i * 18 + 15] = p3.normal.x;
				vertexData[i * 18 + 16] = p3.normal.y;
				vertexData[i * 18 + 17] = p3.normal.z;
			}
		}
		fclose(file);
		printf("%s loaded!\n\n", filePath);
	}
}
bool PlyModel::needInit() {
	return !inited;
}


void  PlyModel::afterMultiInit() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * faceCount * 18, vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	inited = true;
}