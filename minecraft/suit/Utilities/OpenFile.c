#include "OpenFile.h"

void openAndUseFile(struct Graph* graph) {
	FILE* file;
	fopen_s(&file, "Data Sets/OsfjollDataset3.txt", "r");
	char line[256];

	int nodeID, sequenceID;
	float x, y, z;

	do {
		fscanf_s(file, "%d %d %f %f %f", &nodeID, &sequenceID, &x, &y, &z);
		addNode(graph, createNode(nodeID, 0, sequenceID, x, y, z));
	} while (fgets(line, sizeof(line), file));

	fclose(file);
}
