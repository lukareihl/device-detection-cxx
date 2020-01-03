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

#ifndef FIFTYONE_DEGREES_ENGINE_HASH_HPP
#define FIFTYONE_DEGREES_ENGINE_HASH_HPP

#include <sstream>
#include "../common-cxx/resource.h"
#include "../EvidenceDeviceDetection.hpp"
#include "../EngineDeviceDetection.hpp"
#include "../common-cxx/RequiredPropertiesConfig.hpp"
#include "../common-cxx/Date.hpp"
#include "../common-cxx/overrides.h"
#include "ConfigPattern.hpp"
#include "ResultsPattern.hpp"
#include "MetaDataPattern.hpp"

using namespace std;
using namespace FiftyoneDegrees::Common;

namespace FiftyoneDegrees {
	namespace DeviceDetection {
		namespace Pattern {
			/**
			 * Encapsulates the Pattern engine class which implements
			 * #EngineDeviceDetection. This carries out processing using a
			 * Pattern data set.
			 *
			 * An engine is constructed with a configuration, and either a
			 * data file, or an in memory data set, then used to process
			 * evidence in order to return a set of results. It also exposes
			 * methods to refresh the data using a new data set, and get
			 * properties relating to the data set being used by the engine.
			 *
			 * ## Usage Example
			 *
			 * ```
			 * using namespace FiftyoneDegrees::Common;
			 * using namespace FiftyoneDegrees::DeviceDetection;
			 * using namespace FiftyoneDegrees::DeviceDetection::Pattern;
			 * ConfigPattern *config;
			 * string dataFilePath;
			 * void *inMemoryDataSet;
			 * long inMemoryDataSetLength;
			 * RequiredPropertiesConfig *properties;
			 * EvidenceDeviceDetection *evidence;
			 *
			 * // Construct the engine from a data file
			 * EnginePattern *engine = new EnginePattern(
			 *     dataFilePath,
			 *     config,
			 *     properties);
			 *
			 * // Or from a data file which has been loaded into continuous
			 * // memory
			 * EnginePattern *engine = new EnginePattern(
			 *     inMemoryDataSet,
			 *     inMemoryDataSetLength,
			 *     config,
			 *     properties);
			 *
			 * // Process some evidence
			 * ResultsPattern *results = engine->process(evidence);
			 *
			 * // Or just process a single User-Agent string
			 * ResultsPattern *results = engine->process("some User-Agent");
			 *
			 * // Or do either of the above, while specifying the value of
			 * // closestSignatures to use for processing
			 * ResultsPattern *results = engine->process(evidence, 200);
			 *
			 * // Do something with the results
			 * // ...
			 *
			 * // Delete the results and the engine
			 * delete results;
			 * delete engine;
			 * ```
			 */
			class EnginePattern  : public EngineDeviceDetection {
			public:
				/**
				 * @name Constructors
				 * @{
				 */

				 /**
				  * @copydoc Common::EngineBase::EngineBase
				  * The data set is constructed from the file provided.
				  * @param fileName path to the file containing the data file
				  * to load
				  */
				EnginePattern(
					const char *fileName,
					ConfigPattern *config,
					RequiredPropertiesConfig *properties);

				/**
				 * @copydoc Common::EngineBase::EngineBase
				 * The data set is constructed from the file provided.
				 * @param fileName path to the file containing the data file to
				 * load
				 */
				EnginePattern(
					const string &fileName,
					ConfigPattern *config,
					RequiredPropertiesConfig *properties);

				/**
				 * @copydoc Common::EngineBase::EngineBase
				 * The data set is constructed from data stored in memory
				 * described by the data and length parameters.
				 * @param data pointer to the memory containing the data set
				 * @param length size of the data in memory
				 */
				EnginePattern(
					void *data,
					long length,
					ConfigPattern *config,
					RequiredPropertiesConfig *properties);

				/**
				 * @copydoc Common::EngineBase::EngineBase
				 * The data set is constructed from data stored in memory
				 * described by the data and length parameters.
				 * @param data pointer to the memory containing the data set
				 * @param length size of the data in memory
				 */
				EnginePattern(
					unsigned char data[],
					long length,
					ConfigPattern *config,
					RequiredPropertiesConfig *properties);

				/**
				 * @}
				 * @name Engine Methods
				 * @{
				 */

				 /**
				  * Process the evidence and return the results.
				  * @param evidence to process
				  * @param closestSignatures maximum signatures to compare when
				  * looking for a closes match.
				  * @return new Results instance containing the result of
				  * processing the evidence
				  */
				ResultsPattern* process(
					EvidenceDeviceDetection *evidence,
					int closestSignatures);

				/**
				 * Process the User-Agent and return the results.
				 * @param userAgent to process
				 * @param closestSignatures maximum signatures to compare when
				 * looking for a closes match.
				 * @return new Results instance containing the result of
				 * processing the User-Agent
				 */
				ResultsPattern* process(
					const char *userAgent,
					int closestSignatures);

				/**
				 * @copydoc EngineDeviceDetection::processDeviceDetection(EvidenceDeviceDetection*)
				 */
				ResultsPattern* process(EvidenceDeviceDetection *evidence);

				/**
				 * @copydoc EngineDeviceDetection::processDeviceDetection(const char*)
				 */
				ResultsPattern* process(const char *userAgent);

				/**
				 * @}
				 * @name Common::EngineBase Implementation
				 * @{
				 */

				void refreshData();

				void refreshData(const char *fileName);

				void refreshData(void *data, long length);

				void refreshData(unsigned char data[], long length);

				ResultsBase* processBase(EvidenceBase *evidence);

				Date getPublishedTime();

				Date getUpdateAvailableTime();

				string getDataFilePath();

				string getDataFileTempPath();

				string getProduct();

				string getType();

				/**
				 * @}
				 * @name DeviceDetection::EngineDeviceDetection Implementation
				 * @{
				 */

				ResultsDeviceDetection* processDeviceDetection(
					EvidenceDeviceDetection *evidence);

				ResultsDeviceDetection* processDeviceDetection(
					const char *userAgent);

				/**
				 * @}
				 */

			protected:
				/**
				 * @copydoc EngineDeviceDetection::init
				 */
				void init(fiftyoneDegreesDataSetPattern *dataSet);

			private:
				void initMetaData();

				void init();
			};
		}
	}
}

#endif
