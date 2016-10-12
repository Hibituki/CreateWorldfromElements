////////////////////////////////////////////////////////////
//
//			�t�@�C�����[�_�[�p�w�b�_�t�@�C��
//
//
//
////////////////////////////////////////////////////////////


#ifndef INCLUDED_CWE_FILELOADER_H
#define INCLUDED_CWE_FILELOADER_H



#include "CWEMath.h"
#include <fstream>
#include <vector>



namespace CWE
{


// PMX�t�@�C���f�[�^�^
struct PMXData
{
	// ���_�f�[�^�^
	struct Vertex
	{
		// �E�G�C�g���^
		struct Weight
		{
			// �E�G�C�g���̃^�C�v
			enum class Type
			{
				BDEF1,
				BDEF2,
				BDEF4,
				SDEF,
				Unknown,
			}m_Type;

			int m_Born1,
				m_Born2,
				m_Born3,
				m_Born4;
			float m_Weight1,
				  m_Weight2,
				  m_Weight3,
				  m_Weight4;
			math::Float3 m_C,
						 m_R0,
						 m_R1;
		};//Weight

		
		//���_�ʒu
		math::Float3 m_VertexV;
		//�@��
		math::Float3 m_NormalV;
		//UV���W
		math::Float2 m_UV;
		//�ǉ�UV���W
		std::vector<math::Float4> m_AddingUV;
		//�E�G�C�g���
		Weight m_Weight;
		//�G�b�W�{��
		float m_EdgeMagnif;
	};//Vertex

	// �ʃf�[�^�^
	struct Surface
	{
		int m_VertexIndex;
	};//Surface

	// �}�e���A���f�[�^�^
	struct Material
	{
		math::Float4 m_Diffuse; //���s��
		math::Float3 m_Specular; //�g�U��
		float m_Specularity; //�g�U�W��
		math::Float3 m_Ambient; //����

		int m_TextureIndex; //�e�N�X�`���̎Q�ƃC���f�b�N�X
		//�X�t�B�A�e�N�X�`���͔�Ή�
		int m_ToonTextureIndex; //�g�[���e�N�X�`���̎Q�ƃC���f�b�N�X

		int m_VertexNum; //�ގ��ɑΉ����钸�_��
	};//Material

	// �{�[���f�[�^�^
	struct Bone
	{
		std::wstring m_Name; //�{�[����(vmd�t�@�C���Ƃ̏ƍ��p)
		std::string m_NameE; //�p�{�[����(�V)
		math::Float3 m_Position; //�ʒu
		int m_FatherIndex; //�e�{�[���̃{�[���C���f�b�N�X
		int m_TransformationLevel; //�ό`�K�w
		unsigned short m_Flag; //�{�[���t���O
		math::Float3 m_CoordOffset; //�{�[���ʒu����̑��ΓI�ȍ��W�I�t�Z�b�g
		int m_SonIndex; //�ڑ���̃{�[���C���f�b�N�X
		int m_GrantParentIndex; //�t�^�e�{�[���̃{�[���C���f�b�N�X
		float m_GrantRate; //�t�^��
		math::Float3 m_FixedAxis; //�Œ莲�̕����x�N�g��
		math::Float3 m_LocalAxisX; //���[�J��X���̕����x�N�g��
		math::Float3 m_LocalAxisZ; //���[�J��Z���̕����x�N�g��
		int m_ExternalParentKey;  //�O���e�L�[
		int m_IKTarget; //IK�^�[�Q�b�g�̃{�[���C���f�b�N�X
		int m_IKLoop; //IK���[�v��
		float m_IKUnitAngle; //IK���[�v�v�Z��1�񓖂���̐����p�x
		struct IKLink
		{
			int m_LinkIndex; //�����N�{�[���̃{�[���C���f�b�N�X
			bool m_IsAngleLimited; //�p�x���E�����邩
			math::Float3 m_LimitAngleMin; //�����p(x,y,z)
			math::Float3 m_LimitAngleMax; //����p(x,y,z)
		};
		std::vector<IKLink> m_IKLinks; //IK�̃����N�{�[���f�[�^
	};//Bone


	// ���_�f�[�^
	std::vector<Vertex> m_Vertex;
	// �ʃf�[�^
	std::vector<Surface> m_Surface;
	// �e�N�X�`���p�X
	std::vector<std::wstring> m_TexturePath;
	// �}�e���A���f�[�^
	std::vector<Material> m_Material;
	// �{�[���f�[�^
	std::vector<Bone> m_Bone;

};


// VMD�t�@�C���f�[�^�^
struct VMDData
{
	// VMD�t�@�C���̃t���[���f�[�^�^
	struct FrameData
	{
		std::wstring m_BoneName; //�{�[����
		unsigned m_FrameNumber; //�t���[���ԍ�(���t���[���ڂ̏�����)
		math::Float3 m_BonePosition; //�{�[���̈ʒu
		math::Quaternion m_BoneQuaternion; //�{�[���̃N�H�[�^�j�I��
		char m_InterpolationParam[64]; //��ԃp�����[�^
	};

	std::vector<FrameData> m_FrameData;
};






// �t�@�C���S�ʂ̃��[�h�N���X
class FileLoader
{
private:
	std::ifstream m_File;

	// PMX�t�@�C����UTF-16�e�L�X�g�f�[�^�����C�h������Ɋi�[���Ď擾
	// (�K�v�ɂȂ�����UTF-8�ɑΉ�)
	bool PMXGetString16(std::wstring &dest/*, unsigned char encodeType*/);

	// �t�@�C���̃t���p�X(���΃p�X)����f�B���N�g���p�X�𒊏o����
	void DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[]);
	void DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[], unsigned fullPathLen);

public:
	FileLoader():m_File(){}
	~FileLoader(){}


	// PMX�t�@�C���p
	bool PMX(PMXData& data, const wchar_t file[]);
	// VMD�t�@�C���p
	bool VMD(VMDData& data, const wchar_t file[]);
	
};



}// CWE



#endif