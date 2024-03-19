#include "Billboard.h"
#include "utils.h"

void GE::Billboard::BindTexture(const GLuint* PIDref)
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLuint textID = glGetUniformLocation(*PIDref, "sampler");
	GLCall(glUniform1i(textID, 0));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture->getTexture()));
}
