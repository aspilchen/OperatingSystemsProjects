KERNAL := linux-stable/
TESTLIB := sys_call_tests/
TESTS := ancestor_test/ array_test/

.PHONY: all
all: kernal tests

.PHONY: kernal
kernal: $(KERNAL)
	make --directory=$<

.PHONY: tests
test: $(TESTS)

.PHONY: $(TESTS)
$(TESTS):
	make --directory=$(TESTLIB)$@
