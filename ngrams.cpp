// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <iostream>
#include <fstream>
#include "filelib.h"
#include "console.h"
#include "queue.h"
#include "map.h"
#include "vector.h"
#include "random.h"
#include "simpio.h"
using namespace std;

void addPrefixToMap(Map< Queue<string>, Vector<string>>& m, Queue<string>& window, string word);
void readFileToMap(Map< Queue<string>, Vector<string>>& m, int& n);
void displayMap(Map< Queue<string>, Vector<string>>& m);
void generateRandomText(Map< Queue<string>, Vector<string>>& m, int numWords);


int main() {

    Map< Queue<string> , Vector<string> > nGramsMap;

    int n = 0;
    int numWords = 0;
    readFileToMap(nGramsMap, n);
    //displayMap(nGramsMap);

    numWords = getInteger("\n# of random words to generate (0 to quit): ", "Illegal integer format. Try again.");

    while (numWords != 0) {
        if (numWords < n){
            cout << "Must be at least " << n << " words." << endl;
        } else {
            generateRandomText(nGramsMap, numWords);
        }
        numWords = getInteger("\n# of random words to generate (0 to quit): ", "Illegal integer format. Try again.");
    }

    cout << "Exiting." << endl;
    return 0;
}


void readFileToMap(Map< Queue<string>, Vector<string>>& m, int& n){

    ifstream infile;
    promptUserForFile(infile, "Dictionary file name: ", "Unable to open that file. Try again.");

    string word;
    Queue<string> wrappedWords;
    Queue<string> window;

    n = getInteger("Value of N? ", "Illegal integer format. Try again.");
    while (n < 2){
        cout << "N must be 2 or greater." << endl;
        n = getInteger("Value of N? ", "Illegal integer format. Try again.");
    }

    int counter = 0;

    while (infile >> word) {
        if (counter < n - 1){
            wrappedWords.enqueue(word);
        }
        counter++;

        if (window.size() < n - 1){
            window.enqueue(word);
        } else {
            addPrefixToMap(m, window, word);
        }
    }
    infile.close();

    for (string word: wrappedWords){
        addPrefixToMap(m, window, word);
    }
}


void addPrefixToMap(Map< Queue<string>, Vector<string>>& m, Queue<string>& window, string word){

    Queue<string> newKey = window;

    if (m.containsKey(newKey)){
        m[newKey].add(word);
    }else {
        m[newKey] = {word};
    }

    window.dequeue();
    window.enqueue(word);
}


void displayMap(Map< Queue<string>, Vector<string>>& m){
    for (Queue<string> nGram: m){
        cout << nGram << " : " << m[nGram] << endl;
    }
}


void generateRandomText(Map< Queue<string>, Vector<string>>& m, int numWords){

    int prefixInd = randomInteger(0,m.keys().size()-1);
    Queue<string> window = m.keys()[prefixInd];

    cout << "... ";
    for (int i = 0; i < numWords; i++){

        int suffixInd = randomInteger(0,m[window].size()-1);
        string suffix = m[window][suffixInd];

        cout << window.dequeue() << " ";
        window.enqueue(suffix);
    }

    cout << "..."<< endl;
}



