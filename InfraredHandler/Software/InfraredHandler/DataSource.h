/*
 * DataSource.h
 *
 *  Created on: 2014-03-01
 *      Author: kigunda
 */

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

using namespace std;
#include <list>

#include "includes.h"

class DataSource {
public:
	DataSource();
	virtual ~DataSource();

	/*
	 * Adds a listener queue to this data source.
	 * @param queue - the queue to add
	 */
	void addListener(OS_EVENT *queue);

	/*
	 * Removes a listener queue from this data source.
	 * @param queue - the queue to remove
	 */
	void removeListener(OS_EVENT *queue);

protected:
	/* The queues used to send messages to listeners. */
	list<OS_EVENT *> listeners;
};

#endif /* DATASOURCE_H_ */
