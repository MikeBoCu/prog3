#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <locale>
#include <random>

void SkipBOM(std::ifstream &in) {
    char container[3] = {0};
    in.read(container, 3);
    if ((unsigned char) container[0] == 0xEF && (unsigned char) container[1] == 0xBB &&
        (unsigned char) container[2] == 0xBF) {
        return;
    }
    in.seekg(0);
}

bool containsInvalidChar(std::string &token) {
    for (char const &c: token) {
        if(c< 0 || c > 255)
            return true;
        if (isdigit(c))
            return true;
    }
}

void setCaps(std::string &token) {
    std::locale loc;
    for (std::string::size_type i = 0; i < token.size(); i++) {
        if(i == 0)
            token[i] = toupper(token[i], loc);
        else
            token[i]= tolower(token[i], loc);
    }


}

int main() {
    std::string line;
    std::string delimter = " ";
    std::string const delimiters{" "};
    std::string potentialToken;
    std::string goodToken;
    std::ifstream myfile("./input.txt");
    std::set<std::string> passwordTokens;
    std::string passwordString;

    SkipBOM(myfile);
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            std::istringstream iss(line);
            do {
                iss >> potentialToken;
                unsigned int tokenSize = potentialToken.size();
                if (tokenSize <= 6 && tokenSize >= 3 && !containsInvalidChar(potentialToken)) {
                    setCaps(potentialToken);
                    passwordTokens.insert(potentialToken);
                    if(passwordTokens.size() == 207)
                        int i = 10;
                }
            } while (iss);
        }
        myfile.close();
    }

    std::random_device seed;
    std::mt19937 randomGenerator{seed()};
    std::uniform_int_distribution<int> distribution{1,static_cast<int>(passwordTokens.size())};
    int numOfIterations = distribution(randomGenerator);

    auto iterator = passwordTokens.begin();
    for(int i = 0; i < 4; i++) {
        for (int i = 0; i < numOfIterations; i++) {
            iterator++;
        }
        passwordString.append(" " + *iterator);
        iterator = passwordTokens.begin();
        numOfIterations = distribution(randomGenerator);
    }
    std::cout << passwordString;
    return 0;
}
