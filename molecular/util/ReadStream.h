/*	ReadStream.h

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

namespace molecular
{

/// Abstract base class for data storage streams
/** @deprecated Most subsystems around molecular now cast file contents to a struct. */
class ReadStream
{
public:
	virtual ~ReadStream() = default;

	virtual	void	Read(uint8_t&	value) = 0;
	virtual	void	Read(uint16_t&	value) = 0;
	virtual	void	Read(uint32_t&	value) = 0;
	virtual	void	Read(uint64_t&	value) = 0;

	virtual	void	Read(int& value, const int precision);
	virtual	void	Read(float& value);
	virtual	void	Read(double& value);
	virtual	void	Read(bool& value);

	/// Read an array.
	virtual void	Read(void*	ptr, const size_t size);

	virtual void Read(int8_t&  value) {Read(reinterpret_cast<uint8_t&>(value));} ///< Signed conversion.
	virtual void Read(int16_t& value) {Read(reinterpret_cast<uint16_t&>(value));} ///< Signed conversion.
	virtual void Read(int32_t& value) {Read(reinterpret_cast<uint32_t&>(value));} ///< Signed conversion.
	virtual void Read(int64_t& value) {Read(reinterpret_cast<uint64_t&>(value));} ///< Signed conversion.
	
	virtual void Read(std::string& value);
};

}

#endif
