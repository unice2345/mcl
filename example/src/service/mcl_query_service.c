#include "mcl/service/mcl_query_service.h"
#include "mcl/service/statistics/mcl_statistics.h"
#include "repo/mcl_aggregator_repo.h"
#include "repo/mcl_entity_repo.h"
#include "factory/mcl_aggregator_factory.h"
#include "factory/mcl_entity_factory.h"
#include "aggregator/mcl_aggregator.h"
#include "entity/mcl_entity.h"
#include "mcl/lock/lockobj.h"
#include "mcl/assert.h"

size_t MclQueryService_QueryEntityCount() {
	size_t result = MclEntityRepo_GetSize();

	MCL_LOG_SUCC("Query Service: Query entity count (%zu) OK!", result);
	return result;
}

size_t MclQueryService_QueryAggregatorCount() {
	size_t result = MclAggregatorRepo_GetSize();

	MCL_LOG_SUCC("Query Service: Query aggregator count (%zu) OK!", result);
	return result;
}

size_t MclQueryService_QueryEntityCountOfAggregator(MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE_NIL(MclAggregatorId_IsValid(aggregatorId));

	MCL_LOCK_OBJ_AUTO const MclAggregator *aggregator = MclAggregatorRepo_FetchConst(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Query Service: not found aggregator (%u)!", aggregatorId);
		return 0;
	}

	size_t result = MclAggregator_GetEntityCount(aggregator);

	MCL_LOG_SUCC("Query Service: Query entity count (%zu) of aggregator (%u) OK!", result, aggregatorId);
	return result;
}

MclInteger MclQueryService_QueryValueOf(MclEntityId entityId) {
	MCL_ASSERT_TRUE_NIL(MclEntityId_IsValid(entityId));

	MCL_LOCK_OBJ_AUTO const MclEntity *entity = MclEntityRepo_FetchConst(entityId);
	if (!entity) {
		MCL_LOG_WARN("Query Service: not found entity (%u)!", entityId);
		return 0;
	}

	MclInteger result = MclEntity_GetValue(entity);

	MCL_LOG_SUCC("Query Service: Query value (%u) of entity (%u) OK!", result, entityId);
	return result;
}

MclInteger MclQueryService_QuerySumValueOf(MclAggregatorId aggregatorId) {
	MCL_ASSERT_TRUE_NIL(MclAggregatorId_IsValid(aggregatorId));

	MCL_LOCK_OBJ_AUTO const MclAggregator *aggregator = MclAggregatorRepo_FetchConst(aggregatorId);
	if (!aggregator) {
		MCL_LOG_WARN("Query Service: not found aggregator (%u)!", aggregatorId);
		return 0;
	}

	MclInteger result = MclAggregator_GetSumValue(aggregator);

	MCL_LOG_SUCC("Query Service: Query sum value (%u) of aggregator (%u) OK!", result, aggregatorId);
	return result;
}

MclStatistics MclQueryService_QueryStatistics() {
	MclStatistics statistics = {0};
	statistics.unreleasedAggregatorCount = MclAggregatorFactory_GetUnreleasedCount();
	statistics.unreleasedEntityCount = MclEntityFactory_GetUnreleasedCount();
	return statistics;
}
