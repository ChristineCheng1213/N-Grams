/* Name: Xiaoyu Cheng (Christine)
 * Course/Section: CS106B Thur 3:30pm
 * Description: This file reads a txt file from user and generates text
 *              that simulates the style of original author by implementing
 *              Markov chain.
 * Source: Stanford Library Documentation
 */

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
void buildMap(Map< Queue<string>, Vector<string>>& m, int& n);
void generateRandomText(const Map< Queue<string>, Vector<string>>& m, int numWords);


int main() {

    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl
         << "This program makes random text based on a document." << endl
         << "Give me an input file and an 'N' value for groups of" << endl
         << "words, and I will create random text for you." << endl << endl;

    int n = 0;
    Map< Queue<string> , Vector<string> > nGramsMap;

    buildMap(nGramsMap, n);

    int numWords = getInteger("\n# of random words to generate (0 to quit): ", "Illegal integer format. Try again.");

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


/*
 * Reads the user input file and build the map according to Markov chain algorithm.
 * The function accepts a reference to a map, which is a compound type whose key is
 * a queue of string (since we don't need to access by index) and value is vector of
 * string, and a reference to int, so that the function could directly make changes
 * to these variables.
 */
void buildMap(Map< Queue<string>, Vector<string>>& m, int& n){

    string word;
    Queue<string> wrappedWords;
    Queue<string> window;

    ifstream infile;
    promptUserForFile(infile, "Input file name: ", "Unable to open that file. Try again.");

    n = getInteger("Value of N: ", "Illegal integer format. Try again.");
    while (n < 2){
        cout << "N must be 2 or greater." << endl;
        n = getInteger("Value of N? ", "Illegal integer format. Try again.");
    }

    int count = 0;
    while (infile >> word) {
        if (count < n - 1){             //Store the first n-1 words to perform word wrap
            wrappedWords.enqueue(word);
        }
        count++;

        if (window.size() < n - 1){
            window.enqueue(word);       //Set window to be the first n-1 words
        } else {
            addPrefixToMap(m, window, word);
        }
    }
    infile.close();

    for (string word: wrappedWords){    //perform n-1 more iterations to wrap words
        addPrefixToMap(m, window, word);
    }
}


/*
 * Store the current prefix at window and its corresponding suffix as key-value pairs
 * to the map, then slide the window to the right by one word.
 * This function accepts a reference to map to change it directly, reference to queue
 * window, and a word, which is the suffix of the window.
 */
void addPrefixToMap(Map< Queue<string>, Vector<string>>& m, Queue<string>& window, string word){

    if (m.containsKey(window)){
        m[window].add(word);
    } else {
        m[window] = {word};
    }

    window.dequeue();
    window.enqueue(word);
}


/*
 * Generates and prints out a given number of random words according to algorithm.
 * Function accepts a constant reference to map (since map does not need to be changed)
 * and a target number of words to generate.
 */
void generateRandomText(const Map< Queue<string>, Vector<string>>& m, int numWords){

    int prefixInd = randomInteger(0,m.keys().size()-1);
    Queue<string> window = m.keys()[prefixInd];             //randomly choose a starting point

    cout << "... ";
    for (int i = 0; i < numWords; i++){

        int suffixInd = randomInteger(0,m[window].size()-1);
        string suffix = m[window][suffixInd];               //randomly choose a suffix

        cout << window.dequeue() << " ";
        window.enqueue(suffix);
    }

    cout << "..." << endl;
}
