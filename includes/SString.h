// SciTE - Scintilla based Text Editor
/** @file SString.h
 ** A simple string class.
 **/
 // Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
 // The License.txt file describes the conditions under which this software may be distributed.

#ifndef SSTRING_H
#define SSTRING_H

// Define another string class.
// While it would be 'better' to use std::string, that doubles the executable size.
// An SString may contain embedded nul characters.

/**
 * Base class from which SString is derived.
 */
class SContainer
{
public:
	/** Type of string lengths (sizes) and positions (indexes). */
	typedef size_t lenpos_t;
	/** Out of bounds value indicating that the string argument should be measured. */
	enum { measure_length = 0xffffffffU };

protected:
	char* s; ///< The C string
	lenpos_t sSize; ///< The size of the buffer, less 1: ie. the maximum size of the string

	SContainer() : s(0), sSize(0) {}
	~SContainer()
	{
		delete[]s; // Suppose it was allocated using StringAllocate
		s = 0;
		sSize = 0;
	}
	/** Size of buffer. */
	lenpos_t size() const
	{
		if (s)
		{
			return sSize;
		}
		else
		{
			return 0;
		}
	}
public:
	/**
	 * Allocate uninitialized memory big enough to fit a string of the given length.
	 * @return the pointer to the new string
	 */
	static char* StringAllocate(lenpos_t len)
	{
		if (len != measure_length)
		{
			return new char[len + 1];
		}
		else
		{
			return 0;
		}
	}
	/**
	 * Duplicate a buffer/C string.
	 * Allocate memory of the given size, or big enough to fit the string if length isn't given;
	 * then copy the given string in the allocated memory.
	 * @return the pointer to the new string
	 */
	static char* StringAllocate(const char* s, lenpos_t len = measure_length)
	{
		if (s == 0)
		{
			return 0;
		}
		if (len == measure_length)
		{
			len = strlen(s);
		}
		char* sNew = new char[len + 1];
		if (sNew)
		{
			memcpy(sNew, s, len);
			sNew[len] = '\0';
		}
		return sNew;
	}
};

/**
 * @brief A simple string class.
 *
 * Hold the length of the string for quick operations,
 * can have a buffer bigger than the string to avoid too many memory allocations and copies.
 * May have embedded zeroes as a result of @a substitute, but relies too heavily on C string
 * functions to allow reliable manipulations of these strings, other than simple appends, etc.
 */
class SString : protected SContainer
{
	lenpos_t sLen; ///< The size of the string in s
	lenpos_t sizeGrowth; ///< Minimum growth size when appending strings
	enum { sizeGrowthDefault = 64 };

	bool grow(lenpos_t lenNew)
	{
		while (sizeGrowth * 6 < lenNew)
		{
			sizeGrowth *= 2;
		}
		char* sNew = new char[lenNew + sizeGrowth + 1];
		if (sNew)
		{
			if (s)
			{
				memcpy(sNew, s, sLen);
				delete[]s;
			}
			s = sNew;
			s[sLen] = '\0';
			sSize = lenNew + sizeGrowth;
		}
		return sNew != 0;
	}
	SString &assign(const char* sOther, lenpos_t sSize_ = measure_length)
	{
		if (!sOther)
		{
			sSize_ = 0;
		}
		else if (sSize_ == measure_length)
		{
			sSize_ = strlen(sOther);
		}
		if (sSize > 0 && sSize_ <= sSize) // Does not allocate new buffer if the current is big enough
		{
			if (s)
			{
				if (sSize_)
				{
					memcpy(s, sOther, sSize_);
				}
				s[sSize_] = '\0';
			}
			sLen = sSize_;
		}
		else
		{
			delete[]s;
			s = StringAllocate(sOther, sSize_);
			if (s)
			{
				sSize = sSize_; // Allow buffer bigger than real string, thus providing space to grow
				sLen = sSize_;
			}
			else
			{
				sSize = sLen = 0;
			}
		}
		return *this;
	}

public:
	SString() : sLen(0), sizeGrowth(sizeGrowthDefault) {}
	SString(const SString &source) : SContainer(), sizeGrowth(sizeGrowthDefault)
	{
		s = StringAllocate(source.s, source.sLen);
		sSize = sLen = (s) ? source.sLen : 0;
	}
	SString(const char* s_) : sizeGrowth(sizeGrowthDefault)
	{
		s = StringAllocate(s_);
		sSize = sLen = (s) ? strlen(s) : 0;
	}
	SString(const char* s_, lenpos_t first, lenpos_t last) : sizeGrowth(sizeGrowthDefault)
	{
		// note: expects the "last" argument to point one beyond the range end (a la STL iterators)
		s = StringAllocate(s_ + first, last - first);
		sSize = sLen = (s) ? last - first : 0;
	}
	~SString()
	{
		sLen = 0;
	}
	void clear()
	{
		if (s)
		{
			*s = '\0';
		}
		sLen = 0;
	}
	/** Size of buffer. */
	lenpos_t size() const
	{
		return SContainer::size();
	}
	/** Size of string in buffer. */
	lenpos_t length() const
	{
		return sLen;
	}
	/** Read access to a character of the string. */
	char operator[](lenpos_t i) const
	{
		return (s && i < sSize) ? s[i] : '\0';
	}
	SString &operator=(const char* source)
	{
		return assign(source);
	}
	SString &operator=(const SString &source)
	{
		if (this != &source)
		{
			assign(source.s, source.sLen);
		}
		return *this;
	}
	bool contains(char ch) const
	{
		return (s && *s) ? strchr(s, ch) != 0 : false;
	}
	void setsizegrowth(lenpos_t sizeGrowth_)
	{
		sizeGrowth = sizeGrowth_;
	}
	const char* c_str() const
	{
		return s ? s : "";
	}
	/** Give ownership of buffer to caller which must use delete[] to free buffer. */
	char* detach()
	{
		char* sRet = s;
		s = 0;
		sSize = 0;
		sLen = 0;
		return sRet;
	}
	SString &append(const char* sOther, lenpos_t sLenOther = measure_length, char sep = '\0')
	{
		if (!sOther)
		{
			return *this;
		}
		if (sLenOther == measure_length)
		{
			sLenOther = strlen(sOther);
		}
		int lenSep = 0;
		if (sLen && sep) // Only add a separator if not empty
		{
			lenSep = 1;
		}
		lenpos_t lenNew = sLen + sLenOther + lenSep;
		// Conservative about growing the buffer: don't do it, unless really needed
		if ((lenNew < sSize) || (grow(lenNew)))
		{
			if (lenSep)
			{
				s[sLen] = sep;
				sLen++;
			}
			memcpy(&s[sLen], sOther, sLenOther);
			sLen += sLenOther;
			s[sLen] = '\0';
		}
		return *this;
	}
	SString &operator+=(const char* sOther)
	{
		return append(sOther, static_cast<lenpos_t>(measure_length));
	}
	SString &operator+=(const SString &sOther)
	{
		return append(sOther.s, sOther.sLen);
	}
	SString &operator+=(char ch)
	{
		return append(&ch, 1);
	}
	SString &appendwithseparator(const char* sOther, char sep)
	{
		return append(sOther, strlen(sOther), sep);
	}
	SString &change(lenpos_t pos, char ch)
	{
		if (pos < sLen)
		{
			*(s + pos) = ch;
		}
		return *this;
	}
	/** Read an integral numeric value from the string. */
	int value() const
	{
		return s ? atoi(s) : 0;
	}
};

#endif
