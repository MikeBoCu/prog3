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
    char endChar = token[token.size()-1];
    if(ispunct(endChar))
        return true;
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

void printPasswords(std::string *passwords){

    for(int i = 0; i < 20; i++){
        std::cout << passwords[i] << std::endl;
    }
}

int getRandInt(int leftLim,int rightLim){
    std::random_device seed;
    std::mt19937 randomGenerator{seed()};
    std::uniform_int_distribution<int> distribution{leftLim,static_cast<int>(rightLim)};
    return distribution(randomGenerator);
}

template <std::size_t N>
bool arrayContains(int const (&array)[N], int lookingFor){

    for (auto& x : array){
        if(x == lookingFor)
            return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    //setting up variables for use
    std::string line;
    std::string potentialToken;
    std::set<std::string> passwordTokens;
    std::string passwordString;
    int usedIterations[5];
    std::string allPasswords[20];

    //will read input.txt, unless command line arg is provided.
    std::ifstream myfile("./input.txt");
    if(argc > 1)
        std::ifstream myfile(argv[1]);

    //text files have gibberish at the beginning, this skips it.
    SkipBOM(myfile);

    //reads the file and adds token to a set.
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            std::istringstream iss(line);
            do {
                iss >> potentialToken;
                unsigned int tokenSize = potentialToken.size();
                //check to make sure token meets criteria
                if (tokenSize <= 6 && tokenSize >= 3 && !containsInvalidChar(potentialToken)) {
                    setCaps(potentialToken);
                    passwordTokens.insert(potentialToken);
                }
            } while (iss);
        }
        myfile.close();
    }

    //get random num to iterate through set, this will be a password token
    int numOfIterations = getRandInt(1, static_cast<int>( passwordTokens.size()));
    usedIterations[0] = numOfIterations;

    auto iterator = passwordTokens.begin();
    for(int i = 0; i < 20; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < numOfIterations; k++) {
                iterator++;
            }
            passwordString.append(" " + *iterator);
            iterator = passwordTokens.begin();

            //checks that the same token isnt used twice in the same password
            do {
                numOfIterations = getRandInt(1, static_cast<int>( passwordTokens.size()));
            } while (arrayContains(usedIterations, numOfIterations));
            usedIterations[j+1] = numOfIterations;
        }
        passwordString.append("1");
        passwordString.erase(0, 1);
        allPasswords[i] = passwordString;
        passwordString.clear();
    }
    printPasswords(allPasswords);
    return 0;
}
