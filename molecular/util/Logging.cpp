/*	Logging.cpp
	Copyright 2015-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "Logging.h"

INITIALIZE_EASYLOGGINGPP

namespace Logging
{

void Initialize(int argc, char* argv[], const std::string& logfile)
{
	START_EASYLOGGINGPP(argc, argv);
	el::Configurations defaultConf;
	defaultConf.setToDefault();
	defaultConf.setGlobally(el::ConfigurationType::Format, "%level %msg");
	defaultConf.set(el::Level::Error, el::ConfigurationType::Format, "%level %fbase:%line: %msg");
	if(!logfile.empty())
	{
		defaultConf.setGlobally(el::ConfigurationType::Filename, logfile);
		defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
	}
	el::Loggers::reconfigureAllLoggers(defaultConf);
}

}

