#include "Billboard.h"

void GE::Billboard::BindTexture(const GLuint* PIDref)
{
	GLuint textID = glGetUniformLocation(*PIDref, "sampler");
	glUniform1i(textID, 0);
	glBindTexture(GL_TEXTURE_2D, texture->getTexture());
}
