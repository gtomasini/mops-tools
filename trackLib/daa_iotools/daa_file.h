#pragma once

#include <fstream>
#include <vector>

namespace daa
{
    namespace io
    {

        template<typename T>
        class data_t
        {           
            T _the_data;
        public:
            T& operator()() { return _the_data; }
            const T& data() const { return _the_data; }
        };

        class file
        {

            public:
                template<typename T>
                static std::vector<data_t<T>> read(const std::string& a_filename);

            private:

                static bool is_file(const std::string& a_filename);

        };


        template<typename T>
        inline std::vector<data_t<T>> file::read(const std::string & a_filename)
        {
            std::vector<data_t<T>> out_data;
            if (!is_file(a_filename)) return out_data;

            std::ifstream is;
            is.open(a_filename, std::ios::in | std::ios::binary);
            
            if (!is.is_open()) return out_data;
            
            while (!is.eof())
            {
                data_t<T> current;
                is.read(static_cast<char*>(current()), sizeof(T));
                out_data.push_back(current);
            }
            is.close();

            return out_data;
        }
    }
}
