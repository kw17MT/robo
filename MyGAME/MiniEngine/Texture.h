#pragma once
class GraphicsEngine;
extern GraphicsEngine* g_graphicsEngine;	//グラフィックスエンジン

class Texture  : public IShaderResource{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Texture() {}
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Texture();

	/// <summary>
	/// ファイルからテクスチャをロードするコンストラクタ
	/// </summary>
	/// <param name="filePath">ロードするテクスチャのファイルパス。</param>
	explicit Texture(const wchar_t* filePath);
	
	/// <summary>
	/// DDSファイルからテクスチャを初期化する。
	/// </summary>
	/// <param name="filePath">ロードするテクスチャのファイルパス。</param>
	void InitFromDDSFile(const wchar_t* filePath);
	/// <summary>
	/// メモリからテクスチャを初期化する。
	/// </summary>
	/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス</param>
	/// <param name="size">テクスチャのサイズ。</param>
	void InitFromMemory(const char* memory, unsigned int size);
	/// <summary>
	/// D3Dリソースからテクスチャを初期化する。
	/// </summary>
	/// <param name="resrouce">D3Dリソース。</param>
	void InitFromD3DResource(ID3D12Resource* texture);
	/// <summary>
	/// SRVに登録。
	/// </summary>
	/// <param name="descriptorHandle"></param>
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo) ;
	/// <summary>
	/// テクスチャが有効か判定。
	/// </summary>
	/// <returns>trueが返ってきたら有効。</returns>
	bool IsValid() const
	{
		return m_texture != nullptr;
	}
	ID3D12Resource* Get()
	{
		return m_texture;

	}

	//テクスチャの幅を取得。
	int GetWidth() const
	{
		return static_cast<int>(m_textureDesc.Width);
	}
	
	//テクスチャの高さを取得。
	int GetHeight() const
	{
		return static_cast<int>(m_textureDesc.Height);
	}

	//テクスチャのフォーマットを取得。
	DXGI_FORMAT GetFormat() const
	{
		return m_textureDesc.Format;
	}
private:
	/// <summary>
	/// DDSファイルからテクスチャをロード。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	void LoadTextureFromDDSFile(const wchar_t* filePath);
	/// <summary>
	/// メモリからテクスチャをロード。
	/// </summary>
	/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス。</param>
	/// <param name="size">テクスチャのサイズ。</param>
	/// <param name="ge12">Dx12版のグラフィックスエンジン</param>
	/// <param name="device">D3Dデバイス</param>
	void LoadTextureFromMemory(const char* memory, unsigned int size );
		
	/// <summary>
	/// D3D12オブジェクトを解放。
	/// </summary>
	/// <param name="obj">開放したいオブジェクト</param>
	//static inline void ReleaseD3D12Object(IUnknown* obj)
	//{
	//	if (obj == nullptr) {
	//		return;
	//	}
	//	if (g_graphicsEngine) {
	//		g_graphicsEngine->ReleaseD3D12Object(obj);
	//	}
	//	else {
	//		obj->Release();
	//	}
	//}

	//void Release();

private:
	ID3D12Resource*	m_texture = nullptr;	//テクスチャ。
	D3D12_RESOURCE_DESC m_textureDesc;	//テクスチャ情報
	bool m_isCubemap = false;
};