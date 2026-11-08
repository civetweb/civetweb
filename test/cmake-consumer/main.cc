#include <CivetServer.h>

int
main()
{
	std::string encoded;
	CivetServer::urlEncode("hello world", encoded);
	return encoded != "hello%20world";
}
