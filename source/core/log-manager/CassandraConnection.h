#ifndef _CASSANDRA_CONNECTION_H_
#define _CASSANDRA_CONNECTION_H_

#include <string>
#include <map>
#include <list>
#include <vector>

#include <util/ThreadModel.h>

#include "LogManagerSingleton.h"

namespace libcassandra
{
class Cassandra;
}

namespace org
{
namespace apache
{
namespace cassandra
{
class ColumnDef;
}
}
}

namespace sf1r {

class CassandraConnection : public LogManagerSingleton<CassandraConnection>
{
public:
    CassandraConnection();

    ~CassandraConnection();

    const std::string& getKeyspaceName() const;

    void setKeyspaceName(const std::string& keyspace_name);

    bool init(const std::string& str);

    bool isEnabled();

    boost::shared_ptr<libcassandra::Cassandra>& getCassandraClient();

    bool createColumnFamily(
            const std::string& in_name,
            const std::string& in_column_type,
            const std::string& in_comparator_type,
            const std::string& in_sub_comparator_type,
            const std::string& in_comment,
            const double in_row_cache_size,
            const double in_key_cache_size,
            const double in_read_repair_chance,
            const std::vector<org::apache::cassandra::ColumnDef>& in_column_metadata,
            const int32_t in_gc_grace_seconds,
            const std::string& in_default_validation_class,
            const int32_t in_id,
            const int32_t in_min_compaction_threshold,
            const int32_t in_max_compaction_threshold,
            const int32_t in_row_cache_save_period_in_seconds,
            const int32_t in_key_cache_save_period_in_seconds,
            const std::map<std::string, std::string>& in_compression_options);

    bool truncateColumnFamily(const std::string& in_name);

    bool dropColumnFamily(const std::string& in_name);

private:
    bool isEnabled_;

    std::string keyspace_name_;

    boost::shared_ptr<libcassandra::Cassandra> cassandra_client_;
};

}

#endif
