#include "common/eqemu_logsys.h"
#include "common/rulesys.h"
#include "zone/zonedb.h"

#include <fmt/format.h>

// Custom:NmsAATables: the NMS AA set lives in nms_aa_* copies of the four AA tables. The repositories
// read their own table, so the filter selects nothing from it and the rows from the nms_ copy.
std::string ZoneDatabase::AATableFilter(const std::string &table, const std::string &columns, const std::string &where)
{
	if (!UseNmsAATables()) {
		return where;
	}
	return fmt::format("FALSE UNION ALL SELECT {} FROM `nms_{}` WHERE {}", columns, table, where);
}

bool ZoneDatabase::UseNmsAATables()
{
	if (!RuleB(Custom, NmsAATables)) {
		return false;
	}
	for (const auto &t : {"aa_ability", "aa_ranks", "aa_rank_effects", "aa_rank_prereqs"}) {
		auto results = QueryDatabase(fmt::format("SHOW TABLES LIKE 'nms_{}'", t));
		if (!results.Success() || results.RowCount() == 0) {
			LogError("Custom:NmsAATables is on but table [nms_{}] is missing. Loading the stock AA tables.", t);
			return false;
		}
	}
	return true;
}
