/* 
 * Copyright (c) 2019 overbejt@miamioh.edu 
 * 
 * File: overbejt_hw8.cpp
 * Author: Josh Overbeck
 * Description: A simple Banking-type web-server.  
 * Created on November 5, 2019, 2:55 PM 
 * 
 * This multi threaded web-server performs simple bank transactions on
 * accounts.  Accounts are maintained in an unordered_map.  
 * 
 */

// All the necessary includes are present
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <iomanip>

// Setup a server socket to accept connections on the socket
using namespace boost::asio;
using namespace boost::asio::ip;
// Create a bank to make reading easier
std::unordered_map<std::string, double> bank;
// Using smart pointer
using TcpStreamPtr = std::shared_ptr<tcp::iostream>;


// Forward declaration for method defined further below
void createAcct(std::ostream& os, std::string acctNum);
void credit(std::ostream& os, std::string acctNum, double ammount);
void debit(std::ostream& os, std::string acctNum, double ammount);
void reset(std::ostream& os);
void serveClient(std::istream& is, std::ostream& os);
void splitInput(std::string& line);
void status(std::ostream& os, std::string acctNum);
void response(std::ostream& os, std::string& content);
std::string url_decode(std::string);

/**
 * This method will create a new account.
 * 
 * @param acctNum The account number for the new account.
 */
void createAcct(std::ostream& os, std::string acctNum) {
    std::string output;
    if (bank.find(acctNum) == bank.end()) {
        bank.insert({acctNum, 0.0});
        output = "Account " + acctNum + " created";
    } else {
        output = "Account " + acctNum + " already exists";
    }
    os << output;
}  // End of the 'createAcct' method

/**
 * This is a method that will perform a credit transaction.
 * 
 * @param acctNum The account number.
 * @param ammount The amount to be added to the account.
 */
void credit(std::ostream& os, std::string acctNum, double ammount) {
    auto acct = bank.find(acctNum);
    std::string output;
    if (acct != bank.end()) {
        acct->second += ammount;
        output =  "Account balance updated";
    } else {
        output = "Account not found";
    }
    os << output;
}  // End of the 'credit' method

/**
 * This is the method that will debit an account.
 * 
 * @param acctNum The account number to be debited.
 * @param ammount The amount to subtract from the account.
 */
void debit(std::ostream& os, std::string acctNum, double ammount) {
    auto acct = bank.find(acctNum);
    std::string output;
    if (acct != bank.end()) {
        acct->second -= ammount;
        output = "Account balance updated";
    } else {
        output = "Account not found";
    }
    os << output;
}  // End of the 'debit' method

/**
 * A method that will parse the input and execute the transaction.  
 * 
 * @param input The input supplied from the GET request.
 */
void execute(std::string input) {
    std::stringstream ss(input);
    std::string junk1, junk2, junk3, trans, acct, amt;
    
    // Get the number of commands
    int cmdCnt = std::count_if(input.begin(), input.end(), 
            [](char a){return a == ' ';});
           
    if (cmdCnt == 1) {
        ss >> junk1 >> trans;
    }
    if (cmdCnt == 2) {
        ss >> junk1 >> trans >> junk2 >> acct;
    }
    if (cmdCnt == 3) {
        ss >> junk1 >> trans >> junk2 >> acct >> junk3 >> amt;
    }
}  // End of the 'execute' method


/**
 * This is the method that will reset the bank.  
 */
void reset(std::ostream& os) {
    bank.clear();
    os << "All accounts reset";
}  // End of the 'reset' method


/**
 * This is the method that will get the status of the indicated bank account.
 * 
 * @param acctNum The indicated account number.
 * @return The balance of the indicated account.
 */
void status(std::ostream& os, std::string acctNum) {
    std::string balance;
    auto acct = bank.find(acctNum);
    if (acct != bank.end()) {
        os << "Account " << acctNum << ": $";
        os << std::fixed << std::setprecision(2) 
                << std::to_string(acct->second);
    }
}  // End of the 'status' method

/**
 * A method that will split up the input.  
 * 
 * @param line The input from the GET method.
 */
void splitInput(std::string& line) {
    // I'm awesome, and so are lambda expressions
    std::replace_if(line.begin(), line.end(), 
            [](char a){return a == '&';}, ' ');
    std::cout << "After replace: " << line << std::endl;
    // Get the number of variables
//    int varCnt = std::count_if(line.begin(), line.end(), 
//            [](char a){return a == '=';});
//    std::cout << "VarCnt: " << std::to_string(varCnt) << std::endl;
    // Strip out the =
    std::replace_if(line.begin(), line.end(), 
            [](char a){return a == '=';}, ' ');
    
//    // Split the args up
//    std::stringstream ss(line);
//    std::string junk1, junk2, junk3, trans, acct, amt;
//    if (varCnt == 1) {
//        ss >> junk1 >> trans;
//    }
//    if (varCnt == 2) {
//        ss >> junk1 >> trans >> junk2 >> acct;
//    }
//    if (varCnt == 3) {
//        ss >> junk1 >> trans >> junk2 >> acct >> junk3 >> amt;
//    }
//    
//    std::cout << "trans: " << trans << "\nacct: " << acct << "\namt: " << 
//            amt << std::endl;
}  // End of the 'transactionType' method

/**
 * This is a method that will serve the client.  
 * 
 * @param is
 * @param os
 */
void serveClient(std::istream& is, std::ostream& os) {
    // Todo: Maybe implement this
    std::cout << "The serveClient method was called" << std::endl;
     // 1.) Make sure Input starts with "GET /TransactionInfo"
    for (std::string line; getline(is, line);) {
        if (line.find("GET") != std::string::npos) {                
            std::cout << "Line: " << line << std::endl;
            line = line.erase(0, 5);
            line = line.erase(line.size() - 10);
            std::cout << "subStr: " << line << std::endl;
            // Print the header out
            std::string idk = "It reached the response method";
            // Decode the input
            line = url_decode(line);
            std::cout << "Decoded: " << line << std::endl;
            // Split the input up
            splitInput(line);
            std::cout << "Split Line: " << line << "\n";
            // Execute command
            
                            
            response(os, idk);                           
        }
    }
}  // End of the 'serveClient' method


/**
 * This is a method that will printing the header.
 * 
 * @param os Ostream for output.
 * @param contentLength Length of the content.
 */
void response(std::ostream& os, std::string& content) {
//    if (err) {
//        os << "HTTP/1.1 200 OK\r\n";
//    } else {
//        os << "HTTP/1.1 404 "
//    }
    os << "HTTP/1.1 200 OK\r\n";
    os << "Server: BankServer\r\n";
    os << "Content-Length: " << content.size() << "\r\n";
    os << "Connection: Close\r\n";
    os << "Content-Type: text/plain\r\n";
    os << "Account " << content << " created";
}  // End of the 'header' method

/**
 * This is a method that will act as the main for a thread.  
 * 
 * @param stream The iostream associated to the client connection
 */
void thrdInit(TcpStreamPtr stream) {
    serveClient(*stream, *stream);
}  // End of the 'thrdinit' method

/**
 * Top-level method to run a custom HTTP server to process bank
 * transaction requests using multiple threads. Each request should
 * be processed using a separate detached thread. This method just loops 
 * for-ever.
 *
 * @param server The boost::tcp::acceptor object to be used to accept
 * connections from various clients.
 */
void runServer(tcp::acceptor& server) {
    // Implement this method to meet the requirements of the homework.
    // Needless to say first you should create stubs for the various 
    // operations, write comments, and then implement the methods.
    //
    // First get the base case to be operational. Then you can multithread.

    // Process client connections one-by-one...forever
    while (true) {       
//        TcpStreamPtr client = std::make_shared<tcp::iostream>();
        tcp::iostream client;
        // Wait for a client to connect
        server.accept(*client.rdbuf());
        serveClient(client, client);
//        std::thread thr(thrdInit, client);
//        thr.detach();
    }
}  // End of the 'runServer' method

//-------------------------------------------------------------------
//  DO  NOT   MODIFY  CODE  BELOW  THIS  LINE
//-------------------------------------------------------------------

/** Convenience method to decode HTML/URL encoded strings.

    This method must be used to decode query string parameters
    supplied along with GET request.  This method converts URL encoded
    entities in the from %nn (where 'n' is a hexadecimal digit) to
    corresponding ASCII characters.

    \param[in] str The string to be decoded.  If the string does not
    have any URL encoded characters then this original string is
    returned.  So it is always safe to call this method!

    \return The decoded string.
*/
std::string url_decode(std::string str) {
    // Decode entities in the from "%xx"
    size_t pos = 0;
    while ((pos = str.find_first_of("%+", pos)) != std::string::npos) {
        switch (str.at(pos)) {
            case '+': str.replace(pos, 1, " ");
            break;
            case '%': {
                std::string hex = str.substr(pos + 1, 2);
                char ascii = std::stoi(hex, nullptr, 16);
                str.replace(pos, 3, 1, ascii);
            }
        }
        pos++;
    }
    return str;
}

// Helper method for testing.
void checkRunClient(const std::string& port);

/*
 * The main method that performs the basic task of accepting
 * connections from the user and processing each request using
 * multiple threads.
 */
int main(int argc, char** argv) {  
    // Setup the port number for use by the server
    const int port = (argc > 1 ? std::stoi(argv[1]) : 0);
    io_service service;
    // Create end point.  If port is zero a random port will be set
    tcp::endpoint myEndpoint(tcp::v4(), port);
    tcp::acceptor server(service, myEndpoint);  // create a server socket
    // Print information where the server is operating.    
    std::cout << "Listening for commands on port "
              << server.local_endpoint().port() << std::endl;
    // Check run tester client.
#ifdef TEST_CLIENT
    checkRunClient(argv[1]);
#endif

    // Run the server on the specified acceptor
    runServer(server);
    
    // All done.
    return 0;
}

// End of source code
