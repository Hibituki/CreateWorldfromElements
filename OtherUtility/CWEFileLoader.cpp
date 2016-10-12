#include "CWEFileLoader.h"




namespace CWE
{


// =================== �����g�p�֐� ===========================================


// PMX�t�@�C���̃e�L�X�g�����C�h������ɕϊ����Ď擾
bool FileLoader::PMXGetString16(std::wstring &dest/*, unsigned char encodeType*/)
{
//	char buffer[1024] ={};
	wchar_t wBuffer[512] ={};
	int textSize;

	// �e�L�X�g�T�C�Y�̎擾
	m_File.read(reinterpret_cast<char*>(&textSize), 4);

	// UTF16
//	if(encodeType == 0)
//	{
	m_File.read(reinterpret_cast<char*>(&wBuffer), textSize);
	dest =std::wstring(wBuffer, wBuffer+textSize/2);
//	}
	// UTF8
//	else if(encodeType == 1)
//	{
		//for(int i=0; i<textSize; ++i) {buffer[i] =m_File.get();}
		//buffer[textSize] ='\0';
		//MultiByteToWideChar( CP_UTF8, 0, buffer, -1, dest, 1024);
//	}

	return true;
}





// �t�@�C���̃t���p�X(���΃p�X)����f�B���N�g���p�X�𒊏o����
void FileLoader::DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[])
{
	const int length =wcslen(fullPath);

	DirectoryExtraction(directoryPath, fullPath, length);
}

// �t�@�C���̃t���p�X(���΃p�X)����f�B���N�g���p�X�𒊏o����(�t���p�X�̒����w��t��)
void FileLoader::DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[], unsigned fullPathLen)
{
	if(!directoryPath) {return;}

	for(unsigned i=1; i<=fullPathLen; ++i)
	{
		if(fullPath[fullPathLen-i] == L'\\')
		{
			for(unsigned j=0; j<fullPathLen-i+1; ++j)
			{
				directoryPath[j] =fullPath[j];
			}
		}
	}
}




// ================== �O���g�p�֐� =============================================


// PMX�t�@�C���p
bool FileLoader::PMX(PMXData &data, const wchar_t file[])
{
	if(!file) {return false;}
	// �t�@�C���p�X�̒����̐���(512�����ȉ�)
	if(512 < wcslen(file)) {return false;}
	// �t�@�C���p�X�̃f�B���N�g���p�X���擾
	wchar_t directoryPath[512] ={};
	DirectoryExtraction(directoryPath, file);

	// �f�[�^�̏�����
	data.m_Vertex.clear();
	data.m_Surface.clear();
	data.m_TexturePath.clear();
	data.m_Material.clear();


	// �t�@�C���I�[�v��
	m_File.open(file, (std::ios::binary|std::ios::in));
	if(m_File.fail()) {m_File.close(); return false;}


	// �ꎞ�i�[�ϐ�
	// �w�b�_�[
	unsigned char pmxHeder[4];
	float version =0;
	unsigned char hederDataLength =0;
	unsigned char hederData[8];
	unsigned arrayLength =0;
	// ���_
	int vertexNum =0;
	unsigned char weightCMethod =0;
	// ��
	int surfaceNum =0;
	// �e�N�X�`��
	int textureNum =0;
	std::wstring path;
	// �}�e���A��
	int materialNum =0;
	unsigned char shareToonFlag =0;
	// �{�[��
	int boneNum =0;
	// ���[�t
	int morphNum =0;


	// �ǂݍ���
	// ==================== �w�b�_�[ ====================================
	for(int i=0; i<4; ++i) {pmxHeder[i] =m_File.get();}
	if(pmxHeder[0]!=0x50 || pmxHeder[1]!=0x4d || pmxHeder[2]!=0x58 || pmxHeder[3]!=0x20) {m_File.close(); return false;} //PMX�t�@�C���łȂ��ꍇ
	m_File.read(reinterpret_cast<char*>(&version), 4);
	if(version != 2.0f) {m_File.close(); return false;} //Version2.0�łȂ��ꍇ
	hederDataLength =m_File.get();
	if(hederDataLength != 8) {m_File.close(); return false;} //��Ή��̃w�b�_���̏ꍇ
	for(int i=0; i<8; ++i) {hederData[i] =m_File.get();}
	if(hederData[0] != 0) {m_File.close(); return false;} //UTF-8�͍��̂Ƃ����Ή�

	// ================ ���f�������R�����g(�ǂݔ�΂�) ==================
	for(int j=0; j<4; ++j)
	{
		m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
		for(unsigned i=0; i<arrayLength; ++i) {m_File.get();}
	}

	// ======================== ���_�� ==================================
	m_File.read(reinterpret_cast<char*>(&vertexNum), 4);

	// ======================== ���_ ====================================
	// ���_���Ƀ��T�C�Y
	data.m_Vertex.resize(vertexNum);
	// �ǉ�UV���Ƀ��T�C�Y
	if(hederData[1] != 0)
	{
		for(int i=0; i<vertexNum; ++i)
		{
			data.m_Vertex[i].m_AddingUV.resize(hederData[1]);
		}
	}

	// ���_�f�[�^�̓ǂݍ���
	for(int i=0; i<vertexNum; ++i)
	{
		m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_VertexV), 12);
		m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_NormalV), 12);
		m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_UV), 8);
		if(hederData[1] != 0)
		{
			for(int j=0; j<hederData[1]; ++j)
			{
				m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_AddingUV[j]), 16);
			}
		}
		weightCMethod =m_File.get();

		switch(weightCMethod)
		{
		case 0: //BDEF1
			data.m_Vertex[i].m_Weight.m_Type =PMXData::Vertex::Weight::Type::BDEF1;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born1), hederData[5]);
			data.m_Vertex[i].m_Weight.m_Born2 =-1;
			data.m_Vertex[i].m_Weight.m_Born3 =-1;
			data.m_Vertex[i].m_Weight.m_Born4 =-1;
			data.m_Vertex[i].m_Weight.m_Weight1 =1.0f;
			break;

		case 1: //BDEF2
			data.m_Vertex[i].m_Weight.m_Type =PMXData::Vertex::Weight::Type::BDEF2;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born1), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born2), hederData[5]);
			data.m_Vertex[i].m_Weight.m_Born3 =-1;
			data.m_Vertex[i].m_Weight.m_Born4 =-1;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Weight1), 4);
			data.m_Vertex[i].m_Weight.m_Weight2 =1.0f -data.m_Vertex[i].m_Weight.m_Weight1;
			break;

		case 2: //BDEF4
			data.m_Vertex[i].m_Weight.m_Type =PMXData::Vertex::Weight::Type::BDEF4;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born1), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born2), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born3), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born4), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Weight1), 4);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Weight2), 4);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Weight3), 4);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Weight4), 4);
			break;

		case 3: //SDEF
			data.m_Vertex[i].m_Weight.m_Type =PMXData::Vertex::Weight::Type::SDEF;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born1), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Born2), hederData[5]);
			data.m_Vertex[i].m_Weight.m_Born3 =-1;
			data.m_Vertex[i].m_Weight.m_Born4 =-1;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_Weight1), 4);
			data.m_Vertex[i].m_Weight.m_Weight2 =1.0f -data.m_Vertex[i].m_Weight.m_Weight1;
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_C), 12);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_R0), 12);
			m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_Weight.m_R1), 12);
			break;

		default:
			m_File.close();
			return false;
			break;
		}

		m_File.read(reinterpret_cast<char*>(&data.m_Vertex[i].m_EdgeMagnif), 4);

		// ��Q�ƃ`�F�b�N
		if(data.m_Vertex[i].m_Weight.m_Born1==-1)
		{
			m_File.close();
			return false;
		}
	}


	// ======================= �ʐ� ==========================
	m_File.read(reinterpret_cast<char*>(&surfaceNum), 4);

	// ======================= �� ============================
	// �ʐ��Ƀ��T�C�Y
	data.m_Surface.resize(surfaceNum);
	// �ǂݍ���
	for(int i=0; i<surfaceNum; ++i)
	{
		m_File.read(reinterpret_cast<char*>(&data.m_Surface[i].m_VertexIndex), hederData[2]);

		// ��Q�ƃ`�F�b�N
		if(data.m_Surface[i].m_VertexIndex==-1 || data.m_Surface[i].m_VertexIndex==-1 || data.m_Surface[i].m_VertexIndex==-1)
		{
			m_File.close();
			return false;
		}
	}


	// ====================== �e�N�X�`���� ===================
	m_File.read(reinterpret_cast<char*>(&textureNum), 4);

	// ====================== �e�N�X�`�� =====================
	// �e�N�X�`�����Ƀ��T�C�Y
	data.m_TexturePath.resize(textureNum);
	// �ǂݍ���
	for(int i=0; i<textureNum; ++i)
	{
		PMXGetString16(path);
		data.m_TexturePath[i] =directoryPath;
		data.m_TexturePath[i] +=path;
	}


	// ====================== �}�e���A���� ====================
	m_File.read(reinterpret_cast<char*>(&materialNum), 4);

	// ====================== �}�e���A�� ======================
	// �}�e���A�����Ƀ��T�C�Y
	data.m_Material.resize(materialNum);
	// �ǂݍ���
	for(int i=0; i<materialNum; ++i)
	{
		// �ގ����͓ǂݔ�΂�
		for(int j=0; j<2; ++j)
		{
			m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
			for(unsigned i=0; i<arrayLength; ++i) {m_File.get();}
		}

		// ���C�e�B���O�f�[�^
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Diffuse), 16);
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Specular), 12);
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Specularity), 4);
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Ambient), 12);

		// �`��t���O�͓ǂݔ�΂�
		m_File.get();
		// �G�b�W�F�A�G�b�W�T�C�Y�͓ǂݔ�΂�
		for(int i=0; i<16; ++i) {m_File.get();}
		for(int i=0; i<4; ++i) {m_File.get();}

		// �e�N�X�`���̎Q�ƃC���f�b�N�X
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_TextureIndex), hederData[3]);
		// �X�t�B�A�e�N�X�`���̎Q�ƃC���f�b�N�X�͓ǂݔ�΂�
		for(unsigned char i=0; i<hederData[3]; ++i) {m_File.get();}
		// �X�t�B�A���[�h�͓ǂݔ�΂�
		m_File.get();
		// ���L�g�[���t���O�̓ǂݍ���
		shareToonFlag =m_File.get();
		if(shareToonFlag)
		{
			// ���L�g�[���͓ǂݔ�΂�
			m_File.get();
		}
		else
		{
			// �g�[���e�N�X�`���̎Q�ƃC���f�b�N�X
			m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_ToonTextureIndex), hederData[3]);
		}

		// �������̂��̑��̋L�q�͔�Ή�
		m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
		for(unsigned i=0; i<arrayLength; ++i) {m_File.get();}

		// �}�e���A���ɑΉ����钸�_��
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_VertexNum), 4);
	}


	// ======================== �{�[���� ========================
	m_File.read(reinterpret_cast<char*>(&boneNum), 4);

	// ======================== �{�[�� ==========================
	//�{�[�����Ƀ��T�C�Y
	data.m_Bone.resize(boneNum);
	// �ǂݍ���
	int ikLinkSize =0;
	unsigned char angleLim =0;
	for(int i=0; i<boneNum; ++i)
	{
		// �{�[����
		PMXGetString16(data.m_Bone[i].m_Name);
		m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
		data.m_Bone[i].m_NameE.resize(arrayLength);
		for(unsigned j=0; j<arrayLength; ++j)
		{
			data.m_Bone[i].m_NameE[j] =m_File.get();
		}
		// �ʒu
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_Position), 12);
		// �e�{�[��
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_FatherIndex), hederData[5]);
		if(boneNum <= data.m_Bone[i].m_FatherIndex)
		{
			data.m_Bone[i].m_FatherIndex =-1;
		}
		// �ό`�K�w
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_TransformationLevel), 4);
		//�{�[���t���O
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_Flag), 2);
		// �ڑ���
		if(data.m_Bone[i].m_Flag & 0x0001)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_SonIndex), hederData[5]);
			if(boneNum <= data.m_Bone[i].m_SonIndex)
			{
				data.m_Bone[i].m_SonIndex =-1;
			}
		}
		else
		{
			data.m_Bone[i].m_SonIndex =-1;
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_CoordOffset), 12);
		}
		// ��]�t�^����шړ��t�^
		if((data.m_Bone[i].m_Flag & 0x0100) || (data.m_Bone[i].m_Flag & 0x0200))
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_GrantParentIndex), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_GrantRate), 4);
		}
		// ���Œ�
		if(data.m_Bone[i].m_Flag & 0x0400)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_FixedAxis), 12);
		}
		// ���[�J����
		if(data.m_Bone[i].m_Flag & 0x0800)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_LocalAxisX), 12);
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_LocalAxisZ), 12);
		}
		// �O���e�ό`
		if(data.m_Bone[i].m_Flag & 0x2000)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_ExternalParentKey), 4);
		}
		// IK
		if(data.m_Bone[i].m_Flag & 0x0020)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_IKTarget), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_IKLoop), 4);
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_IKUnitAngle), 4);
			m_File.read(reinterpret_cast<char*>(&ikLinkSize), 4);
			data.m_Bone[i].m_IKLinks.resize(ikLinkSize);
			for(int j=0; j<ikLinkSize; ++j)
			{
				m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_IKLinks[j].m_LinkIndex), hederData[5]);
				angleLim =m_File.get();
				data.m_Bone[i].m_IKLinks[j].m_IsAngleLimited =false;
				if(angleLim == 1)
				{
					m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_IKLinks[j].m_LimitAngleMin), 12);
					m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_IKLinks[j].m_LimitAngleMax), 12);
					data.m_Bone[i].m_IKLinks[j].m_IsAngleLimited =true;
				}
			}
		}
		else
		{
			data.m_Bone[i].m_IKTarget =-1;
		}
	}


	// ======================== ���[�t�� ========================
	m_File.read(reinterpret_cast<char*>(&morphNum), 4);

	// ======================== ���[�t ==========================



	// �t�@�C���N���[�Y
	m_File.close();

	return true;
}


// VMD�t�@�C���p
bool FileLoader::VMD(VMDData& data, const wchar_t file[])
{
	if(!file) {return false;}

	// �f�[�^�̏�����
	data.m_FrameData.clear();

	// �t�@�C���I�[�v��
	m_File.open(file, (std::ios::binary|std::ios::in));
	if(m_File.fail()) {m_File.close(); return false;}


	// �w�b�_�̓ǂݍ���
	char magicNumber[30] ={};
	char magicNumberSample[30] ="Vocaloid Motion Data 0002";
	for(unsigned i=0; i<30; ++i) {magicNumber[i] =m_File.get();}
	if(strcmp(magicNumber, magicNumberSample) != 0) {m_File.close(); return false;} //VMD�t�@�C���łȂ��ꍇ
	for(unsigned i=0; i<20; ++i) {m_File.get();}
	unsigned frameDataNum =0;
	m_File.read(reinterpret_cast<char*>(&frameDataNum), 4);

	//�t���[���f�[�^�̓ǂݍ���
	data.m_FrameData.resize(frameDataNum);
	char boneNameMulti[15] ={};
	wchar_t boneNameWide[15] ={};
	size_t convertedChars =0;
	setlocale(LC_ALL, "jpn");
	for(unsigned i=0; i<frameDataNum; ++i)
	{
		// �{�[����
		for(unsigned j=0; j<15; ++j)
		{
			boneNameMulti[j] =m_File.get();
		}
		mbstowcs_s(&convertedChars, boneNameWide, 15, boneNameMulti, _TRUNCATE);
		data.m_FrameData[i].m_BoneName =std::wstring(boneNameWide, boneNameWide+wcslen(boneNameWide));
		convertedChars =0;
		// �t���[���ԍ�
		m_File.read(reinterpret_cast<char*>(&data.m_FrameData[i].m_FrameNumber), 4);
		// �{�[���̈ʒu
		m_File.read(reinterpret_cast<char*>(&data.m_FrameData[i].m_BonePosition), 12);
		// �{�[���̃N�H�[�^�j�I��
		m_File.read(reinterpret_cast<char*>(&data.m_FrameData[i].m_BoneQuaternion), 16);
		// ��ԃp�����[�^
		for(unsigned j=0; j<64; ++j)
		{
			data.m_FrameData[i].m_InterpolationParam[j] =m_File.get();
		}
	}

	setlocale(LC_ALL, "C");

	// �t�@�C���N���[�Y
	m_File.close();

	return true;
}



}// CWE