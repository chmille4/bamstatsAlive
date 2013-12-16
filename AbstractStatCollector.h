#ifndef ABSTRACTSTATCOLLECTOR_H
#define ABSTRACTSTATCOLLECTOR_H

#pragma once

#include <vector>
#include <api/BamAlignment.h>
#include <jansson.h>

namespace BamstatsAlive {

	class AbstractStatCollector;

	typedef std::vector<AbstractStatCollector *> StatCollectorPtrVec;

	/**
	 * The base class for all statistics collectors
	 *
	 * A statistics collector will implement two virtual functions: 
	 *   - processAlignment() to update statistics
	 *   - appendJson() to create the json representation of the statistics
	 *
	 * These statistics can be organized into a tree with the addChild() and 
	 * removeChild() functions. User code will only need to call the public
	 * processAlignment() and appendJson() functions on the root object, and 
	 * the action will be propagated across all child nodes. The actual 
	 * implementation of specific collectors is encapsulated by the protected
	 * processAlignmentImpl() and appendJsonImpl()
	 */
	class AbstractStatCollector {
		protected:
			StatCollectorPtrVec _children;

			/**
			 * Process the alignment and update statistics
			 *
			 * @param al The alignment read
			 * @param refVector The reference the read is aligned to
			 */
			virtual void processAlignmentImpl(const BamTools::BamAlignment& al, const BamTools::RefVector& refVector) = 0;

			/**
			 * Append statistics as json
			 *
			 * @param jsonRootObj The json root object to which the outputs are appended
			 */
			virtual void appendJsonImpl(json_t * jsonRootObj) = 0;

		public:
			AbstractStatCollector();
			virtual ~AbstractStatCollector();

			/**
			 * Add a statistics collector as the child of the current collector
			 * 
			 * @param child The child collector to be added
			 */
			void addChild(AbstractStatCollector * child);

			/**
			 * Remove a statistics collector from the children list of the current collector
			 *
			 * @param child The child collector to be removed
			 */
			void removeChild(AbstractStatCollector * child);

			/**
			 * Process an alignment by the collector tree
			 *
			 * The alignment will be passed to the processAlignmentImpl function of the
			 * current collector, and the processAlignment function of all the children
			 * collectors.
			 *
			 * @param al The alignment read
			 * @param refVector The reference the read is aligned to
			 */
			void processAlignment(const BamTools::BamAlignment& al, const BamTools::RefVector& refVector);

			/**
			 * Create json of the collector tree
			 *
			 * Json objects representing all the statistics collected by the
			 * current tree will be appended into the given json root object.
			 * If no root object is given, a new one will be created. Current
			 * collector's appendJsonImpl function and all the children's 
			 * appendJson function will be called on the root object.
			 *
			 * @param jsonRootObj The json root object to which all statistics are appended to
			 * @return The json object representing the root of all the statistics
			 */
			json_t * appendJson(json_t * jsonRootObj = NULL);
	};

}

#endif
