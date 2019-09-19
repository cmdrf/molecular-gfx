/*	GfxFile.cpp
	Copyright 2015 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "GfxFile.h"
#include <molecular/util/Parser.h>
#include <cassert>
#include <stdexcept>
#include <cstdlib>

using namespace molecular::Parser;

GfxFile::GfxFile(const char* fileData, std::size_t fileSize)
{
	using IdentifierChar = Alternation<Alpha, Digit, Char<'_'>>;
	using Identifier = Concatenation<IdentifierChar, Repetition<IdentifierChar>>;
	using UpperCaseIdentifier = Concatenation<UpperCaseLetter, Repetition<IdentifierChar>>;
	using LowerCaseIdentifier = Concatenation<LowerCaseLetter, Repetition<IdentifierChar>>;

	using FalseKeyword = Concatenation<Char<'f'>, Char<'a'>, Char<'l'>, Char<'s'>, Char<'e'>>;
	using TrueKeyword = Concatenation<Char<'t'>, Char<'r'>, Char<'u'>, Char<'e'>>;
	using Vec2Keyword = Concatenation<Char<'v'>, Char<'e'>, Char<'c'>, Char<'2'>>;
	using Vec3Keyword = Concatenation<Char<'v'>, Char<'e'>, Char<'c'>, Char<'3'>>;
	using Vec4Keyword = Concatenation<Char<'v'>, Char<'e'>, Char<'c'>, Char<'4'>>;
	using IVec2Keyword = Concatenation<Char<'i'>, Char<'v'>, Char<'e'>, Char<'c'>, Char<'2'>>;
	using IVec3Keyword = Concatenation<Char<'i'>, Char<'v'>, Char<'e'>, Char<'c'>, Char<'3'>>;
	using IVec4Keyword = Concatenation<Char<'i'>, Char<'v'>, Char<'e'>, Char<'c'>, Char<'4'>>;

	using Value = Alternation
	<
		Action<FalseKeyword, kFalseValueAction>,
		Action<TrueKeyword, kTrueValueAction>,
		Action<Real, kRealValueAction>,
//		Action<Integer, kIntegerValueAction>,
		Concatenation
		<
			Action<Vec2Keyword, kVec2TypeAction>,
			Option<Whitespace>,
			Char<'('>,
			Option<Whitespace>,
			Action<Real, kVecXValueAction>,
			Option<Whitespace>,
			Char<','>,
			Option<Whitespace>,
			Action<Real, kVecYValueAction>,
			Option<Whitespace>,
			Char<')'>
		>
	>;

	using Variable = Concatenation
	<
		Action<LowerCaseIdentifier, kVariableNameAction>,
		Option<Whitespace>,
		Char<':'>,
		Option<Whitespace>,
		Value
	>;

	using Reference = Concatenation<Char<'@'>, Action<LowerCaseIdentifier, kReferenceAction>>;

	class Object : public Concatenation
	<
		Action<UpperCaseIdentifier, kObjectAction>,
		Option<Concatenation<Whitespace, Action<LowerCaseIdentifier, kNodeNameAction>>>,
		Option<Whitespace>,
		Char<'{'>,
		Option<Whitespace>,
		Repetition
		<
			Concatenation
			<
				Alternation<Object, Variable, Reference>,
				Option<Whitespace>
			>
		>,
		Action<Char<'}'>, kObjectCloseAction>
	>{};

	using File = Concatenation<Option<Whitespace>, Object, Option<Whitespace>>;

	if(!File::Parse(fileData, fileData + fileSize, this))
		throw std::runtime_error("Could not parse GFX file");
}

void GfxFile::ParserAction(int action, const char* begin, const char* end)
{
	if(action == kObjectAction)
	{
		Node node;
		node.classBegin = begin;
		node.classEnd = end;
		node.parent = mNodeLevelStack.empty() ? -1 : mNodeLevelStack.top();
		mNodeLevelStack.push(mNodes.size());
		mNodes.push_back(node);
	}
	else if(action == kObjectCloseAction)
	{
		mNodeLevelStack.pop();
	}
	else if(action == kVariableNameAction)
	{
		assert(!mNodeLevelStack.empty());
		Variable var;
		var.nameBegin = begin;
		var.nameEnd = end;
		var.parent = mNodeLevelStack.top();
		var.type = Variable::kUnknown;
		mVariables.push_back(var);
	}
	else if(action == kNodeNameAction)
	{
		assert(!mNodes.empty());
		mNodes.back().nameBegin = begin;
		mNodes.back().nameEnd = end;
	}
	else if(action == kReferenceAction)
	{
		assert(!mNodeLevelStack.empty());
		Reference reference;
		reference.nameBegin = begin;
		reference.nameEnd = end;
		reference.parent = mNodeLevelStack.top();
		mReferences.push_back(reference);
	}
	else
	{
		assert(!mVariables.empty());
		if(action == kTrueValueAction || action == kFalseValueAction)
		{
			mVariables.back().type = Variable::kBool;
			mVariables.back().value.boolean = (action == kTrueValueAction);
		}
		else if(action == kIntegerValueAction)
		{
			mVariables.back().type = Variable::kInteger;
			mVariables.back().value.integer = atoi(begin);
		}
		else if(action == kRealValueAction)
		{
			mVariables.back().type = Variable::kReal;
			mVariables.back().value.real = atof(begin);
		}
		else if(action == kVec2TypeAction)
			mVariables.back().type = Variable::kVec2;
		else if(action == kVec3TypeAction)
			mVariables.back().type = Variable::kVec3;
		else if(action == kVec4TypeAction)
			mVariables.back().type = Variable::kVec4;
		else if(action == kVecXValueAction)
			mVariables.back().value.vec[0] = atof(begin);
		else if(action == kVecYValueAction)
			mVariables.back().value.vec[1] = atof(begin);
		else if(action == kVecZValueAction)
			mVariables.back().value.vec[2] = atof(begin);
		else if(action == kVecWValueAction)
			mVariables.back().value.vec[3] = atof(begin);
	}
}
