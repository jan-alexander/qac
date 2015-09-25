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
            return true;
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
public:
    string get_opening_string_token() override { return "\\("; }
    string get_closing_string_token() override { return "\\)"; }
    token_enum get_opening_token() override { return token_enum::LATEX_OPENING; }
    token_enum get_in_between_token() override { return token_enum::LATEX_CODE; }
    token_enum get_closing_token() override { return token_enum::LATEX_CLOSING; }
};

class lexer_state_centered_latex : public lexer_state {
public:
    string get_opening_string_token() override { return "\\["; }
    string get_closing_string_token() override { return "\\]"; }
    token_enum get_opening_token() override { return token_enum::LATEX_CENTERED_OPENING; }
    token_enum get_in_between_token() override { return token_enum::LATEX_CODE; }
    token_enum get_closing_token() override { return token_enum::LATEX_CENTERED_CLOSING; }
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

    auto check_for_closing_token = [&](string &word) {
        auto closes = cur_lexer_state->matches_closing_token(word);

        if (!word.empty()) {
            tokens.push_back(token(cur_lexer_state->get_in_between_token(), word));
        }

        if (get<bool>(closes)) {
            tokens.push_back(token(cur_lexer_state->get_closing_token(), cur_lexer_state->get_closing_string_token()));

            auto appended = get<string>(closes);
            if (!appended.empty()) {
                tokens.push_back(token(token_enum::WORD, appended));
            }

            cur_lexer_state = nullptr;
        }
    };

    bool first_word = true;
    bool has_table = false;

    for(auto word : words) {
        bool was_q_or_a = false;

        if (!cur_lexer_state) {
            bool lexed_word = false;
            for (auto& possible_state : possible_states) {
                if(possible_state->matches_opening_token(word)) {
                    cur_lexer_state = possible_state.get();
                    lexed_word = true;

                    tokens.push_back(token(cur_lexer_state->get_opening_token(), cur_lexer_state->get_opening_string_token()));
                    check_for_closing_token(word);

                    break;
                }
            }

            if(lexed_word) {
                continue;
            }

            if (first_word) {
                if (word.compare("Q:") == 0) {
                    tokens.push_back(token(token_enum::QUESTION, word));
                    was_q_or_a = true;
                } else if (word.compare("A:") == 0) {
                    tokens.push_back(token(token_enum::ANSWER, word));
                    was_q_or_a = true;
                } else if (word.compare("-") == 0) {
                    tokens.push_back(token(token_enum::UNORDERED_LIST_ITEM, word));
                } else if (word.compare("#.") == 0) {
                    tokens.push_back(token(token_enum::ORDERED_LIST_ITEM, word));
                } else if (word.compare("|") == 0) {
                    has_table = true;
                    tokens.push_back(token(token_enum::TABLE_CELL, word));
                } else if (word.compare("|<") == 0) {
                    has_table = true;
                    tokens.push_back(token(token_enum::TABLE_CELL_LEFT_ALIGNED, word));
                } else if (word.compare("|>") == 0) {
                    has_table = true;
                    tokens.push_back(token(token_enum::TABLE_CELL_RIGHT_ALIGNED, word));
                } else if (word.compare("|-") == 0) {
                    has_table = true;
                    tokens.push_back(token(token_enum::TABLE_CELL_CENTER_ALIGNED, word));
                } else if (word.size() >= 3 && boost::starts_with(word, "---")) {
                    tokens.push_back(token(token_enum::TABLE_DIVIDER, word));
                }
                else {
                    tokens.push_back(token(token_enum::WORD, word));
                }
            } else {
                if (has_table) {
                    if (word.compare("|") == 0) {
                        has_table = true;
                        tokens.push_back(token(token_enum::TABLE_CELL, word));
                    } else if (word.compare("|<") == 0) {
                        has_table = true;
                        tokens.push_back(token(token_enum::TABLE_CELL_LEFT_ALIGNED, word));
                    } else if (word.compare("|>") == 0) {
                        has_table = true;
                        tokens.push_back(token(token_enum::TABLE_CELL_RIGHT_ALIGNED, word));
                    } else if (word.compare("|-") == 0) {
                        has_table = true;
                        tokens.push_back(token(token_enum::TABLE_CELL_CENTER_ALIGNED, word));
                    } else {
                        tokens.push_back(token(token_enum::WORD, word));
                    }
                } else {
                    tokens.push_back(token(token_enum::WORD, word));
                }
            }
        } else {
            check_for_closing_token(word);
        }

        if (first_word && !was_q_or_a) {
            first_word = false;
        }

        cout << word << "\n";
    }

    tokens.push_back(token(token_enum::NEW_LINE, ""));
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
