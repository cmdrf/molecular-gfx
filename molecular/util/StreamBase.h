/*	StreamBase.h

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MOLECULAR_STREAMBASE_H
#define MOLECULAR_STREAMBASE_H

#include <string>
#include "Types.h"

#include <molecular/util/AttributeInfo.h>

namespace molecular
{

/// Abstract base class for data storage streams
class WriteStreamBase
{
public:
	virtual ~WriteStreamBase(){}

	virtual void Write(const uint8_t  value, const AttributeInfo* info = nullptr) = 0;
	virtual void Write(const uint16_t value, const AttributeInfo* info = nullptr) = 0;
	virtual void Write(const uint32_t value, const AttributeInfo* info = nullptr) = 0;
	virtual void Write(const uint64_t value, const AttributeInfo* info = nullptr) = 0;

	virtual void Write(const int    value, const int precision, const AttributeInfo* info = nullptr);
	virtual void Write(const float  value, const AttributeInfo* info = nullptr);
	virtual void Write(const double value, const AttributeInfo* info = nullptr);
	virtual void Write(const bool   value, const AttributeInfo* info = nullptr);

	/// Write an array.
	virtual void Write(const void* ptr, const size_t size, const AttributeInfo* info = nullptr);
	void Write(void* ptr, const size_t size, const char* description)
	{
		AttributeInfo info(description);
		Write(ptr, size, &info);
	}

	virtual	void Write(const int8_t  value, const AttributeInfo* info = nullptr); ///< Signed conversion.
	virtual void Write(const int16_t value, const AttributeInfo* info = nullptr); ///< Signed conversion.
	virtual void Write(const int32_t value, const AttributeInfo* info = nullptr); ///< Signed conversion.
	virtual void Write(const int64_t value, const AttributeInfo* info = nullptr); ///< Signed conversion.

	virtual void Write(const std::string&	value, const AttributeInfo* info = nullptr);
};

class ReadStreamBase
{
public:
	virtual ~ReadStreamBase(){}

	virtual	void	Read(uint8_t&	value, const AttributeInfo* info = nullptr) = 0;
	virtual	void	Read(uint16_t&	value, const AttributeInfo* info = nullptr) = 0;
	virtual	void	Read(uint32_t&	value, const AttributeInfo* info = nullptr) = 0;
	virtual	void	Read(uint64_t&	value, const AttributeInfo* info = nullptr) = 0;

	virtual	void	Read(int& value, const int precision, const AttributeInfo* info = nullptr);
	virtual	void	Read(float& value, const AttributeInfo* info = nullptr);
	virtual	void	Read(double& value, const AttributeInfo* info = nullptr);
	virtual	void	Read(bool& value, const AttributeInfo* info = nullptr);

	/// Read an array.
	virtual void	Read(void*	ptr, const size_t size, const AttributeInfo* info = nullptr);
	void Read(void* ptr, const size_t size, const char* description)
	{
		AttributeInfo info(description);
		Read(ptr, size, &info);
	}

	virtual void Read(int8_t&  value, const AttributeInfo* info = nullptr) {Read(reinterpret_cast<uint8_t&>(value), info);} ///< Signed conversion.
	virtual void Read(int16_t& value, const AttributeInfo* info = nullptr) {Read(reinterpret_cast<uint16_t&>(value), info);} ///< Signed conversion.
	virtual void Read(int32_t& value, const AttributeInfo* info = nullptr) {Read(reinterpret_cast<uint32_t&>(value), info);} ///< Signed conversion.
	virtual void Read(int64_t& value, const AttributeInfo* info = nullptr) {Read(reinterpret_cast<uint64_t&>(value), info);} ///< Signed conversion.
	
	virtual void Read(std::string& value, const AttributeInfo* info = nullptr);
};

}

#endif
