#pragma once

#include <cassert>
#include <cstdint>
#include <istream>
#include <limits>
#include <string>

#include "bar-simulator.h"

enum class program {
    Fraud,
    InternationalSchemes,
    CreativeNihilism,
    SubjectiveResearches,
    FileAnalizis,
    Micromanagement,
    AppliedMaterialization,
    Subjectivistics,
    MagnetismAndClearing
};

static program string2program(const std::string &str) {
    if (str == "Fraud")
        return program::Fraud;
    if (str == "International Schemes")
        return program::InternationalSchemes;
    if (str == "Creative Nihilism")
        return program::CreativeNihilism;
    if (str == "Subjective Researches")
        return program::SubjectiveResearches;
    if (str == "File Analizis")
        return program::FileAnalizis;
    if (str == "Micromanagement")
        return program::Micromanagement;
    if (str == "Applied Materialization")
        return program::AppliedMaterialization;
    if (str == "Subjectivistics")
        return program::Subjectivistics;
    if (str == "Magnetism and Clearing")
        return program::MagnetismAndClearing;
    throw incorrect_simulation{"Cannot convert given string to enum class program."};
}

static std::string program2string(program p) {
    switch (p) {
        case program::Fraud:
            return "Fraud";
        case program::InternationalSchemes:
            return "International Schemes";
        case program::CreativeNihilism:
            return "Creative Nihilism";
        case program::SubjectiveResearches:
            return "Subjective Researches";
        case program::FileAnalizis:
            return "File Analizis";
        case program::Micromanagement:
            return "Micromanagement";
        case program::AppliedMaterialization:
            return "Applied Materialization";
        case program::Subjectivistics:
            return "Subjectivistics";
        case program::MagnetismAndClearing:
            return "Magnetism and Clearing";
    }
    throw incorrect_simulation{"Cannot convert given enum class program to string."};
}

struct student {
    friend std::istream &operator>>(std::istream &is, student &st) {
        is >> st.m_id >> st.m_arrival;

        st.m_program = [&] {
            static constexpr auto max_istream = std::numeric_limits<std::streamsize>::max();
            std::string program_as_str;
            for (std::string s; !is.eof() && !std::isdigit(is.peek()); is.ignore(max_istream, ' ')) {
                is >> s;
                if (!program_as_str.empty())
                    program_as_str.push_back(' ');
                program_as_str.append(s);
            }
            return string2program(program_as_str);
        }();

        is >> st.m_stay;

        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const student &st) {
        os << st.m_id << " " << st.m_arrival << " " << program2string(st.m_program) << " " << st.m_stay;
        return os;
    }

    std::uint64_t m_id;
    std::uint64_t m_arrival;
    std::uint64_t m_stay;
    program m_program;

    std::uint64_t m_enter{0};
};

class smart_student {

public:
    smart_student(student *st_ptr) : m_st_ptr{st_ptr} {}

    static smart_student read_from(std::istream &is) {
        auto st = new student;
        is >> *st;
        return smart_student{st};
    }

    ~smart_student() noexcept {
        delete m_st_ptr;
    }

    smart_student(const smart_student &) = delete;

    smart_student &operator=(const smart_student &) = delete;

    student &operator*() { return *m_st_ptr; }

    const student &operator*() const { return *m_st_ptr; }

    student *get() { return m_st_ptr; }

    const student *get() const { return m_st_ptr; }

private:
    student *m_st_ptr;
};
