#include "qac/generator/html-generator.h"

using namespace qac;
using namespace std;

string html_generator::get_name() {
    return "html";
}

string html_generator::get_default_file_extension() {
    return "html";
}

string html_generator::get_header() {
    return "html header";
}

string html_generator::get_footer() {
    return "html footer";
}
