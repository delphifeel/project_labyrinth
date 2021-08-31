#include <stdlib.h>
#include <time.h>

#include "../include/labyrinth/lab-generation.h"
#include "../include/CORE.h"
#include "../include/labyrinth.h"
#include "../include/disjoint-set.h"

/*****************************************************************************************************************************/

#define MATRIX_SIZE 		(41)
#define SPAWN_POINTS_COUNT 	(20)

typedef struct Edge
{
	uint32 From;
	uint32 To;
} Edge;

static void LabGeneration_InternalFillRectangleLab(LabPointsMap TempPointsMap)
{
	LabPointStruct LabPoint;
	int32 ConnectionID, ID, P;


	if (MATRIX_SIZE < 7)
		CORE_DebugError("MATRIX_SIZE need to be >= 7\n");

	if (MATRIX_SIZE % 2 == 0)
		CORE_DebugError("MATRIX_SIZE need to be odd\n");


	// create all points
	P = MATRIX_SIZE * MATRIX_SIZE;
	for (uint32 i = 0; i < P; i++)
	{
		LabPoint.Id = i + 1;
		
		LabPoint.TopConnectionId = 0;
		LabPoint.RightConnectionId = 0;
		LabPoint.LeftConnectionId = 0;
		LabPoint.BottomConnectionId = 0;
		LabPoint.IsExit = FALSE;
		LabPoint.IsSpawn = FALSE;

		LabPointsMap_AddPoint(TempPointsMap, LabPoint);
	}


	// set exit point
	LabPointsMap_GetPointByID(TempPointsMap, P / 2 + 1, &LabPoint);
	CORE_DebugPrint("Set %ld as exit\n", P / 2 + 1);
	LabPoint.IsExit = TRUE;
	LabPointsMap_ChangePoint(TempPointsMap, LabPoint);


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

		LabPointsMap_GetPointByID(TempPointsMap, PossibleSpawnPoints[i], &LabPoint);

		LabPoint.IsSpawn = TRUE; 
		LabPointsMap_ChangePoint(TempPointsMap, LabPoint);
		AddedPoints++;

		if (AddedPoints == SPAWN_POINTS_COUNT)
			break;
	}


	// fill connections for every point
	for (uint32 i = 0; i < P; i++)
	{
		ID = i + 1;
		LabPointsMap_GetPointByID(TempPointsMap, ID, &LabPoint);

		// top connection
		ConnectionID = ID - MATRIX_SIZE;
		if (ConnectionID > 0)
			LabPoint.TopConnectionId = ConnectionID;

		// right connection
		ConnectionID = ID + 1;
		if (ConnectionID % MATRIX_SIZE != 1)
			LabPoint.RightConnectionId = ConnectionID;

		// bottom connection
		ConnectionID = ID + MATRIX_SIZE;
		if (ConnectionID <= P)
			LabPoint.BottomConnectionId = ConnectionID;
		
		// left connection
		ConnectionID = ID - 1;
		if (ConnectionID % MATRIX_SIZE != 0)
			LabPoint.LeftConnectionId = ConnectionID; 

		LabPointsMap_ChangePoint(TempPointsMap, LabPoint);
	}
}

static int LabGeneration_InternalSortEdgesRandomly(const void *left, const void *right)
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

static void LabGeneration_InternalCopyConnectionsAccordingToEdge(LabPointsMap TempPointsMap, LabPointsMap ResultLabPointsMapHandle, uint32 LabPointID, uint32 ConnectionLabPointID)
{
	LabPointStruct SourcePoint, ResultPoint;

	LabPointsMap_GetPointByID(TempPointsMap, LabPointID, &SourcePoint);
	LabPointsMap_GetPointByID(ResultLabPointsMapHandle, LabPointID, &ResultPoint);

	if (SourcePoint.TopConnectionId == ConnectionLabPointID)
	{
		ResultPoint.TopConnectionId = ConnectionLabPointID;
	}
	else if (SourcePoint.RightConnectionId == ConnectionLabPointID)
	{
		ResultPoint.RightConnectionId = ConnectionLabPointID;
	}
	else if (SourcePoint.BottomConnectionId == ConnectionLabPointID)
	{
		ResultPoint.BottomConnectionId = ConnectionLabPointID;
	}
	else if (SourcePoint.LeftConnectionId == ConnectionLabPointID)
	{
		ResultPoint.LeftConnectionId = ConnectionLabPointID;
	}

	LabPointsMap_ChangePoint(ResultLabPointsMapHandle, ResultPoint);
}

static void LabGeneration_BuildMSTMaze(LabPointsMap TempPointsMap, LabPointsMap MSTPointsMapHandle)
{
	LabPointStruct 	CurrentLabPointHandle, TempLabPointHandle, LabPoint; 
	DisjointSet 	DisjointSetHadle;
	uint32 			VertexCount, ID;
	Edge 			*SortedEdges, *MSTEdges;
	uint32 			SortedEdgesSize, MSTEdgesSize, SubsetsLeft;


	SortedEdges = CORE_MemAlloc(sizeof(Edge) * MATRIX_SIZE * MATRIX_SIZE * 4);
	MSTEdges = CORE_MemAlloc(sizeof(Edge) * MATRIX_SIZE * MATRIX_SIZE * 4);

	// get all possible edges
	SortedEdgesSize = 0;
	LabPointsMap_GetSize(TempPointsMap, &VertexCount);
	for (uint32 i = 0; i < VertexCount; i++)
	{
		ID = i + 1;
		LabPointsMap_GetPointByID(TempPointsMap, ID, &CurrentLabPointHandle);

		if (CurrentLabPointHandle.TopConnectionId != 0)
		{
			SortedEdges[SortedEdgesSize].From = ID; 
			SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle.TopConnectionId;
			SortedEdgesSize++;
		}
		
		if (CurrentLabPointHandle.RightConnectionId != 0)
		{
			SortedEdges[SortedEdgesSize].From = ID; 
			SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle.RightConnectionId;
			SortedEdgesSize++;
		}

		if (CurrentLabPointHandle.BottomConnectionId != 0)
		{
			SortedEdges[SortedEdgesSize].From = ID; 
			SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle.BottomConnectionId;
			SortedEdgesSize++;
		}

		if (CurrentLabPointHandle.LeftConnectionId != 0)
		{
			SortedEdges[SortedEdgesSize].From = ID; 
			SortedEdges[SortedEdgesSize].To = CurrentLabPointHandle.LeftConnectionId;
			SortedEdgesSize++;
		}
	}


	// sort edges randomly
	srand(time(NULL));
	qsort(SortedEdges, SortedEdgesSize, sizeof(Edge), LabGeneration_InternalSortEdgesRandomly);

	CORE_DebugPrint("SortedEdgesSize: %ld\n", SortedEdgesSize);


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


	// create new lab points map accordoing to MSTEdges
	for (uint32 i = 0; i < VertexCount; i++)
	{
		LabPointsMap_GetPointByID(TempPointsMap, i + 1, &TempLabPointHandle);

		LabPoint.Id = i + 1; 
		LabPoint.TopConnectionId = 0;
		LabPoint.RightConnectionId = 0;
		LabPoint.LeftConnectionId = 0;
		LabPoint.BottomConnectionId = 0;
		LabPoint.IsExit = TempLabPointHandle.IsExit;
		LabPoint.IsSpawn = TempLabPointHandle.IsSpawn;

		LabPointsMap_AddPoint(MSTPointsMapHandle, LabPoint);
	}

	for (uint32 i = 0; i < MSTEdgesSize; i++)
	{
		if ((MSTEdges[i].From == 0) || (MSTEdges[i].To == 0))
			continue;

		LabGeneration_InternalCopyConnectionsAccordingToEdge(TempPointsMap, MSTPointsMapHandle, MSTEdges[i].From, MSTEdges[i].To);
		LabGeneration_InternalCopyConnectionsAccordingToEdge(TempPointsMap, MSTPointsMapHandle, MSTEdges[i].To, MSTEdges[i].From);
	}

	CORE_MemFree(SortedEdges);
	CORE_MemFree(MSTEdges);
}

/*****************************************************************************************************************************/

void LabGeneration_Execute(LabPointsMap GeneratedLabPointsMap)
{
	LabPointsMap 		TempPointsMap;


	LabPointsMap_Create(&TempPointsMap);

	LabGeneration_InternalFillRectangleLab(TempPointsMap);
	LabGeneration_BuildMSTMaze(TempPointsMap, GeneratedLabPointsMap);

	LabPointsMap_Free(&TempPointsMap);
}

/*****************************************************************************************************************************/
