#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include "logging/Log.hpp"

namespace sim_mob {

/// The session class provides serialization primitives on top of a socket.
/**
 * Each message sent using this class consists of:
 * 8-byte header containing the length of the serialized data in hexadecimal.
 * and then The serialized data.
 */
class Session;
typedef boost::shared_ptr<Session> session_ptr;;
class Session : public boost::enable_shared_from_this<Session>
{
public:
  /// Constructor.
  Session(boost::asio::io_service &io_service)
    : socket_(io_service)
  {
  }

  ~Session()
  {
//	  inbound_data_.clear();
  }

  /// Get the underlying socket. Used for making a Session or for accepting
  /// an incoming connection.
  boost::asio::ip::tcp::socket& socket()
  {
    return socket_;
  }

	bool makeWriteBuffer(std::string &input,
			std::vector<boost::asio::const_buffer> &output,
			boost::system::error_code &ec) {

		// Format the header.
		std::ostringstream header_stream;
		header_stream << std::setw(header_length) << std::hex << input.size();
		if (!header_stream || header_stream.str().size() != header_length) {
			// Something went wrong, inform the caller.
			ec = boost::system::error_code(
					boost::asio::error::invalid_argument);
			return false;
		}
		outbound_header_ = header_stream.str(); //not used

		output.push_back(boost::asio::buffer(outbound_header_));
		output.push_back(boost::asio::buffer(input));
		return true;
	}

	bool write(std::string &input, boost::system::error_code &ec) {
		boost::system::error_code ec_;
		std::vector<boost::asio::const_buffer> buffers;
		makeWriteBuffer(input, buffers, ec_);
		ec = ec_;
		if (ec_) {
			return false;
		}

		boost::asio::write(socket_, buffers, ec_);
		ec = ec_;
		if (ec_) {
			return false;
		}
		return true;
	}
	bool write(std::string &input) {
			boost::system::error_code ec_;
			std::vector<boost::asio::const_buffer> buffers;
			makeWriteBuffer(input, buffers, ec_);
			if (ec_) {
				return false;
			}

			boost::asio::write(socket_, buffers, ec_);
			if (ec_) {
				return false;
			}
			return true;
		}

  /// Asynchronously write a data structure to the socket.
  template <typename Handler>
  void async_write(std::string &data, Handler handler)
  {
//	outbound_data_ = data; //extra copy
    // Format the header.
    std::ostringstream header_stream;
    header_stream << std::setw(header_length)
      << std::hex << data.size();
    if (!header_stream || header_stream.str().size() != header_length)
    {
      // Something went wrong, inform the caller.
      boost::system::error_code error(boost::asio::error::invalid_argument);
      socket_.get_io_service().post(boost::bind(handler, error));
      return;
    }
    outbound_header_ = header_stream.str(); //not used

	    std::vector<boost::asio::const_buffer> buffers;
	    buffers.push_back(boost::asio::buffer(outbound_header_));
	    buffers.push_back(boost::asio::buffer(data));
	    boost::asio::async_write(socket_, buffers, handler);

//    boost::asio::async_write(socket_, boost::asio::buffer(/*outbound_data_*/data), handler);
  }

  /// Asynchronously read a data structure from the socket.
  template <typename Handler>
  void async_read(std::string &input, Handler handler)
  {
	  input.clear();
    // Issue a read operation to read exactly the number of bytes in a header.
    void (Session::*f)(const boost::system::error_code&,/*std::vector<char>*,*/ std::string &, boost::tuple<Handler>) = &Session::handle_read_header<Handler>;
    boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),boost::bind(f,shared_from_this(), boost::asio::placeholders::error,boost::ref(input)/*, t*/,boost::make_tuple(handler)));
  }

  /// Handle a completed read of a message header. The handler is passed using
  /// a tuple since boost::bind seems to have trouble binding a function object
  /// created using boost::bind as a parameter.
  template <typename Handler>
  void handle_read_header(const boost::system::error_code& e,/*std::vector<char>*t*/std::string &input, boost::tuple<Handler> handler)
  {
    if (e)
    {
      boost::get<0>(handler)(e);
    }
    else
    {
      std::istringstream is(std::string(inbound_header_, header_length));
      std::size_t inbound_data_size = 0;
      is >> std::hex >> inbound_data_size;
      if (!(inbound_data_size))
      {
        WarnOut( "ERROR in session-Handle_read_header\n" );
        return;
      }
      inbound_data_.resize(inbound_data_size);

      void (Session::*f)(const boost::system::error_code&,/*std::vector<char>**/std::string &, boost::tuple<Handler>) = &Session::handle_read_data<Handler>;
      boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),boost::bind(f, shared_from_this(),boost::asio::placeholders::error, /*t,*/ boost::ref(input), handler));
    }
  }

  /// Handle a completed read of message data.
  template <typename Handler>
  void handle_read_data(const boost::system::error_code& e,/*std::vector<char> * t*/std::string &input, boost::tuple<Handler> handler)
  {
	  if(e)
    {
      boost::get<0>(handler)(e);
    }
    else
    {
      try
      {
    	  std::string archive_data(&inbound_data_[0], inbound_data_.size());
    	  input = archive_data;
          boost::get<0>(handler)(e);
      }
      catch (std::exception& e)
      {
        // Unable to decode data.
    	WarnOut("Something wrong in the handle_read_data" << std::endl);
        boost::system::error_code error(boost::asio::error::invalid_argument);
        boost::get<0>(handler)(error);
        return;
      }
    }

	  inbound_data_.clear();
  }

private:
  /// The underlying socket.
  boost::asio::ip::tcp::socket socket_;

  /// The size of a fixed length header.
  enum { header_length = 8 };

  /// Holds an outbound header.
  std::string outbound_header_;

  /// Holds the outbound data.
//  std::string outbound_data_;

  /// Holds an inbound header.
  char inbound_header_[header_length];

  /// Holds the inbound data.
  std::vector<char> inbound_data_;
};

} // namespace sim_mob

