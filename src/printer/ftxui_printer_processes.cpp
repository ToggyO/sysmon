#include "common/conversions.h" // MEGABYTES
#include "ftxui_printer.hpp"
#include <ftxui/dom/elements.hpp>
#include <optional> // std::optional, std::nullopt, std::make_optional
#include <tuple>    // std::tuple, std::make_tuple

namespace
{
    size_t get_displayed_processes_count()
    {
        // Берем примерно 80 процентов от высоты терминала и делим на высоту одной строки,
        // которая имеет размер 1 в единицах измерения ftxui. Затем выбираем максимальное между получившимся числом и
        // единицей.
        // TODO: обдумать различные варианты алгоритма отображения кол-ва процессов.

        return std::max((int)(Terminal::Size().dimy * 0.8) / 1, 1);
    }
} // namespace

const std::array<std::tuple<std::string, std::optional<ftxui::Decorator>>, 6> FtxUiPrinter::m_header_names = {
    std::make_tuple("PID", std::nullopt), std::make_tuple("USER", std::nullopt),
    std::make_tuple("CPU[%]", center),    std::make_tuple("RAM[MB]", align_right),
    std::make_tuple("TIME+", center),     std::make_tuple("COMMAND", std::nullopt)};

Element FtxUiPrinter::build_process(const std::shared_ptr<ISystemInfoProvider>& provider_ptr)
{
    const auto& processes_info_provider = provider_ptr->get_processes_info_provider();

    auto table_renderer = Renderer(
        [&]
        {
            std::vector<Elements> table_content;

            Elements header_row;
            header_row.reserve(m_header_names.size());
            for (const auto& pair : m_header_names)
            {
                auto name = text(std::get<0>(pair));
                auto alignment_opt = std::get<1>(pair);
                if (alignment_opt)
                {
                    name = name | alignment_opt.value();
                }
                header_row.push_back(std::move(name));
            }
            table_content.push_back(std::move(header_row));

            size_t pidColumnWidth = 8;
            size_t userColumnWidth = 20;
            size_t cpuColumnWidth = 12, ramColumnWidth = 12, timeColumnWith = 12;
            size_t widthSum = pidColumnWidth + userColumnWidth + cpuColumnWidth + ramColumnWidth + timeColumnWith;
            size_t command_width = Terminal::Size().dimx - widthSum;

            std::vector<Element> row;

            size_t displayed_processes_count = get_displayed_processes_count();
            size_t i = 0;

            auto processes_iterator = processes_info_provider.create_iterator();
            while (processes_iterator->has_next())
            {
                const auto process_info_ptr = processes_iterator->next();
                if (i == displayed_processes_count)
                {
                    break;
                }

                row.reserve(m_header_names.size());

                row.push_back(text(process_info_ptr->get_pid_string()));
                row.push_back(text(process_info_ptr->get_user()));
                row.push_back(text(process_info_ptr->get_cpu_usage_percent(2)) | center);
                row.push_back(text(process_info_ptr->get_converted_mem_usage_value(MEGABYTES, 2)) | align_right);
                row.push_back(text(process_info_ptr->get_uptime()) | center);
                row.push_back(text(process_info_ptr->get_command_slice(0, command_width)));

                table_content.push_back(std::move(row));

                i++;
            }

            auto table = Table(std::move(table_content));

            table.SelectColumn(0).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, pidColumnWidth));
            table.SelectColumn(1).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, userColumnWidth));
            table.SelectColumn(2).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, cpuColumnWidth));
            table.SelectColumn(3).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, ramColumnWidth));
            table.SelectColumn(4).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, timeColumnWith));
            table.SelectColumn(5).Decorate(size(WidthOrHeight::WIDTH, LESS_THAN, command_width));

            return hbox({FtxUiHelpers::hindent(1), table.Render(), FtxUiHelpers::hindent(1)});
        });

    return table_renderer->Render();
}
