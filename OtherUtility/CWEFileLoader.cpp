#include "CWEFileLoader.h"




namespace CWE
{


// =================== 内部使用関数 ===========================================


// PMXファイルのテキストをワイド文字列に変換して取得
bool FileLoader::PMXGetString16(std::wstring &dest/*, unsigned char encodeType*/)
{
//	char buffer[1024] ={};
	wchar_t wBuffer[512] ={};
	int textSize;

	// テキストサイズの取得
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





// ファイルのフルパス(相対パス)からディレクトリパスを抽出する
void FileLoader::DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[])
{
	const int length =wcslen(fullPath);

	DirectoryExtraction(directoryPath, fullPath, length);
}

// ファイルのフルパス(相対パス)からディレクトリパスを抽出する(フルパスの長さ指定付き)
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




// ================== 外部使用関数 =============================================


// PMXファイル用
bool FileLoader::PMX(PMXData &data, const wchar_t file[])
{
	if(!file) {return false;}
	// ファイルパスの長さの制限(512文字以下)
	if(512 < wcslen(file)) {return false;}
	// ファイルパスのディレクトリパスを取得
	wchar_t directoryPath[512] ={};
	DirectoryExtraction(directoryPath, file);

	// データの初期化
	data.m_Vertex.clear();
	data.m_Surface.clear();
	data.m_TexturePath.clear();
	data.m_Material.clear();


	// ファイルオープン
	m_File.open(file, (std::ios::binary|std::ios::in));
	if(m_File.fail()) {m_File.close(); return false;}


	// 一時格納変数
	// ヘッダー
	unsigned char pmxHeder[4];
	float version =0;
	unsigned char hederDataLength =0;
	unsigned char hederData[8];
	unsigned arrayLength =0;
	// 頂点
	int vertexNum =0;
	unsigned char weightCMethod =0;
	// 面
	int surfaceNum =0;
	// テクスチャ
	int textureNum =0;
	std::wstring path;
	// マテリアル
	int materialNum =0;
	unsigned char shareToonFlag =0;
	// ボーン
	int boneNum =0;
	// モーフ
	int morphNum =0;


	// 読み込み
	// ==================== ヘッダー ====================================
	for(int i=0; i<4; ++i) {pmxHeder[i] =m_File.get();}
	if(pmxHeder[0]!=0x50 || pmxHeder[1]!=0x4d || pmxHeder[2]!=0x58 || pmxHeder[3]!=0x20) {m_File.close(); return false;} //PMXファイルでない場合
	m_File.read(reinterpret_cast<char*>(&version), 4);
	if(version != 2.0f) {m_File.close(); return false;} //Version2.0でない場合
	hederDataLength =m_File.get();
	if(hederDataLength != 8) {m_File.close(); return false;} //非対応のヘッダ情報の場合
	for(int i=0; i<8; ++i) {hederData[i] =m_File.get();}
	if(hederData[0] != 0) {m_File.close(); return false;} //UTF-8は今のところ非対応

	// ================ モデル名＆コメント(読み飛ばす) ==================
	for(int j=0; j<4; ++j)
	{
		m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
		for(unsigned i=0; i<arrayLength; ++i) {m_File.get();}
	}

	// ======================== 頂点数 ==================================
	m_File.read(reinterpret_cast<char*>(&vertexNum), 4);

	// ======================== 頂点 ====================================
	// 頂点数にリサイズ
	data.m_Vertex.resize(vertexNum);
	// 追加UV数にリサイズ
	if(hederData[1] != 0)
	{
		for(int i=0; i<vertexNum; ++i)
		{
			data.m_Vertex[i].m_AddingUV.resize(hederData[1]);
		}
	}

	// 頂点データの読み込み
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

		// 非参照チェック
		if(data.m_Vertex[i].m_Weight.m_Born1==-1)
		{
			m_File.close();
			return false;
		}
	}


	// ======================= 面数 ==========================
	m_File.read(reinterpret_cast<char*>(&surfaceNum), 4);

	// ======================= 面 ============================
	// 面数にリサイズ
	data.m_Surface.resize(surfaceNum);
	// 読み込み
	for(int i=0; i<surfaceNum; ++i)
	{
		m_File.read(reinterpret_cast<char*>(&data.m_Surface[i].m_VertexIndex), hederData[2]);

		// 非参照チェック
		if(data.m_Surface[i].m_VertexIndex==-1 || data.m_Surface[i].m_VertexIndex==-1 || data.m_Surface[i].m_VertexIndex==-1)
		{
			m_File.close();
			return false;
		}
	}


	// ====================== テクスチャ数 ===================
	m_File.read(reinterpret_cast<char*>(&textureNum), 4);

	// ====================== テクスチャ =====================
	// テクスチャ数にリサイズ
	data.m_TexturePath.resize(textureNum);
	// 読み込み
	for(int i=0; i<textureNum; ++i)
	{
		PMXGetString16(path);
		data.m_TexturePath[i] =directoryPath;
		data.m_TexturePath[i] +=path;
	}


	// ====================== マテリアル数 ====================
	m_File.read(reinterpret_cast<char*>(&materialNum), 4);

	// ====================== マテリアル ======================
	// マテリアル数にリサイズ
	data.m_Material.resize(materialNum);
	// 読み込み
	for(int i=0; i<materialNum; ++i)
	{
		// 材質名は読み飛ばす
		for(int j=0; j<2; ++j)
		{
			m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
			for(unsigned i=0; i<arrayLength; ++i) {m_File.get();}
		}

		// ライティングデータ
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Diffuse), 16);
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Specular), 12);
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Specularity), 4);
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_Ambient), 12);

		// 描画フラグは読み飛ばす
		m_File.get();
		// エッジ色、エッジサイズは読み飛ばす
		for(int i=0; i<16; ++i) {m_File.get();}
		for(int i=0; i<4; ++i) {m_File.get();}

		// テクスチャの参照インデックス
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_TextureIndex), hederData[3]);
		// スフィアテクスチャの参照インデックスは読み飛ばす
		for(unsigned char i=0; i<hederData[3]; ++i) {m_File.get();}
		// スフィアモードは読み飛ばす
		m_File.get();
		// 共有トーンフラグの読み込み
		shareToonFlag =m_File.get();
		if(shareToonFlag)
		{
			// 共有トーンは読み飛ばす
			m_File.get();
		}
		else
		{
			// トーンテクスチャの参照インデックス
			m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_ToonTextureIndex), hederData[3]);
		}

		// メモ等のその他の記述は非対応
		m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
		for(unsigned i=0; i<arrayLength; ++i) {m_File.get();}

		// マテリアルに対応する頂点数
		m_File.read(reinterpret_cast<char*>(&data.m_Material[i].m_VertexNum), 4);
	}


	// ======================== ボーン数 ========================
	m_File.read(reinterpret_cast<char*>(&boneNum), 4);

	// ======================== ボーン ==========================
	//ボーン数にリサイズ
	data.m_Bone.resize(boneNum);
	// 読み込み
	int ikLinkSize =0;
	unsigned char angleLim =0;
	for(int i=0; i<boneNum; ++i)
	{
		// ボーン名
		PMXGetString16(data.m_Bone[i].m_Name);
		m_File.read(reinterpret_cast<char*>(&arrayLength), 4);
		data.m_Bone[i].m_NameE.resize(arrayLength);
		for(unsigned j=0; j<arrayLength; ++j)
		{
			data.m_Bone[i].m_NameE[j] =m_File.get();
		}
		// 位置
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_Position), 12);
		// 親ボーン
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_FatherIndex), hederData[5]);
		if(boneNum <= data.m_Bone[i].m_FatherIndex)
		{
			data.m_Bone[i].m_FatherIndex =-1;
		}
		// 変形階層
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_TransformationLevel), 4);
		//ボーンフラグ
		m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_Flag), 2);
		// 接続先
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
		// 回転付与および移動付与
		if((data.m_Bone[i].m_Flag & 0x0100) || (data.m_Bone[i].m_Flag & 0x0200))
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_GrantParentIndex), hederData[5]);
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_GrantRate), 4);
		}
		// 軸固定
		if(data.m_Bone[i].m_Flag & 0x0400)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_FixedAxis), 12);
		}
		// ローカル軸
		if(data.m_Bone[i].m_Flag & 0x0800)
		{
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_LocalAxisX), 12);
			m_File.read(reinterpret_cast<char*>(&data.m_Bone[i].m_LocalAxisZ), 12);
		}
		// 外部親変形
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


	// ======================== モーフ数 ========================
	m_File.read(reinterpret_cast<char*>(&morphNum), 4);

	// ======================== モーフ ==========================



	// ファイルクローズ
	m_File.close();

	return true;
}


// VMDファイル用
bool FileLoader::VMD(VMDData& data, const wchar_t file[])
{
	if(!file) {return false;}

	// データの初期化
	data.m_FrameData.clear();

	// ファイルオープン
	m_File.open(file, (std::ios::binary|std::ios::in));
	if(m_File.fail()) {m_File.close(); return false;}


	// ヘッダの読み込み
	char magicNumber[30] ={};
	char magicNumberSample[30] ="Vocaloid Motion Data 0002";
	for(unsigned i=0; i<30; ++i) {magicNumber[i] =m_File.get();}
	if(strcmp(magicNumber, magicNumberSample) != 0) {m_File.close(); return false;} //VMDファイルでない場合
	for(unsigned i=0; i<20; ++i) {m_File.get();}
	unsigned frameDataNum =0;
	m_File.read(reinterpret_cast<char*>(&frameDataNum), 4);

	//フレームデータの読み込み
	data.m_FrameData.resize(frameDataNum);
	char boneNameMulti[15] ={};
	wchar_t boneNameWide[15] ={};
	size_t convertedChars =0;
	setlocale(LC_ALL, "jpn");
	for(unsigned i=0; i<frameDataNum; ++i)
	{
		// ボーン名
		for(unsigned j=0; j<15; ++j)
		{
			boneNameMulti[j] =m_File.get();
		}
		mbstowcs_s(&convertedChars, boneNameWide, 15, boneNameMulti, _TRUNCATE);
		data.m_FrameData[i].m_BoneName =std::wstring(boneNameWide, boneNameWide+wcslen(boneNameWide));
		convertedChars =0;
		// フレーム番号
		m_File.read(reinterpret_cast<char*>(&data.m_FrameData[i].m_FrameNumber), 4);
		// ボーンの位置
		m_File.read(reinterpret_cast<char*>(&data.m_FrameData[i].m_BonePosition), 12);
		// ボーンのクォータニオン
		m_File.read(reinterpret_cast<char*>(&data.m_FrameData[i].m_BoneQuaternion), 16);
		// 補間パラメータ
		for(unsigned j=0; j<64; ++j)
		{
			data.m_FrameData[i].m_InterpolationParam[j] =m_File.get();
		}
	}

	setlocale(LC_ALL, "C");

	// ファイルクローズ
	m_File.close();

	return true;
}



}// CWE