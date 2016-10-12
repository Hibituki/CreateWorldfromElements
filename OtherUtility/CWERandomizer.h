////////////////////////////////////////////////////////////
//
//			���������w�b�_�t�@�C��
//
//
//
////////////////////////////////////////////////////////////



#ifndef INCLUDED_CWE_OTHERUTILITY_RANDOMIZER_H
#define INCLUDED_CWE_OTHERUTILITY_RANDOMIZER_H


#include <random>


namespace CWE
{


//����������
class Randomizer
{
private:
	friend class Uniform_int;
	friend class Uniform_double;
	friend class Bernoulli;

	std::random_device m_Seed;
	std::mt19937 m_MersenneTwister;
	std::uniform_int_distribution<> m_UniDist;


	Randomizer();
	~Randomizer();

public:
	//�C���X�^���X�̎擾
	static Randomizer& GetInstance();
	Randomizer(const Randomizer&) =delete;
	Randomizer& operator=(const Randomizer&) =delete;

	//����������Ɏg�������V�[�h�l�̃��Z�b�g(�����̐��x�̌���̂���)
	void ResetSeed();

	//�����̎擾
	//(1�`100000)
	int GetRandom();

};



//��l���z����(int)
class Uniform_int
{
private:
	Randomizer& m_Randomizer;
	std::uniform_int_distribution<> m_UniDist;

public:
	//�f�t�H���g��0�`100
	Uniform_int();
	Uniform_int(const int& min, const int& max);
	~Uniform_int();

	//�����̎擾
	int GetRandom();

};


//��l���z����(double)
class Uniform_double
{
private:
	Randomizer& m_Randomizer;
	std::uniform_real_distribution<> m_UniDist;

public:
	//�f�t�H���g��0.0�`1.0
	Uniform_double();
	Uniform_double(const double& min, const double& max);
	~Uniform_double();

	//�����̎擾
	double GetRandom();

};


//�x���k�[�C���z����
class Bernoulli
{
private:
	Randomizer& m_Randomizer;
	std::bernoulli_distribution m_BernoDist;

public:
	//�f�t�H���g��0.5(50%)
	Bernoulli();
	//probability=true�ɂȂ�m��
	//(0.5=50%)
	Bernoulli(const double& probability);
	~Bernoulli();

	//�����̎擾
	bool GetRandom();

};


//���K���z�Ȃǂ̑��̕��z���@�͕K�v�ɂȂ�����ǉ�����



}//CWE


#endif