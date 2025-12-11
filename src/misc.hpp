#ifndef MISC_HPP
#define MISC_HPP

#include <format>
#include <GL/glew.h>
#include <iterator>
#include <source_location>
#include <string_view>

#if 0
template<>
struct std::formatter<GLubyte const*> : public std::formatter<char const*>
{
    constexpr auto parse(format_parse_context& ctx)  {
        return formatter<char const*>::parse(ctx);
    }

    auto format(GLubyte const* ptr, std::format_context& ctx) const {
        if (!ptr) // print "(null)" like many formatters do
            return formatter<char const*>::format("(null)", ctx);
        // reinterpret as char* for C-string formatting
        return formatter<char const*>::format(reinterpret_cast<const char*>(ptr), ctx);
    }

};
#endif

template<>
struct std::formatter<std::source_location>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        const auto* it  = ctx.begin();
        const auto* end = ctx.end();
        if (it == end || *it == '}')
        {
            return it;
        }

        // read specifier into string_view
        const auto* start = it;
        while (it != end && *it != '}') { ++it; }
        std::string_view const spec(&*start, std::distance(start, it));

        if (spec == "short")
        {
            style = Style::Short;
        }
        else if (spec == "file")
        {
            style = Style::File;
        }
        else if (spec == "line")
        {
            style = Style::Line;
        }
        else if (spec == "func")
        {
            style = Style::Func;
        }
        else
        {
            style = Style::Default;
        }
        return it;
    }

    auto format(const std::source_location& loc, std::format_context& ctx) const
    {
        auto out              = ctx.out();
        std::string_view file = loc.file_name();
        unsigned int line     = loc.line();
        std::string_view func = loc.function_name();

        auto basename = [](std::string_view path) -> std::string_view
        {
            auto pos = path.find_last_of("/\\");
            return pos == std::string_view::npos ? path : path.substr(pos + 1);
        };

        switch (style)
        {
            case Style::Short:
                return std::format_to(
                    out, "{}:{}({})", basename(file),
                    (!func.empty()) ? func : std::string_view(""), line);
            case Style::File: return std::format_to(out, "{}", file);
            case Style::Line: return std::format_to(out, "{}", line);
            case Style::Func: return std::format_to(out, "{}", func);
            case Style::Default:
            default:
                return std::format_to(
                    out, "{}:{}({})", file,
                    (!func.empty()) ? func : std::string_view(""), line);
        }
    }

private:
    enum class Style
    {
        Default,
        Short,
        File,
        Line,
        Func
    };
    Style style = Style::Default;
};

#endif // MISC_HPP
