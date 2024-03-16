## voom editor
**voom** is a command line *code/text editor* built-in c ; inspired by vim editor, "explains the name "voom" ".

It is built on top of the ncurses library 
## installation
To manually install voom go to your program directory and

    git clone https://github.com/gathukia/voom_editor.git
    cd voom_editor
    sudo make PREFIX=/usr install
   Thats it, installed!!
   ## Usage
   To display the usage menu use the following commands:
   

    voom -h
    or
    voom -help
  But the main usage format follows:
  

    voom <tags> <file names>
   ## status
  Still under development.
  
  It is not stable enough for editing more than one file simultaneously.
  ## Todos
  *  add multi-file editing.
  * implement code formatting and autocompletion for **C and asm**
