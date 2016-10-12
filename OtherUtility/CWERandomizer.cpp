////////////////////////////////////////////////////////////
//
//			���������\�[�X�t�@�C��
//
//
//
////////////////////////////////////////////////////////////



#include "CWERandomizer.h"


namespace CWE
{



//======================= ���������� =================================

//�R���X�g���N�^
Randomizer::Randomizer():
m_MersenneTwister(m_Seed()), m_UniDist(1, 100000)
{
}

//�f�X�g���N�^
Randomizer::~Randomizer()
{
}

//�C���X�^���X�̎擾
Randomizer& Randomizer::GetInstance()
{
	static Randomizer instance;
	return instance;
}


//����������Ɏg�������V�[�h�l�̃��Z�b�g(�����̐��x�̌���̂���)
void Randomizer::ResetSeed()
{
	m_MersenneTwister =std::mt19937(m_Seed());
}


//�����̎擾
//(1�`100000)
int Randomizer::GetRandom()
{
	return m_UniDist(m_MersenneTwister);
}



//====================== ��l���z(int) =================================

//�R���X�g���N�^
Uniform_int::Uniform_int():
m_Randomizer(Randomizer::GetInstance()), m_UniDist(0, 100)
{
}

//�����t���R���X�g���N�^
Uniform_int::Uniform_int(const int& min, const int& max):
m_Randomizer(Randomizer::GetInstance()), m_UniDist(min, max)
{
}

//�f�X�g���N�^
Uniform_int::~Uniform_int()
{
}


//�����̎擾
int Uniform_int::GetRandom()
{
	return m_UniDist(m_Randomizer.m_MersenneTwister);
}



//====================== ��l���z(double) =================================

//�R���X�g���N�^
Uniform_double::Uniform_double():
m_Randomizer(Randomizer::GetInstance()), m_UniDist(0.0, 1.0)
{

}

//�����t���R���X�g���N�^
Uniform_double::Uniform_double(const double& min, const double& max):
m_Randomizer(Randomizer::GetInstance()), m_UniDist(min, max)
{
}

//�f�X�g���N�^
Uniform_double::~Uniform_double()
{
}


//�����̎擾
double Uniform_double::GetRandom()
{
	return m_UniDist(m_Randomizer.m_MersenneTwister);
}



//====================== �x���k�[�C���z =================================

//�R���X�g���N�^
Bernoulli::Bernoulli():
m_Randomizer(Randomizer::GetInstance()), m_BernoDist(0.5)
{

}

//�����t���R���X�g���N�^
Bernoulli::Bernoulli(const double& probability):
m_Randomizer(Randomizer::GetInstance()), m_BernoDist(probability)
{
}

//�f�X�g���N�^
Bernoulli::~Bernoulli()
{
}


//�����̎擾
bool Bernoulli::GetRandom()
{
	return m_BernoDist(m_Randomizer.m_MersenneTwister);
}



}//CWE