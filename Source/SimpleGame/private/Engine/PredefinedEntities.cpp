#include "PredefinedEntities.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/DynamicSpriteComponent.h"

SimpleBlock::SimpleBlock(String inName, World *inWorld)
    : Entity(inName, inWorld)
{
    Visual = CreateComponent<DynamicSpriteComponent>("Visual");
    Visual->SetSize(Vector2D(32, 32));
    Visual->SetSortOrder(1);
    Visual->SetOnDraw([](ID2D1BitmapRenderTarget* renderTarget){
        ID2D1SolidColorBrush *BorderBrush, *BackgroundBrush;
        renderTarget->CreateSolidColorBrush(Color(1.0f, 0.7f, 0.0f,1.0f).ToD2D1ColorF(), D2D1::BrushProperties(), &BorderBrush);
        renderTarget->CreateSolidColorBrush(Color(0.1f,0.1f,0.1f,1.0f).ToD2D1ColorF(), D2D1::BrushProperties(), &BackgroundBrush);

        Vector2D Size = renderTarget->GetSize();
        renderTarget->FillRectangle(Bounds(0, 0, Size.x, Size.y).ToD2DRect(), BackgroundBrush);
        
        renderTarget->DrawLine(Vector2D(0,0), Vector2D(Size.x, 0), BorderBrush); // Top
        renderTarget->DrawLine(Vector2D(0,Size.y), Size, BorderBrush); // Bottom
        renderTarget->DrawLine(Vector2D(0,0), Vector2D(0, Size.y), BorderBrush); // Left
        renderTarget->DrawLine(Vector2D(Size.x,0), Size, BorderBrush); // Bottom


        BorderBrush->Release();
        BackgroundBrush->Release();
    });
    
    Body = CreateComponent<PhysicsBodyComponent>("Body");
    Body->SetBox(Vector2D(32, 32));
}

SimpleBlock::~SimpleBlock()
{

}

void SimpleBlock::SetSize(Vector2D newSize)
{
    Body->SetBox(newSize);
    Visual->SetSize(newSize);
};

Vector2D SimpleBlock::GetSize() const
{
    return Body->GetBoxSize();
};