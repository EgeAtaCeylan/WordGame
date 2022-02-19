// Written by : Ege Ata Ceylan
//THIS PROGRAM IS A WORD GAME WHERE WE GET TWO WORDS FROM THE USER (STARTING WORD AND ENDING WORD) AND TRYING TO FIND IF IT IS
//POSSIBLE TO TRANSFORM STARTING WORD TO END WORD WITH SINGLE LETTER CHANGES,INSERTIONS OR DELETIONS
/*
  

 IF IT IS POSSIBLE TO TRANSFORM THE STARTING WORD TO END WORD WITH MULTIPLE DIFFERENT SEQUENCE OF TRANSFORMATIONS WE ARE GOING TO DISPLAY THE SHORTEST ONE TO THE USER
 
 ALL THE INTERMEDIATE WORDS MUST BE IN THE WORD LIST FILE
 
 WE GET NEW STARTING WORD AND ENDING WORD INPUTS FROM THE USER UNTIL THEY ENTER *
 
 BOTH THE STARTING WORD AND ENDING WORD MUST BE IN THE WORD LIST FILE
 
 WE FIRST READ ALL THE WORDS FROM THE GIVEN FILE AND
 THEN GENERATE ALL POSSIBLE TRANSFORMATIONS BETWEEN THE WORDS AND STORE THEM INSIDE A GRAPH
 
 SO RATHER THAN CHECKING FOR NEW TRANSFORMATIONS EVERY TIME NEW WORDS ARE ENTERED, ALL THE POSSIBLE TRANSFORMATIONS ARE GENERATED AND STORED ONLY ONCE AT THE STARTING OF THE PROGRAM.
 
 SO WHENEVER STARTING WORD AND ENDING WORD IS INPUTTED FROM THE USER, WE PERFORM BFS ON THE GRAPH WHICH CONTAINS ALL THE POSSIBLE TRANSFORMATIONS TO CHECK IF
 IT POSSIBLE TO TRANSFORM STARTING WORD TO END WORD
 */

#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <fstream>

using namespace std;


//FUNCTION TO CREATE A NEW NODE FOR A WORD IN THE GRAPH IF THERE ISN'T A NODE FOR THAT WORD
bool addNewWordNode(unordered_map<string, vector<string>> & graph, string word)
{
    if (graph.find(word) != graph.end())
    {
        return false;
    }
    
    vector<string> value;
    pair<string, vector<string>> keyvalue(word,value);
    graph.insert(keyvalue);
    return true;
}

//FUNCTION TO CREATE A NEW EDGE BETWEEN THE GIVEN WORDS IN THE GRAPH, IF THERE AREN'T NODES FOR ANY OF THE WORDS GIVEN IT ALSO CREATES A NODE FOR THAT(THOSE) WORD(S)
void addWordEdge(unordered_map<string, vector<string>> & graph, string u, string v)
{
    if (graph.find(u) == graph.end())
    {
        addNewWordNode(graph, u);
    }
    if (graph.find(v) == graph.end())
    {
        addNewWordNode(graph, v);
    }
    graph[u].push_back(v);
}


//FUNCTION WHICH TAKES THE WORD LIST AND THE INITIALLY EMPTY GRAPH AND CREATES A GRAPH WHICH CONTAINS THE THE POSSIBLE WORD TRANSFORMATION
void createWordGraph(unordered_map<string, string>  & wordList,unordered_map<string, vector<string>> & wordGraph)
{
    string tempWord;
//  FOR EACH WORD IN THE WORD LIST
    for (auto& x: wordList) {
        tempWord=  wordList[x.first];
        string originalStartingWord = tempWord;
//      FIRST CHECKING IF CHANGING EACH SINGLE LETTER WILL RESULT IN A NEW WORD
        for (int pos =0; pos <tempWord.length(); ++pos)
        {
            char originalChar = tempWord[pos];
//          AT CURRENT POSITION TRYING ALL POSSIBLE LETTERS
            for (char c = 'a'; c <= 'z'; ++c)
            {
                tempWord[pos] = c;
//              CHECKING IF THE WORD IS NOT EQAUL TO STARTING WORD AND IF THE CREATED WORD IS IN THE WORD LIST
//              IF SO ADDING AN EDGE FROM THE STARTING WORD TO THE CREATED WORD
                if ( tempWord!= originalStartingWord && wordList.find(tempWord) != wordList.end())
                {
                    addWordEdge(wordGraph, originalStartingWord , tempWord);
                }
            }
            tempWord[pos] = originalChar;
        }
//      SECONDLY CHECKING IF INSERTING LETTERS WILL RESULT IN A NEW WORD
        for (int i = 0; i<=tempWord.length(); i++)
        {
            for (char c = 'a'; c <= 'z'; ++c)
            {
//              IF I=0 WHICH MEANS THAT WE SHOULD INSERT THE LETTER AT THE BEGGINNING OF THE WORD
                if (i==0)
                {
                    tempWord = c  + tempWord;
//                  CHECKING IF THE TEMP WORD IS NOT EQAUL TO STARTING WORD AND IF THE CREATED WORD IS IN THE WORD LIST
//                  IF SO ADDING AN EDGE FROM THE STARTING WORD TO THE CREATED WORD
                    if ( tempWord!= originalStartingWord && wordList.find(tempWord) != wordList.end())
                    {
                        addWordEdge(wordGraph, originalStartingWord , tempWord);
                    }
                    tempWord = originalStartingWord;
                }
//              IF I=TEMPWORD.LENGHT WHICH MEANS THAT WE SHOULD INSERT THE LETTER AT THE END OF THE WORD
                else if (i==tempWord.length())
                {
                    tempWord += c ;
//                  CHECKING IF THE TEMP WORD IS NOT EQAUL TO STARTING WORD AND IF THE CREATED WORD IS IN THE WORD LIST
//                  IF SO ADDING AN EDGE FROM THE STARTING WORD TO THE CREATED WORD
                    if ( tempWord!= originalStartingWord && wordList.find(tempWord) != wordList.end())
                    {
                        addWordEdge(wordGraph, originalStartingWord , tempWord);
                    }
                    tempWord = originalStartingWord;
                }
//              WHICH MEANS THAT WE SHOULD INSERT THE LETTER NEXT TO POSITION I
                else
                {
                    tempWord = tempWord.substr(0,i) + c + tempWord.substr(i);
//                  CHECKING IF THE TEMP WORD IS NOT EQAUL TO STARTING WORD AND IF THE CREATED WORD IS IN THE WORD LIST
//                  IF SO ADDING AN EDGE FROM THE STARTING WORD TO THE CREATED WORD
                    if ( tempWord!= originalStartingWord && wordList.find(tempWord) != wordList.end())
                    {
                        addWordEdge(wordGraph, originalStartingWord , tempWord);
                    }
                    tempWord = originalStartingWord;
                }
            }
        }
//      THIRDLY CHECKING IF DELETING LETTERS WILL RESULT IN A NEW WORD
//      IF THE WORD IS A SINGLE LETTER WE DON'T DO DELETIONS
        if (originalStartingWord.length()!=1)
        {
            tempWord=originalStartingWord;
//          FOR EACH LETTER WE DELETE IT AND CHECKING IF THE TEMP WORD IS NOT EQAUL TO STARTING WORD AND IF THE CREATED WORD IS IN THE WORD LIST
//          IF SO ADDING AN EDGE FROM THE STARTING WORD TO THE CREATED WORD
            for (int i=0; i<tempWord.length(); i++)
            {
                tempWord =  tempWord.substr(0,i) + tempWord.substr(i+1);
                if ( tempWord!= originalStartingWord && wordList.find(tempWord) != wordList.end())
                {
                    addWordEdge(wordGraph, originalStartingWord , tempWord);
                }
                tempWord=originalStartingWord;
            }
        }
    }
}

//FUNCTION THAT SEARCHES FOR THE SHORTEST PATH TO GO FROM STARTING WORD TO THE ENDING WORD WITHIN THE GIVEN GRAPH
//ALSO TAKES AND PATH VECTOR AS REFERENCE AND IF A PATH IS FOUND STORES THE PATH IN THAT VECTOR
//THE FUNCTION PERFORMS A BFS ON THE GIVEN GRAPH TO FIND THE SHORTEST PATH FROM START WORD TO END WORD
bool shortestWordPath(unordered_map<string, vector<string>> & wordGraph,string startWord, string endWord,vector<string> & path)
{
    deque<string> processQueue; //PROCESS QUEUE IN ORDER TO STORE THE NODES TO BE PROCESSED
    unordered_map<string, bool> visited; //MAPPING TO KEEP TRACK OF THE VISITED NODES
    unordered_map<string, string> preNode; //MAPPING TO STORE THE PREVIOUS NODE OF A NODE IF IT WAS POSSIBLE TO REACH FROM ONE TO ANOTHER ONE
    
//  FOR EACH NODE IN THE GRAPH MAKING THEIR VISITED VALUE EQUAL TO FALSE
    for (auto& x: wordGraph) {
        visited[x.first]=false;
    }
    processQueue.push_front(startWord); //TO START FROM THE START WORD ADDING IT TO THE PROCESS QUEUE
    bool found = false;
//  GOING OVER THE NODES AND EDGES UNTIL EITHER THE PROCESS QUEUE IS EMPTY OR WE HAVE FOUND A PATH TO THE END WORD
    while (processQueue.size() > 0 && found == false)
    {
//      TAKING THE NODE FROM THE PROCESS QUEUE AND MARKING IT AS VISITED
        string current= processQueue.back();
        processQueue.pop_back();
        visited[current]= true;
//      FOR EACH ADJACENT NODE OF THE CURRENT NODE
        for (int i= 0; i < wordGraph[current].size() ; i++)
        {
//          CHECKING IF THE ADJACENT NODE IS VISITED OR NOT, IF IT IS VISITED SKIPPING THAT NODE
            if (visited[wordGraph[current][i]]== false)
            {
//              CHECKING IF THE ADJACENT NODE IS EQUAL TO ENDING WORD, IF SO MAKING THE BOOL FOUND EQUAL TO TRUE AND SETTING THE PREVIOUS NODE OF THE ADJACENT NODE(IN THIS CASE THE NODE WHICH HAS THE ENDING WORD) TO THE CURRENT NODE
                if (wordGraph[current][i] == endWord)
                {
                    found = true;
                    preNode[wordGraph[current][i]] = current;
                }
//              IF THE ADJACENT NODE IS NOT EQAUL TO THE ENDING WORD
                else
                {
//                  CHECKING IF THE PREVIOUS NODE EXIST FOR THE ADJACENT NODE, IF IT DOES HAVE ONE THAT MEANS THAT IT WAS ALREADY ADDED TO THE PROCESS QUEUE SO WE DON'T ADD IT TO THE PROCESS QUEUE ONE MORE TIME
                    if (preNode.find(wordGraph[current][i]) == preNode.end())
                    {
//                      SINCE WE WANT TO FIND THE SHORTEST PATH WE CHECK IF THE PREVIOUS NODE OF THE ADJACENT NODE IS ALSO ADJACENT TO THE CURRENT NODE, IF SO THAT MEANS THAT WE SHOULD CHANGE THE PREVIOUS NODE OF THE ADJACENT NODE AS THE CURRENT NODE SINCE IT WILL MAKE THE PATH LONGER
                        bool isAdjacent =false;
                        for (int j= 0; j < wordGraph[current].size() && isAdjacent==false  ; j++)
                        {
                            if (wordGraph[current][j] == preNode[wordGraph[current][i]] )
                            {
                                isAdjacent = true;
                            }
                        }
//                      IF THE PREVIOUS NODE OF THE ADJACENT NODE AND THE CURRENT NODE IS NOT ADJACENT, WE SET IT'S PREVIOUS NODE AS THE CURRENT NODE AND ADD IT TO THE PROCESS QUEUE
                        if (!isAdjacent)
                        {
                            preNode[wordGraph[current][i]] = current;
                            processQueue.push_front(wordGraph[current][i]);
                        }
                    }
                }
            }
        }
    }
    
//  AT THE END OF THE LOOP IF THERE WAS A PATH FOUND, WE STORE THE PATH IN THE PATH VECTOR
    if (found==true)
    {
        string toFind;
        toFind = preNode[endWord];
//      WHICH MEANS THAT WITH ONE LETTER CHANGE WE CAN DIRECTLY CONVERt FROM STATING WORD TO END WORD
        if (toFind==startWord)
        {
            path.push_back(endWord);
            path.push_back(toFind);
        }
//      WHICH MEANS THAT THERE ARE MORE THAN ONE INTEMEDIATE WORD
        else
        {
            path.push_back(endWord);
//          UNTIL WE REACH TO THE STARTING WORD, WE GET THE PREVIOUS NODES AND STORE THEM INSIDE THE PATH
            while (toFind != startWord)
            {
                path.push_back(toFind);
                toFind = preNode[toFind];
            }
            path.push_back(toFind);
        }
    }
//  RETURNING THE FOUND BOOL VALUE
    return found;
}

//FINDING THE DIFFERENT LETTER BETWEEN TWO WORDS IF THE TRANSFORMATION WAS WITH A CHANGE OF A LETTER
void findAndPrintWordDiffForChange(string firstWord,string secondWord)
{
    bool diffFound = false;
//  CHECKING FOR EACH LETTER OF THE WORDS AND PRINTING THE MESSAGE OF THE CHANGED LETTER AND ITS POSITON
    for (int i=0; i<firstWord.length() && diffFound==false; i++)
    {
        if (firstWord.at(i)!= secondWord.at(i))
        {
            cout<<secondWord<<" (change "<<firstWord.at(i)<<" at position "<<i+1<<" to "<<secondWord.at(i)<<")"<<endl;
            diffFound=true;
        }
    }
}

//FINDING THE DIFFERENT LETTER BETWEEN TWO WORDS IF THE TRANSFORMATION WAS WITH A INSERTION OF A LETTER
void findAndPrintWordDiffForInsertion(string firstWord,string secondWord)
{
    bool diffFound = false;
//  GOING OVER THE LENGTH OF THE FIRST WORDS SINCE THE SECOND WORD IS LONGER BECAUSE IT HAD A LETTER INSERTION
//  IF THE DIFFERENCE IS FOUND PRINTING THE MESSAGE OF THE INSERTED LETTER AND ITS POSITON
    for (int i=0; i<firstWord.length() && diffFound==false; i++)
    {
        if (firstWord.at(i)!= secondWord.at(i))
        {
            cout<<secondWord<<" (insert "<<secondWord.at(i)<<" after position "<<i<<")"<<endl;
            diffFound=true;
        }
    }
    
    //WHICH MEANS THE LOOP EXITED WITH DIFF FOUND EQUAL TO FALSE AND THE LETTER WAS INSERTED AT THE END OF THE WORD
    if (diffFound!=true)
    {
        cout<<secondWord<<" (insert "<<secondWord.at(secondWord.length()-1)<<" after position "<<secondWord.length()-1<<")"<<endl;
        diffFound=true;
    }
}

//FINDING THE DIFFERENT LETTER BETWEEN TWO WORDS IF THE TRANSFORMATION WAS WITH A DELETION OF A LETTER
void findAndPrintWordDiffForDeletion(string firstWord,string secondWord)
{
    bool diffFound = false;
//  GOING OVER THE LENGHT OF THE SECOND WORDS SINCE THE SECOND WORD IS SMALLER BECAUSE IT HAD A LETTER DELETION
//  //  IF THE DIFFERENCE IS FOUND PRINTING THE MESSAGE OF THE INSERTED LETTER AND ITS POSITON
    for (int i=0; i<secondWord.length() && diffFound==false; i++)
    {
        if (firstWord.at(i)!= secondWord.at(i))
        {
            cout<<secondWord<<" (delete "<<firstWord.at(i)<<" at position "<<i+1<<")"<<endl;
            diffFound=true;
        }
    }
    
    //WHICH MEANS THE LOOP EXITED WITH DIFF FOUND EQUAL TO FALSE AND THE LETTER WAS DELETED AT THE END OF THE WORD
    if (diffFound!=true)
    {
        cout<<secondWord<<" (delete "<<firstWord.at(firstWord.length()-1)<<" at position "<<firstWord.length()<<")"<<endl;
        diffFound=true;
    }
}

int main()
{
//  IN ORDER TO REPRESENT THE GRAPH WE ARE GOING TO USE ADJANCENCY LIST REPRESENATION FOR THE GRAPH
    unordered_map<string, vector<string>> wordGraph;
    
//  OPENING THE FILE WHICH HAS THE WORDS AND READING AND STORING ALL THE WORDS FROM THE FILE
    ifstream inFile;
    string inputFileName = "words.txt";
    inFile.open(inputFileName.c_str());
    string currentWord;
    unordered_map<string, string> wordList; //CONTAINER TO STORE THE WORDS IN THE FILE
    while (inFile>>currentWord)
    {
        wordList[currentWord]=currentWord;
    }
    inFile.close();
    
//  CREATING THE WORD GRAPH WHICH HAS ALL THE POSSIBLE SINGLE LETTER TRANSFORMATIONS MODELLED, DETAILS OF THE FUNCTION IS EXPLAINED ABOVE
    createWordGraph(wordList, wordGraph);

//  PRINTING OUT THE INFORMATION MESSAGE AND INPUTTING THE STARTING WORD AND THE ENDING WORD FROM THE USER
    string startingWord, endingWord;
    cout<<"Please enter the starting word and the ending word: ";
    cin>> startingWord >> endingWord;
//  THE EXIT CONDITION FOR THE PROGRAM IS FIRST LETTER OF THE FIRST WORD BEING EQUAL TO * CHARECTER, SO WE KEEP ON EXECUTING THE PROGRAM UNTIL THE USER END THE PROGRAM
    while (startingWord.at(0)!= '*')
    {
//      IF BOTH THE STARTING WORD AND THE ENDING WORD ISN'T IN THE WORD LIST, WE PRINT OUT AN ERROR MESSAGE AND ASK FOR NEW STARTING AND ENDING WORDS AGAIN
        if (wordList.find(startingWord) == wordList.end() && wordList.find(endingWord) == wordList.end() )
        {
            cout<<"Both the staring word and ending word is not found in the word list"<<endl;
            cout<<endl;
            cout<<"Please enter the starting word and the ending word (enter * to exit): ";
            cin>> startingWord >> endingWord;
        }
//      IF ONLY THE STARTING WORD ISN'T IN THE WORD LIST, WE PRINT OUT AN ERROR MESSAGE AND ASK FOR NEW STARTING AND ENDING WORDS AGAIN
        else if (wordList.find(startingWord) == wordList.end())
        {
            cout<<"Starting word is not found in the word list"<<endl;
            cout<<endl;
            cout<<"Please enter the starting word and the ending word (enter * to exit): ";
            cin>> startingWord >> endingWord;
        }
//      IF ONLY THE ENDING WORD ISN'T IN THE WORD LIST, WE PRINT OUT AN ERROR MESSAGE AND ASK FOR NEW STARTING AND ENDING WORDS AGAIN
        else if (wordList.find(endingWord) == wordList.end())
        {
            cout<<"Ending word is not found in the word list"<<endl;
            cout<<endl;
            cout<<"Please enter the starting word and the ending word (enter * to exit): ";
            cin>> startingWord >> endingWord;
        }
//      IF BOTH WORDS ARE IN THE LIST WE CHECK IF IT IS POSSIBLE TO TRANSFROM STARTING WORD TO ENDING WORD
        else
        {
//          CREATING AND EMPTY PATH VECTOR TO STORE THE PATH IF A PATH IS FOUND
            vector<string> path;
//          CHECKING IF IT IS POSSBILE TO TRANSFROM THE STARTING WORD TO ENDING WORD AND ALSO RETURING THE PATH IF IT IS POSSIBLE, DETAILS OF THE FUNCTION IS EXPLAINED ABOVE
            bool pathFound = shortestWordPath(wordGraph, startingWord, endingWord,path);
            
//          IF IT IS POSSIBLE TO TRANSFROM STARTING WORD TO ENDING WORD
            if (pathFound==true)
            {
//              WE PROCESS ALL THE WORDS IN THE PATH STARTING FROM THE STARTING WORD AND PRINTING THE TRANSFORMATIONS FOR EACH WORD
                string firstWord =path.back();
                string secondWord;
                cout<<firstWord<<endl;
                path.pop_back();
                while (path.size()!= 0)
                {
                    secondWord = path.back();
                    path.pop_back();
                    //WHICH MEANS THAT THE TRANSFORMATION WAS A CHANGE OF A LETTER
                    if (firstWord.length()==secondWord.length())
                    {
//                      PRINT THE DIFFERENCE BETWEEN THE WORDS, DETAILS OF THE FUNCTION IS EXPLAINED ABOVE
                        findAndPrintWordDiffForChange(firstWord,secondWord);
                    }
                    
                    //WHICH MEANS THAT THE TRANSFORMATION WAS A INSERTION OF A LETTER
                    else if (firstWord.length() < secondWord.length())
                    {
//                      PRINT THE DIFFERENCE BETWEEN THE WORDS, DETAILS OF THE FUNCTION IS EXPLAINED ABOVE
                        findAndPrintWordDiffForInsertion(firstWord, secondWord);
                    }
                    
                    //WHICH MEANS THAT THE TRANSFORMATION WAS A DELETION OF A LETTER
                    else if (secondWord.length() < firstWord.length())
                    {
//                      PRINT THE DIFFERENCE BETWEEN THE WORDS, DETAILS OF THE FUNCTION IS EXPLAINED ABOVE
                        findAndPrintWordDiffForDeletion(firstWord, secondWord);
                    }
                    
//                  MAKING THE FIRST WORD EQUAL TO SECOND WORD IN ORDER TO CONTIUNE TO PROCESS ALL WORDS IN THE PATH
                    firstWord = secondWord;
                }
                
//              AFTER PROCESSING ALL THE WORDS IN THE PATH ASK FOR NEW STARTING AND ENDING WORDS AGAIN
                cout<<endl;
                cout<<"Please enter the starting word and the ending word (enter * to exit): ";
                cin>> startingWord >> endingWord;
            }
            
//          IF IT IS NOT POSSIBLE TO TRANSFROM STARTING WORD TO ENDING WORD
            else
            {
//              PRINTING AND ERROR MESSASGE AND ASK FOR NEW STARTING AND ENDING WORDS AGAIN
                cout<<"There are no sequence of one letter transformations exists to convert "<<startingWord<<" to "<< endingWord<<endl;
                cout<<endl;
                cout<<"Please enter the starting word and the ending word (enter * to exit): ";
                cin>> startingWord >> endingWord;
            }
        }
    }
    
//  AFTER THE USERS ENDS THE PROGRAM PRINTING A MESSAGE TO THE SCREEN
    cout<<endl;
    cout<<"Thanks for playing the word game, goodbye."<<endl;

    return 0;
}
