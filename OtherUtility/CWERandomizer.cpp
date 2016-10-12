////////////////////////////////////////////////////////////
//
//			乱数生成ソースファイル
//
//
//
////////////////////////////////////////////////////////////



#include "CWERandomizer.h"


namespace CWE
{



//======================= 乱数生成器 =================================

//コンストラクタ
Randomizer::Randomizer():
m_MersenneTwister(m_Seed()), m_UniDist(1, 100000)
{
}

//デストラクタ
Randomizer::~Randomizer()
{
}

//インスタンスの取得
Randomizer& Randomizer::GetInstance()
{
	static Randomizer instance;
	return instance;
}


//乱数生成器に使う初期シード値のリセット(乱数の精度の向上のため)
void Randomizer::ResetSeed()
{
	m_MersenneTwister =std::mt19937(m_Seed());
}


//乱数の取得
//(1～100000)
int Randomizer::GetRandom()
{
	return m_UniDist(m_MersenneTwister);
}



//====================== 一様分布(int) =================================

//コンストラクタ
Uniform_int::Uniform_int():
m_Randomizer(Randomizer::GetInstance()), m_UniDist(0, 100)
{
}

//引数付きコンストラクタ
Uniform_int::Uniform_int(const int& min, const int& max):
m_Randomizer(Randomizer::GetInstance()), m_UniDist(min, max)
{
}

//デストラクタ
Uniform_int::~Uniform_int()
{
}


//乱数の取得
int Uniform_int::GetRandom()
{
	return m_UniDist(m_Randomizer.m_MersenneTwister);
}



//====================== 一様分布(double) =================================

//コンストラクタ
Uniform_double::Uniform_double():
m_Randomizer(Randomizer::GetInstance()), m_UniDist(0.0, 1.0)
{

}

//引数付きコンストラクタ
Uniform_double::Uniform_double(const double& min, const double& max):
m_Randomizer(Randomizer::GetInstance()), m_UniDist(min, max)
{
}

//デストラクタ
Uniform_double::~Uniform_double()
{
}


//乱数の取得
double Uniform_double::GetRandom()
{
	return m_UniDist(m_Randomizer.m_MersenneTwister);
}



//====================== ベルヌーイ分布 =================================

//コンストラクタ
Bernoulli::Bernoulli():
m_Randomizer(Randomizer::GetInstance()), m_BernoDist(0.5)
{

}

//引数付きコンストラクタ
Bernoulli::Bernoulli(const double& probability):
m_Randomizer(Randomizer::GetInstance()), m_BernoDist(probability)
{
}

//デストラクタ
Bernoulli::~Bernoulli()
{
}


//乱数の取得
bool Bernoulli::GetRandom()
{
	return m_BernoDist(m_Randomizer.m_MersenneTwister);
}



}//CWE