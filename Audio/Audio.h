#pragma once


#include <XAudio2.h>



// �����I�u�W�F�N�g�N���X
template<class tFileType>
class Sound
{
public:
	Sound();
	~Sound();
	Sound();

private:
	tFileType m_Source;

};


// �����t�@�C���t�H�[�}�b�g���N���X
class Source
{
public:
	Source() {}
	virtual ~Source() {}

	virtual void Read() =0;
	

private:

};


// WAVE�t�@�C���t�H�[�}�b�g�Ǘ��N���X
class WAVE
{
public:
	WAVE();
	~WAVE();

private:

};