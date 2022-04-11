// Used to hide using inet_addr error. Not sure what to use instead.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "MinecraftClient.h"

#include<stdio.h>
#include<winsock2.h>

#include "Graph.h"
#include "Directions.h"

#pragma comment(lib,"ws2_32.lib")

WSADATA wsa;
SOCKET s;

char MC_GET_LOCATION = 1;
char MC_GET_NODES = 2;
char MC_UPSERT_GRAPH = 3;
char MC_RESET_GRAPH = 4;

char MC_GRAPH_NODE = 1;
char MC_GRAPH_EDGE = 2;

// Thank you Silver Moon https://www.binarytides.com/winsock-socket-programming-tutorial/
// and https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedcode1d.html
int mcInit() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed initing winsock: %d", WSAGetLastError());
		return 1;
	}

	return 0;
}

int mcConnect(int port) {
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket: %d", WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
		printf("Connect error");
		return 1;
	}

	return 0;
}

int mcClose() {
	closesocket(s);
	WSACleanup();

	return 0;
}

int mcRecvInt() {
	int i;
	recv(s, &i, sizeof(i), 0x8);
	return ntohl(i);
}

float mcRecvFloat() {
	int i;
	recv(s, &i, sizeof(i), 0x8);
	return ntohf(i);
}

void mcSendByte(char b) {
	send(s, &b, sizeof(b), 0);
}

void mcSendInt(int i) {
	i = htonl(i);
	send(s, &i, sizeof(i), 0);
}

void mcSendFloat(float f) {
	int i = htonf(f);
	send(s, &i, sizeof(i), 0);
}

void mcGetLocation(struct Location* location) {
	mcSendByte(MC_GET_LOCATION);

	location->x = mcRecvFloat();
	location->y = mcRecvFloat();
	location->z = mcRecvFloat();
	location->yaw = mcRecvFloat();
}

void mcGetNodes(struct Graph* graph) {
	mcSendByte(MC_GET_NODES);

	int len = mcRecvInt();
	int id;
	float x, y, z;
	for (int i = 0; i < len; i++) {
		mcRecvInt();
		mcRecvInt();
		id = mcRecvInt();
		x = mcRecvFloat();
		y = mcRecvFloat();
		z = mcRecvFloat();

		addNode(graph, createNode(id, x, y, z));
	}
}

void mcStartUpsertGraph() {
	mcSendByte(MC_UPSERT_GRAPH);
}

void mcUpsertNode(int id, float x, float y, float z, int color, float size) {
	mcSendByte(MC_GRAPH_NODE);
	mcSendInt(id);
	mcSendFloat(x);
	mcSendFloat(y);
	mcSendFloat(z);
	mcSendInt(color);
	mcSendFloat(size);
}

void mcUpsertEdge(int aId, int bId, int color) {
	mcSendByte(MC_GRAPH_EDGE);
	mcSendInt(aId);
	mcSendInt(bId);
	mcSendInt(color);
}

void mcStopUpsertGraph() {
	mcSendByte(-1);
}

void mcResetGraph() {
	mcSendByte(MC_RESET_GRAPH);
}
