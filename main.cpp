#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <string>
// -------------------------------------------text container ------------------------------------------------------------------------------------------------------
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


//-------------------------------------text editor---------------------------------------------------------------------------------------------------------------
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


// ----------------------------------------- caesar lib ----------------------------------------------------------------------------------------------------------
class Caesar {
public:
    Caesar();
    ~Caesar();

    std::string encrypt(const std::string& text, int key);
    std::string decrypt(const std::string& text, int key);
    int searchIndex(char c);

private:
    HINSTANCE handle;
    typedef int(*search_index_ptr_t)(char);
    typedef std::string(*encrypt_ptr_t)(std::string, int);
    typedef std::string(*decrypt_ptr_t)(std::string, int);

    search_index_ptr_t search_index;
    encrypt_ptr_t encrypt_fn;
    decrypt_ptr_t decrypt_fn;

    bool loadFunctions(const std::string& libraryPath);
};

Caesar::Caesar() : handle(nullptr), search_index(nullptr), encrypt_fn(nullptr), decrypt_fn(nullptr) {
    std::string libraryPath = "C:/!uni/programing-paradigms/caesar/cmake-build-debug/libcaesar.dll";

    if (!loadFunctions(libraryPath)) {
        std::cerr << "Failed to load functions from the DLL: " << libraryPath << std::endl;
        exit(1);
    }
}

Caesar::~Caesar() {
    if (handle) {
        FreeLibrary(handle);
    }
}

bool Caesar::loadFunctions(const std::string& libraryPath) {
    handle = LoadLibrary(libraryPath.c_str());
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        std::cerr << "DLL not found: " << libraryPath << std::endl;
        return false;
    }

    search_index = (search_index_ptr_t)GetProcAddress(handle, "search_index");
    if (!search_index) return false;

    encrypt_fn = (encrypt_ptr_t)GetProcAddress(handle, "encrypt");
    if (!encrypt_fn) return false;

    decrypt_fn = (decrypt_ptr_t)GetProcAddress(handle, "decrypt");
    if (!decrypt_fn) return false;

    return true;
}

std::string Caesar::encrypt(const std::string& text, int key) {
    return encrypt_fn(text, key);
}

std::string Caesar::decrypt(const std::string& text, int key) {
    return decrypt_fn(text, key);
}

int Caesar::searchIndex(char c) {
    return search_index(c);
}


// ----------------------------------------------- user interface -----------------------------------------------------------------------------------------------
int getCommand() {
    int command;
    std::cout << "Enter 1 to encrypt" << std::endl;
    std::cout << "Enter 2 to decrypt" << std::endl;
    std::cout << "Enter 0 to exit" << std::endl;
    std::cout << ">>>";
    std::cin >> command;
    return command;
}

int getKey() {
    int key;
    std::cout << "Enter the key for encrypt/decrypt" << std::endl;
    std::cout << ">>>";
    std::cin >> key;
    return key;
}



int main() {
    int command;
    int key;
    TextEditor editor;
    TextContainer loadedText;
    Caesar caesar;
    std::string cryptedtext;
    editor.saveToFile("new_example.txt", const_cast<char*>(cryptedtext.data()));

    do {
        command = getCommand();
        key = getKey();

        std::string loadfilename;
        std::cout << "Enter the name of file to load from" << std::endl;
        std::cin >> loadfilename;
        editor.loadFromFile(loadfilename, loadedText);
        std::cout << "Text loaded" << std::endl;

        switch (command) {
            case 1: {
                cryptedtext = caesar.encrypt(loadedText.c_str(), key);
                break;
            }
            case 2: {
                cryptedtext = caesar.decrypt(loadedText.c_str(), key);
                break;
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
        std::string savefilename;
        std::cout << "Enter the name of file to save to" << std::endl;
        std::cin >> savefilename;
        editor.saveToFile(savefilename, const_cast<char*>(cryptedtext.data()));
        std::cout << "Text saved:" << std::endl;
    } while (command != 0);
    return 0;
}