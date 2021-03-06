#include<iostream>
#include<boost/asio.hpp>
#include<string>
#include "player.h"

using namespace std;
using namespace boost::asio;
using namespace ip;

void connection();
string id(tcp::socket& sock,player p);
string chooseGround(tcp::socket& sock);
void game(tcp::socket& sock1, tcp::socket& sock2);

int main()
{
	system("Color B");
	connection();
}
void connection()
{
	player p1,p2;
	io_service io;
	tcp::socket sock1(io);
	tcp::socket sock2(io);
	tcp::acceptor acc(io, tcp::endpoint(tcp::v4(), 1234));
	acc.accept(sock1);
    id(sock1,p1);

	acc.accept(sock2);
	id(sock2,p2);
	
	write(sock2, boost::asio::buffer("false\n"));
	write(sock1, boost::asio::buffer("true\n"));
	string pg = chooseGround(sock1);

	pg += "\n";
	write(sock1, boost::asio::buffer(pg));
	write(sock2, boost::asio::buffer(pg));

	game(sock1, sock2);
	p1.setOff();
	p2.setOff();
}
string id(tcp::socket& sock,player p) {
	boost::asio::streambuf buffer;
	string ID;

	read_until(sock, buffer, "\n");
	ID = buffer_cast<const char*>(buffer.data());
	ID = ID.substr(0, ID.size() - 1);
	p.setId(ID);
	p.setOn();
	cout << ID << " is Online \n"<<endl;
	return ID;
}
string chooseGround(tcp::socket& sock)
{
	boost::asio::streambuf buff;
	string pg;

	read_until(sock, buff, "\n");
	pg = buffer_cast<const char*>(buff.data());
	pg = pg.substr(0, pg.size() - 1);

	cout << "playground " << pg << " is chosen\n";
	return pg;
}
void game(tcp::socket& sock1, tcp::socket& sock2)
{
	string msg, gameover = "false\n";
	int turn = 1;

	while (gameover == "false\n")
	{
		boost::asio::streambuf buff1, buff2, bufGO1, bufGO2;
		if (turn == 1)
		{
			read_until(sock1, buff1, "\n");
			msg = buffer_cast<const char*>(buff1.data());
			write(sock2, boost::asio::buffer(msg));
			turn = 2;

		}
		else if (turn == 2)
		{
			read_until(sock2, buff2, "\n");
			msg = buffer_cast<const char*>(buff2.data());
			write(sock1, boost::asio::buffer(msg));
			turn = 1;
		}

		read_until(sock1, bufGO1, "\n");
		read_until(sock2, bufGO2, "\n");
		gameover = buffer_cast<const char*>(bufGO1.data());
	}
	cout << "\ngame finished!\n";
}
