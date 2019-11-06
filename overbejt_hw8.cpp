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


// Forward declaration for method defined further below
void createAcct(std::string acctNum);
void credit(std::string acctNum, double ammount);
void debit(std::string acctNum, double ammount);
void reset();
std::string status(std::string acctNum);
void response(std::ostream& os, std::string account, bool err);
std::string url_decode(std::string);

/**
 * This method will create a new account.
 * 
 * @param acctNum The account number for the new account.
 */
void createAcct(std::string acctNum) {
    if (bank.find() != bank.end()) {
        bank.insert({acctNum : 0.0});
    }
}  // End of the 'createAcct' method

/**
 * This is a method that will perform a credit transaction.
 * 
 * @param acctNum The account number.
 * @param ammount The amount to be credited.
 */
void credit(std::string acctNum, double ammount) {
    
}  // End of the 'credit' method

/**
 * This is the method that will debit an account.
 * 
 * @param acctNum The account number to be debited.
 * @param ammount The amount to debit the account.
 */
void debit(std::string acctNum, double ammount) {
    
}  // End of the 'debit' method


/**
 * This is the method that will reset the bank.  
 */
void reset() {
    bank.clear();
}  // End of the 'reset' method


/**
 * This is the method that will get the status of the indicated bank account.
 * 
 * @param acctNum The indicated account number.
 * @return The balance of the indicated account.
 */
std::string status(std::string acctNum) {
    
}  // End of the 'status' method


/**
 * This is a method that will printing the header.
 * 
 * @param os Ostream for output.
 * @param contentLength Length of the content.
 */
void response(std::ostream& os, std::string account, bool err) {
    if (err) {
        os << "HTTP/1.1 404 Not Found\r\n";
    } else {
        os << "HTTP/1.1 200 OK\r\n";
    }
    os << "Server: BankServer\r\n";
    os << "Content-Length: " << account.size() << "\r\n";
    os << "Connection: Close\r\n";
    os << "Content-Type: text/plain\r\n";
    os << "Account " << account << " created";
}  // End of the 'header' method

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
}

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

