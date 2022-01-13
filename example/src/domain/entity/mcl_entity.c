#include "entity/mcl_entity.h"
#include "mcl/assert.h"

MCL_TYPE(MclEntity) {
	MclEntityId id;
	MclInteger value;
	MclAggregatorId aggregatorId;
};

const size_t MCL_ENTITY_SIZE = sizeof(MclEntity);

MCL_PRIVATE const uint32_t DOUBLE_TIME = 2;

MCL_PRIVATE bool MclEntity_IsOverflowByTime(const MclEntity *self, uint32_t time) {
	return  ((uint64_t)self->value) * time > MCL_INTEGER_MAX;
}

MclStatus MclEntity_Init(MclEntity *self, MclEntityId id) {
	self->id = id;
	MclEntity_ClearValue(self);
	self->aggregatorId = MCL_AGGREGATOR_ID_INVALID;
	return MCL_SUCCESS;
}

void MclEntity_Destroy(MclEntity *self) {
	MclEntity_ClearValue(self);
	self->id = MCL_ENTITY_ID_INVALID;
	self->aggregatorId = MCL_AGGREGATOR_ID_INVALID;
}

MclEntityId MclEntity_GetId(const MclEntity *self) {
	return self ? self->id : MCL_ENTITY_ID_INVALID;
}

MclInteger MclEntity_GetValue(const MclEntity *self) {
	return self ? self->value : 0;
}

MclAggregatorId MclEntity_GetAggregatorId(const MclEntity *self) {
	return self ? self->aggregatorId : MCL_AGGREGATOR_ID_INVALID;
}

void MclEntity_OnInsertToAggregator(MclEntity *self, MclAggregatorId aggregatorId) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	MCL_ASSERT_TRUE_VOID(MclAggregatorId_IsValid(aggregatorId));
	MCL_ASSERT_TRUE_VOID(!MclAggregatorId_IsValid(self->aggregatorId));

	self->aggregatorId = aggregatorId;
}

void MclEntity_OnRemoveFromAggregator(MclEntity *self) {
	MCL_ASSERT_VALID_PTR_VOID(self);
	self->aggregatorId = MCL_AGGREGATOR_ID_INVALID;
	MclInteger_Clear(&self->value);
}

MclStatus MclEntity_DoubleValue(MclEntity *self) {
	MCL_ASSERT_VALID_PTR(self);

	if (self->value == 0) return MCL_STATUS_NOTHING_CHANGED;

	if (MclEntity_IsOverflowByTime(self, DOUBLE_TIME)) return MCL_FAILURE;

	self->value *= DOUBLE_TIME;
	return MCL_SUCCESS;
}

void MclEntity_ClearValue(MclEntity *self) {
	MclInteger_Clear(&self->value);
}
