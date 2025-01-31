#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <csignal>

using namespace std;

int interrupt = 0;
int quit = 0;
int stop = 0;

void parse_args(string line, vector<string> &cmds){
    stringstream liness(line);
 
    string token;
    while (getline(liness, token, ' ')) {
        cmds.push_back(token);
    }
}

void signal_handler(int sig){
    if(sig == 2)
        interrupt += 1;
    else if(sig == 3)
        quit += 1;
    else if(sig == 20)
        stop += 1;
}

int main(){

    vector<int> history;
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTSTP, signal_handler);

    while(true){

        cout << "mj385s$";

        string cmd;
        getline(cin, cmd);

        if(cmd == "" || cmd.size() == 0)
            continue;

        cout << "Command entered: " << cmd << endl;
        
        //built-in commands
        if(cmd == "help"){
            cout << "//****************************************************************//" << endl;
            cout << "//                                                                //" << endl;
            cout << "//             <Designed by: Michael Jakob>                       //" << endl;
            cout << "//             <CSC360 - Operating Systems>                       //" << endl;
            cout << "//      Project #1: My Shell - Writing Your Own Shell             //" << endl;
            cout << "//This Shell supports the following commands: help, exit, history //" << endl;
            cout << "//                                                                //" << endl;
            cout << "//****************************************************************//" << endl;
            
            continue;
        }
        else if(cmd == "exit"){
            cout << "Shell terminated. Goodbye!" << endl;
            cout << "Number of interrupt signals: " << interrupt << endl;
            cout << "Number of stop signals: " << stop << endl;
            cout << "Number of terminate signals: " << quit << endl;                        
            exit(0);
        }
        else if(cmd == "history"){
            for(int j = 0; j<history.size(); j++)
                cout << history[j] << endl;
            continue;
        }

        //parse command
        vector<string> cmd_args;
        parse_args(cmd, cmd_args);

        //Converting vector into data type accepted by execvp
        vector<const char*> execArgs;
        for (int x = 0; x<cmd_args.size(); x++){
            execArgs.push_back(cmd_args[x].c_str());
        }
        execArgs.push_back(nullptr);
        //fork and execute program
        int pid = fork();
        int status;
        if(pid == 0){
            execvp(execArgs[0], const_cast<char**>(execArgs.data()));
            cout << "Error: Command not recognized" << endl;
            exit(1);
        }
        else{
            waitpid(pid, &status, 0);
            if(history.size() < 5)
                history.push_back(pid);
            else{
                history.push_back(pid);
                history.erase(history.begin());
            }
        }
    }
}