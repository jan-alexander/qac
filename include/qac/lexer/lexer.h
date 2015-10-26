#ifndef QAC_LEXER_H
#define QAC_LEXER_H

#include <set>
#include <string>
#include <vector>

namespace qac {

enum class token_enum {
    END_OF_FILE = 0,
    QUESTION,
    ANSWER,
    WORD,
    LATEX_OPENING,
    LATEX_CENTERED_OPENING,
    LATEX_CODE,
    LATEX_CLOSING,
    LATEX_CENTERED_CLOSING,
    NEW_LINE,
    EMPTY_LINE,
    UNORDERED_LIST_ITEM,
    ORDERED_LIST_ITEM,
    TABLE_CELL,
    TABLE_CELL_LEFT_ALIGNED,
    TABLE_CELL_RIGHT_ALIGNED,
    TABLE_CELL_CENTER_ALIGNED,
    TABLE_DIVIDER,
    BOLD_OPENING,
    BOLD_CLOSING,
    UNDERLINE_OPENING,
    UNDERLINE_CLOSING,
    CHAPTER,
    SECTION,
    SUBSECTION,
    CODE_OPENING,
    CODE_CLOSING,
};

std::string to_string(token_enum tenum);
std::ostream& operator<<(std::ostream& os, token_enum tenum);

class lexer_state {
   public:
    bool matches_opening_token(std::string& word);
    std::pair<bool, std::string> matches_closing_token(std::string& word);

    virtual token_enum get_opening_token() = 0;
    virtual token_enum get_in_between_token() = 0;
    virtual token_enum get_closing_token() = 0;
    virtual std::string get_opening_string_token() = 0;
    virtual std::string get_closing_string_token() = 0;

   private:
    bool ends_with_punctuation(const std::string& word);
};

class token {
    friend std::ostream& operator<<(std::ostream& os, const token& token);

   public:
    token(token_enum token, const std::string& value, uint16_t line)
        : token_(token), value_(value), line_(line) {}
    bool is(token_enum token) const;
    token_enum get_token() const;
    std::string get_value() const;

    uint16_t line() const {
        return line_;
    }

    inline bool operator==(const token& rhs) const {
        return token_ == rhs.token_ && value_ == rhs.value_ &&
               line_ == rhs.line_;
    }

   private:
    token_enum token_;
    std::string value_;
    uint16_t line_;
};

std::ostream& operator<<(std::ostream& os, const token& token);

class lexer {
   public:
    lexer();
    std::vector<token> lex(std::istream& input);
    
    static bool include_file(std::string filename) {
        auto ret = included_files_.insert(filename);
        return ret.second;
    }

   private:
    const std::string TOKEN_COMMENT = "#";
    const std::string TOKEN_CHAPTER = "CHA:";
    const std::string TOKEN_SECTION = "SEC:";
    const std::string TOKEN_SUBSECTION = "SUB:";
    const std::string TOKEN_QUESTION = "Q:";
    const std::string TOKEN_ANSWER = "A:";
    const std::string TOKEN_FILE = "FILE:";
    const std::string TOKEN_UNORDERED_LIST_ITEM = "-";
    const std::string TOKEN_ORDERED_LIST_ITEM = "#.";
    const std::string TOKEN_TABLE_CELL = "|";
    const std::string TOKEN_TABLE_CELL_LEFT_ALIGNED = "|<";
    const std::string TOKEN_TABLE_CELL_RIGHT_ALIGNED = "|>";
    const std::string TOKEN_TABLE_CELL_CENTER_ALIGNED = "|-";

    void lex_line(std::vector<token>& tokens, const std::string& line,
                  int line_nr);

    void lex_file(const std::string &line, uint16_t line_nr);

    bool next_char_equals(const std::string& line, const size_t& cur_pos,
                          const char& letter) const;
    bool nth_next_char_equals(const size_t& nth, const std::string& line,
                              const size_t& cur_pos, const char& letter) const;
    bool has_nth_char(const size_t& pos, const std::string& line) const;

    bool in_latex_ = false;
    lexer_state* cur_lexer_state_ = nullptr;
    std::vector<std::unique_ptr<qac::lexer_state>> possible_states_;
    std::vector<token> tokens_;
    
    static std::set<std::string> included_files_;
};
}

#endif  // QAC_LEXER_H
