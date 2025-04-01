#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QString>
#include <QMap>
/**
 * @brief The RecordManager class
 */
class RecordManager {
public:
    /**
     * @brief RecordManager constructor without parameters
     */
    RecordManager();
    /**
     * @brief getRecord get the record for a specific board size and color
     * @param rows
     * @param cols
     * @param numColors
     * @return int
     */
    int getRecord(int rows, int cols, int numColors)const ;
    /**
     * @brief updateRecord update the record for a specific board size and color
     * @param rows
     * @param cols
     * @param numColors
     * @param moves
     */
    void updateRecord(int rows, int cols, int numColors, int moves);
    /**
     * @brief loadRecords load the records
     */
    void loadRecords();
    /**
     * @brief saveRecords save the records
     */
    void saveRecords()const;
    /**
     * @brief operator = overload the operator =
     * @param other
     */
    void operator=(const RecordManager& other);

private:
    QString fileName;
    QMap<QString, int> records;  // Maps a key to the record for that board size/color
    QString generateKey(int rows, int cols, int numColors)const;  // Generates a unique key for each board size/color
};

#endif
