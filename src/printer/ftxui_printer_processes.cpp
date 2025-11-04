#include "ftxui_printer.hpp"

static Box box; // TODO: remove

// TODO: чет хуита
const std::array<std::string, 6> FtxUiPrinter::m_header_names = { "PID", "USER", "CPU[%%]", "RAM[MB]", "TIME+", "COMMAND" };

Element FtxUiPrinter::build_process(const SystemInfo& system_info)
{
    auto table_renderer = Renderer([&]
    {
        std::vector<Elements> table_content;

        Elements header_row;
        header_row.reserve(m_header_names.size());
        for (const auto& name : m_header_names)
        {
            header_row.push_back(text(name));
        }
        table_content.push_back(std::move(header_row));

        std::string process_uptime;
        std::stringstream formatter;
        fixed(formatter);
        formatter.precision(2);

        std::vector<Element> row;

        // Берем примерно 80 процентов от высоты терминала и делим на высоту одной строки,
        // которая имеет размер 1 в единицах измерения ftxui. Затем выбираем максимальное между получившимся числом и единицей.
        // TODO: обдумать различные варианты алгоритма отображения кол-ва процессов.
        size_t displayed_processes_count = std::max((int)(Terminal::Size().dimy * 0.8) / 1, 1);
        size_t i = 0;
        for (const auto& process : system_info.processes)
        {
            if (i == displayed_processes_count)
            {
                break;
            }

            row.reserve(m_header_names.size());

            formatter.clear();
            formatter.str("");

            //You need to take care of the fact that the cpu utilization has already been multiplied by 100.
            formatter << convert_unit(static_cast<double>(process.memory_usage), MEGABYTES, KILOBYTES);
            elapsed_time(process_uptime, process.uptime);

            row.push_back(text(std::to_string(process.pid)));
            row.push_back(text(process.user));
            row.push_back(text(std::to_string(process.cpu_usage * 100).substr(0, 6)));  // TODO: чет хуита
            row.push_back(text(formatter.str()));
            row.push_back(text(process_uptime));
            row.push_back(text(process.command));

            table_content.push_back(std::move(row));

            i++; // TODO: remove
        }

        auto table = Table(std::move(table_content));

        size_t pidColumnWidth = 8;
        size_t userColumnWidth = 20;
        size_t cpuColumnWidth = 12, ramColumnWidth = 12, timeColumnWith = 12;
        size_t widthSum = pidColumnWidth + userColumnWidth + cpuColumnWidth + ramColumnWidth + timeColumnWith;

        table.SelectColumn(0).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, pidColumnWidth));
        table.SelectColumn(1).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, userColumnWidth));
        table.SelectColumn(2).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, cpuColumnWidth));
        table.SelectColumn(3).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, ramColumnWidth));
        table.SelectColumn(4).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, timeColumnWith));
        table.SelectColumn(5).Decorate(size(WidthOrHeight::WIDTH, LESS_THAN, Terminal::Size().dimx - widthSum));

        return hbox(
        {
            FtxUiHelpers::hindent(1),
            table.Render(),
            FtxUiHelpers::hindent(1)
        });
    });

    return table_renderer->Render();
}
