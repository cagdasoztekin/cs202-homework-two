/*
 * CS202-HW02
 * hw02.cpp
 * Çağdaş Öztekin 21201651 Section 1
 */

#include <stdio.h>

#include "hw02.h"

using namespace std;

// initialize an empty WordTree, only constructor
WordTree::WordTree(){
    root = NULL;
}
// deletes the whole tree, deleteTree is a recursive function that calls itself for the left and right children
WordTree::~WordTree(){
    deleteTree(root);
}

void WordTree::deleteTree(WordNode*& node) throw(TreeException)
{
    // nothing to delete if root is null, and the best part here is root is always deleted at the end after all the
    // other nodes are deleted!
    if(root == NULL)
    {
        throw TreeException("Delete failed");
    }
    // go as far as you can go to find all the leaves and delete from leaves to the root
    else if (node != NULL){
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
        node = NULL;
    }
}

/* I can't really fathom why there should be a TreeExcetpion for addWord, there is no explanation in the assignment */
void WordTree::addWord(string& newWord) throw(TreeException)
{
    /* as the assignment sheet dictates, capitalization should be ignored so I choose to convert all given words
     * to lower case to avoid any conflict
     */
    
    /* I can't fathom how C++ can't have a proper function to toggle case, so I have to initialize a char array here
     * it's length is given word's length
     */
    
    char s[newWord.length()];
    // tolower only applies to chars so I convert each letter to lower case one by one in a for loop
    for(int i = 0; i < newWord.length(); i++)
    {
        s[i] = tolower(newWord[i]);
    }
    
    // this will be the given word converted to lower case
    string str = "";
    
    // each lower case is appended to str because somehow char array s can also reach the original words letters unless stopped
    for(int i = 0; i < newWord.length(); i++)
    {
        str += s[i];
    }
    
    // if tree is empty the first word to come becomes root node's word
    if(root == NULL)
    {
        root = new WordNode(str, NULL, NULL);
        // cout<<"root is "<<str<<endl;
    }
    else
    {
        WordNode* ptr = root;
        
        // if root is already occupied, the most suitable node is found by comparing next nodes' words alphabetically
        while(ptr != NULL)
        {
            // means given word already added to the tree
            if(str.compare(ptr->word) == 0)
            {
                // in this case word count is incremented
                ptr->count++;
                // cout<<str<<" appears again"<<endl;
                break;
            }
            // means given word alphabetically appears earlier in a dictionary than the one it's compared now
            else if(str.compare(ptr->word) < 0)
            {
                // if current node's left child is empty than current word could be inserted
                if(ptr->leftChild == NULL)
                {
                    ptr->leftChild = new WordNode(str, NULL, NULL);
                    // cout<<"left child is "<<str<<endl;
                    break;
                }
                // else further comparisons will be made with current node's left child
                else
                {
                    ptr = ptr->leftChild;
                }
            }
            // means given word alphabetically appears later in a dictionary than the one it's compared now
            else
            {
                // if current node's right child is empty than current word could be inserted
                if(ptr->rightChild == NULL)
                {
                    ptr->rightChild = new WordNode(str, NULL, NULL);
                    // cout<<"right child is "<<str<<endl;
                    break;
                }
                // else further comparisons will be made with current node's right child
                else
                {
                    ptr = ptr->rightChild;
                }
            }
        }
    }
}

void WordTree::printWordCounts()
{
    // prints word counts for each node in pre-order
    WordNode* ptr = root;
    // only root's info is printed here so the output can be as it is on the assignment
    printf("\n\"%s\" appears %d time(s)\n", ptr->word.c_str(), ptr->count);
    ptr->leftChild->printCount();
    ptr->rightChild->printCount();
}

int WordTree::getTotalWordCount()
{
    // traverses each node summing up their word counts
    int count = 0;
    WordNode* ptr = root;
    ptr->countWords(count);
    return count;
}

bool WordTree::isComplete()
{
    // status 2 means every node with height h-1 has two children so far
    int height = 0;
    int status = 2;
    // height is found here
    height = root->check(height);
    int curHeight = 1;
    // count is used to check if all nodes with height h-1 apply the rules
    int count = 0;
    return root->checkComplete(curHeight, status, height, count);
}

void WordNode::countWords(int& count)
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

void WordNode::printCount()
{
    // recursively prints current node's word and word count traversing the tree in pre-order
    if(this != NULL)
    {
        printf("\"%s\" appears %d time(s)\n", this->word.c_str(), this->count);
        this->leftChild->printCount();
        this->rightChild->printCount();
    }
}

int WordNode::check(int height)
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

bool WordNode::checkComplete(int curHeight, int &status, int height, int &count)
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

// main function
int main(int argc, char **argv)
{
    WordTree wordTree;
    char word[1000];
    FILE * pFile;
    if (! (pFile = fopen("input.txt","r"))){
        return 1;
    }
    while (fscanf(pFile, "%s", word) != EOF)
    {
        string s(word);
        wordTree.addWord(s);
        printf("%s ", word);
    }
    fclose(pFile);
    printf("\nTotal word count:%d", wordTree.getTotalWordCount());
    wordTree.printWordCounts();
    printf("Word tree is complete: %s", wordTree.isComplete() ? "Yes" :
           "No");
    return 0;
}
