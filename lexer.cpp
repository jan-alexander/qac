#include "lexer.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include <boost/algorithm/string.hpp>

using namespace qac;
using namespace std;

class lexer_state {
public:
    bool matches_opening_token(string &word) {
        string opening_token = get_opening_string_token();
        if (word.length() >= opening_token.length() && boost::starts_with(word, opening_token)) {
            boost::erase_head(word, static_cast<int>(opening_token.length()));
        }

        return false;
    }

    pair<bool, string> matches_closing_token(string &word) {
        string closing_token = get_closing_string_token();

        if (word.length() < closing_token.length()) {
            return make_pair<bool, string>(false, string(""));
        }

        if (word.length() >= closing_token.length() && boost::ends_with(word, closing_token)) {
            boost::erase_tail(word, static_cast<int>(closing_token.length()));
            return make_pair<bool, string>(true, string(""));
        }

        if (ends_with_punctuation(word)) {
            string tmp_word = word.substr(0, word.length() - 1);
            if (tmp_word.length() >= closing_token.length() && boost::ends_with(tmp_word, closing_token)) {
                string punctuation{word[word.length() - 1]};
                boost::erase_tail(word, static_cast<int>(closing_token.length() + 1));
                return make_pair<bool, string>(true, string(punctuation));
            }
        }

        return make_pair<bool, string>(false, string(""));
    };

    virtual token_enum get_opening_token() = 0;
    virtual token_enum get_in_between_token() = 0;
    virtual token_enum get_closing_token() = 0;

protected:
    virtual string get_opening_string_token() = 0;
    virtual string get_closing_string_token() = 0;

private:
    bool ends_with_punctuation(const string &word) {
        if (word.empty()) {
            return false;
        }

        char last_char = word[word.length() - 1];
        auto check = boost::is_any_of(".,!?:;");
        return check(last_char);
    }
};

class lexer_state_latex : public lexer_state {
protected:
    string get_opening_string_token() { return "\\("; }
    string get_closing_string_token() { return "\\)"; }
    token_enum get_opening_token() { return token_enum::LATEX_OPENING; }
    token_enum get_in_between_token() { return token_enum::LATEX_CODE; }
    token_enum get_closing_token() { return token_enum::LATEX_CLOSING; }
};

class lexer_state_centered_latex : public lexer_state {
protected:
    string get_opening_string_token() { return "\\["; }
    string get_closing_string_token() { return "\\]"; }
    token_enum get_opening_token() { return token_enum::LATEX_CENTERED_OPENING; }
    token_enum get_in_between_token() { return token_enum::LATEX_CODE; }
    token_enum get_closing_token() { return token_enum::LATEX_CENTERED_CLOSING; }
};

token::token(token_enum token, const string &value)
    : token_(token), value_(value) {}

bool token::is(token_enum token) const {
    return token_ == token;
}

token_enum token::get_token() const {
    return token_;
}

string token::get_value() const {
    return value_;
}

vector<token> lexer::lex(const char *filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        throw runtime_error("Couldn't open file");
    }

    int line_nr = 0;
    vector<token> tokens;
    while (input.good()) {
        string line;
        getline(input, line);

        lex_line(tokens, line, ++line_nr);
    }

    return tokens;
}

void lexer::lex_line(vector<token> &tokens, const string &line, int line_nr) {
    bool first_word = true;

    string trimmed_line = boost::trim_copy(line);

    cout << line_nr << ": " << trimmed_line << endl;

    if (trimmed_line.empty()) {
        tokens.push_back(token(token_enum::EMPTY_LINE, ""));
        return;
    }

    vector<string> words;
    boost::split(words, trimmed_line, boost::is_any_of(" "));

    lexer_state *cur_lexer_state = nullptr;
    vector<unique_ptr<lexer_state>> possible_states;
    possible_states.push_back(make_unique<lexer_state_latex>());
    possible_states.push_back(make_unique<lexer_state_centered_latex>());

    for(auto word : words) {
        if (!cur_lexer_state) {
            for (auto& possible_state : possible_states) {
                if(possible_state->matches_opening_token(word)) {
                    cur_lexer_state = possible_state.get();

                    tokens.push_back(token(cur_lexer_state->get_opening_token(), ""));
                    auto closes = cur_lexer_state->matches_closing_token(word);

                    if (!word.empty()) {
                        tokens.push_back(token(cur_lexer_state->get_in_between_token(), word));
                    }

                    if (get<bool>(closes)) {
                        cout << "matches closing token \n";

                        tokens.push_back(token(cur_lexer_state->get_closing_token(), ""));

                        auto appended = get<string>(closes);
                        if (!appended.empty()) {
                            tokens.push_back(token(token_enum::WORD, appended));
                        }

                        cur_lexer_state = nullptr;
                    }

                    break;
                }
            }


        } else {

        }


        cout << word << "\n";
    }


/*
    string tmp_word = "";
    for (size_t i = 0; i < trimmed_line.length(); ++i) {
        char cur_char = trimmed_line[i];

        switch (cur_char) {
            case 'Q':
                if (first_word && tmp_word.empty() && next_char_equals(trimmed_line, i, ':')) {
                    first_word = false;
                    i++;
                    tokens.push_back(token(token_enum::QUESTION, "Q:"));
                } else {
                    tmp_word.push_back(cur_char);
                }
                break;

            case 'A':
                if (first_word && tmp_word.empty() && next_char_equals(trimmed_line, i, ':')) {
                    i++;
                    tokens.push_back(token(token_enum::ANSWER, "A:"));
                } else {
                    tmp_word.push_back(cur_char);
                }
                break;


            case ' ':
                if (in_latex_) {
                    tmp_word.push_back(' ');
                } else {
                    if (!tmp_word.empty()) {
                        tokens.push_back(token(token_enum::WORD, tmp_word));
                        tmp_word = "";
                        first_word = false;
                    }
                }
                break;

            case '#':
                if (first_word && next_char_equals(trimmed_line, i, '.')) {
                    tokens.push_back(token(token_enum::ORDERED_LIST_ITEM, "#."));
                    i++;
                    first_word = false;
                } else {
                    tmp_word.push_back(cur_char);
                }
                break;

            default:
                tmp_word.push_back(cur_char);
                break;
        }

    }

    if(!tmp_word.empty()) {
        tokens.push_back(token(in_latex_ ? token_enum::LATEX_CODE : token_enum::WORD, tmp_word));
    }
    */
}

bool lexer::next_char_equals(const string &line, const size_t &cur_pos, const char &letter) const {
    return nth_next_char_equals(1, line, cur_pos, letter);
}

bool lexer::nth_next_char_equals(const size_t &nth, const string &line, const size_t &cur_pos, const char &letter) const {
    size_t pos = cur_pos + nth;
    return has_nth_char(pos, line) && line.at(pos) == letter;
}

bool lexer::has_nth_char(const size_t &pos, const std::string &line) const {
    return pos < line.length();
}
