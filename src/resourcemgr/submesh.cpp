#include <memory>
#include <vector>
#include "submesh.h"
#include "renderlib/vertexbuffer.h"

float vertices[] = {
    -1.0f, -1.0f, 1.0f, // 左下角
     1.0f, -1.0f, 1.0f,  // 右下角
     1.0f,  1.0f, 1.0f,   // 右上
    -1.0f,  1.0f, 1.0f,   //左上

    -1.0f, -1.0f, -1.0f, // 左下角
     1.0f, -1.0f, -1.0f,  // 右下角
     1.0f,  1.0f, -1.0f,   // 右上
    -1.0f,  1.0f, -1.0f,   //左上
};


float verticesquad[] = {
    -0.2f, -0.2f, 0.0f, // 左下角
     0.2f, -0.2f, 0.0f,  // 右下角
     0.2f,  0.2f, 0.0f,   // 右上
    -0.2f,  0.2f, 0.0f,   //左上
};

float uvs[] =
{
    0.0f,0.0f,
    0.0f,0.0f,
    0.0f,0.0f,
    0.0f,0.0f,

    0.0f,0.0f,
    0.0f,0.0f,
    0.0f,0.0f,
    0.0f,0.0f,
};

float uvsquad[] =
{
    0.0f,0.0f,
    0.0f,0.0f,
    0.0f,0.0f,
    0.0f,0.0f,

};

int idexs[] = {
    0,1,2,
    0,1,3,
    4,5,6,
    4,5,7,
    0,4,5,
    0,5,1,
    3,7,6,
    3,6,2,
    0,4,7,
    0,7,3,
    1,5,2,
    1,2,6,

};

unsigned int idexsquad[] = {
    0,1,2,
    0,1,3,
};



SubMesh::SubMesh()
	:m_Start(0)
	,m_Length(0)
	,m_pVertexBuffer()
{
}


SubMesh::SubMesh(const std::shared_ptr<VertexBuffer>& pVertexBuffer)
	:m_Start(0)
	, m_Length(0)
	, m_pVertexBuffer(pVertexBuffer)
{
}

SubMesh::~SubMesh()
{
}


void SubMesh::BuildDefaultBox()
{
    m_pVertexBuffer = std::make_shared<VertexBuffer>();

    std::vector<void*> buffers = { verticesquad ,uvsquad };
    std::vector<unsigned int> bufferssize = { 4*3 * sizeof(float) ,4*2 * sizeof(float) };
    std::vector<unsigned int> buffersstride = { 3 * sizeof(float) ,2 * sizeof(float) };

    m_pVertexBuffer->SetData(buffers, bufferssize, buffersstride, idexsquad, 6*sizeof(unsigned int));
    m_Length = 6;
}