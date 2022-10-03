//
//  main.cpp
//  Project3
//
//  Created by Arteen Abrishami on 7/22/22.
// no while for goto or STL algorithms, no global variables/statics, and leave function parameters alone -
// must use MAXRESULTS and MAXDICTWORDS
// changing stack size - can't find it. ulimit -s 8000
//
#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <string>
using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 10; // Max words that can be read in
// no std algs, no do while for goto, onlysubstr and size, all repition with recursion

int lexiconBuilderHelper(istream &dictfile, string dict[], int count)
{
    if (count >= MAXDICTWORDS)
        return MAXDICTWORDS;


    if (getline(dictfile, dict[0]))
    {
        return lexiconBuilderHelper(dictfile, dict + 1, count + 1);
    }
        
    
    return count;
}

int lexiconBuilder(istream &dictfile, string dict[])

{
    return lexiconBuilderHelper(dictfile, dict, 0);
}

void checkSameness(string word, const string dict, int d, bool& setter)

{
    if (word.size() == 0)
    {
        setter = true;
        return;
    }
    if (d >= dict.size())
        return;
    
    if (word[0] == dict[d])
    {
        checkSameness(word.substr(1, word.size() - 1), dict.substr(0, d) + dict.substr(d + 1, dict.size() - d), 0, setter);
    }
    
    checkSameness(word, dict, d + 1, setter);
}

bool secondary (string& word, const string & dict)

{
    bool set = false;
    checkSameness(word, dict, 0, set);
    return set;
}

        
int theJumblerHelper(string& word, const string dict[], int size,
string results[], int& count)
{
    if (size == 0)
    {
        return count;
    }
    
    if (count == MAXRESULTS)
    {
        return MAXRESULTS;
    }
    
    if (word.size() == dict[0].size())
    {
        if (secondary(word, dict[0]))
        {
                results[count] = dict[0];
                count++;
        }
    }
    return count + theJumblerHelper(word, dict + 1, size - 1, results, count);
    
}

int theJumbler(string word, const string dict[], int size,
string results[])
{
    if (word == "")
        return 0;
    
    int count = 0;
    theJumblerHelper(word, dict, size, results, count);
    return count;
   
}


void divulgeSolutions(const string results[], int size)
/* prints out size number of strings from results*/
{
    if (size <= 0)
        return;
    
    cout << "Matching word: " << results[0] << endl;
    
    divulgeSolutions(results + 1, size - 1);
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>

using namespace std;

void testone(int n)
{
    string dictionary[MAXDICTWORDS];
    string results[MAXRESULTS];

    switch (n)
    {
        default: {
            cout << "Bad argument" << endl;
        } break; case 1: {
            istringstream iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod");
            int numResults = lexiconBuilder(iss, dictionary);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 9 && dictionary[0] == "act" && dictionary[1] == "art");
        } break; case 2: {
            // File is empty, checks that file is empty and lexiconBuilder returns 0.
            istringstream iss("");
            int numResults = lexiconBuilder(iss, dictionary);
            assert(numResults == 0 && dictionary[0] == "");
        } break; case 3: {
            // Dictionary has been read properly
            istringstream iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod");
            int numResults = lexiconBuilder(iss, dictionary);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 9 && dictionary[0] == "act" && dictionary[numResults-1] == "tar");
        } break; case 4: {
            // Input file is larger than the dictionary size
            istringstream iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod\ntoo\nmany\nwords");
            int numResults = lexiconBuilder(iss, dictionary);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 10 && dictionary[MAXDICTWORDS-1] == "too");
        } break; case 5: {
            // If a certain word with repeated letter is shown in results more than once - still accept.
            string dictionary[] = { "one", "oone", "ne", "e", "too" };
            int numResults = theJumbler("oto", dictionary, 5, results);
            assert((numResults == 1 || numResults == 2) && results[0] == "too");
        } break; case 6: {
            // Doesn't check numResults nor duplicates.
            string dictionary[] = { "one", "oone", "ne", "e", "too" };
            theJumbler("oto", dictionary, 5, results);
            assert(results[0] == "too");
        } break; case 7: {
            // If word wasn't found, numResults = 0 and results array is empty.
            string dictionary[] = { "one", "oone", "ne", "e" };
            int numResults = theJumbler("look", dictionary, 4, results);
            assert(numResults == 0 && results[0] == "" && results[1] == "");
        } break; case 8: {
            // No fraction of a permutation is being searched in dictionary
            string dictionary[] = { "one", "oone", "non", "oon" };
            int numResults = theJumbler("on", dictionary, 4, results);
            assert(numResults == 0 && results[0] == "" && results[1] == "");
        } break; case 9: {
            // No fraction of a permutation is being searched in dictionary
            string dictionary[] = { "one", "oone", "ne", "e", "neoo", "oneo" };
            int numResults = theJumbler("neo", dictionary, 6, results);
            assert(numResults == 1 && results[0] == "one" && results[1] == "");
        } break; case 10: {
            // Checking that no error occurs if more than MAXRESULTS are found.
            string dictionary[] = { "true",  "treu", "teru", "teur", "ture",
        "tuer", "rtue", "rteu", "retu","reut", "ruet", "rute", "utre",
        "uter", "uetr", "uert", "urte", "uret", "etru", "etur", "ertu",
        "erut", "eurt", "eutr" };
            // All 24 permutations
            int numResults = theJumbler("true", dictionary, 24, results);
            assert(numResults == MAXRESULTS);
        } break; case 11: {
            // Checking one word was found, no duplicates.
            string dictionary[] = { "ppp" };
            int numResults = theJumbler("ppp", dictionary, 1, results);
            assert(numResults == 1 && results[0] == "ppp" && results[1] == "");
        } break; case 12: {
            string dictionary[] = { "run", "dodge", "break", "urn", "defeat" };
            int numResults = theJumbler("nru", dictionary, 5, results);
            sort(results, results + numResults);
            assert(numResults == 2 && results[0] == "run" && results[1] == "urn");
        } break; case 13: {
            streambuf* oldCoutStreamBuf = cout.rdbuf();
            ostringstream strCout;
            cout.rdbuf(strCout.rdbuf());
            string results[] = { "cat", "act"};
            divulgeSolutions(results, 2);
            cout.rdbuf(oldCoutStreamBuf);
            string temp = strCout.str();
            bool match1 = temp.find("act") != string::npos;
            bool match2 = temp.find("cat") != string::npos;
            assert(match1 && match2);
        } break; case 14: {
            istringstream iss ("tier\nrite\nbate\ntire\nttir");
            int numWords = lexiconBuilder(iss, dictionary);
            sort(dictionary, dictionary + numWords);
            assert(numWords == 5 && dictionary[0] == "bate");
            int numResults = theJumbler("tier", dictionary, numWords, results);
            assert(numResults == 3 && (results[2] == "tire" || results[2] == "tier" || results[2] == "rite"));
        } break; case 15: {
            string example[] = { "kool", "moe", "dee" };
            int numResults = theJumbler("look", example, 3, results);
            assert(numResults == 1 && results[0] == "kool");
        } break;
    }
}

int main()
{
    for (int n = 1; n <= 15; n++) {
         testone(n);
        cout << n << " passed" << endl;
    }

    return 0;
}


//
//int main()
//{
//    string results[MAXRESULTS];
//    string dict[MAXDICTWORDS];
//    ifstream dictfile;         // file containing the list of words
//    int nwords;                // number of words read from dictionary
//    string word;
//
//    dictfile.open("words.txt");
//    if (!dictfile) {
//        cout << "File not found!" << endl;
//        return (1);
//    }
//
//    nwords = lexiconBuilder(dictfile, dict);
//
//    cout << nwords << endl;
//
//    cout << "Please enter a string for an anagram: ";
//    cin >> word;
//
//    int numMatches = theJumbler(word, dict, nwords, results);
//    if (!numMatches)
//        cout << "No matches found" << endl;
//    else
//    {
//        divulgeSolutions(results, numMatches);
//    }
//
//    cout << "numMatches: " << numMatches << endl;
//
////
////    string exampleDict[] = {"kool", "moe", "dee"};
////
////    int numResults = theJumbler("kloo", exampleDict, 3, results);
////    assert(numResults == 1 && results[0] == "kool");
//
////    string a = "arteen";
////    cerr << a.substr(1, a.size() - 1) << endl;
//    return 0;
//}

//string theAlteratorNonRecursive(const string alteratee, size_t size)
//{
//    string alteration = alteratee;
//
//    if (size == 0)
//        return "";
//
//
//    for (size_t k = size; k != 0 ; k --) {
//        int i = 0;
//    while (i < size - 1)
//    {
//        if (alteration[i] > alteration[i + 1])
//        {
//            char a = alteration[i];
//            alteration[i] = alteration[i + 1];
//            alteration[i + 1] = a;
//        }
//
//        i ++;
//    }
//    }
//
//    return alteration;
//}
//
//void Loop(string& alteration, int i, const size_t end)
//{
//    if (i >= end)
//        return;
//
//    if (alteration[i] > alteration[i + 1])
//    {
//        char a = alteration[i];
//        alteration[i] = alteration[i + 1];
//        alteration[i + 1] = a;
//    }
//
//    Loop(alteration, i + 1, end);
//}
//
//void mainLoop(string& alteration, const size_t size, size_t k, int i)
//{
//    if (k <= 0)
//        return;
//
//    Loop(alteration, i, size - 1);
//
//    mainLoop(alteration, size, k - 1, 0);
//}
//
//string theAlterator(const string alteree, size_t size)
//{
//    string alteration = alteree;
//
//    if (size == 0)
//        return "";
//
//    mainLoop(alteration, size, size, 0);
//
//
//    return alteration;
//}


//bool checkPermutation(string& word, const string dict, size_t size, int i, const size_t oSize)
//{
//    if (size == 0)
//        return true;
//    if (i >= oSize)
//        return false;
//
//    string a = {dict[i]};
//    if (word.substr(size - 1, 1) == a)
//    {
//        if (size != 0)
//            checkPermutation(word, dict, size - 1, 0, oSize);
//        else
//            return true;
//    }
//    else
//        checkPermutation(word, dict, size, i + 1, oSize);
//
//    return false;
//}             // not working with repeat o's because it processes and skips one that's needeed by counting twice
