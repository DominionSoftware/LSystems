#pragma once
 
#pragma optimize("",off)


#define MAXSTR 30000
#define MAXAXIOM 100;



template<typename T>
class Buffer
{
public:

	explicit Buffer(size_t sz) 
	{	
		buffer1 = new T[sz];
		buffer2 = new T[sz];
		for (int i = 0; i < MAXSTR; i++)
			*(buffer1 + i) = *(buffer2 + i) = 0;
		limPtr = buffer2 + 1 + MAXSTR - MAXAXIOM;
	}

    virtual ~Buffer()
	{
		delete[] buffer1;
		delete[] buffer2;

	}

	std::tuple<char *,char* > GetBasePointers()
	{
		return std::make_tuple(buffer1 + 1, buffer2 + 1);
	}
	 
 
	T * nextPtr;
	T * curPtr;
	T * limPtr;

	T * buffer1;
	T * buffer2;
	Buffer() = delete;

};