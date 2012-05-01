#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <string>
using namespace std;

class MsgException: public exception {
private:
	string msg;
public:
	MsgException(string msg) {
		this->msg = msg;
	}
	virtual ~MsgException() throw (){}

	virtual const char* what() const throw () {
		return this->msg.c_str();
	}
};
#endif
