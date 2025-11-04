#include "ftxui_helpers.hpp"

Decorator FtxUiHelpers::get_gauge_color(const double& percent)
{
    Color::Palette256 gauge_color;
    if (percent < 45) // TODO: пересмотреть и расширить границы
    {
        gauge_color = CommonColors::ImportanceLevel::normal;
    }
    else if (percent >= 45 && percent <= 85)
    {
        gauge_color = CommonColors::ImportanceLevel::medium;
    }
    else
    {
        gauge_color = CommonColors::ImportanceLevel::critical;
    }

    return color(gauge_color);
}

Element FtxUiHelpers::hindent(int indent_size)
{
    return emptyElement() | size(WidthOrHeight::WIDTH, EQUAL, indent_size);
}

Element FtxUiHelpers::vindent(int indent_size)
{
    return emptyElement() | size(WidthOrHeight::HEIGHT, EQUAL, indent_size);
}