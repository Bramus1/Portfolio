#include "RecordManager.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

RecordManager::RecordManager() {
    // Use the application's directory as the base path for the record file
    QString appDir = QCoreApplication::applicationDirPath();
    fileName = appDir + "/records.txt";

    loadRecords();
}

int RecordManager::getRecord(int rows, int cols, int numColors) const {
    QString key = generateKey(rows, cols, numColors);
    int record = records.value(key, 0);  // Return 0 if no record exists for this configuration

    return record;
}

void RecordManager::updateRecord(int rows, int cols, int numColors, int score) {
    QString key = generateKey(rows, cols, numColors);
    int currentRecord = getRecord(rows, cols, numColors);
    if (currentRecord == 0 || score < currentRecord) {
        records[key] = score;
        saveRecords();
    }
}

QString RecordManager::generateKey(int rows, int cols, int numColors) const {
    return QString("%1x%2-%3").arg(rows).arg(cols).arg(numColors);
}

void RecordManager::loadRecords() {
    QFile file(fileName);

    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly)) {
            return;
        }
        file.close();
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("=");
        if (parts.size() == 2) {
            records[parts[0]] = parts[1].toInt();
        }
    }
    file.close();
}

void RecordManager::saveRecords() const {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (auto it = records.begin(); it != records.end(); ++it) {
        out << it.key() << "=" << it.value() << "\n";
    }

    file.close();
}

