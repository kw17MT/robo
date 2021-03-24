#pragma once
#include <list>
#include <vector>

struct CaslData {
	//レベルの中の画像の名前
	std::unique_ptr<char[]> name;
	//レベルのファイルのパス。
	std::unique_ptr<char[]> filePath;
	//DDSのファイルのパス。
	std::unique_ptr<char[]> ddsFilePath;
	//座標、拡大率。
	Vector2 position;
	Vector2 scale;
	//横幅、縦幅、レイヤーの優先度
	int width;
	int height;
	int numberLayer = 0;
};
class CaslFile
{
private:
	//1文字だけ読み込むのに使う。
	// , や \n　を読み込むのに使う。
	void ReadOnlyOneCharacter(FILE* file) const;
	//int型を読み込むのに使う。
	int ReadInteger(FILE* file) const;
	//float型を読み込むのに使う。
	float ReadDecimal(FILE* file) const;

	std::vector<std::unique_ptr<CaslData>> m_caslDataList;		//Caslデータ。

public:
	//Caslファイルをロードする。
	void Load(const char* filePath);
	//Caslデータの数を返す。後ろのconstはこのメンバ関数はメンバ変数を変更しないという意味。
	const int GetNumCaslData() const
	{
		return m_caslDataList.size();
	}
	//number目のCaslデータを取得する。
	CaslData* GetCaslData(int number) const {
		return m_caslDataList[number].get();
	}
};

