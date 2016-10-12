////////////////////////////////////////////////////////////
//
//			ファイルローダー用ヘッダファイル
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


// PMXファイルデータ型
struct PMXData
{
	// 頂点データ型
	struct Vertex
	{
		// ウエイト情報型
		struct Weight
		{
			// ウエイト情報のタイプ
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

		
		//頂点位置
		math::Float3 m_VertexV;
		//法線
		math::Float3 m_NormalV;
		//UV座標
		math::Float2 m_UV;
		//追加UV座標
		std::vector<math::Float4> m_AddingUV;
		//ウエイト情報
		Weight m_Weight;
		//エッジ倍率
		float m_EdgeMagnif;
	};//Vertex

	// 面データ型
	struct Surface
	{
		int m_VertexIndex;
	};//Surface

	// マテリアルデータ型
	struct Material
	{
		math::Float4 m_Diffuse; //平行光
		math::Float3 m_Specular; //拡散光
		float m_Specularity; //拡散係数
		math::Float3 m_Ambient; //環境光

		int m_TextureIndex; //テクスチャの参照インデックス
		//スフィアテクスチャは非対応
		int m_ToonTextureIndex; //トーンテクスチャの参照インデックス

		int m_VertexNum; //材質に対応する頂点数
	};//Material

	// ボーンデータ型
	struct Bone
	{
		std::wstring m_Name; //ボーン名(vmdファイルとの照合用)
		std::string m_NameE; //英ボーン名(〃)
		math::Float3 m_Position; //位置
		int m_FatherIndex; //親ボーンのボーンインデックス
		int m_TransformationLevel; //変形階層
		unsigned short m_Flag; //ボーンフラグ
		math::Float3 m_CoordOffset; //ボーン位置からの相対的な座標オフセット
		int m_SonIndex; //接続先のボーンインデックス
		int m_GrantParentIndex; //付与親ボーンのボーンインデックス
		float m_GrantRate; //付与率
		math::Float3 m_FixedAxis; //固定軸の方向ベクトル
		math::Float3 m_LocalAxisX; //ローカルX軸の方向ベクトル
		math::Float3 m_LocalAxisZ; //ローカルZ軸の方向ベクトル
		int m_ExternalParentKey;  //外部親キー
		int m_IKTarget; //IKターゲットのボーンインデックス
		int m_IKLoop; //IKループ回数
		float m_IKUnitAngle; //IKループ計算時1回当たりの制限角度
		struct IKLink
		{
			int m_LinkIndex; //リンクボーンのボーンインデックス
			bool m_IsAngleLimited; //角度限界があるか
			math::Float3 m_LimitAngleMin; //下限角(x,y,z)
			math::Float3 m_LimitAngleMax; //上限角(x,y,z)
		};
		std::vector<IKLink> m_IKLinks; //IKのリンクボーンデータ
	};//Bone


	// 頂点データ
	std::vector<Vertex> m_Vertex;
	// 面データ
	std::vector<Surface> m_Surface;
	// テクスチャパス
	std::vector<std::wstring> m_TexturePath;
	// マテリアルデータ
	std::vector<Material> m_Material;
	// ボーンデータ
	std::vector<Bone> m_Bone;

};


// VMDファイルデータ型
struct VMDData
{
	// VMDファイルのフレームデータ型
	struct FrameData
	{
		std::wstring m_BoneName; //ボーン名
		unsigned m_FrameNumber; //フレーム番号(何フレーム目の処理か)
		math::Float3 m_BonePosition; //ボーンの位置
		math::Quaternion m_BoneQuaternion; //ボーンのクォータニオン
		char m_InterpolationParam[64]; //補間パラメータ
	};

	std::vector<FrameData> m_FrameData;
};






// ファイル全般のロードクラス
class FileLoader
{
private:
	std::ifstream m_File;

	// PMXファイルのUTF-16テキストデータをワイド文字列に格納して取得
	// (必要になったらUTF-8に対応)
	bool PMXGetString16(std::wstring &dest/*, unsigned char encodeType*/);

	// ファイルのフルパス(相対パス)からディレクトリパスを抽出する
	void DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[]);
	void DirectoryExtraction(wchar_t directoryPath[], const wchar_t fullPath[], unsigned fullPathLen);

public:
	FileLoader():m_File(){}
	~FileLoader(){}


	// PMXファイル用
	bool PMX(PMXData& data, const wchar_t file[]);
	// VMDファイル用
	bool VMD(VMDData& data, const wchar_t file[]);
	
};



}// CWE



#endif