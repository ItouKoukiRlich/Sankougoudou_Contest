#ifndef ___FADE_H___
#define ___FADE_H___

class Fade
{
public:
	Fade();
	virtual ~Fade();
	void Update();
	void Draw();
	void Start();

	bool IsFinish();
	bool IsFadeIn();
	bool IsFadeOut();
	float GetRate();
	float GetAlpha();
private:
	bool m_isFadeIn;
	float m_time;
	float m_maxTime;
};

#endif