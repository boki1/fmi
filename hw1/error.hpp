#ifndef OOPCOIN_ERROR_HPP_INCLUDED_
#define OOPCOIN_ERROR_HPP_INCLUDED_

#include <exception>

#include "mystd/exception.hpp"

namespace oopcoin {

    class bad_cmd : public mystd::explained_exception {
    public:
        explicit bad_cmd(const mystd::string &msg)
                : mystd::explained_exception{msg} {}
    };

    class duplicate_user : public mystd::explained_exception {
    public:
        // TODO: Would implement the ctor to accept the name that is already present, but
        //       that would require me to implement concatenation in mystd::string and
        //       although it's not difficult it would mean that the chances that mystd
        //       is bigger than the task solution itself grow more and more :D.
        explicit duplicate_user()
                : mystd::explained_exception{"Cannot create user with duplicate name"} {}
    };

    class unknown_user : public mystd::explained_exception {
    public:
        explicit unknown_user() : mystd::explained_exception("Cannot find such user") {}
    };

    class successful_exit {
    };

}

#endif // OOPCOIN_CMDPARSER_HPP_INCLUDED_
