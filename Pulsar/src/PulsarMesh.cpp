//#include <Pulsar.hpp>
#include <PulsarMesh.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace Pulsar;

bool Model::load(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path,
		aiProcess_CalcTangentSpace       |
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType		  |
	 	aiProcess_GenSmoothNormals);
	if(scene == NULL)
	{
		cout << "Faild to load model " << path << "." << endl;
		return 0;
	}
	//cout << "there are " << scene->mNumMeshes << " meshes in model" << endl;
	int meshCount = scene->mNumMeshes;
	for(int i=0;i<meshCount;i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int meshFaces = mesh->mNumFaces;
		Mesh* newMesh = new Mesh;
		int verticesNum = mesh->mNumVertices;

		//Copy from Assimp to a float array
		float* vertices = new float[mesh->mNumVertices*3];
		for(int j=0;j<verticesNum;j++)
		{
			vertices[j*3+0] = mesh->mVertices[j][0];
			vertices[j*3+1] = mesh->mVertices[j][1];
			vertices[j*3+2] = mesh->mVertices[j][2];

		}
		newMesh->setVertices((vec3*)vertices,verticesNum);

		unsigned int* indices = new unsigned int[meshFaces*3];
		for(int j=0;j<meshFaces;j++)
		{
			indices[j*3+0] = mesh->mFaces[j].mIndices[0];
			indices[j*3+1] = mesh->mFaces[j].mIndices[1];
			indices[j*3+2] = mesh->mFaces[j].mIndices[2];
		}

		newMesh->setIndices(indices,meshFaces*3);

		float* normals = new float[mesh->mNumVertices*3];
		if(mesh->HasNormals() == true)
		{
			for(int j=0;j<verticesNum;j++)
			{
				normals[j*3+0] = mesh->mNormals[j][0];
				normals[j*3+1] = mesh->mNormals[j][1];
				normals[j*3+2] = mesh->mNormals[j][2];
			}
		}
		else
			for(int j=0;j<verticesNum;j++)
			{
				normals[j*3+0] = 1;
				normals[j*3+1] = 1;
				normals[j*3+2] = 1;
			}
		newMesh->setNormals((vec3*)normals,verticesNum);

		float* uvs = new float[mesh->mNumVertices*2];
		for(int j=0;j<verticesNum;j++)
		{
			uvs[j*2+0] = 0;
			uvs[j*2+1] = 0;
		}

		newMesh->setTextureCoord((vec2*)uvs, verticesNum);

		delete [] vertices;
		delete [] indices;
		delete [] normals;
		delete [] uvs;
		meshList.push_back(newMesh);
	}
	return true;
}

void Model::render()
{
	int size = meshList.size();
	for(int i=0;i<size;i++)
		meshList[i]->render();
}

void Model::unload()
{
	int size = meshList.size();
	for(int i=0;i<size;i++)
		delete meshList[i];
	meshList.clear();
}

bool Model::good()
{
	if(meshList.size() != 0)
		return true;
	return false;
}

int Model::getMeshNum()
{
	return meshList.size();
}

const Mesh** Model::getMeshes()
{
	return (const Mesh**)&meshList[0];
}

Mesh::Mesh()
{
	glGenVertexArrays(1, &vao);
}

Mesh::~Mesh()
{
	if(vao != 0)
		glDeleteVertexArrays(1,&vao);
	if(vbo != 0)
		glDeleteBuffers(1,&vbo);
	if(ebo != 0)
		glDeleteBuffers(1,&ebo);
	if(tco != 0)
		glDeleteBuffers(1,&tco);
	if(nbo != 0)
		glDeleteBuffers(1,&nbo);
}

void Mesh::setVertices(vec3* vertices, int size)
{
	glBindVertexArray(vao);
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,size*sizeof(vec3),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::setIndices(unsigned int* indices, int size)
{
	dataSize = size;

	glBindVertexArray(vao);
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,size*sizeof(int),indices,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::setTextureCoord(vec2* uvCoord, int size)
{
	glBindVertexArray(vao);
	glGenBuffers(1,&tco);
	glBindBuffer(GL_ARRAY_BUFFER,tco);
	glBufferData(GL_ARRAY_BUFFER,size*sizeof(vec2),uvCoord,GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::setNormals(vec3* normals, int size)
{
	glBindVertexArray(vao);
	glGenBuffers(1,&nbo);
	glBindBuffer(GL_ARRAY_BUFFER,nbo);
	glBufferData(GL_ARRAY_BUFFER,size*sizeof(vec3),normals,GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::render()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//glDrawArrays(GL_TRIANGLES, 0, dataSize);
	glDrawElements(GL_TRIANGLES, dataSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
