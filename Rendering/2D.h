#pragma once


#include "Texture.h"




// GUIオブジェクト
class GUI
{
public:

	static void Set2D(); //2D用シェーディングを開始(グローバル関数にするのもあり)
	void Draw(float x, float y) const;

private:
	Texture m_Texture;

};