
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TRAITS_DELIMITERS_IPP
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TRAITS_DELIMITERS_IPP

#include <boost/network/tags.hpp>

namespace boost { namespace network { namespace http {

    // specialize on the tags::http type
    template <>
        struct delimiters<tags::http> {
            static char const * const string_crlf() {
                static char const * const CRLF = "\x0D\x0A";
                return CRLF;
            };

            static char const * const string_http_version() {
                static char const * const HTTP_VERSION = "HTTP/1.1";
                return HTTP_VERSION;
            };

            static char const * const header_name_value_delimiter() {
                static char const * const HEADER_NAME_VALUE_DELIMITER = ": ";
                return HEADER_NAME_VALUE_DELIMITER;
            };
        };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TRAITS_DELIMITERS_IPP
