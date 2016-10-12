#pragma once


#include <XAudio2.h>



// 音声オブジェクトクラス
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


// 音声ファイルフォーマット基底クラス
class Source
{
public:
	Source() {}
	virtual ~Source() {}

	virtual void Read() =0;
	

private:

};


// WAVEファイルフォーマット管理クラス
class WAVE
{
public:
	WAVE();
	~WAVE();

private:

};