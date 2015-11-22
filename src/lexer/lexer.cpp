#include "qac/lexer/lexer.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include <boost/algorithm/string.hpp>

using namespace qac;
using namespace std;

std::set<std::string> lexer::included_files_;

bool lexer_state::matches_opening_token(string &word) {
    string opening_token = get_opening_string_token();
    if (word.length() >= opening_token.length() &&
        boost::starts_with(word, opening_token)) {
        boost::erase_head(word, static_cast<int>(opening_token.length()));
        return true;
    }

    return false;
}

pair<bool, string> lexer_state::matches_closing_token(string &word) {
    string closing_token = get_closing_string_token();

    if (word.length() < closing_token.length()) {
        return make_pair<bool, string>(false, string(""));
    }

    if (word.length() >= closing_token.length() &&
        boost::ends_with(word, closing_token)) {
        boost::erase_tail(word, static_cast<int>(closing_token.length()));
        return make_pair<bool, string>(true, string(""));
    }

    if (ends_with_punctuation(word)) {
        string tmp_word = word.substr(0, word.length() - 1);
        if (tmp_word.length() >= closing_token.length() &&
            boost::ends_with(tmp_word, closing_token)) {
            string punctuation{word[word.length() - 1]};
            boost::erase_tail(word,
                              static_cast<int>(closing_token.length() + 1));
            return make_pair<bool, string>(true, string(punctuation));
        }
    }

    return make_pair<bool, string>(false, string(""));
};

bool lexer_state::ends_with_punctuation(const string &word) {
    if (word.empty()) {
        return false;
    }

    char last_char = word[word.length() - 1];
    auto check = boost::is_any_of(".,!?:;");
    return check(last_char);
}

class lexer_state_latex : public lexer_state {
   public:
    string get_opening_string_token() override { return "\\("; }
    string get_closing_string_token() override { return "\\)"; }
    token_enum get_opening_token() override {
        return token_enum::LATEX_OPENING;
    }
    token_enum get_in_between_token() override {
        return token_enum::LATEX_CODE;
    }
    token_enum get_closing_token() override {
        return token_enum::LATEX_CLOSING;
    }
};

class lexer_state_centered_latex : public lexer_state {
   public:
    string get_opening_string_token() override { return "\\["; }
    string get_closing_string_token() override { return "\\]"; }
    token_enum get_opening_token() override {
        return token_enum::LATEX_CENTERED_OPENING;
    }
    token_enum get_in_between_token() override {
        return token_enum::LATEX_CODE;
    }
    token_enum get_closing_token() override {
        return token_enum::LATEX_CENTERED_CLOSING;
    }
};

class lexer_state_bold : public lexer_state {
   public:
    string get_opening_string_token() override { return "*"; }
    string get_closing_string_token() override { return "*"; }
    token_enum get_opening_token() override { return token_enum::BOLD_OPENING; }
    token_enum get_in_between_token() override { return token_enum::WORD; }
    token_enum get_closing_token() override { return token_enum::BOLD_CLOSING; }
};

class lexer_state_underline : public lexer_state {
   public:
    string get_opening_string_token() override { return "_"; }
    string get_closing_string_token() override { return "_"; }
    token_enum get_opening_token() override {
        return token_enum::UNDERLINE_OPENING;
    }
    token_enum get_in_between_token() override { return token_enum::WORD; }
    token_enum get_closing_token() override {
        return token_enum::UNDERLINE_CLOSING;
    }
};

class lexer_state_code : public lexer_state {
   public:
    string get_opening_string_token() override { return "`"; }
    string get_closing_string_token() override { return "`"; }
    token_enum get_opening_token() override { return token_enum::CODE_OPENING; }
    token_enum get_in_between_token() override { return token_enum::WORD; }
    token_enum get_closing_token() override { return token_enum::CODE_CLOSING; }
};

bool token::is(token_enum token) const { return token_ == token; }

token_enum token::get_token() const { return token_; }

string token::get_value() const { return value_; }

std::ostream &qac::operator<<(std::ostream &os, const token &token) {
    os << token.token_ << "@" << token.line_ << "[" << token.value_ << "]";
    return os;
}

lexer::lexer() {
    possible_states_.push_back(make_unique<lexer_state_latex>());
    possible_states_.push_back(make_unique<lexer_state_centered_latex>());
    possible_states_.push_back(make_unique<lexer_state_bold>());
    possible_states_.push_back(make_unique<lexer_state_underline>());
    possible_states_.push_back(make_unique<lexer_state_code>());
}

vector<token> lexer::lex(std::istream &input) {
    uint16_t line_nr = 0;
    string line;

    while (getline(input, line)) {
        lex_line(tokens_, line, ++line_nr);
    }

    return tokens_;
}

void lexer::lex_line(vector<token> &tokens, const string &line, int line_nr) {
    string trimmed_line = boost::trim_copy(line);

    if (boost::starts_with(trimmed_line, TOKEN_COMMENT)) {
        // check if it's an list item, and not a comment
        string::size_type min_size = TOKEN_COMMENT.size() + 1;
        if (!(trimmed_line.size() >= min_size && trimmed_line[min_size - 1] == '.')) {
            return;
        }
    }

    if (trimmed_line.empty()) {
        tokens.push_back(token(token_enum::EMPTY_LINE, "", line_nr));
        return;
    }

    vector<string> words;
    boost::split(words, trimmed_line, boost::is_any_of(" "));

    auto check_for_closing_token = [&](string &word) {
        auto closes = cur_lexer_state_->matches_closing_token(word);

        if (!word.empty()) {
            tokens.push_back(
                token(cur_lexer_state_->get_in_between_token(), word, line_nr));
        }

        if (get<bool>(closes)) {
            tokens.push_back(token(cur_lexer_state_->get_closing_token(),
                                   cur_lexer_state_->get_closing_string_token(),
                                   line_nr));

            auto appended = get<string>(closes);
            if (!appended.empty()) {
                tokens.push_back(token(token_enum::WORD, appended, line_nr));
            }

            cur_lexer_state_ = nullptr;
        }
    };

    bool first_word = true;
    bool has_table = false;

    for (auto word : words) {
        bool was_q_or_a = false;

        if (!cur_lexer_state_) {
            bool lexed_word = false;
            for (auto &possible_state : possible_states_) {
                if (possible_state->matches_opening_token(word)) {
                    cur_lexer_state_ = possible_state.get();
                    lexed_word = true;

                    tokens.push_back(token(
                        cur_lexer_state_->get_opening_token(),
                        cur_lexer_state_->get_opening_string_token(), line_nr));
                    check_for_closing_token(word);

                    break;
                }
            }

            if (lexed_word) {
                continue;
            }

            if (first_word) {
                if (word == TOKEN_QUESTION) {
                    tokens.push_back(
                        token(token_enum::QUESTION, word, line_nr));
                    was_q_or_a = true;
                } else if (word == TOKEN_ANSWER) {
                    tokens.push_back(token(token_enum::ANSWER, word, line_nr));
                    was_q_or_a = true;
                } else if (word == TOKEN_CHAPTER) {
                    tokens.push_back(token(token_enum::CHAPTER, word, line_nr));
                } else if (word == TOKEN_SECTION) {
                    tokens.push_back(token(token_enum::SECTION, word, line_nr));
                } else if (word == TOKEN_SUBSECTION) {
                    tokens.push_back(
                        token(token_enum::SUBSECTION, word, line_nr));
                } else if (word == TOKEN_FILE) {
                    lex_file(line, line_nr);
                    return;
                } else if (word == TOKEN_UNORDERED_LIST_ITEM) {
                    tokens.push_back(
                        token(token_enum::UNORDERED_LIST_ITEM, word, line_nr));
                } else if (word == TOKEN_ORDERED_LIST_ITEM) {
                    tokens.push_back(
                        token(token_enum::ORDERED_LIST_ITEM, word, line_nr));
                } else if (word == TOKEN_TABLE_CELL) {
                    has_table = true;
                    tokens.push_back(
                        token(token_enum::TABLE_CELL, word, line_nr));
                } else if (word == TOKEN_TABLE_CELL_LEFT_ALIGNED) {
                    has_table = true;
                    tokens.push_back(token(token_enum::TABLE_CELL_LEFT_ALIGNED,
                                           word, line_nr));
                } else if (word == TOKEN_TABLE_CELL_RIGHT_ALIGNED) {
                    has_table = true;
                    tokens.push_back(token(token_enum::TABLE_CELL_RIGHT_ALIGNED,
                                           word, line_nr));
                } else if (word == TOKEN_TABLE_CELL_CENTER_ALIGNED) {
                    has_table = true;
                    tokens.push_back(token(
                        token_enum::TABLE_CELL_CENTER_ALIGNED, word, line_nr));
                } else if (word.size() >= 3 &&
                           boost::starts_with(word, "---")) {
                    tokens.push_back(
                        token(token_enum::TABLE_DIVIDER, word, line_nr));
                } else if (boost::ends_with(word, ".") &&
                           all_of(word.begin(), word.end() - 1, ::isdigit)) {
                    tokens.push_back(
                        token(token_enum::ORDERED_LIST_ITEM, word, line_nr));
                } else {
                    tokens.push_back(token(token_enum::WORD, word, line_nr));
                }
            } else {
                if (has_table) {
                    if (word == TOKEN_TABLE_CELL) {
                        has_table = true;
                        tokens.push_back(
                            token(token_enum::TABLE_CELL, word, line_nr));
                    } else if (word == TOKEN_TABLE_CELL_LEFT_ALIGNED) {
                        has_table = true;
                        tokens.push_back(
                            token(token_enum::TABLE_CELL_LEFT_ALIGNED, word,
                                  line_nr));
                    } else if (word == TOKEN_TABLE_CELL_RIGHT_ALIGNED) {
                        has_table = true;
                        tokens.push_back(
                            token(token_enum::TABLE_CELL_RIGHT_ALIGNED, word,
                                  line_nr));
                    } else if (word == TOKEN_TABLE_CELL_CENTER_ALIGNED) {
                        has_table = true;
                        tokens.push_back(
                            token(token_enum::TABLE_CELL_CENTER_ALIGNED, word,
                                  line_nr));
                    } else {
                        tokens.push_back(
                            token(token_enum::WORD, word, line_nr));
                    }
                } else {
                    tokens.push_back(token(token_enum::WORD, word, line_nr));
                }
            }
        } else {
            check_for_closing_token(word);
        }

        if (first_word && !was_q_or_a) {
            first_word = false;
        }
    }

    tokens.push_back(token(token_enum::NEW_LINE, "", line_nr));
}

void lexer::lex_file(const std::string &line, uint16_t line_nr) {
    string filename = line.substr(TOKEN_FILE.length() + 1);
    if (!include_file(filename)) {
        return;
    }

    std::ifstream input(filename);
    if (!input.is_open()) {
        throw runtime_error("Couldn't open '" + filename + "'");
    }

    lexer lexer;
    vector<token> tokens = lexer.lex(input);
    copy(tokens.begin(), tokens.end(), back_inserter(tokens_));
}

bool lexer::next_char_equals(const string &line, const size_t &cur_pos,
                             const char &letter) const {
    return nth_next_char_equals(1, line, cur_pos, letter);
}

bool lexer::nth_next_char_equals(const size_t &nth, const string &line,
                                 const size_t &cur_pos,
                                 const char &letter) const {
    size_t pos = cur_pos + nth;
    return has_nth_char(pos, line) && line.at(pos) == letter;
}

bool lexer::has_nth_char(const size_t &pos, const std::string &line) const {
    return pos < line.length();
}

std::string qac::to_string(qac::token_enum tenum) {
    switch (tenum) {
        case token_enum::QUESTION:
            return "QUESTION";
        case token_enum::ANSWER:
            return "ANSWER";
        case token_enum::WORD:
            return "WORD";
        case token_enum::LATEX_OPENING:
            return "LATEX_OPENING";
        case token_enum::LATEX_CENTERED_OPENING:
            return "LATEX_CENTERED_OPENING";
        case token_enum::LATEX_CODE:
            return "LATEX_CODE";
        case token_enum::LATEX_CLOSING:
            return "LATEX_CLOSING";
        case token_enum::LATEX_CENTERED_CLOSING:
            return "LATEX_CENTERED_CLOSING";
        case token_enum::NEW_LINE:
            return "NEW_LINE";
        case token_enum::EMPTY_LINE:
            return "EMPTY_LINE";
        case token_enum::UNORDERED_LIST_ITEM:
            return "UNORDERED_LIST_ITEM";
        case token_enum::ORDERED_LIST_ITEM:
            return "ORDERED_LIST_ITEM";
        case token_enum::TABLE_CELL:
            return "TABLE_CELL";
        case token_enum::TABLE_CELL_LEFT_ALIGNED:
            return "TABLE_CELL_LEFT_ALIGNED";
        case token_enum::TABLE_CELL_RIGHT_ALIGNED:
            return "TABLE_CELL_RIGHT_ALIGNED";
        case token_enum::TABLE_CELL_CENTER_ALIGNED:
            return "TABLE_CELL_CENTER_ALIGNED";
        case token_enum::TABLE_DIVIDER:
            return "TABLE_DIVIDER";
        case token_enum::BOLD_OPENING:
            return "BOLD_OPENING";
        case token_enum::BOLD_CLOSING:
            return "BOLD_CLOSING";
        case token_enum::UNDERLINE_OPENING:
            return "UNDERLINE_OPENING";
        case token_enum::UNDERLINE_CLOSING:
            return "UNDERLINE_CLOSING";
        case token_enum::CHAPTER:
            return "CHAPTER";
        case token_enum::SECTION:
            return "SECTION";
        case token_enum::SUBSECTION:
            return "SUBSECTION";
        case token_enum::CODE_OPENING:
            return "CODE_OPENING";
        case token_enum::CODE_CLOSING:
            return "CODE_CLOSING";
        case token_enum::END_OF_FILE:
            return "END_OF_FILE";
        default:
            return ">>UNKNOWN_TOKEN<<";
    }
}

std::ostream &qac::operator<<(std::ostream &os, token_enum tenum) {
    os << to_string(tenum);
    return os;
}
