#ifndef SERIALIZER_H_INCLUDED_
#define SERIALIZER_H_INCLUDED_

#include <fstream>
using namespace std;
template<typename T>
class serializer
{
public:
    static void read(std::istream &s, T* v)
    {
        s.read((char *)v, sizeof(T));
    }

    static void readString(std::istream &s, T* v)
    {
        int size = 0;
        s.read((char *)&size, sizeof(size));
        if (size > 0)
        {
            v->resize(size);
            s.read((char *)v->data(), size);
        }
    }

    static void read(std::istream &s, std::vector<T>* v)
    {
        int size = 0;
        s.read((char *)&size, sizeof(size));
        if (size > 0)
        {
            v->resize(size);
            s.read((char *)&v->front(), size * sizeof(T));
        }
    };
	static void readEach(std::istream &s, std::vector<T>* v)
	{
		int size = 0;
		s.read((char *)&size, sizeof(size));
		if (size > 0)
		{
			v->resize(size);
			for (std::vector<T>::iterator it = v->begin(); it != v->end(); it++)
			{
				s >> (*it);
			}
		}
	};

    static void write(std::ostream &s, T* v)
    {
        s.write((char *)v, sizeof(T));
    };

    static void writeString(std::ostream &s, T* v)
    {
        int size = v->size();
        s.write((char *)&size, sizeof(size));
        if (size > 0)
        {
            s.write((char *)v->data(), size);
        }
    }

    static void write(std::ostream &s, std::vector<T>* v)
    {
        int size = v->size();
        s.write((char *)&size, sizeof(size));
        if (size > 0)
        {
			s.write((char *)&v->front(), size * sizeof(T));
        }
    };

	static void writeEach(std::ostream &s, std::vector<T>* v)
	{
		int size = v->size();
		s.write((char *)&size, sizeof(size));
		if (size > 0)
		{
			for (std::vector<T>::iterator it = v->begin(); it != v->end(); it++)
			{
				s << *it;
			}
		}
	};

};

#endif