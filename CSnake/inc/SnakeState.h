#pragma once
typedef struct Vector2D
{
	int x;
	int y;
};

typedef struct _tagSNAKESTATE
{
	Vector2D nextFruit;
	int score;
} SnakeState, *PSNAKESTATE;

static void GetFruitPosition(Vector2D* pt)
{
	pt->x = 0;
	pt->y = 0;
	return;
}