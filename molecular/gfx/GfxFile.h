/*	GfxFile.h
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef GFXFILE_H
#define GFXFILE_H

#include <vector>
#include <stack>

class GfxFile
{
public:
	GfxFile(const char* fileData, std::size_t fileSize);

	void ParserAction(int action, const char* begin, const char* end);

	struct Node
	{
		const char* classBegin = nullptr;
		const char* classEnd = nullptr;
		const char* nameBegin = nullptr;
		const char* nameEnd = nullptr;
		int parent = -1;
	};

	struct Variable
	{
		enum Type
		{
			kUnknown,
			kBool,
			kInteger,
			kReal,
			kVec2,
			kVec3,
			kVec4,
			kIVec2,
			kIVec3,
			kIVec4
		};

		Type type = kUnknown;
		const char* nameBegin = nullptr;
		const char* nameEnd = nullptr;
		int parent = -1;

		union
		{
			bool boolean;
			float real;
			int integer;
			float vec[4];
			int ivec[4];
		} value;
	};

	struct Reference
	{
		const char* nameBegin = nullptr;
		const char* nameEnd = nullptr;
		int parent = -1;
	};

	const std::vector<Node>& GetNodes() const {return mNodes;}
	const std::vector<Variable>& GetVariables() const {return mVariables;}
	const std::vector<Reference>& GetReferences() const {return mReferences;}

private:
	enum
	{
		kObjectAction,
		kNodeNameAction,
		kObjectCloseAction,
		kVariableNameAction,
		kTrueValueAction,
		kFalseValueAction,
		kIntegerValueAction,
		kRealValueAction,
		kVec2TypeAction,
		kVec3TypeAction,
		kVec4TypeAction,
		kVecXValueAction,
		kVecYValueAction,
		kVecZValueAction,
		kVecWValueAction,
		kReferenceAction
	};

	std::vector<Node> mNodes;
	std::vector<Variable> mVariables;
	std::stack<int> mNodeLevelStack;
	std::vector<Reference> mReferences;

};

#endif // GFXFILE_H
