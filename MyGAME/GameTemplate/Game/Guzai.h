#pragma once
class ModelRender;
class Kitchen;
class SpriteRender;



class PathMove;



class Guzai : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	CharacterController m_charaCon;
	
	//具材を初期位置（生成器）に移動させたか。
	bool isSetFirstPos = false;
	//Update()でインクリメントする。
	int timer = 0;
	//Xボタン長押しで積み上げている具材を全部削除する。そのためのタイマー。
	int DeleteTimer = 0;
	//どっち側に流れている具材か。
	int GuzaiNo = 0;
	//０．チーズ　１．エッグ　２．レタス　３．パテ　４．トマト
	int TypeNo = 9;
	//具材からプレイヤーへの距離
	float guzai2Pl = 0;
	//キッチンからプレイヤーへの距離
	float kit2Pl = 0.0f;
	
	//ターゲティング関連
	//ターゲティングされた具材を設定するよう。
	bool targeted = false;
	//1度しかダミー具材を出さないようにするための変数。1ならダミーをすでに出した。
	bool isSetTargetDummy = false;
	//一度ターゲッティングしたものをある程度見つめるための時間。
	//HOLDTIMEが待ち時間。DECREMENTTIMEが徐々に減らしていく役割がある。
	const int holdTime = 10;
	int decrementTime = holdTime;
	//ターゲットする最短距離。
	const int TargetRangeNear = 100;
	//ターゲッティングを切り始める距離。（実際はディレイがあるため消えるのはもうちょっと先になる）
	const int TargetRangeFar = 110;
	//枠線強調のために使用
	Vector3 GuzaiScale = { 1.0f,1.0f,1.0f };

	//以下追加コード。
	std::unique_ptr<PathMove> m_pathMove;		//移動処理を行うクラス。

public:	
	//乱数を用いて具材が生まれると同時にどの具材を流すか設定する。
	Guzai();
	~Guzai();
	bool Start();
	void Update();
	
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);
	void SetScale(Vector3 scale) { GuzaiScale = scale; }
	float GetGuzaiToPlayer() { return guzai2Pl; }

	//具材をキッチンに置いたときにオブジェクトを消すため、初めに設定しておいたTypeNoも消えてしまう。
	//つまり、レーン上に出したオブジェクトと新しくキッチン上に出すオブジェクトが違う。
	//プレイヤー側に保存しているTypeNoをキッチン側でNewGOするオブジェクトに与えてやる。
	//引数にプレイヤーに格納されている種類ナンバーを与える。
	void ChangeGuzai(int num);
	//どちら側に流れている具材か設定する。
	void SetGuzaiNo(int num) { GuzaiNo = num; }
	int GetGuzaiNo() { return GuzaiNo; }

	int GetTypeNo() { return TypeNo; }

	//具材の移動。
	void Move();
	//具材の搬送
	void GrabNPut();
	//ターゲティングの対象選定とターゲットダミーを出す。
	//離れると別のターゲットを探す。
	void TargetingNPopDummy();
	//ターゲティングされている具材の上に画像を配置。後に使用するかもしれないので実装しておく。削除するかも
	//void PopTargetingIcon();

	void Render(RenderContext& rc) { model.Draw(rc); }

	//１ならば持たれている。
	int state = 0;
	//１ならばもうキッチンに置かれている。
	int put = 0;

	ModelRender* pl01 = nullptr;
	ModelRender* pl02 = nullptr;
	Kitchen* ki01 = nullptr;
	Kitchen* ki02 = nullptr;
	//スプライトの3D空間表示が可能になったら使用
	/*SpriteRender* sp01 = nullptr;
	SpriteRender* sp02 = nullptr;*/
};

