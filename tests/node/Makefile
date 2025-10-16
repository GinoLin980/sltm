test:
	gcc -I ../../include mock_event.c mock_utils.c test_node.c ../../src/node.c -o bin/test_node -std=c11
	@echo ""
	./bin/test_node
	@echo ""
	rm ./bin/test_node
