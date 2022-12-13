#include <gtest/gtest.h>
#include "tonnetz.h"
#include "types.h"

TEST(TonnetzTest, BasicAssertions) {
	Tonnetz tonn(1234);

	for (int i = 0; i < 10; i++) {
		Note n = tonn.classicalNoteWalk();
		LOG_DEBUG(n.note);
	}
}