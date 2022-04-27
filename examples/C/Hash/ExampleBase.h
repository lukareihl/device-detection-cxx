/* *********************************************************************
 * This Original Work is copyright of 51 Degrees Mobile Experts Limited.
 * Copyright 2022 51 Degrees Mobile Experts Limited, Davidson House,
 * Forbury Square, Reading, Berkshire, United Kingdom RG1 3EU.
 *
 * This Original Work is licensed under the European Union Public Licence
 * (EUPL) v.1.2 and is subject to its terms as set out below.
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

#ifndef FIFTYONE_DEGREES_EXAMPLE_BASE_C_INCLUDED
#define FIFTYONE_DEGREES_EXAMPLE_BASE_C_INCLUDED
// Windows 'crtdbg.h' needs to be included
// before 'malloc.h'
#if defined(_DEBUG) && defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "../../../src/hash/hash.h"

/*
* Structure that contains the parameters that might be required by an example.
*/
typedef struct fiftyoneDegrees_example_parameters_t{
    char *dataFilePath; /**< Path to a data file */
    char *userAgentsFilePath; /**< Path to a user agents file */
    char *outputFilePath; /**< Path to an output file */
    char *propertiesString; /**< Required properties string */
    fiftyoneDegreesConfigHash *config; /**< Hash Configuration */
    FILE* output; /**< Output target for the example */
} fiftyoneDegreesExampleParameters;

typedef fiftyoneDegreesExampleParameters ExampleParameters;

/**
 * Function pointer for generic function to be implemented by each example.
 * @param param1 example parameters
 */
typedef void (*fiftyoneDegreesExampleRunPtr)(
    fiftyoneDegreesExampleParameters *);

/**
 * Function that perform memory check on example function to run. This function
 * will exit if the memory check found a leak.
 * @param parameters example parameters
 * @param run function pointer to example function to perform memory check on.
 */
EXTERNAL void fiftyoneDegreesExampleMemCheck(
    fiftyoneDegreesExampleParameters *parameters,
    fiftyoneDegreesExampleRunPtr run);

/**
 * Check data file tier and published date.
 * @param dataset pointer to the dataset structure
 */
EXTERNAL void fiftyoneDegreesExampleCheckDataFile(
    fiftyoneDegreesDataSetHash *dataset);

#endif
