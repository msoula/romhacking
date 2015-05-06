/*
 * ScriptConfig.h
 *
 *  Created on: 22 févr. 2011
 *      Author: msoula
 */

#ifndef SCRIPTCONFIG_H_
#define SCRIPTCONFIG_H_

#include "../util/Logger.h"

#include <ostream>
#include <QtCore/QList>
#include <QtCore/QRegExp>

typedef struct _OPCODE_DATA {
	QRegExp regexp;
	int	value;
} T_OPCODE_DATA;

class ScriptConfig {

	/**
	 * @brief Get this ScriptConfig as a string.
	 *
	 * @param os: the stream.
	 * @param config: the config.
	 * @return a string that contains representation of this ScriptConfig.
	 */
	friend std::ostream& operator<<(std::ostream& os, const ScriptConfig & config);
public:
	/**
	 * @brief Constructor of an instance of ScriptConfig.
	 */
	ScriptConfig();

	/**
	 * @brief Destructor of this instance of ScriptConfig.
	 */
	~ScriptConfig();

	/**
	 * @brief Set the maximum line size.
	 *
	 * @param size: the new size.
	 */
	inline void setMaximumLineSize(int size) {
		_maximumLineSize = size;
	}

	/**
	 * @brief Get the maximum line size.
	 *
	 * @return the maximum line size.
	 */
	inline int maximumLineSize() const {
		return _maximumLineSize;
	}

	/**
	 * @brief Get the list of opcodes.
	 */
	inline QList<T_OPCODE_DATA> *opcodes() {
		return _pOpcodes;
	}

	/**
	 * @brief Append a new opcode data.
	 *
	 * @param opcodeData: the new opcode data.
	 */
	void append(const T_OPCODE_DATA & opcodeData);

	/**
	 * @brief Clear this script config.
	 */
	void clear();

private:
#ifndef _WIN_32
	static log4cxx::LoggerPtr logger;	/*!< the logger. */
#endif

	int _maximumLineSize;				/*!< maximum size length. */
	QList<T_OPCODE_DATA> *_pOpcodes;	/*!< opcode data list. */
};

#endif /* SCRIPTCONFIG_H_ */
