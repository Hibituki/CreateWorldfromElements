
#include "OtherUtility\CWEUtility.h"



// ファイルのバイナリデータの取得
std::vector<unsigned char> GetBinaryData(const tchar* file)
{
	std::vector<unsigned char> result;
	using fst =::std::fstream;
	unsigned fSize{};
	std::basic_ifstream<unsigned char> iFile{file, fst::binary|fst::in};

	if(!iFile) {return result;}

	iFile.seekg(0, fst::end);
	fSize =iFile.tellg();
	iFile.clear();
	iFile.seekg(0, fst::beg);
	fSize -=iFile.tellg();

	result.resize(fSize);

	iFile.read(result, fSize);
	iFile.close();

	return result;
}