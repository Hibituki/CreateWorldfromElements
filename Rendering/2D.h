#pragma once


#include "Texture.h"




// GUI�I�u�W�F�N�g
class GUI
{
public:

	static void Set2D(); //2D�p�V�F�[�f�B���O���J�n(�O���[�o���֐��ɂ���̂�����)
	void Draw(float x, float y) const;

private:
	Texture m_Texture;

};