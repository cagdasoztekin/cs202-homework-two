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
    
    void countWords(int& count)
    {
        // recursively sums total word count traversing the tree in pre-order
        if(this != NULL)
        {
            // count is incremented each time the function hits a nonnull node
            count++;
            
            // and goes on to check current node's children
            leftChild->countWords(count);
            rightChild->countWords(count);
        }
    }
    
    void printCount()
    {
        // recursively prints current node's word and word count traversing the tree in pre-order
        if(this != NULL)
        {
            printf("\"%s\" appears %d time(s)\n", this->word.c_str(), this->count);
            this->leftChild->printCount();
            this->rightChild->printCount();
        }
    }
    
    
    int check(int height)
    {
        // recursively find tree's height by always comparing the heights of subtrees belonging to two children
        // and moving on with the child having the max height, the function returns a value multiple times
        // depending on its number of nodes, however the last value it returns is the actual height of the tree
        // it takes the longest time because it recursively calls itself height - 1 times, taking the longest time
        if(this != NULL)
        {
            height++;
            height = max(leftChild->check(height), rightChild->check(height));
        }
        
        return height;
    }
    
    // returns bth power of number a
    int pow(int a, int b)
    {
        int sum = 1;
        for(int i = b; i > 0; i--)
        {
            sum *= a;
        }
        if(b >= 0)
            return sum;
        else // so that a tree with height 1 counts as a complete tree
            return 1;
    }
    
    bool checkComplete(int curHeight, int& status, int height, int& count)
    {
        // a tree with height 2 is checked and it's status determined right away because special conditions apply to it
        // however, it's open to discussion how efficient it is to check every time if the height equals 2 especially
        // in a recursive function that will mostly check completeness of trees with heights much larger than 2
        if(height == 2)
        {
            return (leftChild != NULL && rightChild != NULL) || (leftChild != NULL && rightChild == NULL);
        }
        // goes on looking until it find the nodes with height (height - 1), to check completeness of the tree by checking
        // the children of nodes with height - 1
        if(curHeight < height - 1 && this != NULL)
        {
            this->leftChild->checkComplete(curHeight + 1, status, height, count);
            this->rightChild->checkComplete(curHeight + 1, status, height, count);
        }
        // now that the nodes with height (height - 1) are found, **I'd like to emphasize this checks the nodes from left to right**
        else if(this != NULL)
        {
            // status 2 means there has not yet been encountered a node of height - 1 that does not have two children
            // as long as appropriate trees are encountered count is incremented, if not it is not incremented
            // in the end function will check to see if count equals the number of nodes with height (height - 1)
            if(status == 2)
            {
                // another node to the right, with two children
                if( (this->rightChild != NULL && this->leftChild != NULL))
                {
                    count++;
                }
                // this node only has its left child initialized status is changed to 1
                // status 1 means there has been encountered a node with only its left child initialized
                // if the next node switches the status to 2, or the status remains as 1 it will terminate incrementing
                // count, meaning the tree is not complete
                else if( (this->leftChild != NULL && this->rightChild == NULL))
                {
                    status = 1;
                    count++;
                }
                // current node has no children, status 0 means no children if status 2 or 1 comes after status 0,
                // the function will terminate incrementing count, meaning the tree is not complete
                else
                {
                    count++;
                    status = 0;
                }
            }
            // only status 0 can come after status 1, so count is not incremented for other conditions
            else if(status == 1)
            {
                if( (this ->rightChild != NULL && this->leftChild != NULL) || (this->leftChild != NULL && this->rightChild == NULL) )
                {
                }
                else {
                    count++;
                    status = 0;
                }
            }
            // only status 0 can come after status 0, so count is not incremented for other conditions
            else
            {
                if( (this ->rightChild != NULL && this->leftChild != NULL) || (this->leftChild != NULL && this->rightChild == NULL) )
                {
                }
                else
                {
                    count++;
                }
            }

        }
        // meaning that the tree is not a full tree without considering nodes with height (height)
        else
        {
            return false;
        }
        
        // all nodes with height (height - 1) satisfy the conditions of a complete tree are counted and it equals to their number
        if(count == pow(2,height-2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
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
    virtual void addWord(string& newWord);
    /* So instead I'd like to use it in this new cool function to prove my capability of employing exceptions! */
    virtual void deleteTree(WordNode*& node) throw(TreeException);
    virtual int getTotalWordCount();
    virtual void printWordCounts();
    bool isComplete();
private:
    WordNode* root;
};