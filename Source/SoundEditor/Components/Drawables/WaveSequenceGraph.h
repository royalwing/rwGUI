#pragma once

#include <rwgui.h>

class WaveSequenceGraph : public Drawable
{
public:
	using Drawable::Drawable;

	virtual void Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;

	void SetWaveColor(Color inColor);
	Color GetWaveColor() const { return WaveColor; };

	void SetBackgroundColor(Color inColor);
	Color GetBackgroundColor() const { return BackgroundColor; };


	virtual void OnMouseMove(const Vector2D& PrevPosition, const Vector2D& Position) override;
	virtual void OnMouseWheel(float Delta) override;
private:
	Color BackgroundColor;
	Color WaveColor;

	double Offset = 0.0f;
	double Scale = 1.0f;
	
	double GetTimeAtPixel(int x) const;
	int GetPixelAtTime(double Time) const;

	rwD2DResourceContainer<ID2D1SolidColorBrush> WaveBrush;
	rwD2DResourceContainer<ID2D1SolidColorBrush> BackgroundBrush;
};