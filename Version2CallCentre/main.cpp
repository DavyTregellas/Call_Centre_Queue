//Queue Program Davy Tregellas 2023

//headers and libaries required
#include<conio.h>       //required for getch()
#include<iostream>      //required for cin and cout
#include<windows.h>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

#define MAX 20 //size of que is deifined in 

//C++ time data type I can use to store a certain point in time
chrono::system_clock::time_point lastPoppedTime; //varible for wiattime calculation

//a struct allows me to store diffrent varibles under 
struct incomingCall {
    int customerPhoneNumber;
    int queueIDNumber;
    string timestamp;
};

incomingCall call[MAX];                 //array created with each element a struct (incomingCall)

//set at -1 to indicate the queue is empty
int starterPointer = -1; // stack pointer
int endPointer = -1; // end pointer

int startingQueueID = 0; // Set the starting value for queueIDNumber

int popCounter = 0;
int pushCounter = 0;
int disconnectCounter = 0;

int callsWaiting = 0;
int totalWaitTime = 0;

void initQueue(void);
void push(int);
void pop(void);
void disconnect(void);
void viewQueue();
void gotoxy(int, int);
void clrscr(void);

int main(void) {
    char key;

    initQueue();
    viewQueue();

    do {
        cout << "\nTotal number of calls: " << pushCounter << endl;
        cout << "\nTotal number of answered calls: " << popCounter << endl;
        cout << "\nTotal number of disconncted calls: " << disconnectCounter << endl;
        cout << "\n\tQueue Demo";
        cout << "\n\tWhat would you like to do...";
        cout << "\n\tPress '1' to PUSH data onto the queue";
        cout << "\n\tPress '2' to POP data from the queue";
        cout << "\n\tPress '3' to disconnect data from the queue";
        cout << "\n\tTo Exit press '4'\n\t";

        do {
            cin >> key;
        } while (key != '1' && key != '2' && key != '3' && key != '4');

        clrscr();

        cout << "\n\tChoice = " << key << "\n";

        if (key == '1') {
            push(rand() % 1000);
        }
        if (key == '2') {
            pop();
        }
        if (key == '3') {
            disconnect();
        }
        viewQueue();
    } while (key != '4');

}

//sets default values for each element in our array
void initQueue(void) {
    for (int i = 0; i < MAX; i++) {
        call[i].customerPhoneNumber = -999;
        call[i].timestamp = "";
        call[i].queueIDNumber = -999;
    }
}

void pop(void) {
    //check if queue is empty
    if (endPointer == -1 && starterPointer == -1) cout << "\tQueue is empty!\n"; 
    else {
        //increment the pop counter
        popCounter++;
        auto currentTime = chrono::system_clock::now(); //retrieves the current time
        auto timestamp = chrono::system_clock::to_time_t(currentTime); //allows use of localtime_s
        //checks if there is a single element in the queue
        if (endPointer == starterPointer && endPointer != -1) {
            // Queue is empty after popping the last element
            call[starterPointer].customerPhoneNumber = -999;
            call[starterPointer].timestamp = "";
            call[starterPointer].queueIDNumber = -999;
            endPointer = -1;
            starterPointer = -1;
        }
        else {
            // Queue is not empty
            call[starterPointer].customerPhoneNumber = -999;
            call[starterPointer].timestamp = "";
            call[starterPointer].queueIDNumber = -999;
            starterPointer++; //stack pointer is incremented to next call in queue

            //this code makes my queue circular
            if (starterPointer == MAX) {
                starterPointer = 0;         //by moving the stack pointer to the start when reaching the end (MAX)
            }
            //check if lastPoppedTime is not equal to the current time
            if (lastPoppedTime != chrono::system_clock::time_point()) {
                //if that is the case then it means there has already been a pop 
                // meaning a wait time can be calculated
                auto waitTime = chrono::duration_cast<chrono::seconds>
                    (currentTime - lastPoppedTime).count();
                //duration cast is part of the C++ general library, 
                // you can difine the time metrics you want to cout in 'seconds' in this case
                //then add the duration you want to count and use the 
                // .count() function to get the number of seconds in the duration     
                cout << "\tWait time: " << waitTime << " seconds\n";// Calculate and display wait time
            }
            lastPoppedTime = currentTime;
            //reset lastPoppedTime
        }
        cout << "\tpopped " << call[starterPointer].customerPhoneNumber << endl;
    }
}


void disconnect(void) {
    //first I check if there is onlt one item in the queue
    if (endPointer == starterPointer && endPointer != -1) {
        cout << "\tThere is only one caller in the queue." << endl;

        // Ask the user if they want to disconnect the last caller
        char choice;
        cout << "\tDo you want to disconnect the last caller? (y/n): ";
        cin >> choice;

        //clear call data 
        if (choice == 'y' || choice == 'Y') {
            disconnectCounter++;
            call[starterPointer].customerPhoneNumber = -999;
            call[starterPointer].timestamp = "";
            starterPointer = -1;
            endPointer = -1;
        }
        else {
            // The user chose not to disconnect the last caller
            cout << "\tNo action taken.\n";
            return; //return to menu
        }
    }
    if (endPointer == -1 && starterPointer == -1) cout << "\tQueue is empty!\n"; //check if queue is empty
    else {
        disconnectCounter++;
        //get user input of call they want to disconnect 
        int disconnectPosition;
        cout << "\tEnter the position of the customer's call to disconnect (1-" << MAX << "): ";
        cin >> disconnectPosition;
        //ensure the user is entering a call in queue
        if (disconnectPosition < 1 || disconnectPosition > MAX) {
            cout << "\tInvalid position. Please enter a position between 1 and " << MAX << ".\n";
            return;
        }

        //handel the calculation considering the queue is circular
        int userDisconnectPointer = starterPointer + disconnectPosition - 1; //userDisconnectPointer = call information
        if (userDisconnectPointer >= MAX) {
            userDisconnectPointer -= MAX; //subtracting MAX from disconnectPointer to ensure is finds an index in range
        }
        //int disconnectPointer = (stackPointer + disconnectPosition - 1) 
        // % MAX; short hand for code above, code above easier to explain
        if (call[userDisconnectPointer].customerPhoneNumber == -999) { //the position is already empty
            cout << "\tNo call at position " << disconnectPosition << ".\n";
        }
        else {
            //disconnect the call at the specified position
            cout << "\tDisconnecting call at position " << disconnectPosition 
                << " with phone number " << call[userDisconnectPointer].customerPhoneNumber << endl;
            //new pointers to move each calls up the queue 
            int disconnectPointer = userDisconnectPointer;
            int shiftPointer = (userDisconnectPointer + 1) % MAX;

            while (shiftPointer != (endPointer + 1) % MAX) {
                //this while loop shifts calls up one from the call behind the disconnected call. 
                call[disconnectPointer] = call[shiftPointer];
                disconnectPointer = shiftPointer;
                shiftPointer = (shiftPointer + 1) % MAX; //increase shift pointer
            }
            //clear the last call in the queue as it has been moved
            call[disconnectPointer].customerPhoneNumber = -999;
            call[disconnectPointer].timestamp = "";
            //using the disconnect pointer to reasign endpointer to correct index
            endPointer = (disconnectPointer - 1);
            //update pointers if the disconnected call is at the top of the stack
            if (userDisconnectPointer == starterPointer) {
                starterPointer = userDisconnectPointer; //set stackPointer to userDisconnectPointer
            }
        }
    }
}

void push(int x) {
    //check queue is full
    if (endPointer == MAX - 1 && starterPointer == 0 || endPointer == starterPointer - 1) {
        cout << "\tQueue is full!\n";
    }
    else {
        pushCounter++;
        auto currentTime = chrono::system_clock::now();//retrieves the current time
        auto timestamp = chrono::system_clock::to_time_t(currentTime);//allows use of localtime_s
        startingQueueID++;

        //if queue empty add new call
        if (starterPointer == -1 && endPointer == -1) {
            starterPointer++;
            endPointer++;
            call[starterPointer].customerPhoneNumber = x;
            call[starterPointer].queueIDNumber = startingQueueID;

            struct tm timeinfo; //for iformation regarding year, month, day, hour, minute, second
            localtime_s(&timeinfo, &timestamp); //converts timestamp into local timezone

            //create a stringstream to hold the formatted time
            stringstream ss;

            //use put_time to format the timeinfo structure into a string
            ss << put_time(&timeinfo, "%T %F");

            //retrieve the formatted time as a string and assign it to the timestamp
            call[starterPointer].timestamp = ss.str();

        }
        else {
            //if the queue is not empty or full add the same as above
            endPointer++;
            if (endPointer == MAX) endPointer = 0; //resetting the end pointer to the start of the queue 
            //creating a circular queue
            call[endPointer].customerPhoneNumber = x;
            call[endPointer].queueIDNumber = startingQueueID;
            struct tm timeinfo;
            localtime_s(&timeinfo, &timestamp);

            stringstream ss;
            ss << put_time(&timeinfo, "%T %F");
            call[endPointer].timestamp = ss.str();
        }
        cout << "\tpushed " << x << " at " << call[endPointer].timestamp << "\n";
    }
}

//shows queues content and posistions of the pointers
void viewQueue() {
    cout << "\n";
    for (int i = MAX - 1; i > -1; i--) {
        cout << " Queue ID Number " << call[i].queueIDNumber << " Phone Number = " << 
            call[i].customerPhoneNumber << " Time: " << call[i].timestamp << "\n";
    }
    cout << "Starter Pointer=" << starterPointer << endl;
    cout << "End Pointer=" << endPointer << endl;
}

void gotoxy(int x, int y) {
    static HANDLE hStdout = NULL;
    COORD coord;

    coord.X = x;
    coord.Y = y;
    if (!hStdout) { hStdout = GetStdHandle(STD_OUTPUT_HANDLE); }
    SetConsoleCursorPosition(hStdout, coord);
}

void clrscr(void) {
    static HANDLE hStdout = NULL;
    static CONSOLE_SCREEN_BUFFER_INFO csbi;
    const COORD startCoords = { 0, 0 };
    DWORD dummy;

    if (!hStdout) {
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hStdout, &csbi);
    }
    FillConsoleOutputCharacter(hStdout, ' ', csbi.dwSize.X * csbi.dwSize.Y, startCoords, &dummy);
    gotoxy(0, 0);
}