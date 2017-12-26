#pragma once
//from https ://stackoverflow.com/questions/27760602/one-array-multiple-types

#include <cassert>
#include <string>

struct TaggedUnion
{
	enum Type
	{
		Boolean,
		Char,
		Int,
		Double,
		WString
	};


	TaggedUnion(const bool& value) : type(Boolean), value(value) {}
	TaggedUnion(const char& value) : type(Char), value(value) {}
	TaggedUnion(const int& value) : type(Int), value(value) {}
	TaggedUnion(const double& value) : type(Double), value(value) {}
	TaggedUnion(const std::wstring& value) : type(WString), value(value) {}
	TaggedUnion(const TaggedUnion& ref) : type(ref.type), value(ref.value) {}


	Type getType() const { return type; }
	bool getBoolean() const { assert(type == Boolean); return value.getBoolean(); }
	char getChar() const { assert(type == Char); return value.getChar(); }
	int getInt() const { assert(type == Int);  return value.getInt(); }
	double getDouble() const { assert(type == Double);  return value.getDouble(); }
	std::wstring getWString() const { assert(type == WString);  return value.getWString(); }

private:

	union Union
	{
		Union(const bool& value) : booleanValue(value) {}
		Union(const char& value) : charValue(value) {}
		Union(const int& value) : intValue(value) {}
		Union(const double& value) : doubleValue(value) {}
		Union(const std::wstring& value) : wstringValue(value) {}
		Union(const Union& ref) : 
			booleanValue(ref.booleanValue), 
			charValue(ref.charValue),
			intValue(ref.intValue),
			doubleValue(ref.doubleValue),
			wstringValue(ref.wstringValue)
			{}
		~Union() {}

		Union& operator=(const Union & ref)
		{
			
			booleanValue = ref.booleanValue;
			charValue = ref.charValue;
			intValue = ref.intValue;
			doubleValue = ref.doubleValue;
			wstringValue = ref.wstringValue;
			return *this;
		}


		bool getBoolean() const { return booleanValue; }
		char getChar() const { return charValue; }
		int getInt() const { return intValue; }
		double getDouble() const { return doubleValue; }
		std::wstring getWString() const { return wstringValue; }

	private:
		bool	booleanValue;
		char    charValue;
		int     intValue;
		double  doubleValue;
		std::wstring wstringValue;
	};

	Type type;
	Union value;
};