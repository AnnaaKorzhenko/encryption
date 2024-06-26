#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

class TextContainer {
public:
    TextContainer();
    ~TextContainer();
    char* buffer;
    int currentSize;
    int capacity;
    void reallocate();
    void checkCapacity(int additionalSize);
    void clearBuffer();
};

class TextEditor {
public:
    TextEditor(TextContainer& text);
    ~TextEditor();
    void saveToFile(TextContainer& container);
    TextContainer loadFromFile();
private:
    TextContainer textBuffer;
};

TextContainer::TextContainer() : currentSize(0), capacity(100) {
    buffer = new char[capacity];
    if (!buffer) {
        std::cerr << "Memory allocation failed" << std::endl;
        std::exit(1);
    }
    buffer[0] = '\0';
}

TextContainer::~TextContainer() {
    delete[] buffer;
}

void TextContainer::reallocate() {
    capacity += 100;
    char* newBuffer = new char[capacity];
    if (!newBuffer) {
        std::cerr << "Memory allocation failed" << std::endl;
        std::exit(1);
    }
    std::memcpy(newBuffer, buffer, currentSize);
    delete[] buffer;
    buffer = newBuffer;
}

void TextContainer::checkCapacity(int additionalSize) {
    while (currentSize + additionalSize >= capacity) {
        reallocate();
    }
}

void TextContainer::clearBuffer() {
    delete[] buffer;
    buffer = new char[capacity];
    if (!buffer) {
        std::cerr << "Memory allocation failed" << std::endl;
        std::exit(1);
    }
    currentSize = 0;
    buffer[0] = '\0';
}

TextEditor::TextEditor(TextContainer& text) {
    textBuffer.buffer = new char[text.currentSize + 1];
    std::memcpy(textBuffer.buffer, text.buffer, text.currentSize);
    textBuffer.currentSize = text.currentSize;
    textBuffer.capacity = text.currentSize;
}

TextEditor::~TextEditor() {
    textBuffer.clearBuffer();
}

void TextEditor::saveToFile(TextContainer& container) {
    char fileName[30];
    std::cout << "Enter the name of the file you want to save to. Note that if there is something in this file, your text would be append to the end" << std::endl;
    std::cin >> fileName;
    FILE* file = fopen(fileName, "a");
    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    fwrite(container.buffer, sizeof(char), container.currentSize, file);
    fwrite("\n", sizeof(char), 1, file);
    fclose(file);
    std::cout << "Text saved to file" << std::endl;
}

TextContainer TextEditor::loadFromFile() {
    TextContainer container;

    char fileName[30];
    std::cout << "Enter the name of the file you want to load from" << std::endl;
    std::cin >> fileName;
    FILE* file = fopen(fileName, "r");
    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        return container;
    }

    container.clearBuffer();

    char c;
    while ((c = getc(file)) != EOF) {
        container.checkCapacity(1);
        container.buffer[container.currentSize++] = c;
    }
    container.buffer[container.currentSize] = '\0';

    fclose(file);
    std::cout << "Text loaded from file" << std::endl;

    return container;
}

int getCommand() {
    int command;
    std::cout << "Enter 1 to encrypt" << std::endl;
    std::cout << "Enter 2 to decrypt" << std::endl;
    std::cout << ">>>";
    std::cin >> command;
    return command;
}

int main() {
    int command;
    TextContainer textStorage;
    TextEditor textedit(textStorage);
    do {
        textStorage = textedit.loadFromFile();
        command = getCommand();
        switch (command) {
            case 1: {
                //encrypt
                std::cout << "e"<<std::endl;
                break;
            }
            case 2:{
                //decrypt
                std::cout<< "d"<<std::endl;
            }
            case 0: {
                std::cout << "Exit! Have a good day!" << std::endl;
                break;
            }
            default: {
                std::cout << "There is no such a command, sorry :(" << std::endl;
                break;
            }
        }
        textedit.saveToFile(textStorage);
        std::cout << "Saved!" << std::endl;
    } while (command != 0);
    return 0;
}