#include <stdlib.h>

#include "test-framework/unity.h"
#include "timeline.h"

void setUp(void) {}
void tearDown(void) {}

static void test_it_works(void) {
  timeline *state = malloc(sizeof(timeline));

  double begin = 1654761539;

  timeline_add(state, begin, "w1");
  timeline_add(state, begin + 10, "w1");
  timeline_add(state, begin + 40, "w1");
  timeline_add(state, begin + 340, "w1");
  timeline_add(state, begin + 360, "w2");
  timeline_add(state, begin + 400, "w2");
  timeline_finish(state);

  TEST_ASSERT_EQUAL_STRING(
      "w1,1654761539,1654761579\n"
      "w1,1654761879,1654761899\n"
      "w2,1654761899,1654761939\n",
      state->csv);

  free(state->csv);
  free(state);
}

int main(void) {
  UnityBegin("test.c");
  RUN_TEST(test_it_works);
  return UnityEnd();
}
