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

/**
@example Pattern/MatchMetrics.c
Match metrics example of using 51Degrees device detection.

The example shows how to:

1. Specify the name of the data file and properties the data set should be
initialised with.
```
const char* fileName = argv[1];
fiftyoneDegreesPropertiesRequired properties =
	fiftyoneDegreesPropertiesDefault;
properties.string = "IsMobile";
```

2. Instantiate the 51Degrees data set within a resource manager from the
specified data file with the required properties and the specified
configuration.
```
fiftyoneDegreesStatusCode status =
	fiftyoneDegreesPatternInitManagerFromFile(
		&manager,
		&config,
		&properties,
		dataFilePath,
		exception);
```

3. Create a results instance ready to be populated by the data set.
```
fiftyoneDegreesResultsPattern *results =
	fiftyoneDegreesResultsPatternCreate(
		&manager,
		1,
		0);
```

4. Process a single HTTP User-Agent string to retrieve the values associated
with the User-Agent for the selected properties.
```
fiftyoneDegreesResultsPatternFromUserAgent(
	results,
	mobileUserAgent,
	strlen(mobileUserAgent),
	exception);
```

5. Obtain match method: provides information about the algorithm that was used
to perform detection for a particular User-Agent. For more information on what
each method means please see:
<a href="https://51degrees.com/support/documentation/pattern">
How device detection works</a>.
```
fiftyoneDegreesPatternMatchMethod method = results->items->method;

```

6. Obtain difference:  used when detection method is not Exact or None.
This is an integer value and the larger the value the less confident the
detector is in this result.
```
int difference = results->items->difference;
```

7. Obtain signature rank: an integer value that indicates how popular the
device is. The lower the rank the more popular the signature.
```
int rank = results->items->rank;
```

8. Obtain the matched User-Agent: the matched substrings in the User-Agent
separated with underscored.
```
char *matchedUserAgent = results->items->b.matchedUserAgent;
```

9. Release the memory used by the results.
```
fiftyoneDegreesResultsPatternFree(results);
```

10. Finally release the memory used by the data set resource.
```
fiftyoneDegreesResourceManagerFree(&manager);
```
*/

#ifdef _DEBUG
#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include "dmalloc.h"
#endif
#endif

#include <stdio.h>
#include "../../../src/pattern/pattern.h"
#include "../../../src/pattern/fiftyone.h"

static const char *dataDir = "device-detection-data";

static const char *dataFileName = "51Degrees-LiteV3.2.dat";

static char valueBuffer[1024] = "";

static char* getPropertyValueAsString(
	ResultsPattern *results,
	const char *propertyName) {
	EXCEPTION_CREATE;
	ResultsPatternGetValuesString(
		results,
		propertyName,
		valueBuffer,
		sizeof(valueBuffer),
		",",
		exception);
	EXCEPTION_THROW;
	return valueBuffer;
}

static void outputMatchMetrics(ResultsPattern *results) {
	EXCEPTION_CREATE;
	char deviceId[40];
	PatternGetDeviceIdFromResults(
		results,
		deviceId,
		sizeof(deviceId),
		exception);
	EXCEPTION_THROW;
	int method = results->items->method;
	int difference = results->items->difference;
	int rank = results->items->rank;
	char *matchedUserAgent = results->items->b.matchedUserAgent;
	printf("   IsMobile: %s\n",
		getPropertyValueAsString(results, "IsMobile"));
	printf("   Id: %s\n", deviceId);
	printf("   Method: %d\n", method);
	printf("   Difference: %d\n", difference);
	printf("   Rank: %d\n", rank);
	printf("   Sub Strings: %s\n", matchedUserAgent);
	return;
}

/**
 * Reports the status of the data file initialization.
 */
static void reportStatus(StatusCode status,
	const char* fileName) {
	const char *message = StatusGetMessage(status, fileName);
	printf("%s\n", message);
	Free((void*)message);
}

void fiftyoneDegreesPatternMatchMetrics(
	const char *dataFilePath,
	ConfigPattern *config) {
	EXCEPTION_CREATE;
	ResourceManager manager;

	// Set the properties to be returned for each User-Agent.
	PropertiesRequired properties = PropertiesDefault;
	properties.string = "ScreenPixelsWidth,HardwareModel,IsMobile,BrowserName";

	// Initialise the manager for device detection.
	StatusCode status = PatternInitManagerFromFile(
		&manager,
		config,
		&properties,
		dataFilePath,
		exception);
	if (status != SUCCESS) {
		reportStatus(status, dataFilePath);
		fgetc(stdin);
		return;
	}

	// Create a results instance to store and process User-Agents.
	ResultsPattern *results = ResultsPatternCreate(&manager, 1, 0);

	// User-Agent string of an iPhone mobile device.
	const char* mobileUserAgent = ("Mozilla/5.0 (iPhone; CPU iPhone OS 7_1 "
		"like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 "
		"Mobile/11D167 Safari/9537.53");

	// User-Agent string of Firefox Web browser version 41 on desktop.
	const char* desktopUserAgent = (
		"Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) "
		"Gecko/20100101 Firefox/41.0");

	// User-Agent string of a MediaHub device.
	const char* mediaHubUserAgent = (
		"Mozilla/5.0 (Linux; Android 4.4.2; X8 Quad Core "
		"Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 "
		"Chrome/30.0.0.0 Safari/537.36");

	printf("Starting Match Metrics Example.\n");

	// Carries out a match for a mobile User-Agent.
	printf("\nMobile User-Agent: %s\n", mobileUserAgent);
	ResultsPatternFromUserAgent(
		results,
		mobileUserAgent,
		strlen(mobileUserAgent),
		exception);
	outputMatchMetrics(results);

	// Carries out a match for a desktop User-Agent.
	printf("\nDesktop User-Agent: %s\n", desktopUserAgent);
	ResultsPatternFromUserAgent(
		results,
		desktopUserAgent,
		strlen(desktopUserAgent),
		exception);
	outputMatchMetrics(results);

	// Carries out a match for a MediaHub User-Agent.
	printf("\nMedia hub User-Agent: %s\n", mediaHubUserAgent);
	ResultsPatternFromUserAgent(
		results,
		mediaHubUserAgent,
		strlen(mediaHubUserAgent),
		exception);
	outputMatchMetrics(results);

	// Ensure the results are freed to avoid memory leaks.
	ResultsPatternFree(results);

	// Free the resources used by the manager.
	ResourceManagerFree(&manager);
}

#ifndef TEST

int main(int argc, char* argv[]) {
	StatusCode status = SUCCESS;
	ConfigPattern config = PatternDefaultConfig;
	char dataFilePath[FILE_MAX_PATH];
	if (argc > 1) {
		strcpy(dataFilePath, argv[1]);
	}
	else {
		status = FileGetPath(
			dataDir,
			dataFileName,
			dataFilePath,
			sizeof(dataFilePath));
	}
	if (status != SUCCESS) {
		reportStatus(status, dataFileName);
		fgetc(stdin);
		return 1;
	}
	if (CollectionGetIsMemoryOnly()) {
		config = PatternInMemoryConfig;
	}

#ifdef _DEBUG
#ifndef _MSC_VER
	dmalloc_debug_setup("log-stats,log-non-free,check-fence,log=dmalloc.log");
#else
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif
#endif

	fiftyoneDegreesPatternMatchMetrics(
		dataFilePath,
		&config);

#ifdef _DEBUG
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#else
	printf("Log file is %s\r\n", dmalloc_logpath);
#endif
#endif

	// Wait for a character to be pressed.
	fgetc(stdin);

	return 0;
}

#endif
