#ifndef MVG_REGEX_GENERATOR_HPP_
#define MVG_REGEX_GENERATOR_HPP_

#include <regex>
#include <string>
#include <string_view>

namespace mvg {

namespace regex {

template<typename char_t>
struct char_traits;

template<>
struct char_traits<char> : public std::char_traits<char> {
    struct braces {
        struct square {
            static constexpr char open = '[';
            static constexpr char close = ']';
        };

        struct round {
            static constexpr char open = '(';
            static constexpr char close = ')';
        };

        struct curly {
            static constexpr char open = '{';
            static constexpr char close = '}';
        };
    };

    static constexpr char minus_sign = '-';
    static constexpr char plus = '+';
    static constexpr char or_sign = '|';
    static constexpr char power_sign = '^';
    static constexpr char star = '*';
    static constexpr char comma = ',';

        static constexpr char backslash = '\\';
    static constexpr char forwardslash = '/';

    static constexpr char question_mark = '?';
    static constexpr char colon = ':';

    static constexpr char s = 's';
    static constexpr char w = 'w';
    static constexpr char d = 'd';

    static constexpr char S = 'S';
    static constexpr char W = 'W';
    static constexpr char D = 'D';
};

template<>
struct char_traits<wchar_t> : public std::char_traits<wchar_t> {
    struct braces {
        struct square {
            static constexpr wchar_t open = L'[';
            static constexpr wchar_t close = L']';
        };

        struct round {
            static constexpr wchar_t open = L'(';
            static constexpr wchar_t close = L')';
        };

        struct curly {
            static constexpr wchar_t open = L'{';
            static constexpr wchar_t close = L'}';
        };
    };

    static constexpr wchar_t minus_sign = L'-';
    static constexpr wchar_t plus = L'+';
    static constexpr wchar_t or_sign = L'|';
    static constexpr wchar_t power_sign = L'^';
    static constexpr wchar_t star = L'*';
    static constexpr wchar_t comma = L',';

    static constexpr wchar_t backslash = L'\\';
    static constexpr wchar_t forwardslash = L'/';

    static constexpr wchar_t question_mark = L'?';
    static constexpr wchar_t colon = L':';

    static constexpr wchar_t s = L's';
    static constexpr wchar_t w = L'w';
    static constexpr wchar_t d = L'd';

    static constexpr wchar_t S = L'S';
    static constexpr wchar_t W = L'W';
    static constexpr wchar_t D = L'D';
};

template<typename char_t, typename traits = char_traits<char_t>>
class regex_element {
public:
    using string_t = std::basic_string<char_t, std::char_traits<char_t>>;

    regex_element(
        std::basic_string_view<char_t, std::char_traits<char_t>> reg_str) {
        str_ = reg_str;
    }

    regex_element(regex_element const&) = default;
    regex_element(regex_element&&) = default;

    regex_element& operator=(regex_element const&) = default;
    regex_element& operator=(regex_element&&) = default;

    string_t str() const { return str_; }

private:
    string_t str_;
};

template<typename char_t, typename traits = char_traits<char_t>>
class basic_regex_chain_holder {
public:
    using string_t = std::basic_string<char_t, std::char_traits<char_t>>;
    using string_view_t =
        std::basic_string_view<char_t, std::char_traits<char_t>>;
    using regex_element_t = regex_element<char_t, traits>;

    basic_regex_chain_holder(string_view_t reg_str) : str_(reg_str) {}
    basic_regex_chain_holder(basic_regex_chain_holder const&) = default;
    basic_regex_chain_holder(basic_regex_chain_holder&&) = default;

    basic_regex_chain_holder&
    operator=(basic_regex_chain_holder const&) = default;
    basic_regex_chain_holder& operator=(basic_regex_chain_holder&&) = default;

    string_t str() const { return str_; }

    //@return: *this to be able to chain this operation
    basic_regex_chain_holder& then_match(regex_element_t const& elem) {
        str_ += elem.str();
        return *this;
    }

private:
    string_t str_;
};

template<typename char_t, typename traits = char_traits<char_t>>
class basic_regex_generator {
public:
    using string_t = std::basic_string<char_t, std::char_traits<char_t>>;
    using string_view_t =
        std::basic_string_view<char_t, std::char_traits<char_t>>;
    using regex_element_t = regex_element<char_t, traits>;
    using regex_chain_holder_t = basic_regex_chain_holder<char_t, traits>;

    basic_regex_generator() = default;
    basic_regex_generator(basic_regex_generator const&) = default;
    basic_regex_generator(basic_regex_generator&&) = default;

    basic_regex_generator& operator=(basic_regex_generator const&) = default;
    basic_regex_generator& operator=(basic_regex_generator&&) = default;

    static regex_element_t match_range(char_t start, char_t end) {
        // Build a regex_element in the format of [start-end]
        return regex_element_t(string_t{} + traits::braces::square::open +
                               start + traits::minus_sign + end +
                               traits::braces::square::close);
    }

    // RegElement is either a regex_element_t or a regex_chain_holder_t
    template<typename RegElementA, typename RegElementB>
    static regex_element_t match_or(RegElementA const& a,
                                    RegElementB const& b) {
        // Ensure RegElementA and RegElementB are of the correct types.
        // Templating this function saves us writing 4 overloads for every
        // possible combination
        static_assert((
            std::is_same_v<RegElementA, regex_element_t> ||
            std::is_same_v<
                RegElementA,
                regex_chain_holder_t>)&&(std::is_same_v<RegElementB,
                                                        regex_element_t> ||
                                         std::is_same_v<RegElementB,
                                                        regex_chain_holder_t>));

        return regex_element_t(a.str() + traits::or_sign + b.str());
    }

    static regex_element_t match_any() {
        return regex_element_t(string_t(1, '.'));
    }

    static regex_element_t match_character(char_t ch) {
        return regex_element_t(string_t{} + ch);
    }

    static regex_element_t match_string(string_view_t str) {
        return regex_element_t(str);
    }

    static regex_element_t match_space() {
        return regex_element_t(string_t{} + traits::backslash + traits::s);
    }

    static regex_element_t match_alpha_char() {
        return regex_element_t(string_t{} + traits::backslash + traits::w);
    }

    static regex_element_t match_digit() {
        return regex_element_t(string_t{} + traits::backslash + traits::d);
    }

    static regex_element_t match_not_space() {
        return regex_element_t(string_t{} + traits::backslash + traits::S);
    }

    static regex_element_t match_not_alpha_char() {
        return regex_element_t{string_t{} + traits::backslash + traits::W};
    }

    static regex_element_t match_not_digit() {
        return regex_element_t{string_t{} + traits::backslash + traits::D};
    }

    static regex_element_t match_any_of(string_view_t chars) {
        return regex_element_t(string_t{} + traits::braces::square::open +
                               chars.data() + traits::braces::square::close);
    }

    static regex_element_t match_none_of(string_view_t chars) {
        return regex_element_t(string_t{} + traits::braces::square::open +
                               traits::power_sign + chars.data() +
                               traits::braces::square::close);
    }

    static regex_element_t match_escaped_char(char_t ch) {
        return regex_element_t(string_t{} + traits::backslash + ch);
    }

    template<typename RegElement>
    static regex_element_t match_zero_or_more(RegElement const& to_match) {
        static_assert(std::is_same_v<RegElement, regex_element_t> ||
                      std::is_same_v<RegElement, regex_chain_holder_t>);

        return regex_element_t(to_match.str() + traits::star);
    }

    template<typename RegElement>
    static regex_element_t match_one_or_more(RegElement const& to_match) {
        static_assert(std::is_same_v<RegElement, regex_element_t> ||
                      std::is_same_v<RegElement, regex_chain_holder_t>);

        return regex_element_t(to_match.str() + traits::plus);
    }

    template<typename RegElement>
    static regex_element_t match_zero_or_one(RegElement const& to_match) {
        static_assert(std::is_same_v<RegElement, regex_element_t> ||
                      std::is_same_v<RegElement, regex_chain_holder_t>);

        return regex_element_t(to_match.str() + traits::question_mark);
    }

    template<typename RegElement>
    static regex_element_t match_n(RegElement const& to_match, std::size_t n) {
        static_assert(std::is_same_v<RegElement, regex_element_t> ||
                      std::is_same_v<RegElement, regex_chain_holder_t>);
        return regex_element_t(to_match.str() + traits::braces::curly::open +
                               std::to_string(n) +
                               traits::braces::curly::close);
    }

    template<typename RegElement>
    static regex_element_t match_n_or_more(RegElement const& to_match,
                                           std::size_t n) {
        static_assert(std::is_same_v<RegElement, regex_element_t> ||
                      std::is_same_v<RegElement, regex_chain_holder_t>);
        return regex_element_t(to_match.str() + traits::braces::curly::open +
                               std::to_string(n) + traits::comma +
                               traits::braces::curly::close);
    }

    // Grouping functionality

    // non-capturing group
    static regex_element_t match_group(regex_element_t to_group) {
        return regex_element_t(string_t{} + traits::braces::round::open +
                               traits::question_mark + traits::colon +
                               to_group.str() + traits::braces::round::close);
    }

    static regex_element_t match_group(regex_chain_holder_t to_group) {
        return regex_element_t(string_t{} + traits::braces::round::open +
                               traits::question_mark + traits::colon +
                               to_group.str() + traits::braces::round::close);
    }

    static regex_element_t capture_group(regex_element_t const& to_capture) {
        return regex_element_t(traits::braces::round::open + to_capture.str() +
                               traits::braces::round::close);
    }

    // Overload to allow capturing multiple elements. Call this like:
    // .then_match(
    // regex_generator::capture_group(
    //		regex_generator::create_regex()
    //		.then_match(regex_generator::match_range('a', 'z'))
    //		.then_match(regex_generator::match_space())
    // ));
    static regex_element_t
    capture_group(regex_chain_holder_t const& to_capture) {
        return regex_element_t(traits::braces::round::open + to_capture.str() +
                               traits::braces::round::close);
    }

    //@return: A regex chain holder object that is used to build the regex
    regex_chain_holder_t create_regex() { return regex_chain_holder_t{""}; }

    regex_chain_holder_t create_regex_from_string(string_view_t initial_reg) {
        return regex_chain_holder_t{initial_reg};
    }

private:
    static string_t escape(char_t c) {
        return string_t(1, traits::backslash) + c;
    }
};

using regex_generator = basic_regex_generator<char, char_traits<char>>;
using wregex_generator = basic_regex_generator<wchar_t, char_traits<wchar_t>>;

} // namespace regex

} // namespace mvg

#endif
