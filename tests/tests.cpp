#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "tonnetz.h"
#include "types.h"

TEST(TonnetzTest, GTEST) {
	MockTonnetz tonn;
	EXPECT_CALL(tonn, classicalNoteWalk());
}

class MockTonnetz : public Tonnetz {
	public:
		MOCK_METHOD(Note classicalNoteWalk, (), (override));
}