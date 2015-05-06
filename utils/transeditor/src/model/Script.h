/*
 * Script.h
 *
 *  Created on: 13 nov. 2010
 *      Author: rid
 */

#ifndef SCRIPT_H_
#define SCRIPT_H_

#include <QtCore/QObject>
#include <QtCore/QList>

#include "PointedText.h"
#include "../util/Logger.h"


/**
 * A Script is a list of PointedText.
 * It is offset ordered.
 */
class Script : public QObject {
	Q_OBJECT
public:
	/**
	 * @brief Constructor of an instance of Script.
	 *
	 * @param fileName: script file name
	 */
	Script(const QString & fileName);

	/**
	 * @brief Destructor of this instance of Script.
	 */
	~Script();

	/**
	 * @brief Get this Script filename.
	 *
	 * @return the filename.
	 */
	inline const QString & fileName() const {
		return _fileName;
	}

	/**
	 * @brief Get the number of PointedText in this Script.
	 *
	 * @return the number of PointedText in this Script
	 */
	int size() const;

	/**
	 * @brief Append a PointedText in this Script.
	 *
	 * @param pointedText: the PointedText to append.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool append(const PointedText & pointedText);

	/**
	 * @brief Insert a PointedText in this Script.
	 *
	 * @param index: the index where to insert pointedText
	 * @param pointedText: the PointedText to insert.
	 * @return TRUE if operation succeed, FALSE otherwise.
	 */
	bool insert(int index, const PointedText & pointedText);

	/**
	 * @brief Get a given pointed text.
	 *
	 * @param index: the index of the pointed text to return.
	 * @return a PointedText or NULL if index is invalid.
	 */
	PointedText* getPointedText(int index);

private:
	QString _fileName;					/*!< script filename. */
	QList<PointedText> _pointedTexts;	/*!< the list of PointedText. */

#ifndef _WIN_32
	static log4cxx::LoggerPtr logger; /*!< the logger. */
#endif
};

#endif /* SCRIPT_H_ */
