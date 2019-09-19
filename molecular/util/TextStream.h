/*	TextStream.h
	Copyright 2014-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TEXTSTREAM_H
#define TEXTSTREAM_H

#include <stdexcept>
#include <stdint.h>

class TextReadStreamBase
{
public:
	virtual ~TextReadStreamBase() {}
	virtual char* GetNextLine() = 0;
};

template<class Storage>
class TextReadStream : public TextReadStreamBase
{
public:
	TextReadStream(Storage& storage) : mStorage(storage)
	{
		mLine[0] = 0;
	}

	/// Reads a line and returns it
	/** @returns Pointer to an internal storage of the line, which is only
			valid until the next call to GetNextLine(). Returns nullptr if the
			storage is at its end. */
	char* GetNextLine() override
	{
		if(mStorage.EndOfData())
			return nullptr;

		char c;
		int i;
		for(i = 0; i < kMaxLineLength; ++i)
		{
			if(mStorage.EndOfData() || mStorage.Read(&c, 1) != 1 || c == '\n')
			{
				mLine[i] = 0;
				return mLine;
			}
			else
				mLine[i] = c;
		}
		throw std::range_error("Line too long");
	}

private:
	static const int kMaxLineLength = 1024;
	char mLine[kMaxLineLength];
	Storage& mStorage;
};

#endif // TEXTSTREAM_H
