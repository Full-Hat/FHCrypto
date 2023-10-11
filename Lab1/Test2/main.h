#pragma once

#include <algorithm>
#include <iomanip>
#include <ios>
#include <istream>
#include <limits>
#include <stdexcept>
#include <string>
#include <iostream>
#include <random>
#include <sstream>

class Person
{
public:
    std::string m_first_name;
    std::string m_last_name;
    std::string m_age;

    void WriteMessage(std::ostream &stream)
    {
        stream << m_first_name << " " << m_last_name << " " << m_age << std::endl;
    }

    static std::vector<Person> parsePeople(std::istream& input) 
    {
        int numberOfPeople;
        input >> numberOfPeople;
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<Person> people(numberOfPeople + 1);

        int min = std::numeric_limits<int>::max(), max = 0;
        float average = 0;
        for (int i = 0; i < numberOfPeople; i++) {
            std::string data;
            std::getline(input, data, '\n');

            std::istringstream dataStream(data);
            std::string firstName, lastName, age;

            std::getline(dataStream, firstName, ' ');
            std::getline(dataStream, lastName, ' ');
            std::getline(dataStream, age);
            
            people[i] = {firstName, lastName, age};

            if (age.find('.') != age.npos || age.find(',') != age.npos)
            {
                throw std::invalid_argument("Value must be integer, not floating point type");
            }

            min = std::min(std::stoi(age), min);
            max = std::max(std::stoi(age), max);
            average += std::stoi(age);
        }

        average /= numberOfPeople;

        std::stringstream convertor;
        convertor << std::fixed << std::setprecision(2) << average;
        (*people.rbegin()) = {std::to_string(min), std::to_string(max), convertor.str()};

        return people;
    }

    static void readAndPrint(std::ostream &ostream, std::istream &istream)
    {
        auto people = parsePeople(istream);
        for (auto el : people)
        {
            el.WriteMessage(ostream);
        }
    }
};
