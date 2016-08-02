#include <glog/logging.h>

DEFINE_bool(listgenerators, false, "List available generators.");
DEFINE_bool(printcst, false, "Print parse tree.");
DEFINE_bool(printtokens, false, "Print lexing tokens.");
DEFINE_string(generator, "html", "Used generator.");
DEFINE_string(output, "", "File to write output to.");

DEFINE_string(chapter, "Chapter", "The word chapter used for rendering.");
DEFINE_string(section, "Section", "The word section used for rendering.");
DEFINE_string(subsection, "Subsection",
              "The word subsection used for rendering.");
DEFINE_string(question, "Question", "The word question used for rendering.");
DEFINE_bool(render, true, "Render the content or not.");
DEFINE_bool(offline, false, "Use local MathJax");