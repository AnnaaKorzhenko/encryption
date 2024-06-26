#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>

class TextContainer {
public:
    TextContainer(const char* initialData = "") {
        length = std::strlen(initialData);
        data = new char[length + 1];
        std::strcpy(data, initialData);
    }

    ~TextContainer() {
        delete[] data;
    }

    size_t size() const {
        return length;
    }

    const char* c_str() const {
        return data;
    }

    void setData(const char* newData) {
        delete[] data;
        length = std::strlen(newData);
        data = new char[length + 1];
        std::strcpy(data, newData);
    }

    void print() const {
        std::cout << data << std::endl;
    }
private:
    char* data;
    size_t length;
};

class TextEditor {
public:
    void loadFromFile(std::string filename, TextContainer& textContainer) {
        std::ifstream file(filename);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t length = file.tellg();
            file.seekg(0, std::ios::beg);

            char* buffer = new char[length + 1];
            file.read(buffer, length);
            buffer[length] = '\0';

            textContainer.setData(buffer);

            delete[] buffer;
            file.close();
        } else {
            std::cerr << "Could not open file: " << filename << std::endl;
        }
    }

    void saveToFile(std::string filename, const TextContainer& textContainer) {
        std::ofstream file(filename);
        if (file) {
            file.write(textContainer.c_str(), textContainer.size());
            file.close();
        } else {
            std::cerr << "Could not open file: " << filename << std::endl;
        }
    }
};