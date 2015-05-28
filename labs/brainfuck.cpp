/*
= Brainfuck

If you have gcc:

----
g++ -o brainfuck.exe brainfuck.cpp
brainfuck.exe helloworld.bf
----
*/

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Primitive Brainfuck commands
 */
typedef enum { 
    INCREMENT, // +
    DECREMENT, // -
    SHIFT_LEFT, // <
    SHIFT_RIGHT, // >
    INPUT, // ,
    OUTPUT // .
} Command;

// Forward references. Silly C++!
class CommandNode;
class Loop;
class Program;

/**
 * Visits?!? Well, that'd indicate visitors!
 * A visitor is an interface that allows you to walk through a tree and do stuff.
 */
class Visitor {
    public:
        virtual void visit(const CommandNode * leaf) = 0;
        virtual void visit(const Loop * loop) = 0;
        virtual void visit(const Program * program) = 0;
};

/**
 * The Node class (like a Java abstract class) accepts visitors, but since it's pure virtual, we can't use it directly.
 */
class Node {
    public:
        virtual void accept (Visitor *v) = 0;
};

/**
 * CommandNode publicly extends Node to accept visitors.
 * CommandNode represents a leaf node with a primitive Brainfuck command in it.
 */
class CommandNode : public Node {
    public:
        Command command;
        CommandNode(char c) {
            switch(c) {
                case '+': command = INCREMENT; break;
                case '-': command = DECREMENT; break;
                case '<': command = SHIFT_LEFT; break;
                case '>': command = SHIFT_RIGHT; break;
                case ',': command = INPUT; break;
                case '.': command = OUTPUT; break;
            }
        }
        void accept (Visitor * v) {
            v->visit(this);
        }
};

/**
 * Loop publicly extends Node to accept visitors.
 * Loop represents a loop in Brainfuck.
 */
class Loop : public Node {
    public:
        vector<Node*> children;
        void accept (Visitor * v) {
            v->visit(this);
        }
};

/**
 * Program is the root of a Brainfuck program abstract syntax tree.
 * Because Brainfuck is so primitive, the parse tree is the abstract syntax tree.
 */
class Program : public Node {
    public:
        vector<Node*> children;
        void accept (Visitor * v) {
            v->visit(this);
        }
};

/**
 * Read in the file by recursive descent.
 * Modify as necessary and add whatever functions you need to get things done.
 */
void parse(fstream & file, Program * program) {
    int nesting = 0;
    // How to peek at the next character
   // c = (char)file.peek();
    // How to print out that character
   // cout << c;
    // How to read a character from the file and advance to the next character
    //file >> c;
    // How to print out that character
    //cout << c;
    // How to insert a node into the program.
    //program->children.push_back(new CommandNode(c));

	 int position = 0;
	program(program, file, nesting, position);
	
}
	 char peek(fstream & file, int position){
		char c;
		if(position == ftell(file)){
			return 0;
		}
		c = (char)file.peek();
		return c;
	}
	 void eat(char letter, int position){
		char c;
		file >> c;
		if(letter == c){
			position ++;
		} else{
			return 0;
		}
	}
	 void program(Program* p, fstream & file, int* nesting, int* position) {
		switch(peek(file, position)){
		case '[':
			eat('[', position);
			Loop l;
			p->children.push_back(l);
			sequence(l, file, nesting, position, p);
			break;
		case '+':
			inc(program, file, position);
			break;
		case '-':
			dec(program, file, position);
			break;
		case '.':
			out(program, file, position);
			break;
		case ',':
			in(program, file, position);
			break;
		case '>':
			pinc(program, file, position);
			break;
		case '<':
			pdec(program, file, position);
			break;
		}
	}
	 Program sequence(Loop* l, fstream & file, int* nesting, int* position, Program * p){
		switch(peek(file, position)){
			case '[':
				eat('[', position);
				nesting++;
				loop(l, file, nesting, position, p);
				break;
			case ']':
				eat(']', position);
				end_loop(l, file, nesting, position, p);
				break;
			case '+':
				inc(l, file, nesting, position);
				break;
			case '-':
				dec(l, file,nesting, position);
				break;
			case '.':
				out(l, file,nesting, position);
				break;
			case ',':
				in(l, file,nesting, position);
				break;
			case '>':
				pinc(l, file,nesting, position);
				break;
			case '<':
				pdec(l, file,nesting, position);
				break;
		}
	}
	public void loop(Loop* l, fstream & file, int* nesting, int* position, Program * p){
		l->children.push_back(new CommandNode('['));
		sequence(l, file, nesting, position, p);
	}
	public void end_loop(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		l->children.push_back(new CommandNode(']'));
		if(nesting == 0){
			program(p,file,nesting,position);
		}else{
			nesting--;
			sequence(l, file, nesting, position, p);
		}
	}
	 public void inc(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		if(peek(file, position) == '+'){
			eat('+', position);
			l->children.push_back(new CommandNode('+'));
			sequence(l, file, nesting, position, p);
		}
	}
	public void dec(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		if(peek(file, position) == '-'){
			eat('-', position);
			l->children.push_back(new CommandNode('-'));
			sequence(l, file, nesting, position, p);
		}
	}
	public void out(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		if(peek(file,position) == '.'){
			eat('.', position);
			l->children.push_back(new CommandNode('.'));
			sequence(l, file, nesting, position, p);
		}
	}
	public void in(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		if(peek(file, position) == ','){
			eat(',', position);
			l->children.push_back(new CommandNode(','));
			sequence(l, file, nesting, position, p);
		}
	}
	public void pinc(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		if(peek(file, position) == '>'){
			eat('>', position);
			l->children.push_back(new CommandNode('>'));
			sequence(l, file, nesting, position, p);
		}
	}
	public void pdec(Loop* l, fstream & file, int* nesting, int* position, Program* p){
		if(peek(file, position) == '<'){
			eat('<', position);
			l->children.push_back(new CommandNode('<'));
			sequence(l, file, nesting, position, p);
		}
	}
	
	public void inc(Program* p, fstream & file, int* nesting, int* position){
		if(peek(file, position) == '+'){
			eat('+', position);
			p->children.push_back(new CommandNode('+'));
			program(p, file, nesting, position);
		}
	}
	public void dec(Program* p, fstream & file, int* nesting, int* position){
		if(peek(file, position) == '-'){
			eat('-' position);
			p->children.push_back(new CommandNode('-'));
			program(p, file, nesting, position);
		}
	}
	public void out(Program* p, fstream & file, int* nesting, int* position){
		if(peek(file, position) == '.'){
			eat('.', position);
			p->children.push_back(new CommandNode('.'));
			program(p, file, nesting, position);
		}
	}
	public void in(Program* p, fstream & file, int* nesting, int* position){
		if(peek(file, position) == ','){
			eat(',', position);
			p->children.push_back(new CommandNode(','));
			program(p, file, nesting, position);
		}
	}
	public void pinc(Program* p, fstream & file,int* nesting, int* position){
		if(peek(file, position) == '>'){
			eat('>', position);
			p->children.push_back(new CommandNode('>'));
			program(p, file, nesting, position);
		}
	}
	public void pdec(Program* p, fstream & file, int* nesting, int* position){
		if(peek(file, position) == '<'){
			eat('<', position);
			p->children.push_back(new CommandNode('<'));
			program(p, file, nesting, position);
		}
	}

/**
 * A printer for Brainfuck abstract syntax trees.
 * As a visitor, it will just print out the commands as is.
 * For Loops and the root Program node, it walks trough all the children.
 */
class Printer : public Visitor {
    public:
        void visit(const CommandNode * leaf) {
            switch (leaf->command) {
                case INCREMENT:   cout << '+'; break;
                case DECREMENT:   cout << '-'; break;
                case SHIFT_LEFT:  cout << '<'; break;
                case SHIFT_RIGHT: cout << '>'; break;
                case INPUT:       cout << ','; break;
                case OUTPUT:      cout << '.'; break;
            }
        }
        void visit(const Loop * loop) {
            cout << '[';
            for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                (*it)->accept(this);
            }
            cout << ']';
        }
        void visit(const Program * program) {
            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }
        }
};

int main(int argc, char *argv[]) {
    fstream file;
    Program program;
    Printer printer;
    if (argc == 1) {
        cout << argv[0] << ": No input files." << endl;
    } else if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            file.open(argv[i], fstream::in);
            parse(file, & program);
            program.accept(&printer);
            file.close();
        }
    }
}
