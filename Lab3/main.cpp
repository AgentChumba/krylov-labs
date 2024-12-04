// #include <iostream>
// #include <string>
// #include <vector>
// #include <sstream>
// #include <map>
// #include <stack>

// struct Loop {
//     std::string variable;
//     int min;
//     int max;
//     int current;
//     std::vector<std::string> body;
//     int level;
// };

// class Interpreter {
// public:
//     std::string interpret(const char* text) {
//         std::vector<std::string> lines;
//         std::stringstream stream(text);
//         std::string line;

//         while (getline(stream, line)) {
//             lines.push_back(line);
//         }

//         int index = 0;
//         executeLines(lines, index, 1);

//         return result;
//     }

// private:
//     std::string result;
//     std::stack<Loop> loopStack;
//     std::map<std::string, int> variables;

//     void executeLines(const std::vector<std::string>& lines, int& index, int level) {
//         for (; index < lines.size(); ++index) {
//             std::string trimmedLine = trim(lines[index]);

//             if (trimmedLine.compare(0, 4, "loop") == 0) {
//                 handleLoop(trimmedLine, lines, index, level);
//             }
//             else if (trimmedLine.compare(0, 4, "let ") == 0) {
//                 handleAssignment(trimmedLine);
//             }
//             else if (trimmedLine.compare(0, 10, "experiment") == 0) {
//                 handleExperiment();
//             }
//             else if (trimmedLine.compare(0, 6, "print ") == 0) {
//                 handlePrint(trimmedLine);
//             }
//         }
//     }
//     void handleLoop(const std::string& line, const std::vector<std::string>& lines, int& index, int level) {
//         std::istringstream iss(line);
//         std::string keyword, variable;
//         char equalSign;
//         int min, max;

//         iss >> keyword >> variable >> equalSign;

//         if (equalSign != '=') {
//             std::cerr << "Ошибка: Ожидается '=' после переменной." << std::endl;
//             return;
//         }

//         std::string range;
//         iss >> range;

//         size_t pos = range.find("..");
//         if (pos == std::string::npos || pos == 0 || pos == range.length() - 2) {
//             std::cerr << "Ошибка: Неверный формат диапазона." << std::endl;
//             return;
//         }

//         try {
//             min = std::stoi(range.substr(0, pos));
//             max = std::stoi(range.substr(pos + 2));
//         }
//         catch (const std::invalid_argument& e) {
//             std::cerr << "Ошибка: Невозможно преобразовать строки в числа." << std::endl;
//             return;
//         }

//         Loop loop;
//         loop.variable = variable;
//         loop.min = min;
//         loop.max = max;
//         loop.current = min;
//         loop.level = level;
//         variables[loop.variable] = loop.current;

//         result += "Начало цикла " + std::to_string(loop.level) + " уровня: " + loop.variable + " от " + std::to_string(min) + " до " + std::to_string(max) + "\r\n";
//         int counter_inner_loop = 0;

//         index++;
//         while (index < lines.size()) {
//             std::string nextLine = trim(lines[index]);
//             loop.body.push_back(nextLine);

//             if (nextLine.compare(0, 4, "loop") == 0) {
//                 counter_inner_loop++;
//             }
//             if (nextLine == "endloop") {
//                 if (counter_inner_loop != 0) {
//                     counter_inner_loop--;
//                 }
//                 else {
//                     break;
//                 }
//             }
//             index++;
//         }
//         loopStack.push(loop);

//         executeLoopBody(loop);
//     }
//     void executeLoopBody(Loop& loop) {
//     do {
//         int index = 0;
//         while (index < loop.body.size()) {
//             std::string trimmedLine = trim(loop.body[index]);
//             if (trimmedLine.compare(0, 4, "loop") == 0) {
//                 handleLoop(trimmedLine, loop.body, index, loop.level + 1);
//             }
//             if (trimmedLine.compare(0, 4, "let ") == 0) {
//                 handleAssignment(trimmedLine);
//             }
//             else if (trimmedLine.compare(0, 10, "experiment") == 0) {
//                 handleExperiment();
//             }
//             index++;
//         }

//         if (variables[loop.variable] >= loop.max) {
//             break;
//         }

//         loop.current++;
//         variables[loop.variable] = loop.current;

//     } while (loop.current <= loop.max);

//     result += "Выход из цикла " + std::to_string(loop.level) + " уровня" + "\r\n";
// }
//     void handleAssignment(const std::string& line) {
//         std::istringstream iss(line);
//         std::string keyword, variable, equal;
//         int value;

//         iss >> keyword >> variable >> equal >> value;
//         variables[variable] = value;
//         result += "Присваивание: " + variable + " = " + std::to_string(value) + "\r\n";
//     }
//     void handlePrint(const std::string& line) {
//         result += line.substr(6) + "\r\n";
//     }
//     void handleExperiment() {
//         result += "Запуск эксперимента\r\n";
//     }
//     void handleSaveAll() {
//         result += "Сохранение всех результатов\r\n";
//     }
//     std::string trim(const std::string& str) {
//         size_t first = str.find_first_not_of(' ');
//         size_t last = str.find_last_not_of(' ');
//         return str.substr(first, (last - first + 1));
//     }
// };

// int main() {
    
//     Interpreter interpreter;
//     const char* inputScript = "let x = 0\nloop r = 1..3\nloop c = 1..3\nlet c = 3\nexperiment\nendloop\nendloop";
//     std::string output = interpreter.interpret(inputScript);
//     std::cout << output << std::endl;

//     return 0;
// }
// // 