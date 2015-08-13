#include "dragnumbox.h"
#include "mouse.h"

DragNumbox::DragNumbox(WContainer *par) :
    WComponent(par)
{
    onLeftDown = [&](){
        in_progress = true;
    };
    onLeftUp = [&](){
        in_progress = false;
    };
    pos = {40,150};
}

void DragNumbox::Draw() const
{
    SpriteBatch &sb = *WinS::sb;
    auto pos = globalPos();
    auto mpos = Mouse::getCursorPos();

    sb.drawText(std::to_string(value), pos + glm::vec2{5,5}, WinS::f, Color::White);
    DRAW_BOX(sb, pos, size, false);

    auto dif = (pos - mpos).y;
    auto col = dif >= 0 ? Color::Green : Color::Red;

    if(in_progress)
    {
        sb.resetScissor();
        sb.drawLine(pos, pos - glm::vec2{0,1}*dif, 2, col);
    }

    WComponent::Draw();
}

void DragNumbox::Update(const GameTimer &gt)
{
    auto pos = globalPos();

    if(in_progress)
    {
        auto mpos = Mouse::getCursorPos();
        auto dif = (pos - mpos).y * gt.elapsed;

        switch(g_type)
        {
        case LINEAR:
            value += growth * dif;
            break;
        case EXPONENTIAL:
            value *= dif < 0.f ? 1.0f / glm::pow(growth, dif) : glm::pow(growth, dif);
            break;
        }

        if(lim_type == LIMITED)
        {
            value = glm::clamp(value, min, max);
        }
    }

    WComponent::Update(gt);
}

