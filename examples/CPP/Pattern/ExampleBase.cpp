/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2019 51 Degrees Mobile Experts Limited, 5 Charlotte Close,
 * Caversham, Reading, Berkshire, United Kingdom RG4 7BY.
 *
 * This Original Work is licensed under the European Union Public Licence (EUPL) 
 * v.1.2 and is subject to its terms as set out below.
 *
 * If a copy of the EUPL was not distributed with this file, You can obtain
 * one at https://opensource.org/licenses/EUPL-1.2.
 *
 * The 'Compatible Licences' set out in the Appendix to the EUPL (as may be
 * amended by the European Commission) shall be deemed incompatible for
 * the purposes of the Work and the provisions of the compatibility
 * clause in Article 5 of the EUPL shall not apply.
 * 
 * If using the Work as, or as part of, a network application, by 
 * including the attribution notice(s) required under Article 5 of the EUPL
 * in the end user terms of the application under an appropriate heading, 
 * such notice(s) shall fulfill the requirements of that article.
 * ********************************************************************* */
 
 #include "ExampleBase.hpp"

using namespace FiftyoneDegrees;
using namespace FiftyoneDegrees::Examples::Pattern;

const char *ExampleBase::mobileUserAgent = (
	"Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 like Mac OS X) "
	"AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D167 "
	"Safari/9537.53");

const char *ExampleBase::desktopUserAgent = (
	"Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) "
	"Gecko/20100101 Firefox/41.0");

const char *ExampleBase::mediaHubUserAgent = (
	"Mozilla/5.0 (Linux; Android 4.4.2; X7 Quad Core "
	"Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 "
	"Chrome/30.0.0.0 Safari/537.36");

ExampleBase::ExampleBase(byte *data, long length, ConfigPattern *config) {
	this->config = config;

	// Set the properties to be returned for each User-Agent.
	string propertiesString =
		"ScreenPixelsWidth,HardwareModel,IsMobile,BrowserName";
	properties = new RequiredPropertiesConfig(propertiesString);

	// Initialise the engine for device detection.
	engine = new EnginePattern(
		data,
		length,
		config,
		properties);
}

ExampleBase::ExampleBase(string dataFilePath, ConfigPattern *config) {	
	this->config = config;

	// Set the properties to be returned for each User-Agent.
	string propertiesString =
		"ScreenPixelsWidth,HardwareModel,IsMobile,BrowserName";
	properties = new RequiredPropertiesConfig(propertiesString);

	// Initialise the engine for device detection.
	engine = new EnginePattern(
		dataFilePath,
		config,
		properties);
}

ExampleBase::ExampleBase(string dataFilePath)
	: ExampleBase(dataFilePath, new ConfigPattern()) { }

ExampleBase::~ExampleBase() {
	delete engine;
	delete config;
	delete properties;
}

void ExampleBase::reportStatus(
	fiftyoneDegreesStatusCode status,
	const char* fileName) {
	const char *message = fiftyoneDegreesStatusGetMessage(status, fileName);
	cout << message;
	fiftyoneDegreesFree((void*)message);
}

unsigned long ExampleBase::generateHash(unsigned char *value) {
	unsigned long hashCode = 5381;
	int i = *value++;
	while (i != 0) {
		hashCode = ((hashCode << 5) + hashCode) + i;
		i = *value++;
	}
	return hashCode;
}

unsigned long ExampleBase::getHashCode(ResultsPattern *results) {
	unsigned long hashCode = 0;
	uint32_t requiredPropertyIndex;
	string valueName;

	for (requiredPropertyIndex = 0;
		requiredPropertyIndex < (uint32_t)results->getAvailableProperties();
		requiredPropertyIndex++) {
		valueName = *results->getValueAsString(requiredPropertyIndex);
		hashCode ^= generateHash((unsigned char*)(valueName.c_str()));
	}
	return hashCode;
}

void ExampleBase::processUserAgent(
	const char *userAgent,
	void *state) {
	ThreadState *thread = (ThreadState*)state;

	ResultsPattern *results = thread->engine->process(userAgent);

	thread->hashCode ^= getHashCode(results);

	delete results;
}

void ExampleBase::SharedState::processUserAgentsSingle() {
	const char userAgent[500] = "";
	ThreadState thread(engine);
	fiftyoneDegreesTextFileIterate(
		userAgentFilePath.c_str(),
		userAgent,
		sizeof(userAgent),
		&thread,
		processUserAgent);
	printf("Finished with hash code '%i'\r\n", thread.hashCode);
}

void ExampleBase::SharedState::processUserAgentsMulti(void *state) {
	SharedState *shared = (SharedState*)state;
	shared->processUserAgentsSingle();
	FIFTYONE_DEGREES_INTERLOCK_INC(&shared->threadsFinished);
}

void ExampleBase::SharedState::startThreads() {
	int i;
	for (i = 0; i < THREAD_COUNT; i++) {
		threads[i] = thread(processUserAgentsMulti, this);
	}
}

void ExampleBase::SharedState::joinThreads() {
	int i;
	for (i = 0; i < THREAD_COUNT; i++) {
		threads[i].join();
	}
}

ExampleBase::SharedState::SharedState(
	DeviceDetection::Pattern::EnginePattern *engine,
	string userAgentFilePath) {
	this->engine = engine;
	this->threadsFinished = 0;
	this->userAgentFilePath = userAgentFilePath;
}

ExampleBase::ThreadState::ThreadState(
	DeviceDetection::Pattern::EnginePattern *engine) {
	this->engine = engine;
	this->hashCode = 0;
}