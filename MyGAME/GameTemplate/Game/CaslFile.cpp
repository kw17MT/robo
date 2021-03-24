#include "stdafx.h"
#include "CaslFile.h"
#include <string>
//#include "ExEngine.h"

void CaslFile::Load(const char* filePath)
{
	auto fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		MessageBoxA(nullptr, "caslファイルのオープンに失敗しました。", "エラー", MB_OK);
		return;
	}

	int numLevel = ReadInteger(fp);
	for (int i = 0; i < numLevel; i++)
	{
		//std::unique_ptr<Level2DObject> levelOnject;と同じ意味
		//レベルファイルに保存してあるのは、以下全てchar型で保存してある。
		//一つ一つに(,)で区切りがしてある。
		//名前の長さ(int)、名前(char)、ファイルパスの長さ(int)、ファイルパス(char)
		//座標x,y(float)
		//画像の大きさx,y(int)
		//レイヤー優先度x,y(int)
		//スケール倍率x,y(float)
		//上の値が構造体に記録される。
		auto caslData = std::make_unique<CaslData>();

		//画像の名前の長さを取得する。
		int nameCount = ReadInteger(fp);
		//画像の名前を取得する。
		caslData.get()->name = std::make_unique<char[]>(nameCount + 1);
		fread(caslData.get()->name.get(), nameCount, 1, fp);
		ReadOnlyOneCharacter(fp);

		//ファイルパスの名前の長さを取得。
		int fileNameCount = ReadInteger(fp);
		//ファイルパスを取得。
		caslData.get()->filePath = std::make_unique<char[]>(fileNameCount + 1);
		fread(caslData.get()->filePath.get(), fileNameCount, 1, fp);
		ReadOnlyOneCharacter(fp);

		//座標を取得。
		caslData.get()->position.x = ReadDecimal(fp);
		caslData.get()->position.y = ReadDecimal(fp);

		//幅と高さを取得。
		caslData.get()->width = ReadInteger(fp);
		caslData.get()->height = ReadInteger(fp);

		//レイヤー優先度を取得。
		caslData.get()->numberLayer = ReadInteger(fp);

		//大きさの倍率を取得。
		caslData.get()->scale.x = ReadDecimal(fp);
		caslData.get()->scale.y = ReadDecimal(fp);

		//ddsファイルパスの名前の長さを取得。
		int ddsFileNameCount = ReadInteger(fp);
		//ddsファイルパスを取得。
		caslData.get()->ddsFilePath = std::make_unique<char[]>(ddsFileNameCount + 1);
		fread(caslData.get()->ddsFilePath.get(), ddsFileNameCount, 1, fp);


		//ReadOnlyOneCharacter(fp);
		//改行コードを読み飛ばし。
		char dummy[256];
		fgets(dummy, 256, fp);

		m_caslDataList.push_back(std::move(caslData));
	}
}

void CaslFile::ReadOnlyOneCharacter(FILE* file) const
{
	char a;
	fread(&a, 1, 1, file);
}

int CaslFile::ReadInteger(FILE* file) const
{
	std::string number;
	while (true)
	{
		char c;
		fread(&c, 1, 1, file);
		if (c != ',' && c != '\n')
		{
			number += c;
		}
		else {
			break;
		}
	}
	//string型をint型に変換する。
	return atoi(number.c_str());
}

float CaslFile::ReadDecimal(FILE* file) const
{
	std::string number;
	while (true)
	{
		char c;
		fread(&c, 1, 1, file);
		if (c != ',' && c != '\n')
		{
			number += c;
		}
		else {
			break;
		}
	}
	//string型をfloat型に変換する。
	return atof(number.c_str());
}