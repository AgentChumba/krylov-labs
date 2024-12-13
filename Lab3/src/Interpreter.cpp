#include "../include/Interpreter.h"

Interpreter::Interpreter() {
    variables["resistor"] = 0;
    variables["cuvette"] = 0;
}

bool isInteger(const std::string& str) {
    try {
        std::stoi(str);
        return true;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool Interpreter::isAllowedVariable(const std::string& variable) {
    return variable == "resistor" || variable == "cuvette";
}

std::string Interpreter::interpret(const char *text)
{
    result = "";
    std::vector<std::string> lines;
    std::stringstream stream(text);
    std::string line;
    while (getline(stream, line)) {
        lines.push_back(line);
    }
    int index = 0;
    executeLines(lines, index, 1);
    return result;
}

void Interpreter::executeLines(const std::vector<std::string> &lines, int &index, int level)
{
    for (; index < lines.size(); ++index) {
        std::string trimmedLine = trim(lines[index]);
        
        if (trimmedLine.compare(0, 4, "loop") == 0) {
            handleLoop(trimmedLine, lines, index, level);
        }
        else if (trimmedLine.compare(0, 3, "let") == 0) {
            handleAssignment(trimmedLine);
        }
        else if (trimmedLine.compare(0, 10, "experiment") == 0) {
            handleExperiment();
        }
        else if (trimmedLine.compare(0, 7, "saveall") == 0) {
            handleSaveAll();
        }
    }
}

void Interpreter::handleLoop(const std::string &line, const std::vector<std::string> &lines, int &index, int level)
{
    std::istringstream iss(line);
    std::string keyword, variable;
    char equalSign;
    int min, max;

    iss >> keyword >> variable >> equalSign;

    if (equalSign != '=') {
        result += "Ошибка: Ожидается '=' после переменной.\n";
        return;
    }

    if (!isAllowedVariable(variable)) {
        result += "Ошибка: недопустимое имя переменной '" + variable + "'. Используйте только resistor или cuvette.\r\n";
        return;
    }

    std::string range;
    iss >> range;

    size_t pos = range.find("..");
    if (pos == std::string::npos || pos == 0 || pos == range.length() - 2) {
        result += "Ошибка: Неверный формат диапазона.\n";
        return;
    }

    std::string minPart = range.substr(0, pos);
    if (isAllowedVariable(minPart)) {
        min = variables[minPart];
    } else if (isInteger(minPart)) {
        min = std::stoi(minPart);
    } else {
        result += "Ошибка: Некорректное начальное значение диапазона '" + minPart + "'.\n";
        return;
    }

    std::string maxPart = range.substr(pos + 2);
    if (isInteger(maxPart)) {
        max = std::stoi(maxPart);
    } else {
        result += "Ошибка: Конечное значение диапазона должно быть числом.\n";
        return;
    }

    if (min > max) {
        result += "Ошибка: Начальное значение цикла больше конечного.\n";
        return;
    }

    Loop loop;
    loop.variable = variable;
    loop.min = min;
    loop.max = max;
    loop.current = min;
    loop.level = level;
    variables[loop.variable] = loop.current;

    result += "Начало цикла " + std::to_string(loop.level) + " уровня: " + loop.variable + " от " + std::to_string(min) + " до " + std::to_string(max) + "\r\n";
    int counter_inner_loop = 0;

    index++;
    while (index < lines.size()) {
        std::string nextLine = trim(lines[index]);
        loop.body.push_back(nextLine);

        if (nextLine.compare(0, 4, "loop") == 0) {
            counter_inner_loop++;
        }
        
        if (nextLine == "endloop") {
            if (counter_inner_loop != 0) {
                counter_inner_loop--;
            }
            else {
                break;
            }
        }
        index++;
    }
    loopStack.push(loop);

    executeLoopBody(loop);
}

void Interpreter::executeLoopBody(Loop &loop)
{
    do {
        int index = 0;
        while (index < loop.body.size()) {
            std::string trimmedLine = trim(loop.body[index]);
            if (trimmedLine.compare(0, 4, "loop") == 0) {
                handleLoop(trimmedLine, loop.body, index, loop.level + 1);
            }
            else if (trimmedLine.compare(0, 3, "let") == 0) {
                handleAssignment(trimmedLine);
            }
            else if (trimmedLine.compare(0, 10, "experiment") == 0) {
                handleExperiment();
            }
            else if (trimmedLine.compare(0, 7, "saveall") == 0) {
                handleSaveAll();
            }
            index++;
        }

        if (variables[loop.variable] >= loop.max) {
            break;
        }

        loop.current++;
        variables[loop.variable] = loop.current;

    } while (loop.current <= loop.max);

    result += "Выход из цикла " + std::to_string(loop.level) + " уровня" + "\r\n";
}

void Interpreter::handleAssignment(const std::string &line)
{
    std::istringstream iss(line);
    std::string keyword, variable, equal;
    int value;

    iss >> keyword >> variable >> equal >> value;

    if (!isAllowedVariable(variable)) {
        result += "Ошибка: недопустимое имя переменной '" + variable + "'. Используйте только resistor или cuvette.\r\n";
        return;
    }

    variables[variable] = value;
    result += "Присваивание: " + variable + " = " + std::to_string(value) + "\r\n";
}

void Interpreter::handleExperiment()
{
    result += "Запуск эксперимента со значениями: resistor = " +
                std::to_string(variables["resistor"]) +
                ", cuvette = " + std::to_string(variables["cuvette"]) + "\r\n";
}

void Interpreter::handleSaveAll()
{
    result += "Сохранение результатов\r\n";
}

std::string Interpreter::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
