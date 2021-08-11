#include "../include/CORE.h"
#include "../include/labyrinth.h"
#include "../include/disjoint-set.h"

#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 		(41)
#define SPAWN_POINTS_COUNT 	(20)

typedef struct Edge
{
	uint32 From;
	uint32 To;
} Edge;

void FillRectangleLab(CORE_Handle LabPointsMapHandle)
{
	CORE_Handle CurrentLabPointHandle, CurrentConnectionLabPointHandle;
	int32 TempConnectionID, ID, P;

	if (MATRIX_SIZE < 7)
		CORE_DebugAssert("MATRIX_SIZE need to be >= 7\n");

	if (MATRIX_SIZE % 2 == 0)
		CORE_DebugAssert("MATRIX_SIZE need to be odd\n");

	// create all points
	P = MATRIX_SIZE * MATRIX_SIZE;
	for (uint32 i = 0; i < P; i++)
		{
		LabPoint_Create(&CurrentLabPointHandle);
		LabPoint_SetID(CurrentLabPointHandle, i + 1);
		if (LabPointsMap_AddPoint(LabPointsMapHandle, CurrentLabPointHandle) == FALSE)
			{
			CORE_DebugPrint("AddPoint error\n");
			return;
			}
		}

	// set exit point
	if (LabPointsMap_GetPointByID(LabPointsMapHandle, P / 2 + 1, &CurrentLabPointHandle) == TRUE)
		{
		CORE_DebugPrint("Set %d as exit\n", P / 2 + 1);
		LabPoint_SetIsExit(CurrentLabPointHandle, TRUE);
		}

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

		if (LabPointsMap_GetPointByID(LabPointsMapHandle, PossibleSpawnPoints[i], &CurrentLabPointHandle) == FALSE)
			continue;

		LabPoint_SetIsSpawn(CurrentLabPointHandle, TRUE);
		AddedPoints++;

		if (AddedPoints == SPAWN_POINTS_COUNT)
			break;
		}

	// fill connections for every point
	for (uint32 i = 0; i < P; i++)
		{
		ID = i + 1;
		if (LabPointsMap_GetPointByID(LabPointsMapHandle, ID, &CurrentLabPointHandle) == FALSE)
			{
			CORE_DebugPrint("GetPointID error\n");
			return;
			}

		// top connection
		TempConnectionID = ID - MATRIX_SIZE;
		if (TempConnectionID > 0)
			{
			LabPointsMap_GetPointByID(LabPointsMapHandle, TempConnectionID, &CurrentConnectionLabPointHandle);
			LabPoint_SetConnectionTop(CurrentLabPointHandle, CurrentConnectionLabPointHandle);
			}

		// right connection
		TempConnectionID = ID + 1;
		if (TempConnectionID % MATRIX_SIZE != 1)
			{
			LabPointsMap_GetPointByID(LabPointsMapHandle, TempConnectionID, &CurrentConnectionLabPointHandle);
			LabPoint_SetConnectionRight(CurrentLabPointHandle, CurrentConnectionLabPointHandle);
			}

		// bottom connection
		TempConnectionID = ID + MATRIX_SIZE;
		if (TempConnectionID <= P)
			{
			LabPointsMap_GetPointByID(LabPointsMapHandle, TempConnectionID, &CurrentConnectionLabPointHandle);
			LabPoint_SetConnectionBottom(CurrentLabPointHandle, CurrentConnectionLabPointHandle);
			}

		// left connection
		TempConnectionID = ID - 1;
		if (TempConnectionID % MATRIX_SIZE != 0)
			{
			LabPointsMap_GetPointByID(LabPointsMapHandle, TempConnectionID, &CurrentConnectionLabPointHandle);
			LabPoint_SetConnectionLeft(CurrentLabPointHandle, CurrentConnectionLabPointHandle);
			}
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

void CopyConnectionsAccordingToEdge(CORE_Handle LabPointsMapHandle, CORE_Handle ResultLabPointsMapHandle, uint32 LabPointID, uint32 ConnectionLabPointID)
{
	CORE_Handle LabPointHandle, ConnectionLabPointHandle, ResultLabPointHandle, ResultConnectionLabPointHandle;
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

	LabPoint_GetConnectionTop(LabPointHandle, &ConnectionLabPointHandle);
	if (ConnectionLabPointHandle != NULL)
		{
		LabPoint_GetID(ConnectionLabPointHandle, &CurrentConnectionID);
		if (CurrentConnectionID == ConnectionLabPointID)
			{
			LabPoint_SetConnectionTop(ResultLabPointHandle, ResultConnectionLabPointHandle);
			return;
			}
		}

	LabPoint_GetConnectionRight(LabPointHandle, &ConnectionLabPointHandle);

	if (ConnectionLabPointHandle != NULL)
		{
		LabPoint_GetID(ConnectionLabPointHandle, &CurrentConnectionID);
		if (CurrentConnectionID == ConnectionLabPointID)
			{
			LabPoint_SetConnectionRight(ResultLabPointHandle, ResultConnectionLabPointHandle);
			return;
			}
		}

	LabPoint_GetConnectionBottom(LabPointHandle, &ConnectionLabPointHandle);

	if (ConnectionLabPointHandle != NULL)
		{
		LabPoint_GetID(ConnectionLabPointHandle, &CurrentConnectionID);
		if (CurrentConnectionID == ConnectionLabPointID)
			{
			LabPoint_SetConnectionBottom(ResultLabPointHandle, ResultConnectionLabPointHandle);
			return;
			}
		}

	LabPoint_GetConnectionLeft(LabPointHandle, &ConnectionLabPointHandle);

	if (ConnectionLabPointHandle != NULL)
		{
		LabPoint_GetID(ConnectionLabPointHandle, &CurrentConnectionID);
		if (CurrentConnectionID == ConnectionLabPointID)
			{
			LabPoint_SetConnectionLeft(ResultLabPointHandle, ResultConnectionLabPointHandle);
			return;
			}
		}
}

void BuildMSTMaze(CORE_Handle LabPointsMapHandle, CORE_Handle MSTPointsMapHandle)
{
	CORE_Handle 	CurrentLabPointHandle, TempLabPointHandle, CurrentConnectionLabPointHandle, DisjointSetHadle;
	uint32 			VertexCount, CurrentLabPointID, ID;
	Edge 			SortedEdges[MATRIX_SIZE * MATRIX_SIZE * 4], MSTEdges[MATRIX_SIZE * MATRIX_SIZE * 4];
	uint32 			SortedEdgesSize, MSTEdgesSize, SubsetsLeft;
	CORE_Bool		TempBoolValue;

	#define ADD_SORTED_EDGE_BY_CONNECTION(CONNECTION_DIRECTION) do { 											\
		LabPoint_GetConnection##CONNECTION_DIRECTION(CurrentLabPointHandle, &CurrentConnectionLabPointHandle); 	\
		if (CurrentConnectionLabPointHandle != NULL) 															\
			{ 																									\
			LabPoint_GetID(CurrentConnectionLabPointHandle, &CurrentLabPointID); 								\
			SortedEdges[SortedEdgesSize].From = ID; 															\
			SortedEdges[SortedEdgesSize].To = CurrentLabPointID; 												\
			SortedEdgesSize++; 																					\
			} 																									\
	} while (0) 																								\


	// get all possible edges
	SortedEdgesSize = 0;
	LabPointsMap_GetSize(LabPointsMapHandle, &VertexCount);
	for (uint32 i = 0; i < VertexCount; i++)
		{
		ID = i + 1;
		LabPointsMap_GetPointByID(LabPointsMapHandle, ID, &CurrentLabPointHandle);
		if (CurrentLabPointHandle == NULL)
			continue;

		ADD_SORTED_EDGE_BY_CONNECTION(Top);
		ADD_SORTED_EDGE_BY_CONNECTION(Right);
		ADD_SORTED_EDGE_BY_CONNECTION(Bottom);
		ADD_SORTED_EDGE_BY_CONNECTION(Left);
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
	DisjointSet_Delete(&DisjointSetHadle);

	CORE_DebugPrint("SubsetsLeft: %ld\n", SubsetsLeft);
	CORE_DebugPrint("MSTEdgesSize: %ld\n", MSTEdgesSize);
	// PrintEdges(MSTEdges, MSTEdgesSize);

	// create new lab points map accordoing to MSTEdges
	for (uint32 i = 0; i < VertexCount; i++)
		{
		LabPoint_Create(&CurrentLabPointHandle);
		LabPoint_SetID(CurrentLabPointHandle, i + 1);

		if (LabPointsMap_GetPointByID(LabPointsMapHandle, i + 1, &TempLabPointHandle) == TRUE)
			{
			LabPoint_GetIsExit(TempLabPointHandle, &TempBoolValue);
			LabPoint_SetIsExit(CurrentLabPointHandle, TempBoolValue);

			LabPoint_GetIsSpawn(TempLabPointHandle, &TempBoolValue);
			LabPoint_SetIsSpawn(CurrentLabPointHandle, TempBoolValue);
			}

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
	CORE_Handle 		LabPointsMapHandle, MSTPointsMapHandle;
	CORE_FileHandle 	FileToWrite;
	char 				*JSON;


	LabPointsMap_Create(&LabPointsMapHandle);
	LabPointsMap_Create(&MSTPointsMapHandle);
	FillRectangleLab(LabPointsMapHandle);
	BuildMSTMaze(LabPointsMapHandle, MSTPointsMapHandle);
	LabPointsMap_Delete(&LabPointsMapHandle);
	LabPointsMap_ToJSON(MSTPointsMapHandle, &JSON);

	FileToWrite = CORE_FileOpen("data-prim.json", "w");
	CORE_FileWrite(FileToWrite, sizeof(char), CORE_StringLength(JSON), JSON);
	CORE_FileClose(FileToWrite);

	CORE_MemFree(JSON);
	LabPointsMap_Delete(&MSTPointsMapHandle);
}

int main()
{
	BuildLabyrinth();
}