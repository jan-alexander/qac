# qac - Q&A Compiler BETA

qac is a handy tool to convert pairs of questions and answers into a different format, e.g. [Anki](http://ankisrs.net/) flashcards, in a fast
and easy way. You can completly focus on creating questions on the topic you want to learn, without having to worry
about the exact way on how to learn them. The questions are written in an easy and intuitive format, though they offer [LaTeX](https://en.wikipedia.org/wiki/LaTeX) support.

## Writing Questions

Start your favorite text editor, and create the file `topic.qa`. Insert following content:

```
Q: What is the answer to the ultimate question of life, the universe, and everything?
A: 42
```

That's basically it. Run `qac --output=topic.html topic.qa`, and you will have your question converted to a HTML page.
If you want to comment a line out, simply put a `#` in front of it.

To create text files, that can be imported by Anki, run:
  
  `qac --output=topic.txt --generator=anki topic.qa`

Make sure to check the `Allow HTML` checkbox.

### Supported Formattings

  - Bold: Surround a word with `*`, e.g. `*strong*`
  - Underlined: Surround a word with `_`, e.g. `_underlined_`
  - Code: Surround a word with `` ` ``, e.g. `` `code` ``. (Does not preserve formatting, will be single lined)

### LaTeX Support
 
When converting questions to HTML, [MathJax](https://www.mathjax.org/) is used for LaTeX rendering. Anki converts LaTeX
code to images, so you need to have a LaTeX distribution installed.

Use LaTeX like you are used to:
  - For inline math write `\([LaTeX code]\)`, e.g. `\((a+b)^2\)`
  - For display math write `\[[LaTeX code]\]`, e.g. `\[(a+b)^2\]`

### Lists
Unordered and ordered lists are supported in a natural and intuitive way. Create an unordered list like this:

```
Q: What is 42 the answer for?
A: - The ultimate question of life
   - The universe
   - And everything
```

Ordered lists can be created in a similar way:

```
Q: The first three weekdays are?
A: 1. Monday
   2. Tuesday
   3. Wednesday
```

If you don't want to write a number, you could instead just write `#.`.

### Tables

Tables are supported as well. Take a look:

```
Q: Square numbers of 1, 2 and 3 are?
A: -------------
   | 1 | 2 | 3 |
   -------------
   | 1 | 4 | 9 |
   -------------
```

Each cell can have a different text alignment:

```
Q: How can cells be aligned?
A: ----------------------------------------
   | aaaaaaaaaa | bbbbbbbbbb | cccccccccc |
   ----------------------------------------
   |< left      |-  center   |>     right |
   ----------------------------------------
```

To summarize:
  - `|<` left aligned cell
  - `|-` centered cell
  - `|>` right aligned cell

### Images

The keyword to include images is the following: `IMG(source,width,height)`. You
can use it without the width and height argument, or you can use just one
number, if your image is quadratic. Please note that there must not be a space
in this command.

Keep in mind to copy all images to Ankis `collection.media` Folder. Read
http://ankisrs.net/docs/manual.html#importing-media for further details. Note that
all folders will be stripped from the image source path when generating Anki
questions.

## Organizing Questions

Questions can be organized into chapters, sections and subsections. It's done like this:

```
CHA: Chapter One

Q: ...
A: ...

SEC: First Section in Chapter One

Q: ...
A: ...

SUB: First Subsection in the first Section of Chapter One

Q: ...
A: ...
```

Chapters and sections can have questions, but they don't need to. 

If you want to change the word chapter in the rendered result to your language, do this with command line flags, e.g.
`--chapter=Kapitel` for german. The same is possible for sections and subsections.

### Organize Questions in multiple files

If you have a bunch of questions, and you want to split them into multiple files, feel free to do so. This is the way
how to do it:

```
FILE: chapter1.qa
FILE: chapter2.qa

CHA: Chapter Three

Q: ...
A: ...
```

## Compiling qac

qac depends on the following libraries:
  - [boost](http://www.boost.org/)
  - [glog](https://github.com/google/glog)
  - [gflags](https://github.com/gflags/gflags)

If you have them installed, simply use [CMake](https://cmake.org/) to create makefiles for your compiler. Please note
that your compiler has to be C++14 compatible (due to `std::make_unique`).

### On OS X using Homebrew
The steps should be similar on linux. Make sure you have a compiler installed.

  1. `brew install boost cmake gflags glog`
  2. `git clone https://github.com/jan-alexander/qac.git ~/qac`
  3. `cd ~/qac`
  4. `mkdir build && cd build`
  5. `cmake ..`
  6. `make && make install`

## Extending qac

It's fairly simple to extend qac by writing new generators. You only have to implement the 
[generator](https://github.com/jan-alexander/qac/blob/master/include/qac/generator/generator.h) interface. After that, you
have to add your generator to the generator map [here](https://github.com/jan-alexander/qac/blob/master/src/main.cpp#L73). You can list all available generators with the `--listgenerators` flag. To use a certain generator, use the `--generator` flag, e.g. `--generator=html`.

### Contributing
Contributions are more than welcome! If you have improved qac or fixed some bugs, send me a pull request.

## That's it!
Happy learning! Enjoy! :beers:
