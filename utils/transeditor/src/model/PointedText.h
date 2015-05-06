/*
 * PointedText.h
 *
 *  Created on: 13 nov. 2010
 *      Author: rid
 */

#ifndef POINTEDTEXT_H_
#define POINTEDTEXT_H_

#include <QtCore/QObject>

#include "../util/Logger.h"

/**
 * A PointedText is a text that is pointed.
 * A PointedText can be translated and validated.
 */
class PointedText : public QObject {
	Q_OBJECT
public:
	/**
	 * @brief Constructor of an instance of PointedText.
	 */
	PointedText();

	/**
	 * @brief Copy constructor of an instance of PointedText.
	 *
	 * @param pointedText: the PointedText to copy.
	 */
	PointedText(const PointedText & pointedText);

	/**
	 * @brief Destructor of this instance of PointedText.
	 */
	~PointedText();

	PointedText& operator=(const PointedText&);

	/**
	 * @brief Get the text of this PointedText.
	 *
	 * @return the pointed text.
	 */
	QString text() const;

	/**
	 * @brief Set the text of this PointedText.
	 *
	 * @param text: the new text.
	 */
	void setText(const QString & text);

	/**
	 * @brief Get if this PointedText is translated or not.
	 *
	 * @return TRUE if translated, FALSE otherwise.
	 */
	bool isTranslated() const;

	/**
	 * @brief Set this PointedText translated or not.
	 *
	 * @param isTranslated: TRUE to set translated, FALSE otherwise.
	 */
	void setTranslated(bool isTranslated);

	/**
	 * @brief Get if the translation of this PointedText is done or not.
	 *
	 * @return TRUE if the translation is done, FALSE otherwise.
	 */
	bool isTranslationDone() const;

	/**
	 * @brief Set the translation of this PointedText done or not.
	 *
	 * @param isTranslationDone: TRUE to set translation done, FALSE otherwise.
	 */
	void setTranslationDone(bool isTranslationDone);

	QString getCpuAddress();
	void setCpuAddress(const QString& cpu_address);

private:
    QString _pointedText;		/*!< the text. */
    bool _isTranslated;			/*!< TRUE if translation has been validated, FALSE otherwise. */
    bool _isTranslationDone;	/*!< TRUE if translation has been done, FALSE otherwise. */
    QString _cpu_address;		/*!< text cpu address. */

#ifndef _WIN_32
	static log4cxx::LoggerPtr logger; /*!< the logger. */
#endif
};

#endif /* POINTEDTEXT_H_ */
