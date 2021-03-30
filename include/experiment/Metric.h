//
// Created by Khurram Javed on 2021-03-30.
//

#ifndef FLEXIBLENN_METRIC_H
#define FLEXIBLENN_METRIC_H
#include <vector>
#include <string>
#include <iostream>
#include "Database.h"


class Metric{
    std::vector<std::string> db_columns;
    std::vector<std::string> db_types;
    std::vector<std::string> index_columns;
    std::string table_name;
    std::string database_name;
    Database d = Database();
public:
    Metric(std::string database_name, std::string table_name, std::vector<std::string> keys, std::vector<std::string> type, std::vector<std::string> index);
    int add_values(const std::vector<std::vector<std::string>>& vector_of_values);
    int add_value(std::vector<std::string> values);

};

#endif //FLEXIBLENN_METRIC_H