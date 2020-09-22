#pragma once

#include <iostream>
#include <string>


class Exception {

public:
	Exception(const std::string& msg) {
		msg_ = msg;
	}
	~Exception() {}

	virtual std::string getMessage() const { return(msg_); }
protected:
	std::string msg_;
};

class InputException : public Exception {
public:
	InputException(const std::string& msg) : Exception(msg) { };
	std::string getMessage() const { return(type + msg_); }
private:
	std::string type = "Puzzle format or contents incorrect: ";
};

class UnaccessableCellException : public Exception {
public:
	UnaccessableCellException(const std::string& msg) : Exception(msg) { };
	std::string getMessage() const { return(type + msg_); }
private:
	std::string type = "ERROR!! \nAttempt to fill locked cell at: ";
};

class ImpossiblePuzzleException : public Exception {
public:
	ImpossiblePuzzleException(const std::string& msg) : Exception(msg) { };
	std::string getMessage() const { return(msg_ + type); }
protected:
	std::string type = "\n\nPuzzle is not solvable";

};