#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

using namespace std;

void parse_args(string line, vector<string> &cmds){
    stringstream liness(line);
 
    string token;
    while (getline(liness, token, ' ')) {
        cmds.push_back(token);
    }
}

int main(){

    vector<int> history;
    while(true){
        cout << "mj385s$";

        string cmd;
        getline(cin, cmd);

        if(cmd == "" || cmd.size() == 0)
            continue;

        cout << "Command entered: " << cmd << endl;
        
        //built-in commands
        if(cmd == "help"){
            cout << "Here's where the help text goes" << endl;
            continue;
        }
        else if(cmd == "exit"){
            cout << "Shell terminated. Goodbye!" << endl;
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