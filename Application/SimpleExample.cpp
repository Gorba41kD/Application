#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include<iostream>
#include<chrono>
#include<string>
#include<vector>
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>
std::vector<char> vBuffer(20 * 1024);
void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << "bytes\n\n";
				for (int i = 0; i < length; i++)
				{
					std::cout << vBuffer[i];
				}
				GrabSomeData(socket);
			}
		});
}
int main()
{
	std::string localHost = "127.0.0.1";
	std::string firstIp = "93.184.216.34";
	std::string secondIp = "51.38.81.49";

	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address(firstIp, ec), 80);
	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);
	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}
	if (socket.is_open())
	{
		std::string sRequest =
			"Get /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close \r\n\r\n";
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		socket.wait(socket.wait_read);
		size_t bytes = socket.available();
		std::cout << "Bytes Available: " << bytes << std::endl;
		if (bytes > 0)
		{
			std::vector<char> buffer(bytes);
			socket.read_some(asio::buffer(buffer.data(), buffer.size()),ec);
			for (auto el : buffer)
			{
				std::cout << el;
			}
		}
	}
	/*asio::error_code ec;
	asio::io_context context;
	asio::io_context::work idleWork(context);
	std::thread thrContext(std::thread([&]() {context.run(); }));
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address(secondIp, ec), 80);
	asio::ip::tcp::socket socket(context);
	socket.connect(endpoint, ec);
	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}
	if (socket.is_open())
	{
		GrabSomeData(socket);
		std::string sRequest =
			"Get /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close \r\n\r\n";
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		context.stop();
		if (thrContext.joinable()) thrContext.join();
	}*/
	return 0;
}
