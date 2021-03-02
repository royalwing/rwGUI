#include "WaveSequenceGraph.h"
#define _USE_MATH_DEFINES
#include <math.h>

void WaveSequenceGraph::Init()
{
	SetBackgroundColor(Color(0.2f, 0.2f, 0.3f, 1.0f));
	SetWaveColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	SetPadding(16, 16, 32, 16);
	bInteractive = true;
	Scale = 100.0f;
}

void WaveSequenceGraph::Update(float DeltaTime)
{
	SetSize(this->GetOuterBounds().Size.x, 100);
}

String TimeToString(double Time)
{
	int Scaler = Time / abs(Time);
	Time = abs(Time);
	int Minutes = round(Time / 60);
	int Seconds = int(round(Time)) % 60;
	int Milliseconds = round((Time - round(Time)) * 1000);
	return (Scaler < 0 ? String("-") : String("") )+String::FromInteger(Minutes, 3) + String(":") + String::FromInteger(Seconds, 2) + "." + String::FromInteger(Milliseconds, 4);
}

void WaveSequenceGraph::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	Vector2D Pos = GetBounds().Pos;
	Vector2D Size = GetBounds().Size;
	renderTarget->FillRectangle(GetBounds().ToD2DRect(), BackgroundBrush);
	for (int x = 1; x < Size.x; x++)
	{
		float Time = GetTimeAtPixel(x);
		float Height = rwMath::lerp(Size.y*0.2, Size.y*0.8, (sin(Time*M_PI*2) + 1.0) / 2.0);

		float PrevTime = GetTimeAtPixel(x-1);
		float PrevHeight = rwMath::lerp(Size.y * 0.2, Size.y * 0.8, (sin(PrevTime * M_PI * 2) + 1.0) / 2.0);
		renderTarget->DrawLine(Pos + Vector2D(x-1, PrevHeight), Pos + Vector2D(x, Height), WaveBrush, 0.5f);
	}

	float StartTime = GetTimeAtPixel(0);
	float EndTime = GetTimeAtPixel(Size.x);


	float Timespan = (EndTime - StartTime);
	float Timestep = 0.1f;
	while(Timespan/40>Timestep)
	{
		Timestep *= 10.0f;
	}

	float BackstepTime = StartTime - round(StartTime/Timestep)*Timestep;

	StartTime -= BackstepTime;
	EndTime -= BackstepTime;

	IDWriteTextFormat* textFormat = MAKETEXTFORMAT("Arial", 12, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_EXTRA_EXPANDED);
	IDWriteTextLayout* textLayout = nullptr;

	for (double Time = StartTime; Time < EndTime; Time += Timestep*0.1)
	{
		int x = GetPixelAtTime(Time);
		if (x<0 || x>Size.x) continue;


		renderTarget->DrawLine(Pos + Size * Vector2D(0.0f, 1.0f) + Vector2D(x, 0.0f), Pos + Size * Vector2D(0.0f, 0.95f) + Vector2D(x, 0.0f), WaveBrush, 1.0);
	}

	for (double Time = StartTime; Time < EndTime; Time += Timestep)
	{
		int x = GetPixelAtTime(Time);
		if (x<0 || x>Size.x) continue;


		renderTarget->DrawLine(Pos+Size*Vector2D(0.0f, 1.0f)+Vector2D(x, 0.0f),Pos+Size*Vector2D(0.0f, 0.9f) + Vector2D(x, 0.0f), WaveBrush, 1.0f);
		
		String TimeLabel = TimeToString(Time);
		d2d->GetWriteFactory()->CreateTextLayout(TimeLabel.ToWideString(), TimeLabel.Length(), textFormat, GetBounds().Size.x, GetBounds().Size.y, &textLayout);

		DWRITE_TEXT_METRICS metrics;
		if (textLayout != nullptr)
		{
			textLayout->GetMetrics(&metrics);
			Vector2D StartPos = GetBounds().Pos + GetBounds().Size * Vector2D(0.0, 1.0) + Vector2D(x, 0.0);
			renderTarget->DrawText(TimeLabel.ToWideString(), TimeLabel.Length(), textFormat, Bounds(StartPos.x, StartPos.y, metrics.width, metrics.height).ToD2DRect(), WaveBrush);
			textLayout->Release();
		}
	}

	String ScaleText = String("Scale : ")+String::FromInteger(Scale);
	d2d->GetWriteFactory()->CreateTextLayout(ScaleText.ToWideString(), ScaleText.Length(), textFormat, GetBounds().Size.x, GetBounds().Size.y, &textLayout);

	DWRITE_TEXT_METRICS metrics;
	if (textLayout != nullptr)
	{
		textLayout->GetMetrics(&metrics);
		renderTarget->DrawText(ScaleText.ToWideString(), ScaleText.Length(), textFormat, Bounds(Pos.x+8, Pos.y+8, metrics.width, metrics.height).ToD2DRect(), WaveBrush);
		textLayout->Release();
	}
}

void WaveSequenceGraph::SetWaveColor(Color inColor)
{
	if (WaveColor != inColor)
	{
		WaveColor = inColor;
		if (WaveBrush.isSet())
			WaveBrush.Reset();
		if (ID2D1HwndRenderTarget* RT = GetApplication()->GetRenderer()->GetRenderTarget())
		{
			RT->CreateSolidColorBrush(WaveColor.ToD2D1ColorF(), D2D1::BrushProperties(), WaveBrush);
		};
	}
}

void WaveSequenceGraph::SetBackgroundColor(Color inColor)
{
	if (BackgroundColor != inColor)
	{
		BackgroundColor = inColor;
		if (BackgroundBrush.isSet())
			BackgroundBrush.Reset();
		if (ID2D1HwndRenderTarget* RT = GetApplication()->GetRenderer()->GetRenderTarget())
		{
			RT->CreateSolidColorBrush(BackgroundColor.ToD2D1ColorF(), D2D1::BrushProperties(), BackgroundBrush);
		};
	}
}

void WaveSequenceGraph::OnMouseMove(const Vector2D& PrevPosition, const Vector2D& Position)
{
	if(GetApplication()->GetCurrentHoveredDrawable()==this)
	{
		if (GetApplication()->IsKeyDown(VK_LBUTTON))
		{
			float DeltaX = Position.x - PrevPosition.x;
			Offset -= DeltaX / Scale;
		}
	}
}

void WaveSequenceGraph::OnMouseWheel(float Delta)
{

	if (GetApplication()->GetCurrentHoveredDrawable() == this)
	{
		Vector2D CurrentPosition = GetBounds().Pos - GetApplication()->GetLocalMousePosition();
		float TargetTime = GetTimeAtPixel(CurrentPosition.x);
		Scale += Delta * 5.0f * sqrt(Scale);
		Scale = max(Scale, 0.001);
		float NewTargetTime = GetTimeAtPixel(CurrentPosition.x);
		Offset += NewTargetTime - TargetTime;
	}
}

double WaveSequenceGraph::GetTimeAtPixel(int x) const
{
	return Offset + ((double)(x) / Scale);
}

int WaveSequenceGraph::GetPixelAtTime(double Time) const
{
	return (Time - Offset) * Scale;
}
