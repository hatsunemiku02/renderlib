#include "renderapi.h"
#include "shader.h"
#include "shaderparam.h"
#include "resourcemgr/submesh.h"
#include "vertexbuffer.h"



void RenderApi::SetParam(const std::shared_ptr<Param>& pParam)
{
}

void RenderApi::SetShader(const std::shared_ptr<Shader>& pShader)
{
	error = glGetError();
	glUseProgram(pShader->GetGLShader());
	error = glGetError();
}

void RenderApi::SetSubMesh(const std::shared_ptr<SubMesh>& pSubMesh)
{
	glBindVertexArray(pSubMesh->GetVertexBuffer()->GetGLVertexBuffer());
	error = glGetError();
	drawelementstart = pSubMesh->GetStart();
	drawelementlength = pSubMesh->GetLength();
}

void RenderApi::Draw()
{
	glDrawElements(GL_TRIANGLES, drawelementlength, GL_UNSIGNED_INT, (void*)drawelementstart);
	error = glGetError();
}
