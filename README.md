# RegexGenerator
A C++ Regex Generator. Build regexes as if you were writing them in English. 

Example usage:

```cpp
mvg::regex::regex_generator generator;
using mvg::regex::regex_generator;

auto regex = generator.create_regex()
                     .then_match(regex_generator::match_range('a', 'z'))
                     .then_match(regex_generator::match_string("XXX"))
                     .then_match(regex_generator::match_space())
                     .then_match(regex_generator::match_group(
                         generator.create_regex()
                             .then_match(regex_generator::match_string("Hello"))
                             .then_match(regex_generator::match_space())))
                     .then_match(regex_generator::match_any_of("abcdef"))
                     .then_match(regex_generator::match_none_of("qreoi"))
                     .then_match(regex_generator::match_zero_or_more(
                         regex_generator::match_range('a', 'z')));
                         
std::regex regex_obj(regex.str());
```
