#include "../include/CORE.h"
#include "../include/labyrinth.h"
#include "../include/disjoint-set.h"

#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 		(141)
#define SPAWN_POINTS_COUNT 	(20)

typedef struct Edge
{
	uint32 From;
	uint32 To;
} Edge;

void FillRectangleLab(LabPointsMap LabPointsMapHandle)
{
	LabPointStruct *CurrentLabPointHandle;
	int32 ConnectionID, ID, P;

	if (MATRIX_SIZE < 7)
		CORE_DebugAssert("MATRIX_SIZE need to be >= 7\n");

	if (MATRIX_SIZE % 2 == 0)
		CORE_DebugAssert("MATRIX_SIZE need to be odd\n");

	// create all points
	P = MATRIX_SIZE * MATRIX_SIZE;
	for (uint32 i = 0; i < P; i++)
	{
		CurrentLabPointHandle = (LabPointStruct *) CORE_MemAlloc(sizeof(LabPointStruct)); 
		CurrentLabPointHandle->Id = i + 1;
		LabPointsMap_AddPoint(LabPointsMapHandle, CurrentLabPointHandle);
	}

	// set exit point
	LabPointsMap_GetPointByID(LabPointsMapHandle, P / 2 + 1, &CurrentLabPointHandle);
	CORE_DebugPrint("Set %ld as exit\n", P / 2 + 1);
	CurrentLabPointHandle->IsExit = TRUE; 
	// set spawn points (center square of matrix)
	uint32 PossibleSpawnPoints[P / 2];
	uint32 PossibleSpawnPointsSize = 0;
	uint32 RowIndex, ColumnIndex, PosIndex, AddedPoints;

	PosIndex = MATRIX_SIZE / 4;

	RowIndex = PosIndex;
	for (ColumnIndex = PosIndex + 2; ColumnIndex < MATRIX_SIZE - PosIndex + 1; ColumnIndex++)
	{
		PossibleSpawnPoints[PossibleSpawnPointsSize] = RowIndex * MATRIX_SIZE + ColumnIndex;
		PossibleSpawnPointsSize++;
	}

	ColumnIndex = MATRIX_SIZE - PosIndex;
	for (RowIndex = PosIndex + 1; RowIndex < MATRIX_SIZE - PosIndex; RowIndex++)
	{
		PossibleSpawnPoints[PossibleSpawnPointsSize] = RowIndex * MATRIX_SIZE + ColumnIndex;
		PossibleSpawnPointsSize++;
	}

	RowIndex = MATRIX_SIZE - PosIndex - 1;
	for (ColumnIndex = MATRIX_SIZE - PosIndex - 1; ColumnIndex > PosIndex; ColumnIndex--)
	{
		PossibleSpawnPoints[PossibleSpawnPointsSize] = RowIndex * MATRIX_SIZE + ColumnIndex;
		PossibleSpawnPointsSize++;
	}

	ColumnIndex = PosIndex + 1;
	for (RowIndex = MATRIX_SIZE - PosIndex - 2; RowIndex > PosIndex - 1; RowIndex--)
	{
		PossibleSpawnPoints[PossibleSpawnPointsSize] = RowIndex * MATRIX_SIZE + ColumnIndex;
		PossibleSpawnPointsSize++;
	}

	AddedPoints = 0;
	for (uint32 i = 0; i < PossibleSpawnPointsSize; i++)
	{
		if (i % (PossibleSpawnPointsSize / SPAWN_POINTS_COUNT) != 0)
			continue;

		LabPointsMap_GetPointByID(LabPointsMapHandle, PossibleSpawnPoints[i], &CurrentLabPointHandle);

		CurrentLabPointHandle->IsSpawn = TRUE; 
		AddedPoints++;

		if (AddedPoints == SPAWN_POINTS_COUNT)
			break;
	}

	// fill connections for every point
	for (uint32 i = 0; i < P; i++)
	{
		ID = i + 1;
		LabPointsMap_GetPointByID(LabPointsMapHandle, ID, &CurrentLabPointHandle);

		// top connection
		ConnectionID = ID - MATRIX_SIZE;
		if (ConnectionID > 0)
			CurrentLabPointHandle->TopConnectionId = ConnectionID;

		// right connection
		ConnectionID = ID + 1;
		if (ConnectionID % MATRIX_SIZE != 1)
			CurrentLabPointHandle->RightConnectionId = ConnectionID;

		// bottom connection
		ConnectionID = ID + MATRIX_SIZE;
		if (ConnectionID <= P)
			CurrentLabPointHandle->BottomConnectionId = ConnectionID;
		
		// left connection
		ConnectionID = ID - 1;
		if (ConnectionID % MATRIX_SIZE != 0)
			CurrentLabPointHandle->LeftConnectionId = ConnectionID; 
	}
}

void PrintEdges(Edge *Edges, uint32 EdgesSize)
{
	CORE_DebugPrint("*** EDGES ***\n");
	for (uint32 i = 0; i < EdgesSize; i++)
		{
		CORE_DebugPrint("%ld -> %ld\n", Edges[i].From, Edges[i].To);
		}
	CORE_DebugPrint("*** EDGES ***\n");
}

int SortEdgesRandomly(const void *left, const void *right)
{
	int32 RandomNumber;


	RandomNumber = rand() % 3;
	switch (RandomNumber)
		{
		case 0: return -1;
		case 1: return 0;
		case 2: return 1;
		}

	return 0;
}

void CopyConnectionsAccordingToEdge(LabPointsMap LabPointsMapHandle, LabPointsMap ResultLabPointsMapHandle, uint32 LabPointID, uint32 ConnectionLabPointID)
{
	LabPointStruct *LabPointHandle, *ResultLabPointHandle, *ResultConnectionLabPointHandle;
	uint32 CurrentConnectionID;

	LabPointsMap_GetPointByID(LabPointsMapHandle, LabPointID, &LabPointHandle);
	if (LabPointHandle == NULL)
		return;

	LabPointsMap_GetPointByID(ResultLabPointsMapHandle, LabPointID, &ResultLabPointHandle);
	if (ResultLabPointHandle == NULL)
		return;

	LabPointsMap_GetPointByID(ResultLabPointsMapHandle, ConnectionLabPointID, &ResultConnectionLabPointHandle);
	if (ResultConnectionLabPointHandle == NULL)
		return;

	if (LabPointHandle->TopConnectionId == ConnectionLabPointID)
	{
		ResultLabPointHandle->TopConnectionId = ResultConnectionLabPointHandle->Id;
		return;
	}

	if (LabPointHandle->RightConnectionId == ConnectionLabPointID)
	{
		ResultLabPointHandle->RightConnectionId = ResultConnectionLabPointHandle->Id;
		return;
	}

	if (LabPointHandle->BottomConnectionId == ConnectionLabPointID)
	{
		ResultLabPointHandle->BottomConnectionId = ResultConnectionLabPointHandle->Id;
		return;
	}

	if (LabPointHandle->LeftConnectionId == ConnectionLabPointID)
	{
			ResultLabPointHandle->LeftConnectionId = ResultConnectionLabPointHandle->Id;
			return;
	}
	
}

void BuildMSTMaze(LabPointsMap LabPointsMapHandle, LabPointsMap MSTPointsMapHandle)
{
	LabPointStruct 	*CurrentLabPointHandle, *TempLabPointHandle, *CurrentConnectionLabPointHandle; 
	DisjointSet 	DisjointSetHadle;
	uint32 			VertexCount, CurrentLabPointID, ID;
	Edge 			SortedEdges[MATRIX_SIZE * MATRIX_SIZE * 4], MSTEdges[MATRIX_SIZE * MATRIX_SIZE * 4];
	uint32 			SortedEdgesSize, MSTEdgesSize, SubsetsLeft;
	CORE_Bool		TempBoolValue;


	// get all possible edges
	SortedEdgesSize = 0;
	LabPointsMap_GetSize(LabPointsMapHandle, &VertexCount);
	for (uint32 i = 0; i < VertexCount; i++)
	{
		ID = i + 1;
		LabPointsMap_GetPointByID(LabPointsMapHandle, ID, &CurrentLabPointHandle);
		if (CurrentLabPointHandle == NULL)
			continue;

		SortedEdges[SortedEdgesSize].From = ID; 
		SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle->TopConnectionId;
		SortedEdgesSize++;
		
		SortedEdges[SortedEdgesSize].From = ID; 
		SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle->RightConnectionId;
		SortedEdgesSize++;

		SortedEdges[SortedEdgesSize].From = ID; 
		SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle->BottomConnectionId;
		SortedEdgesSize++;

		SortedEdges[SortedEdgesSize].From = ID; 
		SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle->LeftConnectionId;
		SortedEdgesSize++;
	}


	// sort edges randomly
	srand(time(NULL));
	qsort(SortedEdges, SortedEdgesSize, sizeof(Edge), SortEdgesRandomly);

	CORE_DebugPrint("SortedEdgesSize: %ld\n", SortedEdgesSize);
	// PrintEdges(SortedEdges, SortedEdgesSize);

	// create disjoint set from edges and union all possible edges according to Kruskal's algo
	DisjointSet_Create(&DisjointSetHadle);
	DisjointSet_Setup(DisjointSetHadle, VertexCount + 1);
	MSTEdgesSize = 0;
	
	for (uint32 i = 0; i < SortedEdgesSize; i++)
		{
		if (DisjointSet_Union(DisjointSetHadle, SortedEdges[i].From, SortedEdges[i].To) == FALSE)
			continue;

		MSTEdges[MSTEdgesSize].From = SortedEdges[i].From;
		MSTEdges[MSTEdgesSize].To = SortedEdges[i].To;
		MSTEdgesSize++;
		}

	DisjointSet_GetSubsetsCount(DisjointSetHadle, &SubsetsLeft);
	DisjointSet_Free(&DisjointSetHadle);

	CORE_DebugPrint("SubsetsLeft: %ld\n", SubsetsLeft);
	CORE_DebugPrint("MSTEdgesSize: %ld\n", MSTEdgesSize);
	// PrintEdges(MSTEdges, MSTEdgesSize);

	// create new lab points map accordoing to MSTEdges
	for (uint32 i = 0; i < VertexCount; i++)
	{
		CurrentLabPointHandle = (LabPointStruct * ) CORE_MemAlloc(sizeof(LabPointStruct)); 
		CurrentLabPointHandle->Id = i + 1; 

		LabPointsMap_GetPointByID(LabPointsMapHandle, i + 1, &TempLabPointHandle);
		CurrentLabPointHandle->IsExit = TempLabPointHandle->IsExit;
		CurrentLabPointHandle->IsSpawn = TempLabPointHandle->IsSpawn;

		LabPointsMap_AddPoint(MSTPointsMapHandle, CurrentLabPointHandle);
	}

	for (uint32 i = 0; i < MSTEdgesSize; i++)
	{
		CopyConnectionsAccordingToEdge(LabPointsMapHandle, MSTPointsMapHandle, MSTEdges[i].From, MSTEdges[i].To);
		CopyConnectionsAccordingToEdge(LabPointsMapHandle, MSTPointsMapHandle, MSTEdges[i].To, MSTEdges[i].From);
	}
}

void BuildLabyrinth()
{
	LabPointsMap 		LabPointsMapHandle, MSTPointsMapHandle;
	CORE_FileHandle 	FileToWrite;
	char 				*JSON;


	LabPointsMap_Create(&LabPointsMapHandle);
	LabPointsMap_Create(&MSTPointsMapHandle);
	FillRectangleLab(LabPointsMapHandle);
	BuildMSTMaze(LabPointsMapHandle, MSTPointsMapHandle);
	LabPointsMap_Free(&LabPointsMapHandle);
	LabPointsMap_ToJSON(MSTPointsMapHandle, &JSON);

	FileToWrite = CORE_FileOpen("data-prim.json", "w");
	CORE_FileWrite(FileToWrite, sizeof(char), strlen(JSON), JSON);
	CORE_FileClose(FileToWrite);

	CORE_MemFree(JSON);
	LabPointsMap_Free(&MSTPointsMapHandle);
}

int main()
{
	BuildLabyrinth();
}