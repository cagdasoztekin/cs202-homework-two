/*
 * CS202-HW02
 * hw02.h
 * Çağdaş Öztekin 21201651 Section 1
 */

#include <iostream>
#include <stdio.h>

using namespace std;

class WordNode {
public:
private:
    string word;
    int count;
    WordNode* leftChild;
    WordNode* rightChild;
    
    // count is one since the word should at least appear once for the node to be created
    WordNode(const string word, WordNode* left, WordNode* right) : count(1), leftChild(NULL), rightChild(NULL)
    {
        this->word = word;
        this->leftChild = left;
        this->rightChild = right;
    }
    
    void countWords(int& count);
    void printCount();
    int check(int height);
    bool checkComplete(int curHeight, int& status, int height, int& count);
    
    friend class WordTree;
};

// a custiom exception for trees :)
class TreeException : public exception {
private:
    string msg;
public:
    virtual const char* what() const throw()
    {
        return msg.c_str();
    }
    TreeException(const string & message):
    exception(), msg(message)
    {
    };
    ~TreeException() throw() {};
};


class WordTree
{
public:
    WordTree();
    ~WordTree();
    /* I can't really fathom why there should be a TreeExcetpion for addWord, there is no explanation in the assignment */
    virtual void addWord(string& newWord) throw(TreeException);
    /* So instead I'd like to use it in this new cool function to prove my capability of employing exceptions! */
    virtual void deleteTree(WordNode*& node) throw(TreeException);
    virtual int getTotalWordCount();
    virtual void printWordCounts();
    bool isComplete();
private:
    WordNode* root;
};