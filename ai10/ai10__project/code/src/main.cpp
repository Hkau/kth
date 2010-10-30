/*
 * main.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: soheil
 */

#include <iostream>
#include <sstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>
#include <iomanip>

#include "Board/MABoardSearch.h"
#include "Heuristics/SimpleChessGuru.h"
#include "Heuristics/ReachabilityGuru.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
	try {
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		std::string lHost, lPort, lBoard;
		if (argc == 4) {
			lHost = std::string(argv[1]);
			lPort = std::string(argv[2]);
			lBoard = std::string(argv[3]);
		} else if (argc == 2) {
			lHost = std::string("cvap103.nada.kth.se");
			lPort = std::string("5555");
			lBoard = std::string(argv[1]);
		} else {
			lHost = std::string("cvap103.nada.kth.se");
			lPort = std::string("5555");
			lBoard = std::string("1");
		}
		tcp::resolver::query query(lHost, lPort);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		boost::system::error_code ignored_error;
		if (error)
			;
		else {
			boost::asio::write(socket, boost::asio::buffer(lBoard),
					boost::asio::transfer_all(), ignored_error);
		}
		for (;;) {
			std::string def_board;

			boost::array<char, 1024> buf;
			boost::system::error_code error;
			size_t len(0);
			if (!error) {
				len = socket.read_some(boost::asio::buffer(buf), error);
				if (!strcmp(buf.data(), "Wrong ID\n")) {
					std::cout.write(buf.data(), len);
					break;
				}
			}
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error) {
				std::cerr << "Connection error" << std::endl;
				std::cerr << "Default board will be used" << std::endl;

				def_board
						= "###########\n##     ####\n## ###.   #\n##    # # #\n# .#$ $ # #\n# #  *  # #\n# # $ $#. #\n# # #    ##\n#   .### ##\n####      #\n########  #\n########@ #\n###########\n\n";
			} else {
				std::string lBoardAsString(buf.data(), len);
				def_board = lBoardAsString;
			}
			std::cout << "Solving " << std::endl;
			std::cout << def_board;
			StaticBoard sb(def_board.c_str());
			ReachabilityGuru guru(&sb, BACKWARD);
			list<direction> sol = Multi2SingleAgent(MultiAgentBoardSearch(guru), &sb);

			if (sol.empty())
				break;
			ostringstream oss;

			oss << sol;
			std::string lMySol = oss.str();
			cout << "Solution: " << lMySol << endl;
			if (!error) {
				boost::asio::write(socket, boost::asio::buffer(lMySol),
						boost::asio::transfer_all(), ignored_error);

				len = socket.read_some(boost::asio::buffer(buf), error);
				std::cout.write(buf.data(), len);
			} else
				break;
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
