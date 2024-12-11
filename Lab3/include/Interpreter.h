#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <stack>

class Interpreter {
public:
    Interpreter();
    std::string interpret(const char* text);

private:
    struct Loop {
        std::string variable;
        int min;
        int max;
        int current;
        std::vector<std::string> body;
        int level;
    };

    std::string result = "";
    std::stack<Loop> loopStack;
    std::map<std::string, int> variables;

    void executeLines(const std::vector<std::string>& lines, int& index, int level);
    void handleLoop(const std::string& line, const std::vector<std::string>& lines, int& index, int level);
    void executeLoopBody(Loop& loop);
    void handleAssignment(const std::string& line);
    void handleExperiment();
    void handleSaveAll();
    bool isAllowedVariable(const std::string& variable);
    std::string trim(const std::string& str);
};

#endif // INTERPRETER_H
